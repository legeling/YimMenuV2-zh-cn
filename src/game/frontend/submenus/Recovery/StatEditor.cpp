#include "StatEditor.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/frontend/widgets/imgui_bitfield.hpp"
#include "core/localization/Localization.hpp"
#include "game/backend/AnticheatBypass.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Stats.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/stats/CStatsMgr.hpp"

#include <bit>

namespace YimMenu::Submenus
{
	struct StatInfo
	{
		std::string m_Name;
		std::uint32_t m_NameHash = 0;
		bool m_Normalized = false;
		sStatData* m_Data = nullptr;

		bool IsValid() const
		{
			return m_Data != nullptr;
		}
	};

	struct PackedStatInfo
	{
		int m_Index;
		bool m_IsBoolStat;
		bool m_IsValid;

		bool IsValid() const
		{
			return m_IsValid;
		}
	};

	union StatValue {
		float m_AsFloat;
		int m_AsInt;
		bool m_AsBool;
		std::int64_t m_AsI64;
		std::uint64_t m_AsU64;
		char m_AsString[12];
	};

	// https://stackoverflow.com/questions/66897068/can-trim-of-a-string-be-done-inplace-with-c20-ranges
	static std::string_view TrimString(std::string_view string)
	{
		return std::string_view{
		    std::ranges::find_if_not(
		        string,
		        [](auto c) {
			        return std::isspace(c);
		        }),
		    std::ranges::find_if_not(
		        string | std::views::reverse,
		        [](auto c) {
			        return std::isspace(c);
		        }).base()};
	}

	static StatInfo GetStatInfo(std::string_view name_str)
	{
		StatInfo name{};
		auto len = name_str.length();

		// not sure why people do this
		if (len > 1 && name_str[0] == '$')
		{
			auto it = name_str.begin();
			std::advance(it, 1);
			name_str = std::string_view{it, name_str.end()};
			len--;
			name.m_Normalized = true;
		}

		name.m_Name = name_str;

		if (len > 3 && tolower(name_str[0]) == 'm' && tolower(name_str[1]) == 'p' && tolower(name_str[2]) == 'x')
		{
			if (auto last_char = Pointers.StatsMgr->GetStat("MPPLY_LAST_MP_CHAR"_J))
			{
				name.m_Name[2] = '0' + last_char->GetInt();
				name.m_Normalized = true;
			}
		}

		name.m_NameHash = Joaat(name.m_Name);
		name.m_Data = Pointers.StatsMgr->GetStat(name.m_NameHash);

		if (name.m_Data == nullptr && len > 3 && (tolower(name_str[0]) != 'm' || tolower(name_str[1]) != 'p' || !(tolower(name_str[2]) == '0' || tolower(name_str[2]) == '1')))
		{
			// stat names without a character prefix
			auto last_char = Pointers.StatsMgr->GetStat("MPPLY_LAST_MP_CHAR"_J);
			auto char_index = last_char ? last_char->GetInt() : 0;
			auto char_prefix = char_index == 0 ? "MP0_" : "MP1_";
			auto new_hash = Joaat(char_prefix + name.m_Name);
			auto new_stat = Pointers.StatsMgr->GetStat(new_hash);

			if (new_stat)
			{
				name.m_Name = char_prefix + name.m_Name;
				name.m_NameHash = new_hash;
				name.m_Data = new_stat;
				name.m_Normalized = true;
			}
		}

		return name;
	}

