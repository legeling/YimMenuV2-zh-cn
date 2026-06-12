#include "SavedPlayers.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/localization/Localization.hpp"
#include "core/frontend/widgets/imgui_colors.h"
#include "core/frontend/Notifications.hpp"
#include "game/backend/SavedPlayers.hpp"
#include "game/gta/Network.hpp"
#include "game/pointers/Pointers.hpp"

namespace YimMenu::Submenus
{
	static std::uint64_t g_SelectedRid = 0;
	static SavedPlayerData* g_SelectedPlayer = nullptr;
	static char g_SelectedPlayerName[24]{};
	static char g_NameToSearch[24]{};

	static ImColor GetStatusCircleColor(SavedPlayerData* data)
	{
		if (!data->m_FetchedData)
			return ImGui::Colors::Gray;
		else if (data->m_FetchedData->m_GameState == FetchedPlayerData::GameState::INVALID)
			return ImGui::Colors::Red;
		else if (data->m_FetchedData->m_GameState == FetchedPlayerData::GameState::PUBLIC)
			return ImGui::Colors::Green;
		else
			return ImGui::Colors::Yellow;
	}

	static bool ShouldRenderPlayer(std::string_view name, std::string_view search)
	{
		if (search.empty())
			return true;

		if (name.size() < search.size())
			return false;

		// TODO: this doesn't do what YimMenuV1 did (which is to lowercase both inputs and perform a substring search)
		for (int i = 0; i < search.size(); i++)
			if (tolower(name[i]) != tolower(search[i]))
				return false;

		return true;
	}

	static void RenderPlayerItem(std::uint64_t rid, SavedPlayerData* data)
	{
		ImGui::PushID(rid);

		constexpr float circle_size = 7.5f;
		auto cursor_pos = ImGui::GetCursorScreenPos();

		// render status circle
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(cursor_pos.x + 4.f + circle_size, cursor_pos.y + 4.f + circle_size), circle_size, GetStatusCircleColor(data));

