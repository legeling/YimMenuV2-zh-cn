#include "CallHook.hpp"

namespace YimMenu
{
	CallHookMemory::CallHookMemory()
	{
		m_Memory = VirtualAlloc((void*)((uintptr_t)GetModuleHandle(0) + 0x40000000), 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!m_Memory)
		{
			LOGF(FATAL, "CallHookMemory: Failed to allocate jump sequence memory");
			throw std::runtime_error("Failed to allocate call hook memory within rel32 range");
		}
		m_Offset = 0;
	}

	void* CallHookMemory::AllocateJumpSequenceImpl(void* func)
	{
		m_Offset = m_Offset + ((16 - (m_Offset % 16)) % 16);

		*m_Memory.Add(m_Offset).As<int16_t*>() = 0xB848;
		*m_Memory.Add(m_Offset).Add(2).As<void**>() = func;
		*m_Memory.Add(m_Offset).Add(10).As<int16_t*>() = 0xE0FF;

		m_Offset += 12;

		return m_Memory.Add(m_Offset).Sub(12).As<void*>();
	}

	void CallHookMemory::DestroyImpl()
	{
		VirtualFree(m_Memory.As<void*>(), 0, MEM_RELEASE);
	}

	CallSiteHook::Hook::Hook(void* location, void* hook) :
	    m_Location(location),
	    m_Hook(hook),
	    m_Enabled(false)
	{
		auto seq = CallHookMemory::AllocateJumpSequence(hook);
		m_PatchedBytes[0] = 0xE8;
		*(int32_t*)&m_PatchedBytes[1] = (int32_t)((uint64_t)seq - (uint64_t)location - 5);
		memcpy(m_OriginalBytes, location, 5);
		m_OriginalFunction = PointerCalculator(location).Add(1).Rip().As<void*>();
	}

	void CallSiteHook::Hook::Enable()
	{
		if (!m_Enabled)
		{
			DWORD oldProtect{};
			if (!VirtualProtect(m_Location, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
			{
				LOGF(ERROR, "Failed to make call site writable: {}", GetLastError());
				return;
			}

			memcpy(m_Location, m_PatchedBytes, 5);
			m_Enabled = true;

			FlushInstructionCache(GetCurrentProcess(), m_Location, 5);
			DWORD currentProtect{};
			if (!VirtualProtect(m_Location, 5, oldProtect, &currentProtect))
				LOGF(ERROR, "Failed to restore call site protection: {}", GetLastError());
		}
	}

	void CallSiteHook::Hook::Disable()
	{
		if (m_Enabled)
		{
			DWORD oldProtect{};
			if (!VirtualProtect(m_Location, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
			{
				LOGF(ERROR, "Failed to make call site writable while disabling hook: {}", GetLastError());
				return;
			}

			memcpy(m_Location, m_OriginalBytes, 5);
			m_Enabled = false;

			FlushInstructionCache(GetCurrentProcess(), m_Location, 5);
			DWORD currentProtect{};
			if (!VirtualProtect(m_Location, 5, oldProtect, &currentProtect))
				LOGF(ERROR, "Failed to restore call site protection while disabling hook: {}", GetLastError());
		}
	}

	std::shared_ptr<CallSiteHook::Hook> CallSiteHook::AddHookImpl(void* location, void* hook)
	{
		auto hookPtr = std::make_shared<Hook>(location, hook);

		m_Hooks.push_back(hookPtr);
		return hookPtr;
	}

	void CallSiteHook::DestroyImpl()
	{
		const bool hasAllocatedJumpSequences = !m_Hooks.empty();

		for (auto& hook : m_Hooks)
		{
			if (hook)
				hook->Disable();
		}

		m_Hooks.clear();
		if (hasAllocatedJumpSequences)
			CallHookMemory::Destroy();
	}
}
