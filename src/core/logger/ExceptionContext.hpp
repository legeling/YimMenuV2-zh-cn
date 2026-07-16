#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace YimMenu
{
	struct ExceptionContext
	{
		std::string_view m_Name;
		std::array<std::uintptr_t, 5> m_Values;
	};

	inline thread_local ExceptionContext g_ExceptionContext{};

	class ScopedExceptionContext final
	{
		ExceptionContext m_Previous;

	public:
		explicit ScopedExceptionContext(ExceptionContext context) :
		    m_Previous(g_ExceptionContext)
		{
			g_ExceptionContext = context;
		}

		~ScopedExceptionContext()
		{
			g_ExceptionContext = m_Previous;
		}

		ScopedExceptionContext(const ScopedExceptionContext&) = delete;
		ScopedExceptionContext& operator=(const ScopedExceptionContext&) = delete;
	};
}
