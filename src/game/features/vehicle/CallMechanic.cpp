#include "core/commands/Command.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/localization/Localization.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Scripts.hpp"
#include "game/gta/ScriptLocal.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/script/Timer.hpp"

namespace YimMenu::Features
{
	class CallMechanic : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted || Scripts::IsScriptActive("AM_CONTACT_REQUESTS"_J))
			{
				Notifications::Show("技工", "当前不适合呼叫技工。", NotificationType::Error);
				return;
			}

			if (auto freemode = Scripts::FindScriptThread("freemode"_J))
			{
				auto data = ScriptLocal(freemode, 8854);
				*data.At(3).As<int*>() = 235;
				*data.At(3).At(16).As<int*>() = Self::GetPlayer().GetId();
				*data.At(3).At(1).As<int*>() = 89;
				*data.At(2).As<int*>() = "AM_CONTACT_REQUESTS"_J;

				auto args = data.At(3).As<void*>();
				if (auto id = Scripts::StartScript("AM_CONTACT_REQUESTS"_J, eStackSizes::SCRIPT_XML, args, 21))
				{
					if (auto thread = Scripts::FindScriptThreadByID(id))
					{
						*ScriptLocal(thread, 535).As<int*>() = 1;
						ScriptGlobal(2686124).At(4373).At(260).At(7, 2).As<TIMER*>()->Destroy();
					}
				}
				else
				{
					Notifications::Show("技工", "呼叫技工失败。", NotificationType::Error);
				}
			}
		}
	};

	static CallMechanic _CallMechanic{"callmechanic", "呼叫技工", "打开技工的个人载具选择界面。"};
}
