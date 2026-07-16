#include "game/hooks/Hooks.hpp"
#include "core/logger/ExceptionContext.hpp"
#include "core/util/Joaat.hpp"
#include "types/rage/gameSkeleton.hpp"

#include <mutex>
#include <unordered_set>

namespace YimMenu::Hooks
{
	static bool IsBlockedUpdateNode(std::uint32_t hash)
	{
		return hash == 0xA0F39FB6
		    || hash == "TamperActions"_J
		    || hash == "rageSecEngine"_J
		    || hash == "rageSecGamePluginManager"_J
		    || hash == "fwClothMeshing"_J
		    || hash == "CCreditsText"_J;
	}

	static void LogBlockedUpdateNode(std::uint32_t hash)
	{
		static std::mutex mutex;
		static std::unordered_set<std::uint32_t> loggedHashes;
		std::lock_guard lock(mutex);
		if (loggedHashes.emplace(hash).second)
			LOGF(VERBOSE, "GameSkeletonUpdate: blocked update node 0x{:08X}", hash);
	}

	static void RunUpdateNode(rage::gameSkeletonUpdateBase* node, int type, std::uint32_t groupHash)
	{
		auto vtable = *reinterpret_cast<void***>(node);
		auto run = vtable ? vtable[1] : nullptr;
		ScopedExceptionContext context({
		    "GameSkeletonUpdate(type,node,group_hash,node_hash,run)",
		    {
		        static_cast<std::uintptr_t>(type),
		        reinterpret_cast<std::uintptr_t>(node),
		        static_cast<std::uintptr_t>(groupHash),
		        static_cast<std::uintptr_t>(node->m_Hash),
		        reinterpret_cast<std::uintptr_t>(run),
		    },
		});
		node->Run();
	}

	void Anticheat::GameSkeletonUpdate(rage::gameSkeleton* skeleton, int type)
	{
		for (auto mode = skeleton->m_UpdateModes; mode; mode = mode->m_Next)
		{
			if (mode->m_Type != type)
				continue;

			for (auto group = mode->m_Head; group; group = group->m_Next)
			{
				if (IsBlockedUpdateNode(group->m_Hash))
				{
					LogBlockedUpdateNode(group->m_Hash);
					continue;
				}

				if (group->m_Hash != "Common Main"_J)
				{
					RunUpdateNode(group, type, group->m_Hash);
					continue;
				}

				for (auto item = static_cast<rage::gameSkeletonUpdateGroup*>(group)->m_Head; item; item = item->m_Next)
				{
					if (IsBlockedUpdateNode(item->m_Hash))
					{
						LogBlockedUpdateNode(item->m_Hash);
						continue;
					}

					RunUpdateNode(item, type, group->m_Hash);
				}
			}

			break;
		}
	}
}
