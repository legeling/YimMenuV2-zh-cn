#include "Localization.hpp"

#include <format>
#include <string_view>
#include <unordered_map>

namespace YimMenu::Localization
{
	namespace
	{
		using namespace std::literals;

		const std::unordered_map<std::string_view, std::string_view> g_Translations = {
		    {"Unknown!"sv, "未知"sv},
		    {"Unknown list!"sv, "未知列表"sv},
		    {"Unknown string command!"sv, "未知字符串命令"sv},
		    {"Unknown color picker!"sv, "未知取色器"sv},
		    {"Close"sv, "关闭"sv},
		    {"Clear"sv, "清除"sv},
		    {"Save"sv, "保存"sv},
		    {"Delete"sv, "删除"sv},
		    {"Remove"sv, "移除"sv},
		    {"Add"sv, "添加"sv},
		    {"Search"sv, "搜索"sv},
		    {"Optional"sv, "可选"sv},
		    {"Name"sv, "名称"sv},
		    {"Current"sv, "当前位置"sv},
		    {"Saved..."sv, "已保存..."sv},
		    {"Yes"sv, "是"sv},
		    {"No"sv, "否"sv},
		    {"Join"sv, "加入"sv},
		    {"Refresh"sv, "刷新"sv},
		    {"Write"sv, "写入"sv},
		    {"Reload"sv, "重新加载"sv},
		    {"Unload"sv, "卸载"sv},
		    {"Pause"sv, "暂停"sv},
		    {"Resume"sv, "继续"sv},
		    {"Add Item"sv, "添加项目"sv},
		    {"Apply Font Scale"sv, "应用字体缩放"sv},
		    {"Self"sv, "自身"sv},
		    {"Vehicle"sv, "载具"sv},
		    {"Teleport"sv, "传送"sv},
		    {"Network"sv, "网络"sv},
		    {"Players"sv, "玩家"sv},
		    {"World"sv, "世界"sv},
		    {"Recovery"sv, "恢复"sv},
		    {"Settings"sv, "设置"sv},
		    {"Debug"sv, "调试"sv},
		    {"Main"sv, "主页"sv},
		    {"Globals"sv, "全局"sv},
		    {"Movement"sv, "移动"sv},
		    {"Tools"sv, "工具"sv},
		    {"Special Ability"sv, "特殊能力"sv},
		    {"Wanted"sv, "通缉"sv},
		    {"Info"sv, "信息"sv},
		    {"Toxic"sv, "恶意"sv},
		    {"Troll"sv, "整活"sv},
		    {"Damage"sv, "伤害"sv},
		    {"Griefing"sv, "骚扰"sv},
		    {"Kick"sv, "踢出"sv},
		    {"Kicks"sv, "踢人"sv},
		    {"General"sv, "常规"sv},
		    {"Saved"sv, "已保存"sv},
		    {"New"sv, "新增"sv},
		    {"Notifications"sv, "通知"sv},
		    {"Tracking"sv, "追踪"sv},
		    {"Customize"sv, "自定义"sv},
		    {"Colors"sv, "颜色"sv},
		    {"Rounding"sv, "圆角"sv},
		    {"Layout"sv, "布局"sv},
		    {"Border"sv, "边框"sv},
		    {"Global"sv, "全局"sv},
		    {"Bounty"sv, "悬赏"sv},
		    {"Misc"sv, "杂项"sv},
		    {"Session"sv, "战局"sv},
		    {"Join Session"sv, "加入战局"sv},
		    {"Join Session Type"sv, "加入战局类型"sv},
		    {"The session type to join"sv, "要加入的战局类型"sv},
		    {"Joins the specified session type"sv, "加入指定类型的战局"sv},
		    {"Enhancements"sv, "增强"sv},
		    {"Spoofing"sv, "伪装"sv},
		    {"Matchmaking (Client)"sv, "匹配伪装（客户端）"sv},
		    {"Matchmaking (Server)"sv, "匹配伪装（服务端）"sv},
		    {"Saved Players"sv, "已保存玩家"sv},
		    {"Random Events"sv, "随机事件"sv},
		    {"Hotkeys"sv, "热键"sv},
		    {"GUI"sv, "界面"sv},
		    {"Game"sv, "游戏"sv},
		    {"UI"sv, "界面"sv},
		    {"Player ESP"sv, "玩家透视"sv},
		    {"Ped ESP"sv, "NPC 透视"sv},
		    {"Object ESP"sv, "物体透视"sv},
		    {"Overlay"sv, "叠加层"sv},
		    {"Chat"sv, "聊天"sv},
		    {"Businesses"sv, "产业"sv},
		    {"Casino"sv, "赌场"sv},
		    {"Slot Machines"sv, "老虎机"sv},
		    {"Daily Activities"sv, "每日活动"sv},
		    {"Heists"sv, "抢劫任务"sv},
		    {"Transactions"sv, "交易"sv},
		    {"Stat Editor"sv, "属性编辑器"sv},
		    {"Regular"sv, "普通"sv},
		    {"Packed"sv, "打包"sv},
		    {"Packed Range"sv, "打包范围"sv},
		    {"From Clipboard"sv, "从剪贴板"sv},
		    {"Vehicle Editor"sv, "载具编辑器"sv},
		    {"Spawn Ped"sv, "生成 NPC"sv},
		    {"Outfit Editor"sv, "服装编辑器"sv},
		    {"Weapons"sv, "武器"sv},
		    {"Ammu-Nation"sv, "武装国度"sv},
		    {"Aimbot"sv, "自瞄"sv},
		    {"IPLs"sv, "场景 IPL"sv},
		    {"Weather"sv, "天气"sv},
		    {"Time"sv, "时间"sv},
		    {"Other"sv, "其他"sv},
		    {"Categories"sv, "分类"sv},
		    {"Locations"sv, "地点"sv},
		    {"Category"sv, "分类"sv},
		    {"Location name"sv, "地点名称"sv},
		    {"New location"sv, "新地点"sv},
		    {"Save current location"sv, "保存当前位置"sv},
		    {"Double click to teleport\nShift click to delete"sv, "双击传送\nShift+点击删除"sv},
		    {"Distance: %f"sv, "距离：%f"sv},
		    {"Are you sure you want to delete %s?"sv, "确定要删除 %s 吗？"sv},
		    {"Please enter a valid name"sv, "请输入有效名称"sv},
		    {"Custom Teleport"sv, "自定义传送"sv},
		    {"Location with name {} already exists"sv, "名为 {} 的地点已存在"sv},
		    {"Click on a location to select it. Add more at Teleport > Saved"sv, "点击地点以选择。可在 传送 > 已保存 中添加更多地点"sv},
		    {"Hover over the command name to change its hotkey"sv, "将鼠标悬停在命令名称上可修改热键"sv},
		    {"Press any registered key to remove"sv, "按任意已注册按键可移除"sv},
		    {"Enter a keystroke"sv, "请输入按键"sv},
		    {"Press any button..."sv, "按下任意按键..."sv},
		    {"No hotkey assigned"sv, "未分配热键"sv},
		    {"Modify Colors:"sv, "调整颜色："sv},
		    {"Adjust Rounding:"sv, "调整圆角："sv},
		    {"Layout & Alignment:"sv, "布局与对齐："sv},
		    {"Border Sizes:"sv, "边框尺寸："sv},
		    {"Global Settings:"sv, "全局设置："sv},
		    {"Font Configuration:"sv, "字体配置："sv},
		    {"Current Scale: %.2f"sv, "当前缩放：%.2f"sv},
		    {"Lua Scripts"sv, "Lua 脚本"sv},
		    {"Search:"sv, "搜索："sv},
		    {"All"sv, "全部"sv},
		    {"Class"sv, "类别"sv},
		    {"Garage"sv, "车库"sv},
		    {"Root"sv, "根目录"sv},
		    {"Folder"sv, "文件夹"sv},
		    {"File Name"sv, "文件名"sv},
		    {"Folder Name"sv, "文件夹名"sv},
		    {"Natives not cached yet."sv, "原生函数缓存尚未完成。"sv},
		    {"Stats not loaded yet."sv, "统计数据尚未加载。"sv},
		    {"Saved Vehicles"sv, "已保存载具"sv},
		    {"Type"sv, "类型"sv},
		    {"Category"sv, "分类"sv},
		    {"Action"sv, "操作"sv},
		    {"Value"sv, "值"sv},
		    {"Index"sv, "索引"sv},
		    {"Start"sv, "起始"sv},
		    {"End"sv, "结束"sv},
		    {"Quantity"sv, "数量"sv},
		    {"Price"sv, "价格"sv},
		    {"Stat Value"sv, "属性值"sv},
		    {"Rockstar ID"sv, "Rockstar ID"sv},
		    {"Give Weapon"sv, "给予武器"sv},
		    {"Remove Weapon"sv, "移除武器"sv},
		    {"Kills With: %d"sv, "击杀数：%d"sv},
		    {"Deaths By: %d"sv, "死亡数：%d"sv},
		    {"K/D Ratio: %.2f"sv, "击杀/死亡比：%.2f"sv},
		    {"Headshots: %d"sv, "爆头数：%d"sv},
		    {"Accuracy: %d%%"sv, "命中率：%d%%"sv},
		    {"Custom Weapons"sv, "自定义武器"sv},
		    {"Spawn Inside"sv, "生成在车内"sv},
		    {"Spawn Maxed"sv, "满改生成"sv},
		    {"Spawn Clone"sv, "生成克隆"sv},
		    {"Business"sv, "产业"sv},
		    {"God Mode"sv, "无敌模式"sv},
		    {"Blocks all incoming damage"sv, "抵挡所有传入伤害"sv},
		    {"Vehicle God Mode"sv, "载具无敌"sv},
		    {"Blocks all incoming damage to your current vehicle"sv, "抵挡当前载具受到的所有伤害"sv},
		    {"Locally Visible"sv, "本地可见"sv},
		    {"Be locally visible while remaining invisible to other players"sv, "对其他玩家保持隐身，同时自己本地可见"sv},
		    {"Off The Radar"sv, "脱离雷达"sv},
		    {"No Ragdoll"sv, "无布娃娃"sv},
		    {"No Idle Kick"sv, "防挂机踢出"sv},
		    {"Unlimited Oxygen"sv, "无限氧气"sv},
		    {"Format Money"sv, "格式化金钱"sv},
		    {"Mobile Radio"sv, "移动电台"sv},
		    {"Keep Player Clean"sv, "保持角色整洁"sv},
		    {"Disable Critical Hits"sv, "禁用暴击"sv},
		    {"Skip Cutscene"sv, "跳过过场动画"sv},
		    {"Skip Conversation"sv, "跳过对话"sv},
		    {"Suicide"sv, "自杀"sv},
		    {"Heal"sv, "治疗"sv},
		    {"Clear Damage"sv, "清除伤害"sv},
		    {"Fill Inventory"sv, "填满物品栏"sv},
		    {"Open Wardrobe"sv, "打开衣柜"sv},
		    {"Stand on Vehicles"sv, "站在载具上"sv},
		    {"Disable Action Mode"sv, "禁用动作模式"sv},
		    {"Super Run"sv, "超级奔跑"sv},
		    {"Move Rate Override"sv, "移动速率覆盖"sv},
		    {"Multipler for base run/sprint speed"sv, "基础奔跑/冲刺速度倍率"sv},
		    {"Run/Sprint/Swim Multiplier"sv, "跑步/冲刺/游泳倍率"sv},
		    {"Multipler with which the run/sprint/swim speed is increased with until maximum speed is reached"sv, "提升跑步/冲刺/游泳速度直至最大值的倍率"sv},
		    {"Super Jump"sv, "超级跳"sv},
		    {"Noclip"sv, "穿墙"sv},
		    {"Noclip Speed"sv, "穿墙速度"sv},
		    {"Freecam"sv, "自由视角"sv},
		    {"Freecam Speed"sv, "自由视角速度"sv},
		    {"How fast the freecam will move positions"sv, "自由视角移动速度"sv},
		    {"Infinite Special Ability"sv, "无限特殊能力"sv},
		    {"Enable in MP"sv, "在多人模式启用"sv},
		    {"Special Ability in MP"sv, "多人模式特殊能力"sv},
		    {"The special ability to enable for the MP character"sv, "为多人模式角色启用的特殊能力"sv},
		    {"Matchmaking Region"sv, "匹配地区"sv},
		    {"Matchmaking region to spoof to"sv, "要伪装成的匹配地区"sv},
		    {"Spoof Region Type"sv, "伪装地区类型"sv},
		    {"Spoof Language"sv, "伪装语言"sv},
		    {"Spoof Player Count"sv, "伪装玩家数"sv},
		    {"Multiplex Session"sv, "多路广播战局"sv},
		    {"Region Type"sv, "地区类型"sv},
		    {"Language"sv, "语言"sv},
		    {"Player Count"sv, "玩家数量"sv},
		    {"Multiplex Session Count"sv, "多路广播数量"sv},
		    {"Spoofs the region type of the session"sv, "伪装战局地区类型"sv},
		    {"Spoofs the session language"sv, "伪装战局语言"sv},
		    {"Spoofs the session player count"sv, "伪装战局玩家数量"sv},
		    {"The region to spoof the session to"sv, "将战局地区伪装为"sv},
		    {"The language to spoof the session to"sv, "将战局语言伪装为"sv},
		    {"The player count to spoof the session to"sv, "将战局玩家数伪装为"sv},
		    {"The number of sessions to advertise for matchmaking"sv, "为匹配广播的战局实例数量"sv},
		    {"Makes your session much more visible to other players by hosting multiple instances of it. High values may fill your session in seconds. Use at your own risk"sv, "通过广播多个战局实例显著提升你的战局可见度。数值过高可能会在几秒内塞满战局，请自行承担风险"sv},
		    {"Public"sv, "公开"sv},
		    {"Solo Public"sv, "单人公开"sv},
		    {"SCTV"sv, "SCTV"sv},
		    {"Crew"sv, "帮会"sv},
		    {"Join Crew"sv, "加入帮会"sv},
		    {"Closed Crew"sv, "封闭帮会"sv},
		    {"Closed Friend"sv, "封闭好友"sv},
		    {"Find Friend"sv, "寻找好友"sv},
		    {"Invite Only"sv, "仅邀请"sv},
		    {"Solo"sv, "单人"sv},
		    {"Chinese (Simplified)"sv, "简体中文"sv},
		    {"Chinese (Traditional)"sv, "繁体中文"sv},
		    {"English"sv, "英语"sv},
		    {"French"sv, "法语"sv},
		    {"German"sv, "德语"sv},
		    {"Italian"sv, "意大利语"sv},
		    {"Japanese"sv, "日语"sv},
		    {"Korean"sv, "韩语"sv},
		    {"Polish"sv, "波兰语"sv},
		    {"Portuguese (Brazil)"sv, "葡萄牙语（巴西）"sv},
		    {"Russian"sv, "俄语"sv},
		    {"Spanish (Mexico)"sv, "西班牙语（墨西哥）"sv},
		    {"Spanish (Spain)"sv, "西班牙语（西班牙）"sv},
		    {"Australia"sv, "澳大利亚"sv},
		    {"CIS"sv, "独联体"sv},
		    {"China"sv, "中国"sv},
		    {"Europe"sv, "欧洲"sv},
		    {"Japan"sv, "日本"sv},
		    {"South America"sv, "南美"sv},
		    {"US East"sv, "美国东部"sv},
		    {"US West"sv, "美国西部"sv},
		    {"Unknown"sv, "未知"sv},
		    {"Saved Players"sv, "已保存玩家"sv},
		    {"Player Database Auto Update"sv, "玩家数据库自动更新"sv},
		    {"Tracking Notifications"sv, "追踪通知"sv},
		    {"Auto Update"sv, "自动更新"sv},
		    {"Update Now"sv, "立即更新"sv},
		    {"Track Player"sv, "追踪玩家"sv},
		    {"Data not fetched yet"sv, "数据尚未拉取"sv},
		    {"Username"sv, "用户名"sv},
		    {"Teleport To"sv, "传送到"sv},
		    {"Notify When Joinable"sv, "可加入时通知"sv},
		    {"Notify When Unjoinable"sv, "不可加入时通知"sv},
		    {"Notify When Online"sv, "上线时通知"sv},
		    {"Notify When Offline"sv, "离线时通知"sv},
		    {"Notify On Session Type Change"sv, "战局类型变化时通知"sv},
		    {"Notify On Mission Change"sv, "任务状态变化时通知"sv},
		    {"Notify On Job Lobby Change"sv, "任务大厅变化时通知"sv},
		    {"Automatically updates tracked player status every 3 minutes. This is required for tracking notifications to work"sv, "每 3 分钟自动更新一次已追踪玩家状态，这是追踪通知正常工作的前提"sv},
		    {"Notifies you when the state of a player you track changes"sv, "当你追踪的玩家状态变化时通知你"sv},
		    {"Notifies you when a tracked player becomes joinable"sv, "当追踪玩家变为可加入时通知你"sv},
		    {"Notifies you when a tracked player becomes unjoinable"sv, "当追踪玩家变为不可加入时通知你"sv},
		    {"Notifies you when a tracked player goes online"sv, "当追踪玩家上线时通知你"sv},
		    {"Notifies you when a tracked player goes offline"sv, "当追踪玩家离线时通知你"sv},
		    {"Notifies you when a tracked player's session type changes"sv, "当追踪玩家的战局类型变化时通知你"sv},
		    {"Notifies you when a tracked player joins or leaves a mission"sv, "当追踪玩家加入或离开任务时通知你"sv},
		    {"Notifies you when a tracked player joins or leaves a job lobby"sv, "当追踪玩家加入或离开任务大厅时通知你"sv},
		    {"Session Type: %s"sv, "战局类型：%s"sv},
		    {"Host of Session: %s"sv, "战局主持人：%s"sv},
		    {"Is Spectating: %s"sv, "是否在观战：%s"sv},
		    {"Is Job Lobby: %s"sv, "是否在任务大厅：%s"sv},
		    {"Host of Job Lobby: %s"sv, "任务大厅主持人：%s"sv},
		    {"Mission Type: %s"sv, "任务类型：%s"sv},
		    {"Mission Name: %s"sv, "任务名称：%s"sv},
		    {"Freemode global block is not loaded."sv, "自由模式全局块未加载。"sv},
		    {"Random Events are not initialized."sv, "随机事件尚未初始化。"sv},
		    {"Freemode stack is not valid."sv, "自由模式堆栈无效。"sv},
		    {"Freemode is not running."sv, "自由模式未运行。"sv},
		    {"Active Events: %d/%d"sv, "活动事件：%d/%d"sv},
		    {"Launch Event"sv, "启动事件"sv},
		    {"Kill Event"sv, "终止事件"sv},
		    {"Teleport to Event"sv, "传送到事件"sv},
		    {"State: %s"sv, "状态：%s"sv},
		    {"Location: N/A"sv, "地点：无"sv},
		    {"Trigger Range: N/A"sv, "触发范围：无"sv},
		    {"Location: %d"sv, "地点：%d"sv},
		    {"Trigger Range: %.2f"sv, "触发范围：%.2f"sv},
		    {"Set Cooldown"sv, "设置冷却"sv},
		    {"Set Availability"sv, "设置可用性"sv},
		    {"Open GitHub"sv, "打开 GitHub"sv},
		    {"Open Matrix server"sv, "打开 Matrix 服务器"sv},
		    {"Play with YimMenu users"sv, "仅与 YimMenu 用户游玩"sv},
		    {"Play with everyone (Broken!)"sv, "与所有人游玩（有问题）"sv},
		    {"IMPORTANT! PLEASE READ!"sv, "重要！请先阅读！"sv},
		    {"Rank: %d (%d RP)"sv, "等级：%d（%d RP）"sv},
		    {"Money: %d"sv, "金钱：%d"sv},
		    {"Coords: %.2f, %.2f, %.2f"sv, "坐标：%.2f, %.2f, %.2f"sv},
		    {"Distance: %.2f"sv, "距离：%.2f"sv},
		    {"Ped missing or deleted"sv, "角色不存在或已删除"sv},
		    {"RID:"sv, "RID："sv},
		    {"Xbox User ID:"sv, "Xbox 用户 ID："sv},
		    {"Steam ID:"sv, "Steam ID："sv},
		    {"Epic Account ID:"sv, "Epic 账户 ID："sv},
		    {"IP Address:"sv, "IP 地址："sv},
		    {"Add to Saved"sv, "添加到已保存"sv},
		    {"View SC Profile"sv, "查看 SC 档案"sv},
		    {"Add Friend"sv, "添加好友"sv},
		    {"More Info"sv, "更多信息"sv},
		    {"Average Latency: %.2f"sv, "平均延迟：%.2f"sv},
		    {"Packet Loss: %.2f"sv, "丢包率：%.2f"sv},
		    {"No players yet!"sv, "当前没有玩家！"sv},
		    {"FPS: %d"sv, "帧率：%d"sv},
		    {"Plate Number"sv, "车牌号"sv},
		    {"Please enter a vehicle."sv, "请先进入一辆载具。"sv},
		    {"Refresh Current Vehicle"sv, "刷新当前载具"sv},
		    {"Change Plate"sv, "修改车牌"sv},
		    {"Max Vehicle"sv, "满改载具"sv},
		    {"Slot"sv, "槽位"sv},
		    {"Mod"sv, "改装"sv},
		    {"Style"sv, "样式"sv},
		    {"Normalized name to: %s"sv, "名称规范化为：%s"sv},
		    {"Load from Clipboard"sv, "从剪贴板加载"sv},
		    {"Data type not supported"sv, "不支持的数据类型"sv},
		    {"Warning: You are solely responsible for what you do with this tool. If you don't know what you're doing, you'll likely get banned"sv, "警告：你需要对使用此工具的后果自行负责。如果你不知道自己在做什么，很可能会被封禁"sv},
		    {"Trigger"sv, "触发"sv},
		    {"Start Script"sv, "启动脚本"sv},
		    {"Start Session Script"sv, "启动战局脚本"sv},
		    {"Start Script With Event"sv, "带事件启动脚本"sv},
		    {"Jump to Offset"sv, "跳转到偏移"sv},
		    {"Kill"sv, "终止"sv},
		    {"Log Labels"sv, "记录标签"sv},
		    {"Script Name"sv, "脚本名称"sv},
		    {"Add Offset"sv, "添加偏移"sv},
		    {"Add Read Player Id"sv, "添加读取玩家 ID"sv},
		    {"Remove Offset"sv, "移除偏移"sv},
		    {"Network Bail"sv, "网络脱离"sv},
		    {"DoTeleport"sv, "执行传送"sv},
		    {"Randomize Outfit"sv, "随机服装"sv},
		    {"Start Script With Event"sv, "通过事件启动脚本"sv},
		    {"Skips any currently playing cutscene"sv, "跳过当前播放的过场动画"sv},
		    {"Skips current conversation in the phone."sv, "跳过当前电话对话。"sv},
		    {"Kills you"sv, "将你杀死"sv},
		    {"Restores your health and armour."sv, "恢复生命值与护甲。"sv},
		    {"Clears all the dirt and blood on you."sv, "清除身上的污渍和血迹。"sv},
		    {"Fills all of your snacks and armours."sv, "填满所有零食和护甲。"sv},
		    {"Invisibility"sv, "隐身"sv},
		    {"Be invisible"sv, "使自己隐身"sv},
		    {"Prevents you from ragdolling when standing on top of vehicles."sv, "站在载具顶部时不会摔倒。"sv},
		    {"Disables the player's janky movement during combat."sv, "禁用战斗时的动作模式移动。"sv},
		    {"Run faster than normal"sv, "跑得比平时更快"sv},
		    {"No Clip"sv, "穿墙"sv},
		    {"Allows you to fly through the map"sv, "允许你穿过地图飞行"sv},
		    {"Ensures that the special ability bar never runs out"sv, "确保特殊能力槽不会耗尽"sv},
		    {"Enable Special Abilities in MP"sv, "在多人模式启用特殊能力"sv},
		    {"Enables special abilities in multiplayer. This repurposes cut content and may be unstable"sv, "在多人模式启用特殊能力。该功能基于未启用内容，可能不稳定"sv},
		    {"Infinite Ammo"sv, "无限弹药"sv},
		    {"You will never run out of ammo"sv, "不会耗尽弹药"sv},
		    {"Rapid Fire"sv, "快速射击"sv},
		    {"Increases the fire rate of your weapons substantially"sv, "大幅提高武器射速"sv},
		    {"Infinite Parachutes"sv, "无限降落伞"sv},
		    {"Always have a parachute"sv, "始终拥有降落伞"sv},
		    {"Explosive Ammo"sv, "爆炸弹药"sv},
		    {"Every bullet you shoot explodes."sv, "你射出的每一发子弹都会爆炸。"sv},
		    {"Weapon Damage"sv, "武器伤害"sv},
		    {"Allows altering your weapons damage output"sv, "允许调整武器伤害输出"sv},
		    {"Weapon Damage Scale"sv, "武器伤害倍率"sv},
		    {"Sets the damage scale for weapons (including melee)"sv, "设置武器伤害倍率（包括近战）"sv},
		    {"Prevents your ped from ragdolling"sv, "防止角色进入布娃娃状态"sv},
		    {"Allows you to stay underwater without losing oxygen."sv, "允许你在水下停留而不损失氧气。"sv},
		    {"Hides you from other players on the radar"sv, "在雷达上对其他玩家隐藏自己"sv},
		    {"Never Wanted"sv, "永不通缉"sv},
		    {"Never gain a wanted level"sv, "永远不会获得通缉等级"sv},
		    {"Freeze Wanted"sv, "冻结通缉"sv},
		    {"Freeze your wanted level to the desired level"sv, "将通缉等级冻结在指定等级"sv},
		    {"Clear Wanted"sv, "清除通缉"sv},
		    {"Clears your wanted level"sv, "清除通缉等级"sv},
		    {"Set Wanted"sv, "设置通缉"sv},
		    {"Sets your wanted level to the desired level"sv, "将通缉等级设置为指定等级"sv},
		    {"Wanted Slider"sv, "通缉滑块"sv},
		    {"Wanted level to set/freeze"sv, "要设置或冻结的通缉等级"sv},
		    {"Locks on to enemies and other players"sv, "锁定敌人和其他玩家"sv},
		    {"Aim For Head"sv, "瞄准头部"sv},
		    {"Targets the head when locking on to enemies and other players"sv, "锁定敌人和其他玩家时瞄准头部"sv},
		    {"Target Drivers"sv, "瞄准驾驶员"sv},
		    {"Allows aimbot to lock on to enemies and other players in vehicles"sv, "允许自瞄锁定载具中的敌人和其他玩家"sv},
		    {"Repair Vehicle"sv, "修复载具"sv},
		    {"Fixes any damage to your current vehicle"sv, "修复当前载具的所有损坏"sv},
		    {"Fix All Vehicles"sv, "修复所有载具"sv},
		    {"Fixes all of your destroyed personal vehicles."sv, "修复你所有已损毁的个人载具。"sv},
		    {"Keep Vehicle Fixed"sv, "保持载具完好"sv},
		    {"Keeps your vehicle clean and fixed"sv, "保持载具清洁并自动修复"sv},
		    {"Prevents you from falling out of vehicles."sv, "防止你从载具中甩出。"sv},
		    {"Horn Boost"sv, "喇叭加速"sv},
		    {"Makes the vehicle boost when you hold the horn"sv, "按住喇叭时让载具加速"sv},
		    {"Shows a speedometer whenever you are in a vehicle"sv, "在载具中显示速度表"sv},
		    {"Allows you to request your personal vehicles."sv, "允许你请求个人载具。"sv},
		    {"Enter last vehicle"sv, "进入上一辆载具"sv},
		    {"Enters the last vehicle you were in"sv, "进入你上一次乘坐的载具"sv},
		    {"Requests your current Personal Vehicle."sv, "请求你当前的个人载具。"sv},
		    {"Returns your current Personal Vehicle to storage."sv, "将当前个人载具收回仓库。"sv},
		    {"Lowers your current vehicle's stance. Only some vehicles are supported"sv, "降低当前载具的车身姿态，仅部分载具受支持"sv},
		    {"Allows you to keep your hats or head gear inside vehicles."sv, "允许在载具内保留帽子或头饰。"sv},
		    {"Enables all the DLC vehicles on websites."sv, "在网站中启用所有 DLC 载具。"sv},
		    {"Saves the current vehicle you are in as your personal vehicle."sv, "将当前乘坐的载具保存为个人载具。"sv},
		    {"Bring All Objects"sv, "拉近所有物体"sv},
		    {"Teleports all game objects to you"sv, "将游戏内所有物体传送到你身边"sv},
		    {"Bring All Peds"sv, "拉近所有 NPC"sv},
		    {"Teleports all game peds to you"sv, "将游戏内所有 NPC 传送到你身边"sv},
		    {"Bring All Vehicles"sv, "拉近所有载具"sv},
		    {"Teleports all game vehicles to you"sv, "将游戏内所有载具传送到你身边"sv},
		    {"Kill All Peds"sv, "击杀所有 NPC"sv},
		    {"Kills all peds in the game world"sv, "击杀游戏世界中的所有 NPC"sv},
		    {"Kill All Enemies"sv, "击杀所有敌人"sv},
		    {"Kills all enemies in the game world"sv, "击杀游戏世界中的所有敌人"sv},
		    {"Delete All Objects"sv, "删除所有物体"sv},
		    {"Deletes all objects in the game world, including mission critical objects"sv, "删除游戏世界中的所有物体，包括任务关键物体"sv},
		    {"Delete All Peds"sv, "删除所有 NPC"sv},
		    {"Deletes all peds in the game world, including mission critical peds"sv, "删除游戏世界中的所有 NPC，包括任务关键 NPC"sv},
		    {"Delete All Vehicles"sv, "删除所有载具"sv},
		    {"Deletes all vehicles in the game world, including mission critical vehicles"sv, "删除游戏世界中的所有载具，包括任务关键载具"sv},
		    {"Nearby peds will ignore you and become oblivious to your actions."sv, "附近 NPC 会无视你，不再对你的行为做出反应。"sv},
		    {"When you are wanted, no police will be spawned."sv, "当你被通缉时，不会再生成警察。"sv},
		    {"Enables the developer mode in Creator."sv, "在编辑器中启用开发者模式。"sv},
		    {"You won't die because of border restrictions."sv, "不会因边界限制而死亡。"sv},
		    {"All PEDs attack each other."sv, "所有 NPC 会互相攻击。"sv},
		    {"Sets the current online time"sv, "设置当前线上时间"sv},
		    {"Freezes the online clock at the selected time"sv, "将线上时间冻结在所选时刻"sv},
		    {"Set hour (0-23)"sv, "设置小时（0-23）"sv},
		    {"Set minute (0-59)"sv, "设置分钟（0-59）"sv},
		    {"Set second (0-59)"sv, "设置秒（0-59）"sv},
		    {"Weather to set or force. Note that this is local and cannot be seen by other players"sv, "要设置或强制的天气。该效果仅本地可见，其他玩家无法看到"sv},
		    {"Sets the specifed weather. Note that this effect is local and cannot be seen by other players"sv, "设置指定天气。该效果仅本地可见，其他玩家无法看到"sv},
		    {"Forces the specified weather. Note that this effect is local and cannot be seen by other players"sv, "强制指定天气。该效果仅本地可见，其他玩家无法看到"sv},
		    {"Spawns an object at your current location."sv, "在当前位置生成一个物体。"sv},
		    {"The model name of the ped you wish to spawn."sv, "要生成的 NPC 模型名称。"sv},
		    {"Object Model"sv, "物体模型"sv},
		    {"The model name of the object you wish to spawn."sv, "要生成的物体模型名称。"sv},
		    {"Brings the player to you"sv, "将玩家拉到你身边"sv},
		    {"Teleports you to the selected player"sv, "将你传送到选中的玩家身边"sv},
		    {"Player Teleport Coords"sv, "玩家传送坐标"sv},
		    {"The coordinates to teleport the player to"sv, "要将玩家传送到的坐标"sv},
		    {"Teleport Player To Coord"sv, "将玩家传送到坐标"sv},
		    {"Teleports the player to the specified coordinates"sv, "将玩家传送到指定坐标"sv},
		    {"Send to Property"sv, "送往房产"sv},
		    {"Sends the player to the selected property"sv, "将玩家送到所选房产"sv},
		    {"Send to Interior"sv, "送往室内"sv},
		    {"Sends the player to the selected interior"sv, "将玩家送到所选室内"sv},
		    {"Set Bounty"sv, "设置悬赏"sv},
		    {"Sets the specified bounty to the player"sv, "为玩家设置指定金额的悬赏"sv},
		    {"The bounty amount to set"sv, "要设置的悬赏金额"sv},
		    {"Sets the bounty as anonymous"sv, "将悬赏设置为匿名"sv},
		    {"Deletes the vehicle the player is in"sv, "删除玩家所乘坐的载具"sv},
		    {"Bypasses all casino region restrictions"sv, "绕过所有赌场地区限制"sv},
		    {"Disables death barriers found under the map. This also prevents dying randomly when spectating someone"sv, "禁用地图下方的死亡屏障，同时防止观战时随机死亡"sv},
		    {"Automatically silences all incoming calls"sv, "自动静音所有来电"sv},
		    {"Forces the weather to thunder for all players in the session"sv, "为战局中的所有玩家强制雷暴天气"sv},
		    {"Matchmaking will put you into sessions with other YimMenu users."sv, "匹配系统会将你放入只有其他 YimMenu 用户的战局。"sv},
		    {"Forces you to become freemode script host. Note that this might break some missions."sv, "强制让你成为 freemode 脚本主机。注意这可能会导致部分任务异常。"sv},
		    {"Allows you to pause the game in online (local only)."sv, "允许你在线上模式中暂停游戏（仅本地生效）。"sv},
		    {"Spoofs your position to make it appear as if you're next to all the players in the session at all times. Use with caution"sv, "伪装你的位置，让所有玩家看起来你始终在他们身边。请谨慎使用"sv},
		    {"Change the region you matchmake in to the specified value"sv, "将你的匹配地区改为指定值"sv},
		    {"Claims your safe earnings from the selected business."sv, "从所选产业领取保险箱收益。"sv},
		    {"Allows the player to change the character's appearance for free"sv, "允许玩家免费修改角色外观"sv},
		    {"Allows the player to change the character's gender"sv, "允许玩家更改角色性别"sv},
		    {"Removes the cooldown for changing appearance"sv, "移除更改外观的冷却时间"sv},
		    {"Force-unlocks GTA+ content. Use with caution"sv, "强制解锁 GTA+ 内容。请谨慎使用"sv},
		    {"Allows you to play any mission solo. Note that some missions might break if there aren't enough players"sv, "允许你单人游玩任意任务。注意某些任务在人数不足时可能异常"sv},
		    {"Heist difficulty"sv, "抢劫任务难度"sv},
		    {"Primary target"sv, "主要目标"sv},
		    {"Weapon category"sv, "武器类别"sv},
		    {"Set Cuts"sv, "设置分红"sv},
		    {"Sets heist cut"sv, "设置抢劫分红"sv},
		    {"Force Ready"sv, "强制准备"sv},
		    {"Forces all players to be ready"sv, "强制所有玩家准备完成"sv},
		    {"Skip Hacking"sv, "跳过黑客"sv},
		    {"Skips hacking process"sv, "跳过黑客流程"sv},
		    {"Skip Drilling"sv, "跳过钻孔"sv},
		    {"Skips drilling process"sv, "跳过钻孔流程"sv},
		    {"Skip Swiping"sv, "跳过刷卡"sv},
		    {"Skips card swiping process"sv, "跳过刷卡流程"sv},
		    {"Instant Finish"sv, "立即完成"sv},
		    {"Instantly passes the heist"sv, "立即通过抢劫任务"sv},
		    {"Spectates the selected player"sv, "观战选中的玩家"sv}
		    ,{"Cage Gun"sv, "笼子枪"sv}
		    ,{"Delete Gun"sv, "删除枪"sv}
		    ,{"Gravity Gun"sv, "重力枪"sv}
		    ,{"Steal Vehicle Gun"sv, "偷车枪"sv}
		    ,{"Repair Gun"sv, "修复枪"sv}
		    ,{"Vehicle Gun"sv, "载具枪"sv}
		    ,{"Teleport Gun"sv, "传送枪"sv}
		    ,{"Paint Gun"sv, "喷漆枪"sv}
		    ,{"Weapon Type"sv, "武器类型"sv}
		    ,{"The type of custom weapon to use"sv, "要使用的自定义武器类型"sv}
		    ,{"Custom Weapon"sv, "自定义武器"sv}
		    ,{"Modifies weapon ability"sv, "修改武器能力"sv}
		    ,{"Enable Only if Weapon is Out"sv, "仅在手持武器时启用"sv}
		    ,{"Enables custom weapon only when weapon is out"sv, "仅在手上拿着武器时启用自定义武器"sv}
		    ,{"Launch Entity on Release"sv, "松手时发射实体"sv}
		    ,{"Launches entity when released"sv, "释放时发射抓取的实体"sv}
		    ,{"Vehicle Model"sv, "载具模型"sv}
		    ,{"Model of the vehicle"sv, "载具模型名称"sv}
		    ,{"Paint Gun Color"sv, "喷漆枪颜色"sv}
		    ,{"Color of the paint gun"sv, "喷漆枪的颜色"sv}
		    ,{"Rainbow Color"sv, "彩虹颜色"sv}
		    ,{"Enables rainbow color for paint gun"sv, "为喷漆枪启用彩虹颜色"sv}
		    ,{"Rainbow Color Speed"sv, "彩虹变色速度"sv}
		    ,{"Speed of rainbow color for paint gun"sv, "喷漆枪彩虹颜色变化速度"sv}
		    ,{"Rainbow Color Style"sv, "彩虹颜色样式"sv}
		    ,{"Style of rainbow color for paint gun"sv, "喷漆枪彩虹颜色样式"sv}
		    ,{"Fade"sv, "渐变"sv}
		    ,{"Spasm"sv, "闪烁"sv}
		    ,{"Explosion Type"sv, "爆炸类型"sv}
		    ,{"Select an explosion type"sv, "选择一种爆炸类型"sv}
		    ,{"Explosion Damage Scale"sv, "爆炸伤害倍率"sv}
		    ,{"Sets the damage scale for explosions"sv, "设置爆炸伤害倍率"sv}
		    ,{"Explosion Camera Shake"sv, "爆炸镜头震动"sv}
		    ,{"Controls how much the camera shakes during explosions"sv, "控制爆炸时镜头震动幅度"sv}
		    ,{"Don't Care"sv, "无所谓"sv}
		    ,{"Grenade"sv, "手雷"sv}
		    ,{"Grenade Launcher"sv, "榴弹发射器"sv}
		    ,{"Sticky Bomb"sv, "黏弹"sv}
		    ,{"Molotov"sv, "燃烧瓶"sv}
		    ,{"Rocket"sv, "火箭弹"sv}
		    ,{"Tank Shell"sv, "坦克炮弹"sv}
		    ,{"Car"sv, "汽车"sv}
		    ,{"Plane"sv, "飞机"sv}
		    ,{"Boat"sv, "船只"sv}
		    ,{"Truck"sv, "卡车"sv}
		    ,{"Bullet"sv, "子弹"sv}
		    ,{"Train"sv, "列车"sv}
		    ,{"Barrel"sv, "油桶"sv}
		    ,{"Firework"sv, "烟花"sv}
		    ,{"Snowball"sv, "雪球"sv}
		    ,{"Proximity Mine"sv, "感应地雷"sv}
		    ,{"Pipe Bomb"sv, "土制炸弹"sv}
		    ,{"Explosive Ammo (Shotgun)"sv, "爆炸弹药（霰弹枪）"sv}
		    ,{"Oppressor Mk II Cannon"sv, "暴君 Mk II 机炮"sv}
		    ,{"Ray Gun"sv, "电磁枪"sv}
		    ,{"Flash Grenade"sv, "闪光弹"sv}
		    ,{"Stun Grenade"sv, "震撼弹"sv}
		    ,{"EMP Launcher"sv, "EMP 发射器"sv}
		    ,{"Clear"sv, "晴朗"sv}
		    ,{"Extra Sunny"sv, "烈日"sv}
		    ,{"Clouds"sv, "多云"sv}
		    ,{"Overcast"sv, "阴天"sv}
		    ,{"Rain"sv, "下雨"sv}
		    ,{"Clearing"sv, "转晴"sv}
		    ,{"Thunder"sv, "雷暴"sv}
		    ,{"Smog"sv, "雾霾"sv}
		    ,{"Foggy"sv, "大雾"sv}
		    ,{"Xmas"sv, "圣诞"sv}
		    ,{"Snow"sv, "下雪"sv}
		    ,{"Snow Light"sv, "小雪"sv}
		    ,{"Blizzard"sv, "暴风雪"sv}
		    ,{"Halloween"sv, "万圣节"sv}
		    ,{"Neutral"sv, "中性"sv}
		    ,{"Rain Halloween"sv, "万圣节雨天"sv}
		    ,{"Snow Halloween"sv, "万圣节雪天"sv}
		    ,{"Normal"sv, "普通"sv}
		    ,{"Hard"sv, "困难"sv}
		    ,{"Diamonds"sv, "钻石"sv}
		    ,{"Gold"sv, "黄金"sv}
		    ,{"Artwork"sv, "艺术品"sv}
		    ,{"Cash"sv, "现金"sv}
		    ,{"Silent & Sneaky"sv, "无声潜入"sv}
		    ,{"The Big Con"sv, "惊天骗局"sv}
		    ,{"Aggressive"sv, "强攻"sv}
		    ,{"Remove Gunman"sv, "移除枪手"sv}
		    ,{"Remove Driver"sv, "移除车手"sv}
		    ,{"Remove Hacker"sv, "移除黑客"sv}
		    ,{"Hidden Cache"sv, "隐藏补给箱"sv}
		    ,{"Selected Hidden Cache"sv, "已选隐藏补给箱"sv}
		    ,{"Treasure Chest"sv, "宝箱"sv}
		    ,{"Selected Treasure Chest"sv, "已选宝箱"sv}
		    ,{"Buried Stash"sv, "埋藏战利品"sv}
		    ,{"Selected Buried Stash"sv, "已选埋藏战利品"sv}
		    ,{"Skydive"sv, "高空跳伞"sv}
		    ,{"Selected Skydive"sv, "已选高空跳伞"sv}
		    ,{"Time Trial"sv, "时间挑战赛"sv}
		    ,{"Selected Time Trial"sv, "已选时间挑战赛"sv}
		    ,{"Standard Time Trial"sv, "标准时间挑战赛"sv}
		    ,{"RC Bandito Time Trial"sv, "RC 匪徒时间挑战赛"sv}
		    ,{"Junk Energy Bike Time Trial"sv, "Junk Energy 自行车时间挑战赛"sv}
		    ,{"Street Dealer"sv, "街头贩子"sv}
		    ,{"Selected Street Dealer"sv, "已选街头贩子"sv}
		    ,{"LS Tag"sv, "洛圣都涂鸦"sv}
		    ,{"Selected LS Tag"sv, "已选洛圣都涂鸦"sv}
		    ,{"Animal"sv, "动物"sv}
		    ,{"Selected Animal"sv, "已选动物"sv}
		    ,{"Product"sv, "货物"sv}
		    ,{"Selected Product"sv, "已选货物"sv}
		    ,{"Set All Activities Completed"sv, "完成全部活动"sv}
		    ,{"Reset All Activities"sv, "重置全部活动"sv}
		    ,{"Complete All Challenges"sv, "完成全部挑战"sv}
		    ,{"Teleport to Hidden Cache"sv, "传送到隐藏补给箱"sv}
		    ,{"Collect Hidden Cache"sv, "拾取隐藏补给箱"sv}
		    ,{"Teleport to Treasure Chest"sv, "传送到宝箱"sv}
		    ,{"Collect Treasure Chest"sv, "拾取宝箱"sv}
		    ,{"Enable Treasure Chests in LS"sv, "在洛圣都启用宝箱"sv}
		    ,{"Teleport to Shipwreck"sv, "传送到沉船"sv}
		    ,{"Collect Shipwreck"sv, "拾取沉船奖励"sv}
		    ,{"Teleport to Buried Stash"sv, "传送到埋藏战利品"sv}
		    ,{"Collect Buried Stash"sv, "拾取埋藏战利品"sv}
		    ,{"Enable Buried Stashes in LS"sv, "在洛圣都启用埋藏宝藏"sv}
		    ,{"Teleport to G's Cache"sv, "传送到 G 的补给点"sv}
		    ,{"Collect G's Cache"sv, "拾取 G 的补给点奖励"sv}
		    ,{"Teleport to Stash House"sv, "传送到藏匿屋"sv}
		    ,{"Enter Stash House Safe Code"sv, "输入藏匿屋保险箱密码"sv}
		    ,{"Teleport to Dealer"sv, "传送到贩子"sv}
		    ,{"Open Street Dealer Menu"sv, "打开街头贩子菜单"sv}
		    ,{"Teleport to LS Tag"sv, "传送到洛圣都涂鸦"sv}
		    ,{"Spray LS Tag"sv, "喷涂洛圣都涂鸦"sv}
		    ,{"Spawn Animal"sv, "生成动物"sv}
		    ,{"Photograph Animal"sv, "拍摄动物"sv}
		    ,{"Teleport to Product"sv, "传送到货物"sv}
		    ,{"Collect Product"sv, "收集货物"sv}
		};
	}

	std::string Translate(std::string_view text)
	{
		if (text.empty())
			return {};

		if (const auto it = g_Translations.find(text); it != g_Translations.end())
			return std::string(it->second);

		return std::string(text);
	}

	std::string TranslateLabel(std::string_view text)
	{
		const auto id_pos = text.find("##");
		if (id_pos == std::string_view::npos)
			return Translate(text);

		const auto visible = text.substr(0, id_pos);
		const auto suffix = text.substr(id_pos);
		return std::format("{}{}", Translate(visible), suffix);
	}

	std::string FormatHotkeyWindowTitle(std::string_view label)
	{
		return std::format("{} 热键", Translate(label));
	}
}
