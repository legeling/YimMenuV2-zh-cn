#pragma once
#include "core/filemgr/FileMgr.hpp"
#include <unordered_map>

namespace YimMenu
{
	namespace Outfit
	{
		struct ComponentData
		{
			std::string label;
			int drawable_id = 0;
			int texture_id = 0;
			int palette_var = -1;
			int drawable_id_max = 0;
			int texture_id_max = 0;
		};

		struct OutfitComponents
		{
			std::unordered_map<int, ComponentData> items = {
			    {1, {"面具"}},
			    {2, {"发型"}},
			    {3, {"躯干"}},
			    {4, {"裤装"}},
			    {5, {"包袋"}},
			    {6, {"鞋子"}},
			    {7, {"配饰"}},
			    {8, {"内搭"}},
			    {9, {"护甲"}},
			    {10, {"贴花"}},
			    {11, {"上衣"}}};
		};

		struct OutfitProps
		{
			std::unordered_map<int, ComponentData> items = {
			    {0, {"帽子"}},
			    {1, {"眼镜"}},
			    {2, {"耳饰"}},
			    {6, {"手表"}},
			    {7, {"手链"}}};
		};

		class OutfitEditor
		{
		public:
			static void SetSelfOutfit(OutfitComponents components, OutfitProps props, bool applyHair);

			// JSON persistence
			static Folder CheckFolder(std::string folderName = "");
			static void ApplyOutfitFromJson(std::string folderName, std::string fileName, bool applyHair);
			static void SaveOutfit(std::string fileName, std::string folder);
			static void RefreshList(std::string folderName, std::vector<std::string>& folders, std::vector<std::string>& files);

			// Validate component indices before sending them to the game.
			static void CheckBoundsDrawable(ComponentData& item, const int lower);
			static void CheckBoundsTexture(ComponentData& item, const int lower);
		};
	}
}