		// we need some padding
		ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursor.x + 25.f, cursor.y));

		if (ImGui::Selectable(data->m_Name.data(), rid == g_SelectedRid))
		{
			g_SelectedRid = rid;
			g_SelectedPlayer = data;
			strncpy(g_SelectedPlayerName, data->m_Name.data(), sizeof(g_SelectedPlayerName));
		}

		if (data->m_FetchedData && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", FetchedPlayerData::GameStateToString(data->m_FetchedData->m_GameState).data());

		ImGui::PopID();
	}

	static void RenderPlayerList()
	{
		ImGui::SetNextItemWidth(200.f);
		ImGui::InputTextWithHint(Localization::Translate("Search").c_str(), Localization::Translate("Search").c_str(), g_NameToSearch, sizeof(g_NameToSearch));

		if (ImGui::BeginListBox("###player-list", {180, -100 /* static_cast<float>(*Pointers.ScreenResY - 700 - 38 * 4) */}))
		{
			auto& players = SavedPlayers::GetSavedPlayers();

			if (players.size() == 0)
			{
				ImGui::TextDisabled("%s", Localization::Translate("No saved players").c_str());
				ImGui::EndListBox();
				return;
			}

			// TODO: surely there must be a better way to do this

			for (auto& [rid, data] : players)
				if (data.m_FetchedData && data.m_FetchedData->m_GameState == FetchedPlayerData::GameState::PUBLIC && ShouldRenderPlayer(data.m_Name, g_NameToSearch))
					RenderPlayerItem(rid, &data);

			for (auto& [rid, data] : players)
				if (data.m_FetchedData && data.m_FetchedData->m_GameState != FetchedPlayerData::GameState::PUBLIC
				    && data.m_FetchedData->m_GameState != FetchedPlayerData::GameState::INVALID && ShouldRenderPlayer(data.m_Name, g_NameToSearch))
					RenderPlayerItem(rid, &data);

			for (auto& [rid, data] : players)
				if ((!data.m_FetchedData || (data.m_FetchedData->m_GameState == FetchedPlayerData::GameState::INVALID)) && ShouldRenderPlayer(data.m_Name, g_NameToSearch))
					RenderPlayerItem(rid, &data);


			ImGui::EndListBox();
		}
	}

	static void RenderPlayerEditor()
	{
		if (!g_SelectedPlayer || g_SelectedRid == 0)
			return;

		if (ImGui::BeginChild("##player-editor", {500, -100 /* static_cast<float>(*Pointers.ScreenResY - 688 - 38 * 4) */}, 0, ImGuiWindowFlags_NoBackground))
		{
			ImGui::SetNextItemWidth(180.f);
			if (ImGui::InputText(Localization::Translate("Name").c_str(), g_SelectedPlayerName, sizeof(g_SelectedPlayerName)))
			{
				g_SelectedPlayer->m_Name = g_SelectedPlayerName;
			}

			int old_rid = g_SelectedRid;
			ImGui::SetNextItemWidth(180.0f);
			if (ImGui::InputScalar("Rockstar ID", ImGuiDataType_U64, &g_SelectedRid))
			{
				SavedPlayers::UpdateRockstarId(old_rid, g_SelectedRid);
				g_SelectedPlayer = SavedPlayers::GetPlayerData(g_SelectedRid);
			}

			ImGui::Checkbox(Localization::Translate("Track Player").c_str(), &g_SelectedPlayer->m_TrackPlayer);

			if (g_SelectedPlayer->m_FetchedData)
			{
				auto& data = *g_SelectedPlayer->m_FetchedData;
				const auto yes = Localization::Translate("Yes");
				const auto no = Localization::Translate("No");
				const auto gameState = Localization::Translate(FetchedPlayerData::GameStateToString(data.m_GameState));
				ImGui::Text(Localization::Translate("Session Type: %s").c_str(), gameState.c_str());
				ImGui::Text(Localization::Translate("Host of Session: %s").c_str(), data.m_HostOfSession ? yes.c_str() : no.c_str());
				ImGui::Text(Localization::Translate("Is Spectating: %s").c_str(), data.m_Spectating ? yes.c_str() : no.c_str());
				ImGui::Text(Localization::Translate("Is Job Lobby: %s").c_str(), data.m_InTransition ? yes.c_str() : no.c_str());
				ImGui::Text(Localization::Translate("Host of Job Lobby: %s").c_str(), data.m_HostOfTransition ? yes.c_str() : no.c_str());
				if (data.m_MissionType != FetchedPlayerData::MissionType::NONE)
				{
					const auto missionType = Localization::Translate(FetchedPlayerData::MissionTypeToString(data.m_MissionType));
					ImGui::Text(Localization::Translate("Mission Type: %s").c_str(), missionType.c_str());
					if (data.m_MissionName)
						ImGui::Text(Localization::Translate("Mission Name: %s").c_str(), data.m_MissionName->data());
					else
						; // TODO: add fetch mission name
				}
			}
			else
			{
				ImGui::TextDisabled("%s", Localization::Translate("Data has not been fetched yet").c_str());
			}

			if (ImGui::Button(Localization::Translate("Join").c_str()))
			{
				FiberPool::Push([] {
					Network::JoinRockstarId(g_SelectedRid);
				});
			}

			if (ImGui::Button(Localization::Translate("Save").c_str()))
			{
				SavedPlayers::Save();
			}
			ImGui::SameLine();
			if (ImGui::Button(Localization::Translate("Remove").c_str()))
			{
				SavedPlayers::RemovePlayerData(g_SelectedRid);
				g_SelectedPlayer = nullptr;
				g_SelectedRid = 0;
			}
		}
		ImGui::EndChild();
	}

	static void RenderSavedPlayers()
	{
		RenderPlayerList();
		ImGui::SameLine();
		RenderPlayerEditor();
	}

	static void RenderAddNewPlayer()
	{
		static char name_buf[24]{};

		ImGui::SetNextItemWidth(180.0f);
		ImGui::InputText("用户名", name_buf, sizeof(name_buf));
		ImGui::SameLine();
		if (ImGui::Button(Localization::Translate("Add").c_str()))
			FiberPool::Push([] {
				auto rid = YimMenu::Network::ResolveRockstarId(name_buf);
				if (rid)
				{
					SavedPlayers::AddPlayerData(*rid, name_buf);
				}
				else
				{
					Notifications::Show(Localization::Translate("Saved Players"), Localization::Translate("Failed to get RID from username"), NotificationType::Error);
				}
			});
	}

	std::shared_ptr<Category> BuildSavedPlayersMenu()
	{
		auto menu = std::make_shared<Category>("Saved Players");
		auto players = std::make_shared<Group>("Players");
		auto new_player = std::make_shared<Group>("New");
		auto tracking = std::make_shared<Group>("Tracking");
		auto notifications = std::make_shared<Group>("Notifications");

		players->AddItem(std::make_shared<ImGuiItem>([] {
			RenderSavedPlayers();
		}));

		new_player->AddItem(std::make_shared<ImGuiItem>([] {
			RenderAddNewPlayer();
		}));

		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifywhenjoinable"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifywhenunjoinable"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifywhenonline"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifywhenoffline"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifyonseschange"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifyonmischange"_J));
		notifications->AddItem(std::make_shared<BoolCommandItem>("playerdbnotifyonjoblobby"_J));

		auto update = std::make_shared<Group>("", 1);
		update->AddItem(std::make_shared<BoolCommandItem>("playerdbautoupdate"_J, "Auto Update"));
		update->AddItem(std::make_shared<CommandItem>("playerdbupdatenow"_J, "Update Now"));

		tracking->AddItem(std::move(update));
		tracking->AddItem(std::make_shared<BoolCommandItem>("playerdbnotify"_J, "Tracking Notifications"));
		tracking->AddItem(std::make_shared<ConditionalItem>("playerdbnotify"_J, std::move(notifications)));

		menu->AddItem(players);
		menu->AddItem(new_player);
		menu->AddItem(tracking);

		return menu;
	}
}
