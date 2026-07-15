#include "Weapons.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "core/localization/Localization.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/data/Weapons.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Scripts.hpp"
#include "game/gta/ScriptFunction.hpp"
#include "types/script/scrThread.hpp"
#include "core/commands/Commands.hpp"
#include "game/features/self/CustomWeapon.hpp"

#include <string_view>
#include <unordered_map>
#include <utility>

namespace YimMenu::Submenus
{
	namespace
	{
		using namespace std::literals;

		bool ContainsCjk(std::string_view text)
		{
			for (unsigned char ch : text)
				if (ch & 0x80)
					return true;

			return false;
		}

		bool IsInvalidWeaponText(std::string_view text)
		{
			return text.empty() || text == "NULL"sv || text == "Invalid"sv;
		}

		const std::unordered_map<joaat_t, std::string_view> g_WeaponNameFallbacks = {
		    {"WEAPON_UNARMED"_J, "徒手"sv},
		    {"WEAPON_KNIFE"_J, "小刀"sv},
		    {"WEAPON_NIGHTSTICK"_J, "警棍"sv},
		    {"WEAPON_HAMMER"_J, "铁锤"sv},
		    {"WEAPON_BAT"_J, "棒球棍"sv},
		    {"WEAPON_GOLFCLUB"_J, "高尔夫球杆"sv},
		    {"WEAPON_CROWBAR"_J, "撬棍"sv},
		    {"WEAPON_PISTOL"_J, "手枪"sv},
		    {"WEAPON_COMBATPISTOL"_J, "战斗手枪"sv},
		    {"WEAPON_APPISTOL"_J, "穿甲手枪"sv},
		    {"WEAPON_PISTOL50"_J, ".50 手枪"sv},
		    {"WEAPON_MICROSMG"_J, "微型冲锋枪"sv},
		    {"WEAPON_SMG"_J, "冲锋枪"sv},
		    {"WEAPON_ASSAULTSMG"_J, "突击冲锋枪"sv},
		    {"WEAPON_ASSAULTRIFLE"_J, "突击步枪"sv},
		    {"WEAPON_CARBINERIFLE"_J, "卡宾步枪"sv},
		    {"WEAPON_ADVANCEDRIFLE"_J, "高级步枪"sv},
		    {"WEAPON_MG"_J, "机枪"sv},
		    {"WEAPON_COMBATMG"_J, "战斗机枪"sv},
		    {"WEAPON_PUMPSHOTGUN"_J, "泵动式霰弹枪"sv},
		    {"WEAPON_SAWNOFFSHOTGUN"_J, "短管霰弹枪"sv},
		    {"WEAPON_ASSAULTSHOTGUN"_J, "突击霰弹枪"sv},
		    {"WEAPON_BULLPUPSHOTGUN"_J, "无托式霰弹枪"sv},
		    {"WEAPON_STUNGUN"_J, "电击枪"sv},
		    {"WEAPON_SNIPERRIFLE"_J, "狙击步枪"sv},
		    {"WEAPON_HEAVYSNIPER"_J, "重型狙击步枪"sv},
		    {"WEAPON_REMOTESNIPER"_J, "遥控狙击枪"sv},
		    {"WEAPON_GRENADELAUNCHER"_J, "榴弹发射器"sv},
		    {"WEAPON_GRENADELAUNCHER_SMOKE"_J, "烟雾榴弹发射器"sv},
		    {"WEAPON_RPG"_J, "火箭推进榴弹发射器"sv},
		    {"WEAPON_MINIGUN"_J, "火神机枪"sv},
		    {"WEAPON_GRENADE"_J, "手榴弹"sv},
		    {"WEAPON_STICKYBOMB"_J, "黏弹"sv},
		    {"WEAPON_SMOKEGRENADE"_J, "催泪瓦斯"sv},
		    {"WEAPON_BZGAS"_J, "毒气弹"sv},
		    {"WEAPON_MOLOTOV"_J, "燃烧瓶"sv},
		    {"WEAPON_FIREEXTINGUISHER"_J, "灭火器"sv},
		    {"WEAPON_PETROLCAN"_J, "汽油桶"sv},
		    {"WEAPON_BALL"_J, "棒球"sv},
		    {"WEAPON_FLARE"_J, "信号棒"sv},
		    {"WEAPON_BOTTLE"_J, "破瓶"sv},
		    {"WEAPON_SNSPISTOL"_J, "SNS 手枪"sv},
		    {"WEAPON_HEAVYPISTOL"_J, "重型手枪"sv},
		    {"WEAPON_BULLPUPRIFLE"_J, "无托式步枪"sv},
		    {"WEAPON_SPECIALCARBINE"_J, "特制卡宾步枪"sv},
		    {"WEAPON_SNSPISTOL_MK2"_J, "SNS 手枪 Mk II"sv},
		    {"WEAPON_SPECIALCARBINE_MK2"_J, "特制卡宾步枪 Mk II"sv},
		    {"WEAPON_PUMPSHOTGUN_MK2"_J, "泵动式霰弹枪 Mk II"sv},
		    {"WEAPON_BULLPUPRIFLE_MK2"_J, "无托式步枪 Mk II"sv},
		    {"WEAPON_MARKSMANRIFLE_MK2"_J, "精准步枪 Mk II"sv},
		    {"WEAPON_CANDYCANE"_J, "糖果手杖"sv},
		    {"WEAPON_PISTOLXM3"_J, "XM3 手枪"sv},
		    {"WEAPON_RAILGUNXM3"_J, "XM3 电磁步枪"sv},
		    {"WEAPON_ACIDPACKAGE"_J, "迷幻药包"sv},
		    {"WEAPON_HOMINGLAUNCHER"_J, "追踪发射器"sv},
		    {"WEAPON_PROXMINE"_J, "感应地雷"sv},
		    {"WEAPON_SNOWBALL"_J, "雪球"sv},
		    {"WEAPON_DOUBLEACTION"_J, "双动式左轮手枪"sv},
		    {"WEAPON_REVOLVER_MK2"_J, "重型左轮手枪 Mk II"sv},
		    {"WEAPON_RAYPISTOL"_J, "原子能枪"sv},
		    {"WEAPON_RAYCARBINE"_J, "邪恶冥王"sv},
		    {"WEAPON_RAYMINIGUN"_J, "寡妇制造者"sv},
		    {"WEAPON_GUSENBERG"_J, "古森柏冲锋枪"sv},
		    {"WEAPON_DAGGER"_J, "匕首"sv},
		    {"WEAPON_VINTAGEPISTOL"_J, "古董手枪"sv},
		    {"WEAPON_FIREWORK"_J, "烟火发射器"sv},
		    {"WEAPON_MUSKET"_J, "火枪"sv},
		    {"WEAPON_HATCHET"_J, "短柄斧"sv},
		    {"WEAPON_RAILGUN"_J, "电磁步枪"sv},
		    {"WEAPON_MARKSMANRIFLE"_J, "精准步枪"sv},
		    {"WEAPON_HEAVYSHOTGUN"_J, "重型霰弹枪"sv},
		    {"WEAPON_CERAMICPISTOL"_J, "陶瓷手枪"sv},
		    {"WEAPON_MILITARYRIFLE"_J, "军用步枪"sv},
		    {"WEAPON_GADGETPISTOL"_J, "特工手枪"sv},
		    {"WEAPON_HAZARDCAN"_J, "危化桶"sv},
		    {"WEAPON_COMBATSHOTGUN"_J, "战斗霰弹枪"sv},
		    {"WEAPON_NAVYREVOLVER"_J, "海军左轮手枪"sv},
		    {"WEAPON_FLAREGUN"_J, "信号枪"sv},
		    {"WEAPON_KNUCKLE"_J, "指虎"sv},
		    {"WEAPON_COMBATPDW"_J, "战斗 PDW"sv},
		    {"WEAPON_MARKSMANPISTOL"_J, "神射手手枪"sv},
		    {"WEAPON_DBSHOTGUN"_J, "双管霰弹枪"sv},
		    {"WEAPON_COMPACTRIFLE"_J, "精简步枪"sv},
		    {"WEAPON_MACHINEPISTOL"_J, "冲锋手枪"sv},
		    {"WEAPON_MACHETE"_J, "砍刀"sv},
		    {"WEAPON_FLASHLIGHT"_J, "手电筒"sv},
		    {"WEAPON_SWITCHBLADE"_J, "弹簧刀"sv},
		    {"WEAPON_REVOLVER"_J, "重型左轮手枪"sv},
		    {"WEAPON_WRENCH"_J, "管钳"sv},
		    {"WEAPON_POOLCUE"_J, "台球杆"sv},
		    {"WEAPON_MINISMG"_J, "迷你冲锋枪"sv},
		    {"WEAPON_BATTLEAXE"_J, "战斧"sv},
		    {"WEAPON_AUTOSHOTGUN"_J, "自动霰弹枪"sv},
		    {"WEAPON_COMPACTLAUNCHER"_J, "精简榴弹发射器"sv},
		    {"WEAPON_PIPEBOMB"_J, "土制炸弹"sv},
		    {"WEAPON_SMG_MK2"_J, "冲锋枪 Mk II"sv},
		    {"WEAPON_COMBATMG_MK2"_J, "战斗机枪 Mk II"sv},
		    {"WEAPON_CARBINERIFLE_MK2"_J, "卡宾步枪 Mk II"sv},
		    {"WEAPON_ASSAULTRIFLE_MK2"_J, "突击步枪 Mk II"sv},
		    {"WEAPON_HEAVYSNIPER_MK2"_J, "重型狙击步枪 Mk II"sv},
		    {"WEAPON_PISTOL_MK2"_J, "手枪 Mk II"sv},
		    {"WEAPON_STONE_HATCHET"_J, "石斧"sv},
		    {"WEAPON_TACTICALRIFLE"_J, "战术步枪"sv},
		    {"WEAPON_PRECISIONRIFLE"_J, "精确步枪"sv},
		    {"WEAPON_HEAVYRIFLE"_J, "重型步枪"sv},
		    {"WEAPON_FERTILIZERCAN"_J, "肥料桶"sv},
		    {"WEAPON_EMPLAUNCHER"_J, "电磁脉冲发射器"sv},
		    {"WEAPON_STUNGUN_MP"_J, "电击枪（在线版）"sv},
		    {"WEAPON_TECPISTOL"_J, "战术冲锋手枪"sv},
		    {"WEAPON_SNOWLAUNCHER"_J, "雪球发射器"sv},
		    {"WEAPON_HACKINGDEVICE"_J, "黑客装置"sv},
		    {"WEAPON_BATTLERIFLE"_J, "战斗步枪"sv},
		    {"WEAPON_STUNROD"_J, "电击棒"sv},
		    {"WEAPON_STRICKLER"_J, "斯特里克勒"sv},
		    {"WEAPON_BRIEFCASE_03"_J, "公文包"sv},
		    {"WEAPON_NEWSPAPER"_J, "报纸"sv},
		};

