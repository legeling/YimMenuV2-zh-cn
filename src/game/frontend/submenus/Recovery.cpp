#include "Recovery.hpp"
#include "Recovery/HeistModifier.hpp"
#include "Recovery/StatEditor.hpp"
#include "Recovery/Transactions.hpp"
#include "Recovery/DailyActivities.hpp"
#include "game/frontend/items/Items.hpp"

namespace YimMenu::Submenus
{
	Recovery::Recovery() :
		#define ICON_FA_SACK_DOLLAR "\xef\x93\x80"
	    Submenu::Submenu("账号进度", ICON_FA_SACK_DOLLAR)
	{
		auto main = std::make_shared<Category>("基础");
		auto businesses = std::make_shared<Category>("产业");
		auto shopping = std::make_shared<Category>("商店");
		auto contracts = std::make_shared<Category>("合约");
		auto casino = std::make_shared<Category>("赌场");

		auto generalGroup = std::make_shared<Group>("常规");
		auto businessGroup = std::make_shared<Group>("常规");
		auto gunVanGroup = std::make_shared<Group>("枪支厢型车", 3);
		auto contractGroup = std::make_shared<Group>("任务前置", 2);
		auto autoShopGroup = std::make_shared<Group>("改装铺合约", 3);
		auto casinoSlots = std::make_shared<Group>("老虎机");
		//auto casinoWheel = std::make_shared<Group>("幸运转盘");
		//auto casinoBlackJack = std::make_shared<Group>("二十一点");
		//auto casinoRoulette = std::make_shared<Group>("轮盘");

		generalGroup->AddItem(std::make_shared<BoolCommandItem>("playallmissionssolo"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("unlockgtaplus"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("overriderpmultiplier"_J));
		generalGroup->AddItem(std::make_shared<ConditionalItem>("overriderpmultiplier"_J, std::make_shared<FloatCommandItem>("rpmultiplierinput"_J)));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("freechangeappearance"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("nochangeappearancecooldown"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("allowgenderchange"_J));

		businessGroup->AddItem(std::make_shared<ListCommandItem>("businesssafe"_J));
		businessGroup->AddItem(std::make_shared<CommandItem>("claimsafeearnings"_J));
		businessGroup->AddItem(std::make_shared<CommandItem>("maxnightclubpopularity"_J));
		businessGroup->AddItem(std::make_shared<CommandItem>("clearmoneyfrontsheat"_J));

		gunVanGroup->AddItem(std::make_shared<ListCommandItem>("gunvanslot"_J));
		gunVanGroup->AddItem(std::make_shared<ListCommandItem>("gunvanweapon"_J));
		gunVanGroup->AddItem(std::make_shared<CommandItem>("setgunvanweapon"_J));

		contractGroup->AddItem(std::make_shared<CommandItem>("skipdrdrepreps"_J));
		contractGroup->AddItem(std::make_shared<CommandItem>("skipcluckinbellpreps"_J));
		contractGroup->AddItem(std::make_shared<CommandItem>("skiposcarguzmanpreps"_J));
		contractGroup->AddItem(std::make_shared<CommandItem>("skipknowayoutpreps"_J));

		autoShopGroup->AddItem(std::make_shared<ListCommandItem>("autoshopcontract"_J));
		autoShopGroup->AddItem(std::make_shared<CommandItem>("skipautoshoppreps"_J));

		casinoSlots->AddItem(std::make_shared<BoolCommandItem>("casinomanipulaterigslotmachines"_J));
		

		main->AddItem(generalGroup);
		businesses->AddItem(businessGroup);
		shopping->AddItem(gunVanGroup);
		contracts->AddItem(contractGroup);
		contracts->AddItem(autoShopGroup);
		casino->AddItem(casinoSlots);

		AddCategory(std::move(main));
		AddCategory(std::move(businesses));
		AddCategory(std::move(shopping));
		AddCategory(std::move(contracts));
		AddCategory(std::move(casino));
		AddCategory(BuildStatEditorMenu());
		AddCategory(BuildTransactionsMenu());
		AddCategory(BuildHeistModifierMenu());
		AddCategory(BuildDailyActivitiesMenu());
	}
}
