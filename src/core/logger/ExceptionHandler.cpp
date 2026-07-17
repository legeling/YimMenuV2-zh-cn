#include "ExceptionHandler.hpp"

#include "ExceptionContext.hpp"
#include "StackTrace.hpp"

#include <hde64.h>
#include <unordered_set>


namespace YimMenu
{
	static bool TryRecoverKnownCNetworkTrap(EXCEPTION_POINTERS* exception_info)
	{
		constexpr std::uintptr_t kTrapOffset = 0x1487620;
		constexpr std::uintptr_t kReturnOffset = 0x1485806;
		constexpr std::uint32_t kCommonMainHash = 0xCA49E244;
		constexpr std::uint32_t kCNetworkHash = 0xB6331929;
		constexpr auto kTrapBytes = std::to_array<std::uint8_t>({
		    0x1E,
		    0x4C,
		    0x69,
		    0x3B,
		    0x26,
		    0x66,
		    0x66,
		    0x66,
		    0x66,
		    0x66,
		    0x2E,
		    0x0F,
		    0x1F,
		    0x84,
		    0x00,
		    0x00,
		});

		if (exception_info->ExceptionRecord->ExceptionCode != EXCEPTION_ILLEGAL_INSTRUCTION
		    || g_ExceptionContext.m_Values[0] != 1
		    || g_ExceptionContext.m_Values[2] != kCommonMainHash
		    || g_ExceptionContext.m_Values[3] != kCNetworkHash)
		{
			return false;
		}

		const auto gameBase = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
		if (!gameBase || exception_info->ContextRecord->Rip != gameBase + kTrapOffset)
			return false;

		std::array<std::uint8_t, kTrapBytes.size()> instructionBytes{};
		std::uintptr_t returnAddress = 0;
		SIZE_T instructionBytesRead = 0;
		SIZE_T returnAddressBytesRead = 0;
		if (!ReadProcessMemory(GetCurrentProcess(),
		        reinterpret_cast<const void*>(exception_info->ContextRecord->Rip),
		        instructionBytes.data(),
		        instructionBytes.size(),
		        &instructionBytesRead)
		    || instructionBytesRead != instructionBytes.size()
		    || instructionBytes != kTrapBytes)
		{
			return false;
		}

		if (!ReadProcessMemory(GetCurrentProcess(),
		        reinterpret_cast<const void*>(exception_info->ContextRecord->Rsp),
		        &returnAddress,
		        sizeof(returnAddress),
		        &returnAddressBytesRead)
		    || returnAddressBytesRead != sizeof(returnAddress))
		{
			return false;
		}
		if (returnAddress != gameBase + kReturnOffset)
		{
			LOGF(FATAL,
			    "Known b1158.13 CNetwork trap has unexpected stack return 0x{:X}; expected 0x{:X}, refusing recovery",
			    returnAddress,
			    gameBase + kReturnOffset);
			Logger::FlushQueue();
			return false;
		}

		static thread_local bool loggedRecovery = false;
		if (!loggedRecovery)
		{
			LOGF(WARNING,
			    "Recovered the known b1158.13 CNetwork trap at 0x{:X}; returning to 0x{:X}",
			    exception_info->ContextRecord->Rip,
			    returnAddress);
			Logger::FlushQueue();
			loggedRecovery = true;
		}

		exception_info->ContextRecord->Rip = returnAddress;
		exception_info->ContextRecord->Rsp += sizeof(returnAddress);
		exception_info->ContextRecord->Rax = 0;
		return true;
	}

	static void LogIllegalInstructionBytes(EXCEPTION_POINTERS* exception_info)
	{
		constexpr std::size_t kInstructionWindowSize = 16;
		std::array<std::uint8_t, kInstructionWindowSize> bytes{};
		SIZE_T bytesRead = 0;
		const auto instruction = reinterpret_cast<const void*>(exception_info->ContextRecord->Rip);

		if (!ReadProcessMemory(GetCurrentProcess(), instruction, bytes.data(), bytes.size(), &bytesRead) || bytesRead < 2)
		{
			LOGF(FATAL, "Unable to read instruction bytes at 0x{:X}", exception_info->ContextRecord->Rip);
			return;
		}

		LOGF(FATAL,
		    "Instruction bytes at 0x{:X}: {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} "
		    "{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X}",
		    exception_info->ContextRecord->Rip,
		    bytes[0],
		    bytes[1],
		    bytes[2],
		    bytes[3],
		    bytes[4],
		    bytes[5],
		    bytes[6],
		    bytes[7],
		    bytes[8],
		    bytes[9],
		    bytes[10],
		    bytes[11],
		    bytes[12],
		    bytes[13],
		    bytes[14],
		    bytes[15]);
	}

	inline auto HashStackTrace(std::vector<uint64_t> stack_trace)
	{
		auto data = reinterpret_cast<const char*>(stack_trace.data());
		std::size_t size = stack_trace.size() * sizeof(uint64_t);

		return std::hash<std::string_view>()({data, size});
	}

	ExceptionHandler::ExceptionHandler()
	{
		LOG(INFO) << "ExceptionHandler initialized";
		m_OldErrorMode = SetErrorMode(0);
		m_Handler = reinterpret_cast<void*>(SetUnhandledExceptionFilter(&VectoredExceptionHandler));
	}

