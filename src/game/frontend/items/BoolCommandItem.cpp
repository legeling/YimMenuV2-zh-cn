#include "Items.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/LoopedCommand.hpp"
#include "core/localization/Localization.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle.hpp"

namespace YimMenu
{
	BoolCommandItem::BoolCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<BoolCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void BoolCommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("%s", Localization::Translate("Unknown!").c_str());
			return;
		}

		bool enabled = m_Command->GetState();
		const auto label = Localization::TranslateLabel(m_LabelOverride.has_value() ? m_LabelOverride.value() : m_Command->GetLabel());
		const auto description = Localization::Translate(m_Command->GetDescription());
		if (ImGui::Toggle(label.c_str(), &enabled))
			m_Command->SetState(enabled);

		// TODO: refactor this

		auto windowLabel = Localization::FormatHotkeyWindowTitle(m_Command->GetLabel());

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s", description.c_str());
			if (GetAsyncKeyState(VK_OEM_3) & 0x8000)
				ImGui::OpenPopup(windowLabel.data());
		}

		ImGui::SetNextWindowSize(ImVec2(500, 120));
		if (ImGui::BeginPopupModal(windowLabel.data(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::BulletText("%s", Localization::Translate("Hover over the command name to change its hotkey").c_str());
			ImGui::BulletText("%s", Localization::Translate("Press any registered key to remove").c_str());
			ImGui::Separator();


			ImGui::Spacing();
			if (ImGui::Button(Localization::Translate("Close").c_str()) || ((!ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}
}