	static void ReadStat(StatValue& value, sStatData* data)
	{
		memset(&value, 0, sizeof(StatValue));

		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
			value.m_AsBool = data->GetBool();
			return;
		case sStatData::Type::FLOAT:
			value.m_AsFloat = data->GetFloat();
			return;
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
			value.m_AsInt = data->GetInt();
			return;
		case sStatData::Type::INT64:
			value.m_AsI64 = data->GetInt64();
			return;
		case sStatData::Type::UINT64:
		case sStatData::Type::PACKED:
			value.m_AsU64 = data->GetUInt64();
			return;
		case sStatData::Type::STRING:
			strncpy(value.m_AsString, data->GetString(), sizeof(value.m_AsString));
			return;
		default:
			return; // data type not supported
		}
	}

	static void WriteInt64Stat(std::uint32_t hash, std::int64_t value, sStatData* data)
	{
		// There is no 64-bit setter Native. Seed the local value one step behind,
		// then increment through the Native so the game queues the stat for persistence.
		const auto previousBits = std::bit_cast<std::uint64_t>(value) - 1;
		data->SetInt64(std::bit_cast<std::int64_t>(previousBits));
		STATS::STAT_INCREMENT(hash, 1.0f);
	}

	static void WriteUInt64Stat(std::uint32_t hash, std::uint64_t value, sStatData* data)
	{
		data->SetUInt64(value - 1);
		STATS::STAT_INCREMENT(hash, 1.0f);
	}

	static void WriteStat(std::uint32_t hash, StatValue& value, sStatData* data)
	{
		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
			STATS::STAT_SET_BOOL(hash, value.m_AsBool, true);
			return;
		case sStatData::Type::FLOAT:
			STATS::STAT_SET_FLOAT(hash, value.m_AsFloat, true);
			return;
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
			STATS::STAT_SET_INT(hash, value.m_AsInt, true);
			return;
		case sStatData::Type::INT64:
			WriteInt64Stat(hash, value.m_AsI64, data);
			return;
		case sStatData::Type::UINT64:
			WriteUInt64Stat(hash, value.m_AsU64, data);
			return;
		case sStatData::Type::STRING:
			STATS::STAT_SET_STRING(hash, value.m_AsString, true);
			return;
		case sStatData::Type::PACKED:
			Stats::SetMaskedUInt64(hash, value.m_AsU64);
			return;
		default:
			return; // data type not supported
		}
	}

	static void WriteStatWithStringValue(std::uint32_t hash, std::string_view value, sStatData* data)
	{
		const auto text = std::string(value);
		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
		{
			bool _bool = false;
			std::string as_string(text);
			std::transform(as_string.begin(), as_string.end(), as_string.begin(), [](char c) {
				return tolower(c);
			});

			if (as_string == "true" || as_string == "1")
			{
				_bool = true;
			}

			STATS::STAT_SET_BOOL(hash, _bool, true);
			return;
		}
		case sStatData::Type::FLOAT:
		{
			auto _float = std::strtof(text.c_str(), nullptr);
			STATS::STAT_SET_FLOAT(hash, _float, true);
			return;
		}
		case sStatData::Type::INT:
		case sStatData::Type::UINT32:
		case sStatData::Type::UINT16:
		case sStatData::Type::UINT8:
		{
			auto _int = std::strtol(text.c_str(), nullptr, 10);
			STATS::STAT_SET_INT(hash, _int, true);
			return;
		}
		case sStatData::Type::INT64:
		{
			auto int64_ = std::strtoll(text.c_str(), nullptr, 10);
			WriteInt64Stat(hash, int64_, data);
			return;
		}
		case sStatData::Type::UINT64:
		{
			auto uint64_ = std::strtoull(text.c_str(), nullptr, 10);
			WriteUInt64Stat(hash, uint64_, data);
			return;
		}
		case sStatData::Type::STRING:
			STATS::STAT_SET_STRING(hash, text.c_str(), true);
			return;
		case sStatData::Type::PACKED:
		{
			auto uint64_ = std::strtoull(text.c_str(), nullptr, 10);
			Stats::SetMaskedUInt64(hash, uint64_);
			return;
		}
		default:
			return; // data type not supported
		}
	}

	static bool RenderStatEditor(StatValue& value, sStatData* data)
	{
		ImGui::SetNextItemWidth(150.f);
		switch (data->GetType())
		{
		case sStatData::Type::_BOOL:
			return ImGui::Checkbox("值", &value.m_AsBool);
		case sStatData::Type::FLOAT:
			return ImGui::InputFloat("值", &value.m_AsFloat);
		case sStatData::Type::INT:
			return ImGui::InputInt("值", &value.m_AsInt);
		case sStatData::Type::UINT32:
			return ImGui::InputScalar("值", ImGuiDataType_U32, &value.m_AsInt);
		case sStatData::Type::UINT16:
			return ImGui::InputScalar("值", ImGuiDataType_U16, &value.m_AsInt);
		case sStatData::Type::UINT8:
			return ImGui::InputScalar("值", ImGuiDataType_U8, &value.m_AsInt);
		case sStatData::Type::INT64:
			return ImGui::InputScalar("值", ImGuiDataType_S64, &value.m_AsI64);
		case sStatData::Type::UINT64:
			return ImGui::InputScalar("值", ImGuiDataType_U64, &value.m_AsU64);
		case sStatData::Type::STRING:
			return ImGui::InputText("值", value.m_AsString, sizeof(value.m_AsString));
		case sStatData::Type::PACKED:
			return ImGui::Bitfield("值", &value.m_AsU64);
		default:
			ImGui::BeginDisabled();
			ImGui::Text("%s", "不支持该数据类型");
			ImGui::EndDisabled();
			return false; // data type not supported
		}
	}

	static PackedStatInfo GetPackedStatInfo(int index)
	{
		PackedStatInfo info{};
		int row;
		bool unk;

		info.m_Index = index;
		Pointers.GetPackedStatData(index, &row, &info.m_IsBoolStat, &unk);

		if (row != 0 || index <= 191)
			info.m_IsValid = true;

		return info;
	}

	static void ReadPackedStat(StatValue& value, const PackedStatInfo& info)
	{
		if (info.m_IsBoolStat)
			value.m_AsBool = STATS::GET_PACKED_STAT_BOOL_CODE(info.m_Index, -1);
		else
			value.m_AsInt = STATS::GET_PACKED_STAT_INT_CODE(info.m_Index, -1);
	}

	static void WritePackedStat(const StatValue& value, const PackedStatInfo& info)
	{
		if (info.m_IsBoolStat)
			STATS::SET_PACKED_STAT_BOOL_CODE(info.m_Index, value.m_AsBool, -1);
		else
			STATS::SET_PACKED_STAT_INT_CODE(info.m_Index, value.m_AsInt, -1);
	}

	static void WritePackedStatRange(int start, int end, int value)
	{
		for (int i = start; i <= end; i++)
		{
			auto info = GetPackedStatInfo(i);
			if (!info.m_IsValid)
				break; // the rest are probably not valid, either

			if (info.m_IsBoolStat)
				STATS::SET_PACKED_STAT_BOOL_CODE(info.m_Index, static_cast<bool>(value), -1);
		}
	}

	static bool RenderPackedStatEditor(StatValue& value, const PackedStatInfo& info)
	{
		ImGui::SetNextItemWidth(150.f);
		if (info.m_IsBoolStat)
			return ImGui::Checkbox("值##packed", &value.m_AsBool);
		else
			return ImGui::InputScalar("值##packed", ImGuiDataType_U8, &value.m_AsInt);
	}

	std::shared_ptr<Category> BuildStatEditorMenu()
	{
		auto menu = std::make_shared<Category>("数据编辑器");
		auto normal = std::make_shared<Group>("常规");
		auto packed = std::make_shared<Group>("打包");
		auto packed_range = std::make_shared<Group>("打包范围");
		auto from_clipboard = std::make_shared<Group>("从剪贴板");

		normal->AddItem(std::make_unique<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("%s", Localization::Translate("Natives not cached yet.").c_str());

			static StatInfo current_info;
			static char stat_buf[48]{};
			static StatValue value{};

			ImGui::SetNextItemWidth(300.f);
			if (ImGui::InputText("名称", stat_buf, sizeof(stat_buf)))
			{
				current_info = GetStatInfo(stat_buf);
				if (current_info.IsValid())
					ReadStat(value, current_info.m_Data);
			}

			if (!current_info.IsValid())
				return ImGui::TextDisabled("%s", Localization::Translate("Stat not found").c_str());
			else if (current_info.m_Normalized)
			{
				ImGui::Text("名称规范化为：%s", current_info.m_Name.data());
			}

			bool can_edit = !current_info.m_Data->IsControlledByNetshop();

			RenderStatEditor(value, current_info.m_Data);

			if (ImGui::Button("刷新"))
				ReadStat(value, current_info.m_Data);
			ImGui::SameLine();
			ImGui::BeginDisabled(!can_edit);
			if (ImGui::Button("写入"))
				FiberPool::Push([] {
					WriteStat(current_info.m_NameHash, value, current_info.m_Data);
				});
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				FiberPool::Push([] {
					WriteStat(current_info.m_NameHash, value, current_info.m_Data);
				});
			if (!can_edit && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("%s", "该数据不应由客户端修改。若仍要写入，请右键强制执行。");
			ImGui::EndDisabled();
		}));

		packed->AddItem(std::make_unique<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("%s", Localization::Translate("Natives not cached yet.").c_str());

			// TODO: improve packed stat editor
			static PackedStatInfo current_info{0, false, true};
			static StatValue value{};

			ImGui::SetNextItemWidth(200.f);
			if (ImGui::InputInt("索引", &current_info.m_Index))
			{
				current_info = GetPackedStatInfo(current_info.m_Index);
				if (current_info.IsValid())
					ReadPackedStat(value, current_info);
			}

			if (!current_info.IsValid())
				return ImGui::TextDisabled("%s", Localization::Translate("Invalid index").c_str());

			RenderPackedStatEditor(value, current_info);

			if (ImGui::Button("刷新##packed"))
				ReadPackedStat(value, current_info);
			ImGui::SameLine();
			if (ImGui::Button("写入##packed"))
				FiberPool::Push([] {
					WritePackedStat(value, current_info);
				});
		}));

		packed_range->AddItem(std::make_unique<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("%s", Localization::Translate("Natives not cached yet.").c_str());

			static int start{}, end{}, value{};

			ImGui::SetNextItemWidth(150.f);
			ImGui::InputInt("起始", &start);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150.f);
			ImGui::InputInt("结束", &end);
			ImGui::SetNextItemWidth(150.f);
			ImGui::InputScalar("值##packed_range", ImGuiDataType_U8, &value);
			ImGui::SameLine();
			if (ImGui::Button("写入##packed_range"))
				FiberPool::Push([] {
					WritePackedStatRange(start, end, value);
				});
		}));

		from_clipboard->AddItem(std::make_unique<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("%s", Localization::Translate("Natives not cached yet.").c_str());

			if (ImGui::Button("从剪贴板加载"))
			{
				auto clip_text = std::string(ImGui::GetClipboardText());
				FiberPool::Push([clip_text] {
					for (auto line : clip_text | std::ranges::views::split('\n'))
					{
						auto components = TrimString(std::string_view{line.begin(), line.end()}) | std::ranges::views::split('=') | std::ranges::to<std::vector<std::string>>();

						if (components.size() != 2)
						{
							LOGF(WARNING, "Load From Clipboard: line \"{}\" is malformed", std::string_view{line.begin(), line.end()});
							continue;
						}

						auto info = GetStatInfo(TrimString(components[0]));
						if (!info.IsValid())
						{
							LOGF(WARNING, "Load From Clipboard: cannot find stat {}", components[0]);
							continue;
						}

						WriteStatWithStringValue(info.m_NameHash, TrimString(components[1]), info.m_Data);
					}
				});
			}
		}));

		menu->AddItem(std::move(normal));
		menu->AddItem(std::move(packed));
		menu->AddItem(std::move(packed_range));
		menu->AddItem(std::move(from_clipboard));
		return menu;
	}
}