	ExceptionHandler::~ExceptionHandler()
	{
		SetErrorMode(m_OldErrorMode);
		SetUnhandledExceptionFilter(reinterpret_cast<decltype(&VectoredExceptionHandler)>(m_Handler));
	}

	inline thread_local static StackTrace trace;
	LONG VectoredExceptionHandler(EXCEPTION_POINTERS* exception_info)
	{
		const auto exception_code = exception_info->ExceptionRecord->ExceptionCode;
		if (exception_code == EXCEPTION_BREAKPOINT || exception_code == DBG_PRINTEXCEPTION_C || exception_code == DBG_PRINTEXCEPTION_WIDE_C)
			return EXCEPTION_CONTINUE_SEARCH;
		if (TryRecoverKnownCNetworkTrap(exception_info))
			return EXCEPTION_CONTINUE_EXECUTION;

		static thread_local std::unordered_set<std::size_t> logged_exceptions;

		trace.NewStackTrace(exception_info);
		const auto trace_hash = HashStackTrace(trace.GetFramePointers());
		if (const auto it = logged_exceptions.find(trace_hash); it == logged_exceptions.end())
		{
			if (!g_ExceptionContext.m_Name.empty())
			{
				LOGF(FATAL,
				    "Active exception context [{}]: 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}",
				    g_ExceptionContext.m_Name,
				    g_ExceptionContext.m_Values[0],
				    g_ExceptionContext.m_Values[1],
				    g_ExceptionContext.m_Values[2],
				    g_ExceptionContext.m_Values[3],
				    g_ExceptionContext.m_Values[4]);
			}
			if (exception_code == EXCEPTION_ILLEGAL_INSTRUCTION)
				LogIllegalInstructionBytes(exception_info);
			LOG(FATAL) << trace;
			Logger::FlushQueue();

			logged_exceptions.insert(trace_hash);
		}

		if (exception_info->ExceptionRecord->ExceptionInformation[0] == EXCEPTION_EXECUTE_FAULT)
		{
			auto return_address_ptr = (uint64_t*)exception_info->ContextRecord->Rsp;
			if (IsBadReadPtr(reinterpret_cast<void*>(return_address_ptr), 8))
			{
				LOG(FATAL) << "Cannot resume execution, crashing (failed to find valid return address)";
				return EXCEPTION_CONTINUE_SEARCH;
			}
			else
			{
				LOG(FATAL) << "Force returning from function";
				exception_info->ContextRecord->Rip = *return_address_ptr;
				exception_info->ContextRecord->Rsp += 8;
			}
		}
		else
		{
			hde64s opcode{};
			hde64_disasm(reinterpret_cast<void*>(exception_info->ContextRecord->Rip), &opcode);
			if (opcode.flags & F_ERROR)
			{
				LOG(FATAL) << "Cannot resume execution, crashing (failed to decode insn)";
				return EXCEPTION_CONTINUE_SEARCH;
			}

			if (opcode.opcode == 0xFF && opcode.modrm_reg == 4) // JMP (FF /4)
			{
				auto return_address_ptr = (uint64_t*)exception_info->ContextRecord->Rsp;
				if (IsBadReadPtr(reinterpret_cast<void*>(return_address_ptr), 8))
				{
					LOG(FATAL) << "Cannot resume execution, crashing";
					return EXCEPTION_CONTINUE_SEARCH;
				}
				else
				{
					exception_info->ContextRecord->Rip = *return_address_ptr;
					exception_info->ContextRecord->Rsp += 8;
				}
			}
			else
			{
				if (opcode.opcode == 0x8B && opcode.modrm_mod != 3)
				{
					uint8_t regId = opcode.modrm_reg | (opcode.rex_r << 3);
					if (regId == 4)
						return EXCEPTION_CONTINUE_SEARCH;

					exception_info->ContextRecord->Rip += opcode.len;
					switch (regId)
					{
					case 0: exception_info->ContextRecord->Rax = 0; break;
					case 1: exception_info->ContextRecord->Rcx = 0; break;
					case 2: exception_info->ContextRecord->Rdx = 0; break;
					case 3: exception_info->ContextRecord->Rbx = 0; break;
					case 5: exception_info->ContextRecord->Rbp = 0; break;
					case 6: exception_info->ContextRecord->Rsi = 0; break;
					case 7: exception_info->ContextRecord->Rdi = 0; break;
					case 8: exception_info->ContextRecord->R8 = 0; break;
					case 9: exception_info->ContextRecord->R9 = 0; break;
					case 10: exception_info->ContextRecord->R10 = 0; break;
					case 11: exception_info->ContextRecord->R11 = 0; break;
					case 12: exception_info->ContextRecord->R12 = 0; break;
					case 13: exception_info->ContextRecord->R13 = 0; break;
					case 14: exception_info->ContextRecord->R14 = 0; break;
					case 15: exception_info->ContextRecord->R15 = 0; break;
					}
				}
				else
				{
					exception_info->ContextRecord->Rip += opcode.len;
				}
			}
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}
}

static YimMenu::ExceptionHandler _ExceptionHandler{};
