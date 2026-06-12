#include "Items.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/HotkeySystem.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/localization/Localization.hpp"
#include "DrawHotkey.hpp"

namespace YimMenu
{
	CommandItem::CommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<Command>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void CommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("%s", Localization::Translate("Unknown!").c_str());
			return;
		}

		const auto label = Localization::TranslateLabel(m_LabelOverride.has_value() ? m_LabelOverride.value() : m_Command->GetLabel());
		const auto description = Localization::Translate(m_Command->GetDescription());
		if (ImGui::Button(label.c_str()))
		{
			FiberPool::Push([this] {
				m_Command->Call();
			});
		}

		// TODO: refactor this

		auto windowLabel = Localization::FormatHotkeyWindowTitle(m_Command->GetLabel());

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s", description.c_str());
			if (GetAsyncKeyState(VK_CAPITAL) & 0x8000)
				ImGui::OpenPopup(windowLabel.data());
		}

		ImGui::SetNextWindowSize(ImVec2(500, 120));
		if (ImGui::BeginPopupModal(windowLabel.data(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::BulletText("%s", Localization::Translate("Enter a keystroke").c_str());

			ImGui::Separator();

			HotkeySystem::SetBeingModifed(true);

			if (auto it = g_HotkeySystem.m_CommandHotkeys.find(m_Command->GetHash()); it != g_HotkeySystem.m_CommandHotkeys.end())
				DrawHotkey(&it->second, label);

			ImGui::Spacing();
			if (ImGui::Button(Localization::Translate("Close").c_str()) || ((!ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
			{
				HotkeySystem::SetBeingModifed(false);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
