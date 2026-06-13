#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Scripts.hpp"
#include "types/script/globals/GlobalPlayerBD.hpp"

namespace YimMenu::Features
{
	class OffTheRadar : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto gpbd = GlobalPlayerBD::Get(); gpbd && Scripts::SafeToModifyFreemodeBroadcastGlobals())
				gpbd->Entries[Self::GetPlayer().GetId()].OffRadarActive = true;
		}

		virtual void OnDisable() override
		{
			if (auto gpbd = GlobalPlayerBD::Get(); gpbd && Scripts::SafeToModifyFreemodeBroadcastGlobals())
				gpbd->Entries[Self::GetPlayer().GetId()].OffRadarActive = false;
		}
	};

	static OffTheRadar _OffTheRadar{"otr", "雷达隐身", "让你从其他玩家的雷达上消失"};
}