		std::string LocalizeWeaponName(joaat_t weaponHash, std::string_view display)
		{
			if (!IsInvalidWeaponText(display) && ContainsCjk(display))
				return std::string(display);

			if (!IsInvalidWeaponText(display))
			{
				const auto translatedDisplay = Localization::Translate(display);
				if (translatedDisplay != display)
					return translatedDisplay;
			}

			if (const auto it = g_WeaponNameFallbacks.find(weaponHash); it != g_WeaponNameFallbacks.end())
				return std::string(it->second);

			if (IsInvalidWeaponText(display))
				return {};

			return std::string(display);
		}

		std::string LocalizeWeaponDescription(std::string_view display)
		{
			if (IsInvalidWeaponText(display))
				return {};

			if (ContainsCjk(display))
				return std::string(display);

			const auto translatedDisplay = Localization::Translate(display);
			if (translatedDisplay != display)
				return translatedDisplay;

			return {};
		}
	}

	struct WeaponDisplay
	{
		std::string name;
		std::string desc;
		joaat_t hash;
	};

	static void FetchWeaponStats(joaat_t weaponHash, int& kills, int& deaths, float& kd, int& headshots, int& accuracy)
	{
		uint64_t garbage[4]{};
		if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
		{
			if (auto thread = Scripts::FindScriptThreadByID(id))
			{
				thread->m_Context.m_State = rage::scrThread::State::PAUSED;

				static ScriptFunction getWeaponKills("mp_weapons"_J, ScriptPointer("GetWeaponKills", "5D ? ? ? 39 0F 38 00").Add(1).Rip());
				static ScriptFunction getWeaponDeaths("mp_weapons"_J, ScriptPointer("GetWeaponDeaths", "5D ? ? ? 39 10").Add(1).Rip());
				static ScriptFunction getWeaponKDRatio("mp_weapons"_J, ScriptPointer("GetWeaponKDRatio", "5D ? ? ? 39 12").Add(1).Rip());
				static ScriptFunction getWeaponHeadshots("mp_weapons"_J, ScriptPointer("GetWeaponHeadshots", "5D ? ? ? 39 11").Add(1).Rip());
				static ScriptFunction getWeaponAccuracy("mp_weapons"_J, ScriptPointer("GetWeaponAccuracy", "2D 01 09 00 00"));

				kills     = getWeaponKills.Call<int>(weaponHash, -1);
				deaths    = getWeaponDeaths.Call<int>(weaponHash, -1);
				kd        = getWeaponKDRatio.Call<float>(weaponHash, -1);
				headshots = getWeaponHeadshots.Call<int>(weaponHash, -1);
				accuracy  = static_cast<int>(getWeaponAccuracy.Call<float>(weaponHash));

				thread->Kill();
				thread->m_Context.m_State = rage::scrThread::State::KILLED;
			}
		}
	}

