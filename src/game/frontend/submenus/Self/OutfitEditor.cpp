#include "OutfitEditor.hpp"

#include "core/frontend/manager/UIManager.hpp"
#include "core/localization/Localization.hpp"
#include "game/backend/Self.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/gta/Natives.hpp"

#include <random>

// TODO: clean up more AI generated junk from this file

namespace YimMenu
{
	int GetMaxDrawable(int slot)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot);
	}

	int GetMaxTexture(int slot, int drawable)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot, drawable);
	}

	int GetMaxPropDrawable(int slot)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot);
	}

	int GetMaxPropTexture(int slot, int drawable)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot, drawable);
	}

	void GetOutfitSlot(int slot, int& drawable, int& texture)
	{
		auto ped = Self::GetPed();
		drawable = PED::GET_PED_DRAWABLE_VARIATION(static_cast<int>(ped.GetHandle()), slot);
		texture = PED::GET_PED_TEXTURE_VARIATION(static_cast<int>(ped.GetHandle()), slot);
	}

	void GetPropSlot(int slot, int& drawable, int& texture)
	{
		auto ped = Self::GetPed();
		drawable = PED::GET_PED_PROP_INDEX(static_cast<int>(ped.GetHandle()), slot, 0);
		if (drawable == -1)
		{
			drawable = 0;
			texture = 0;
			return;
		}
		texture = PED::GET_PED_PROP_TEXTURE_INDEX(static_cast<int>(ped.GetHandle()), slot);
	}

	void SetOutfitSlot(int slot, int drawable, int texture)
	{
		auto ped = Self::GetPed();
		PED::SET_PED_COMPONENT_VARIATION(static_cast<int>(ped.GetHandle()), slot, drawable, texture, 0);
	}

	void SetPropSlot(int slot, int drawable, int texture)
	{
		auto ped = Self::GetPed();
		PED::SET_PED_PROP_INDEX(static_cast<int>(ped.GetHandle()), slot, drawable, texture, true, 0);
	}

	// Helper function for underlined text
	static void TextUnderlined(const char* text)
	{
		ImGui::Text("%s", text);
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]));
	}

	static void TextUnderlinedAt(const char* text, float y)
	{
		auto old_cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPosY(y);
		TextUnderlined(text);
		ImGui::SetCursorPos(old_cursor);
	}

	std::shared_ptr<Category> CreateOutfitsMenu()
	{
		auto category = std::make_shared<Category>("服装编辑器");

		category->AddItem(std::make_shared<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("%s", Localization::Translate("Natives not cached yet").c_str());

			auto ped = Self::GetPed();

			if (!ped)
				return ImGui::TextDisabled("%s", Localization::Translate("Player ped not found").c_str());

			// Create two columns layout
			const float windowWidth = ImGui::GetContentRegionAvail().x;
			const float columnWidth = windowWidth * 0.5f;
			const float inputWidth = 120.0f; // Minimal width for number input

			ImGui::Columns(2, "OutfitColumns", false);
			ImGui::SetColumnWidth(0, columnWidth);

			// Components section (Left column)

			float header_y = ImGui::GetCursorPosY();

			TextUnderlined(Localization::Translate("Components").c_str());
			const struct
			{
				const char* name;
				int slot;
			} componentSlots[] = {{"上衣", 11}, {"内搭", 8}, {"裤装", 4}, {"鞋子", 6}, {"配饰", 7}, {"包袋", 5}, {"面具", 1}, {"手套", 3}, {"贴花", 10}, {"护甲", 9}};

			bool first_iter = true;
			for (const auto& component : componentSlots)
			{
				ImGui::PushID(component.slot);

				int drawable, texture;
				GetOutfitSlot(component.slot, drawable, texture);

				const auto componentName = Localization::Translate(component.name);
				ImGui::Text("%s", componentName.c_str());
				ImGui::SameLine();

				ImGui::SetCursorPosX(columnWidth - inputWidth * 2 - 10);

				ImGui::PushItemWidth(inputWidth);
				if (first_iter)
					TextUnderlinedAt(Localization::Translate("Drawable").c_str(), header_y);
				if (ImGui::InputInt("##{}drawable", &drawable))
				{
					drawable = std::clamp(drawable, 0, GetMaxDrawable(component.slot) - 1);
					SetOutfitSlot(component.slot, drawable, texture);
				}
				ImGui::SameLine();
				if (first_iter)
					TextUnderlinedAt(Localization::Translate("Texture").c_str(), header_y); // TODO: this heading is slightly misaligned and I'm not sure why (caused by the above SameLine?)
				if (ImGui::InputInt("##{}texture", &texture))
				{
					texture = std::clamp(texture, 0, GetMaxTexture(component.slot, drawable) - 1);
					SetOutfitSlot(component.slot, drawable, texture);
				}
				ImGui::PopItemWidth();
				ImGui::PopID();

				first_iter = false;
			}

			// Props section (Right column)
			ImGui::NextColumn();
			TextUnderlined(Localization::Translate("Props").c_str());

			const struct
			{
				const char* name;
				int slot;
			} propSlots[] = {{"帽子", 0}, {"眼镜", 1}, {"耳饰", 2}, {"手表", 6}};

			first_iter = true;
			for (const auto& prop : propSlots)
			{
				ImGui::PushID(prop.slot);

				int drawable, texture;
				GetPropSlot(prop.slot, drawable, texture);

				const auto propName = Localization::Translate(prop.name);
				ImGui::Text("%s", propName.c_str());
				ImGui::SameLine();

				ImGui::SetCursorPosX(columnWidth + (columnWidth - inputWidth * 2 - 10));

				ImGui::PushItemWidth(inputWidth);
				if (first_iter)
					TextUnderlinedAt(Localization::Translate("Drawable").c_str(), header_y);
				if (ImGui::InputInt("##pdrawable", &drawable))
				{
					drawable = std::clamp(drawable, 0, GetMaxPropDrawable(prop.slot) - 1);
					SetPropSlot(prop.slot, drawable, texture);
				}
				ImGui::SameLine();
				if (first_iter)
					TextUnderlinedAt(Localization::Translate("Texture").c_str(), header_y);
				if (ImGui::InputInt("##ptexture", &texture))
				{
					texture = std::clamp(texture, 0, GetMaxPropTexture(prop.slot, drawable) - 1);
					SetPropSlot(prop.slot, drawable, texture);
				}
				ImGui::PopItemWidth();
				ImGui::PopID();

				first_iter = false;
			}

			ImGui::Columns(1);

			if (ImGui::Button("随机服装"))
			{
				std::random_device rd;
				std::mt19937 gen(rd());

				// Randomize components
				for (int i = 0; i < 12; ++i)
				{
					int maxDrawable = GetMaxDrawable(i);
					if (maxDrawable > 0)
					{
						int drawable = std::uniform_int_distribution<>(0, maxDrawable - 1)(gen);
						int maxTexture = GetMaxTexture(i, drawable);
						int texture = maxTexture > 0 ? std::uniform_int_distribution<>(0, maxTexture - 1)(gen) : 0;
						SetOutfitSlot(i, drawable, texture);
					}
				}

				// Randomize props
				for (int i : {0, 1, 2, 6, 7})
				{
					int maxDrawable = GetMaxPropDrawable(i);
					if (maxDrawable > 0)
					{
						int drawable = std::uniform_int_distribution<>(0, maxDrawable - 1)(gen);
						int maxTexture = GetMaxPropTexture(i, drawable);
						int texture = maxTexture > 0 ? std::uniform_int_distribution<>(0, maxTexture - 1)(gen) : 0;
						SetPropSlot(i, drawable, texture);
					}
				}
			}
		}));

		return category;
	}
}
