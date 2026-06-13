#include "core/commands/BoolCommand.hpp"
#include "core/commands/IntCommand.hpp"
#include "game/commands/PlayerCommand.hpp"
#include "types/script/ScriptEvent.hpp"
#include "types/script/globals/GSBD_FM_Events.hpp"

namespace YimMenu::Features
{
	static IntCommand _BountyAmount{"bountyamount", "赏金金额", "要设置的赏金金额", 1, 10000, 9000};
	static BoolCommand _AnonymousBounty{"anonymousbounty", "匿名赏金", "将赏金设置为匿名"};

	class SetBounty : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			SCRIPT_EVENT_BOUNTY bounty;
			bounty.Target = player.GetId();
			bounty.FromPed = _AnonymousBounty.GetState();
			bounty.Amount = _BountyAmount.GetState();
			bounty.BountyFlags = 1;
			bounty.AntiReplayValue1 = GSBD_FM_Events::Get()->AntiReplayValue1;
			bounty.AntiReplayValue2 = GSBD_FM_Events::Get()->AntiReplayValue2;
			bounty.SetAllPlayers();
			bounty.Send();
		}
	};

	static SetBounty _SetBounty{"setbounty", "设置赏金", "为玩家设置指定赏金"};
}
