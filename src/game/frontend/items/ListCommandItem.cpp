#include "Items.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/ListCommand.hpp"
#include "core/localization/Localization.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle.hpp"

namespace YimMenu
{
	ListCommandItem::ListCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<ListCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void ListCommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("%s", Localization::Translate("Unknown list!").c_str());
			return;
		}

		int current_val = m_Command->GetState();
		auto& list = m_Command->GetList();
		std::string largestString;
		std::size_t largest_string_len = 0;

		if (!m_SelectedItem.has_value() || !m_ItemWidth.has_value())
		{
			for (auto& item : list)
			{
				if (item.first == current_val)
				{
					m_SelectedItem = Localization::Translate(item.second);
				}

				auto translatedItem = Localization::Translate(item.second);
				if (translatedItem.length() > largest_string_len)
				{
					largestString = std::move(translatedItem);
					largest_string_len = largestString.length();
				}
			}

			if (!m_SelectedItem.has_value())
				m_SelectedItem = "";

			auto size = ImGui::CalcTextSize(largestString.c_str());
			m_ItemWidth = size.x + 40.0f;
		}

		ImGui::SetNextItemWidth(m_ItemWidth.value());
		const auto label = Localization::TranslateLabel(m_LabelOverride.value_or(m_Command->GetLabel()));
		if (ImGui::BeginCombo(label.c_str(), m_SelectedItem.value().c_str()))
		{
			for (auto& el : list)
			{
				const auto translatedItem = Localization::Translate(el.second);
				if (ImGui::Selectable(translatedItem.c_str(), el.first == current_val))
				{
					current_val = el.first;
					m_Command->SetState(el.first);
				}

				if (el.first == current_val)
				{
					m_SelectedItem = translatedItem; // just in case
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}
}