	static void RenderAmmuNationMenu()
	{
		static std::vector<WeaponDisplay> weaponDisplays;
		static std::string selectedWeapon{"请选择"};
		static joaat_t selectedWeaponHash{};
		static char searchWeapon[64];

		static int kills{};
		static int deaths{};
		static float kdRatio{};
		static int headshots{};
		static int accuracy{};

		static bool init = [] {
			FiberPool::Push([] {
				while (Scripts::IsScriptActive("startup"_J))
					ScriptMgr::Yield();

				uint64_t garbage[4]{};
				if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
				{
					if (auto thread = Scripts::FindScriptThreadByID(id))
					{
						thread->m_Context.m_State = rage::scrThread::State::PAUSED;

						for (const auto& weap : g_WeaponHashes)
						{
							static ScriptFunction getWeaponNameLabel("mp_weapons"_J, ScriptPointer("GetWeaponNameLabel", "2D 02 2B 00 00"));
							static ScriptFunction getWeaponDescLabel("mp_weapons"_J, ScriptPointer("GetWeaponDescLabel", "2D 02 A0 00 00"));

							std::string nameGxt = getWeaponNameLabel.Call<const char*>(weap, false); // second arg is for uppercase
							std::string descGxt = getWeaponDescLabel.Call<const char*>(weap, false);

							std::string nameDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(nameGxt.c_str());
							std::string descDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(descGxt.c_str());

							auto localizedName = LocalizeWeaponName(weap, nameDisplay);
							auto localizedDesc = LocalizeWeaponDescription(descDisplay);

							weaponDisplays.push_back({std::move(localizedName), std::move(localizedDesc), weap});
						}

						thread->Kill();
						thread->m_Context.m_State = rage::scrThread::State::KILLED;
					}
				}
			});
			return true;
		}();

		ImGui::BeginCombo("武器", selectedWeapon.c_str());
		if (ImGui::IsItemActive() && !ImGui::IsPopupOpen("##weaponspopup"))
		{
			ImGui::OpenPopup("##weaponspopup");
			memset(searchWeapon, 0, sizeof(searchWeapon));
		}
		if (ImGui::BeginPopup("##weaponspopup", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("%s", "搜索：");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(250.f);
			ImGui::InputText("##searchweapon", searchWeapon, sizeof(searchWeapon));

			std::string searchLower = searchWeapon;
			std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
			for (const auto& weap : weaponDisplays)
			{
				if (weap.name.empty())
					continue;

				std::string weaponLower = weap.name;
				std::transform(weaponLower.begin(), weaponLower.end(), weaponLower.begin(), ::tolower);

				if (weaponLower.find(searchLower) != std::string::npos)
				{
					ImGui::PushID(weap.hash);
					if (ImGui::Selectable(weap.name.c_str()))
					{
						FiberPool::Push([weap] {
							selectedWeapon = weap.name;
							selectedWeaponHash = weap.hash;
							FetchWeaponStats(selectedWeaponHash, kills, deaths, kdRatio, headshots, accuracy);
						});
					}
					ImGui::PopID();
					if (ImGui::IsItemHovered() && !weap.desc.empty())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35);
						ImGui::TextUnformatted(weap.desc.c_str());
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
				}
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("获得武器"))
		{
			FiberPool::Push([] {
				Self::GetPed().GiveWeapon(selectedWeaponHash, true);
			});
		}
		ImGui::SameLine();
		if (ImGui::Button("移除武器"))
		{
			FiberPool::Push([] {
				Self::GetPed().RemoveWeapon(selectedWeaponHash);
			});
		}

		if (*Pointers.IsSessionStarted && selectedWeaponHash != 0)
		{
			ImGui::Text("击杀数：%d", kills);
			ImGui::Text("死亡数：%d", deaths);
			ImGui::Text("击杀/死亡比：%.2f", kdRatio);
			ImGui::Text("爆头数：%d", headshots);
			ImGui::Text("命中率：%d%%", accuracy);
		}
	}

	static std::shared_ptr<Group> RenderCustomWeaponsMenu()
	{
		auto customWeaponsGroup = std::make_shared<Group>("自定义武器");

		auto cutomWeaponTypes = std::make_shared<Group>("", 1);
		auto customWeapons = std::make_shared<Group>("");
		auto paintGunGroup = std::make_shared<Group>("");

		auto cmd = Commands::GetCommand<ListCommand>("customweapontype"_J);
		
		auto isGravityGunEnabled = [cmd] {
			return static_cast<Features::CustomWeapons>(cmd->GetState()) == Features::CustomWeapons::GRAVITY_GUN;
		};

		auto isVehicleGunEnabled = [cmd] {
			return static_cast<Features::CustomWeapons>(cmd->GetState()) == Features::CustomWeapons::VEHICLE_GUN;
		};

		auto isPaintGunEnabled = [cmd] {
			return static_cast<Features::CustomWeapons>(cmd->GetState()) == Features::CustomWeapons::PAINT_GUN;
		};

		cutomWeaponTypes->AddItem(std::make_shared<ListCommandItem>("customweapontype"_J));
		cutomWeaponTypes->AddItem(std::make_shared<ConditionalItem>(isGravityGunEnabled, std::make_shared<BoolCommandItem>("gravitygunlaunchonrelease"_J)));
		cutomWeaponTypes->AddItem(std::make_shared<ConditionalItem>(isVehicleGunEnabled, std::make_shared<StringCommandItem>("vehiclegunmodel"_J)));
		cutomWeaponTypes->AddItem(std::make_shared<ConditionalItem>(isPaintGunEnabled, std::make_shared<ConditionalItem>("paintgunrainbowcolorenabled"_J, std::make_shared<ColorCommandItem>("paintguncolor"_J), true)));

		paintGunGroup->AddItem(std::make_shared<BoolCommandItem>("paintgunrainbowcolorenabled"_J));
		paintGunGroup->AddItem(std::make_shared<ConditionalItem>("paintgunrainbowcolorenabled"_J, std::make_shared<ListCommandItem>("paintgunrainbowcolorstyle"_J)));
		paintGunGroup->AddItem(std::make_shared<ConditionalItem>("paintgunrainbowcolorenabled"_J, std::make_shared<IntCommandItem>("paintgunrainbowcolorspeed"_J)));

		customWeapons->AddItem(std::make_shared<BoolCommandItem>("customweaponenabledonweaponout"_J));
		customWeapons->AddItem(std::move(cutomWeaponTypes));
		customWeapons->AddItem(std::make_shared<ConditionalItem>(isPaintGunEnabled, std::move(paintGunGroup)));

		customWeaponsGroup->AddItem(std::make_shared<BoolCommandItem>("customweapon"_J));
		customWeaponsGroup->AddItem(std::make_shared<ConditionalItem>("customweapon"_J, std::move(customWeapons)));

		return customWeaponsGroup;
	}

	std::shared_ptr<Category> BuildWeaponsMenu()
	{
		auto weapons = std::make_shared<Category>("武器");

		auto weaponsGlobalsGroup = std::make_shared<Group>("全局", 12);
		auto weaponsToolsGroup = std::make_shared<Group>("工具", 1);
		auto weaponsAmmuNationGroup = std::make_shared<Group>("武装国度");
		auto weaponsAimbotGroup = std::make_shared<Group>("自瞄", 1);

		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("infiniteammo"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("infiniteclip"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("rapidfire"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("infiniteparachutes"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("ExplosiveAmmo"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("ExplosiveAmmo"_J, std::make_shared<ListCommandItem>("selectedexplosion"_J)));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("ExplosiveAmmo"_J, std::make_shared<FloatCommandItem>("explosiondamage"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("ExplosiveAmmo"_J, std::make_shared<FloatCommandItem>("explosioncamerashake"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("weapondamage"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("weapondamage"_J, std::make_shared<FloatCommandItem>("weapondamagescale"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("meleedamage"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("meleedamage"_J, std::make_shared<FloatCommandItem>("meleedamagescale"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared<BoolCommandItem>("explosionradius"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared<ConditionalItem>("explosionradius"_J, std::make_shared<FloatCommandItem>("explosionradiusscale"_J, std::nullopt, false)));


		weaponsToolsGroup->AddItem(std::make_shared<CommandItem>("giveallweapons"_J));
		weaponsToolsGroup->AddItem(std::make_shared<CommandItem>("givemaxammo"_J));
		weaponsToolsGroup->AddItem(std::make_shared<CommandItem>("opengunlocker"_J));

		weaponsAmmuNationGroup->AddItem(std::make_shared<ImGuiItem>([] {
			RenderAmmuNationMenu();
		}));

		weaponsAimbotGroup->AddItem(std::make_shared<BoolCommandItem>("aimbot"_J));
		weaponsAimbotGroup->AddItem(std::make_shared<ConditionalItem>("aimbot"_J, std::make_shared<BoolCommandItem>("aimbotaimforhead"_J)));
		weaponsAimbotGroup->AddItem(std::make_shared<ConditionalItem>("aimbot"_J, std::make_shared<BoolCommandItem>("aimbottargetdrivers"_J)));
		weaponsAimbotGroup->AddItem(std::make_shared<ConditionalItem>("aimbot"_J, std::make_shared<BoolCommandItem>("aimbotreleasedeadped"_J)));

		weapons->AddItem(weaponsGlobalsGroup);
		weapons->AddItem(weaponsToolsGroup);
		weapons->AddItem(weaponsAmmuNationGroup);
		weapons->AddItem(weaponsAimbotGroup);
		weapons->AddItem(RenderCustomWeaponsMenu());
		return weapons;
	}
}
