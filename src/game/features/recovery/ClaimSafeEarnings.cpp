#include "core/commands/ListCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/script/globals/GPBD_FM.hpp"
#include "types/script/globals/GPBD_FM_2.hpp"

namespace YimMenu::Features
{
	enum class eAppVinewoodMenuSafe
	{
		NIGHTCLUB,
		ARCADE,
		AGENCY,
		SALVAGE_YARD,
		BAIL_OFFICE,
		GARMENT_FACTORY,
		HANDS_ON_CAR_WASH
	};

	static std::vector<std::pair<int, const char*>> businessNames = {
	    {static_cast<int>(eAppVinewoodMenuSafe::NIGHTCLUB), "夜总会"},
	    {static_cast<int>(eAppVinewoodMenuSafe::ARCADE), "游戏厅"},
	    {static_cast<int>(eAppVinewoodMenuSafe::AGENCY), "事务所"},
	    {static_cast<int>(eAppVinewoodMenuSafe::SALVAGE_YARD), "废车回收场"},
	    {static_cast<int>(eAppVinewoodMenuSafe::BAIL_OFFICE), "保释事务所"},
	    {static_cast<int>(eAppVinewoodMenuSafe::GARMENT_FACTORY), "制衣厂"},
	    {static_cast<int>(eAppVinewoodMenuSafe::HANDS_ON_CAR_WASH), "亲力亲为洗车行"}};

	static ListCommand _SelectedBusiness{"businesssafe", "产业", "要领取保险箱收益的产业", businessNames, 0};

	class ClaimSafeEarnings : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			switch (static_cast<eAppVinewoodMenuSafe>(_SelectedBusiness.GetState()))
			{
			case eAppVinewoodMenuSafe::NIGHTCLUB:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.NightclubData.SafeCashValue > 0)
					*ScriptGlobal(2708832).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::ARCADE:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.ArcadeData.SafeCashValue > 0)
					*ScriptGlobal(2708841).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::AGENCY:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.FixerHQData.SafeCashValue > 0)
					*ScriptGlobal(2708850).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::SALVAGE_YARD:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.SalvageYardData.TotalEarnings > 0)
					*ScriptGlobal(2708859).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::BAIL_OFFICE:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.BailShopData.SafeCashValue > 0)
					*ScriptGlobal(2708868).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::GARMENT_FACTORY:
			{
				if (GPBD_FM::Get()->Entries[Self::GetPlayer().GetId()].PropertyData.HackerDenData.SafeCashValue > 0)
					*ScriptGlobal(2708883).As<BOOL*>() = TRUE;
				break;
			}
			case eAppVinewoodMenuSafe::HANDS_ON_CAR_WASH:
			{
				if (GPBD_FM_2::Get()->Entries[Self::GetPlayer().GetId()].SYVehSaleData.HOWCData.SafeCashValue > 0)
					*ScriptGlobal(2708890).As<BOOL*>() = TRUE;
				break;
			}
			}
		}
	};

	static ClaimSafeEarnings _ClaimSafeEarnings{"claimsafeearnings", "领取保险箱收益", "从所选产业领取保险箱收益"};
}
