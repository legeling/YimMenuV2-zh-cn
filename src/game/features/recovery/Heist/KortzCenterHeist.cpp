#include "core/commands/Command.hpp"
#include "core/commands/BoolCommand.hpp"
#include "core/commands/ListCommand.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	namespace KortzCenterHeist
	{
		// --- Primary Target ---
		static std::vector<std::pair<int, const char*>> kortzCenterTargets = {
		    {0, "La Dernière Débauche"},
		    {1, "Hare Oneself Think"},
		    {2, "The Downfall of Rome"},
		    {3, "Brother Brother"},
		    {4, "A Cast of Characters"},
		    {5, "Gone To Seed"},
		    {6, "True Love"},
		    {7, "Breathless"},
		    {8, "Consumato"},
		    {9, "I Hear Voices"},
		    {10, "Winter, Nowhere in Particular"},
		    {11, "The Girl With the Pearl Necklace"},
		    {12, "Chat on Fruit"},
		    {13, "Pumpkin"},
		    {14, "Twindifference"},
		    {15, "Stacks Study V"},
		    {16, "I, Fruit"},
		    {17, "To Beat About the Bush"},
		    {18, "In Excess of Success"},
		    {19, "Juiced"},
		    {20, "A Winding Road Home"},
		    {21, "Teckels"},
		    {22, "Trust"},
		    {23, "Until Death"},
		    {24, "What Are Melons?"},
		    {25, "The Outcome of Endeavour"},
		    {26, "Mi O Melee"}
		};
		static ListCommand _KortzCenterPrimaryTarget{"kortzcenterheistprimarytarget", "主要目标", "选择科兹中心豪劫的主要目标艺术品。", kortzCenterTargets, 0};

		// --- General Purchases (MPX_K26_GENERAL_BS bits 5-8) ---
		static BoolCommand _KortzCenterGuardRoutes{"kortzcenterheistguardroutes", "保安巡逻路线", "将保安巡逻路线标记为已购买。", true};
		static BoolCommand _KortzCenterGlassCutter{"kortzcenterheistglasscutter", "玻璃切割器", "将玻璃切割器标记为已购买。", true};
		static BoolCommand _KortzCenterPowerDrills{"kortzcenterheistpowerdrills", "电钻", "将电钻标记为已购买。", true};
		static BoolCommand _KortzCenterEMPCharges{"kortzcenterheistempcharges", "EMP 炸药", "将 EMP 炸药标记为已购买。", true};

		// --- Prep Work (MPX_K26_ROBBERY_PROG bits 0-15) ---
		static BoolCommand _KortzCenterScopeOut{"kortzcenterheistscopeout", "前期侦察", "将科兹中心侦察任务标记为完成。", true};
		static BoolCommand _KortzCenterAlphaMail{"kortzcenterheistalphamail", "Alpha Mail 伪装", "将 Alpha Mail 伪装准备标记为完成。", true};
		static BoolCommand _KortzCenterHazmat{"kortzcenterheisthazmat", "防化服", "将防化服准备标记为完成。", true};
		static BoolCommand _KortzCenterStaffKeycard{"kortzcenterheiststaffkeycard", "工作人员钥匙卡", "将工作人员钥匙卡准备标记为完成。", true};
		static BoolCommand _KortzCenterTacticalEquip{"kortzcenterheisttacticalequip", "战术装备", "将战术装备准备标记为完成。", true};
		static BoolCommand _KortzCenterHackingDevice{"kortzcenterheisthackingdevice", "入侵设备", "将入侵设备准备标记为完成。", true};
		static BoolCommand _KortzCenterAccessCode{"kortzcenterheistaccesscode", "访问代码", "将访问代码准备标记为完成。", true};
		static BoolCommand _KortzCenterUnmarkedWeapons{"kortzcenterheistunmarkedweapons", "无标记武器", "将无标记武器准备标记为完成。", true};
		static BoolCommand _KortzCenterCaracara{"kortzcenterheistcaracara", "装甲卡拉卡拉", "将装甲卡拉卡拉准备标记为完成。", true};
		static BoolCommand _KortzCenterAnnihilator{"kortzcenterheistannihilator", "歼灭者隐形版", "将歼灭者隐形版准备标记为完成。", true};
		static BoolCommand _KortzCenterManchez{"kortzcenterheistmanchez", "曼切兹", "将曼切兹准备标记为完成。", true};
		static BoolCommand _KortzCenterPrepEMP{"kortzcenterheistprepemp", "EMP 炸药（准备）", "将 EMP 炸药准备任务标记为完成。", true};
		static BoolCommand _KortzCenterGuardShipments{"kortzcenterheistguardshipments", "保安装备货物", "将保安装备货物准备标记为完成。", true};
		static BoolCommand _KortzCenterGuardRoutesPrep{"kortzcenterheistguardroutesprep", "保安巡逻路线（准备）", "将保安巡逻路线准备标记为完成。", true};
		static BoolCommand _KortzCenterGlassCutterPrep{"kortzcenterheistglasscutterprep", "玻璃切割器（准备）", "将玻璃切割器准备标记为完成。", true};
		static BoolCommand _KortzCenterPowerDrillsPrep{"kortzcenterheistpowerdrillsprep", "电钻（准备）", "将电钻准备标记为完成。", true};

		// --- Scoping ---
		static BoolCommand _KortzCenterScopeSecondary{"kortzcenterheistscopesecondary", "次要目标", "将全部次要目标标记为已侦察。", true};
		static BoolCommand _KortzCenterScopePOI{"kortzcenterheistscopepoi", "兴趣点", "将全部兴趣点标记为已侦察。", true};

		// --- Setup ---
		class Setup : public Command
		{
			using Command::Command;

			virtual void OnCall() override
			{
				// GENERAL_BS: start at -1 (all bits set), clear unchecked bits
				int generalBits = -1;
				if (!_KortzCenterGuardRoutes.GetState())   generalBits &= ~(1 << 5);
				if (!_KortzCenterGlassCutter.GetState())   generalBits &= ~(1 << 6);
				if (!_KortzCenterPowerDrills.GetState())   generalBits &= ~(1 << 7);
				if (!_KortzCenterEMPCharges.GetState())    generalBits &= ~(1 << 8);

				// ROBBERY_PROG: start at -1, clear unchecked bits
				int robberyProg = -1;
				if (!_KortzCenterScopeOut.GetState())        robberyProg &= ~(1 << 0);
				if (!_KortzCenterAlphaMail.GetState())       robberyProg &= ~(1 << 1);
				if (!_KortzCenterHazmat.GetState())          robberyProg &= ~(1 << 2);
				if (!_KortzCenterStaffKeycard.GetState())    robberyProg &= ~(1 << 3);
				if (!_KortzCenterTacticalEquip.GetState())   robberyProg &= ~(1 << 4);
				if (!_KortzCenterHackingDevice.GetState())   robberyProg &= ~(1 << 5);
				if (!_KortzCenterAccessCode.GetState())      robberyProg &= ~(1 << 6);
				if (!_KortzCenterUnmarkedWeapons.GetState()) robberyProg &= ~(1 << 7);
				if (!_KortzCenterCaracara.GetState())        robberyProg &= ~(1 << 8);
				if (!_KortzCenterAnnihilator.GetState())     robberyProg &= ~(1 << 9);
				if (!_KortzCenterManchez.GetState())         robberyProg &= ~(1 << 10);
				if (!_KortzCenterPrepEMP.GetState())         robberyProg &= ~(1 << 11);
				if (!_KortzCenterGuardShipments.GetState())  robberyProg &= ~(1 << 12);
				if (!_KortzCenterGuardRoutesPrep.GetState()) robberyProg &= ~(1 << 13);
				if (!_KortzCenterGlassCutterPrep.GetState()) robberyProg &= ~(1 << 14);
				if (!_KortzCenterPowerDrillsPrep.GetState()) robberyProg &= ~(1 << 15);

				// Scoping
				int scopingBS = _KortzCenterScopeSecondary.GetState() ? -1 : 0;
				int poiBS     = _KortzCenterScopePOI.GetState() ? -1 : 0;

				// Write all stats
				Stats::SetInt("MPX_K26_GENERAL_BS", generalBits);
				Stats::SetInt("MPX_K26_GENERAL_BS2", -1);
				Stats::SetInt("MPX_K26_ROBBERY_PROG", robberyProg);
				Stats::SetInt("MPX_K26_HEIST_TARGET", _KortzCenterPrimaryTarget.GetState());
				Stats::SetInt("MPX_K26_SCOPING_BS", scopingBS);
				Stats::SetInt("MPX_K26_POI_BS", poiBS);
			}
		};

		static Setup _KortzCenterSetup{"kortzcenterheistsetup", "应用设置", "写入所选目标、采购、准备任务和侦察状态。"};
	}
}
