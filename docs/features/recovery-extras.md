# 账号进度补充功能

本文档记录中文维护版从社区实现中筛选并按当前命令架构重新实现的账号进度功能。功能面向 GTA 5: Enhanced `1.73 / 1158.13`，代码已加入菜单，但仍需 Windows 构建和游戏实机验证。

## 功能入口

| 菜单 | 功能 | 写入类型 |
| --- | --- | --- |
| `账号进度 -> 产业` | 补满夜总会人气 | 命名 Stat |
| `账号进度 -> 产业` | 清除三项洗钱生意热度 | Packed Int |
| `账号进度 -> 商店` | 修改枪支厢型车 1-10 号武器槽位 | Tunable |
| `账号进度 -> 合约` | 德瑞博士、当当钟农场突袭、奥斯卡、无路可逃跳过前置 | 命名 Stat、Packed Int/Bool |
| `账号进度 -> 合约` | 选择改装铺合约并跳过前置 | 命名 Stat |
| `载具 -> 基础 -> 工具` | 永久删除当前已呼出的个人载具 | Script Global 请求 |

## 实现约束

- 所有功能只在 GTA 在线模式战局中执行，不在故事模式写入线上进度。
- Packed Stat 和个人载具删除请求按 `1.73 / 1158.13` 记录；游戏更新后必须重新核对索引和全局变量。
- 持久进度写入会先把旧值记入日志，但 Rockstar 保存后不保证能够回滚。
- 当当钟农场突袭由五项写入组成，必须保持为单独按钮，不与其他合约批量执行。
- 改装铺合约要求先在面板上接取合约，并在菜单中选择相同项目。
- 删除个人载具只接受当前已呼出的载具；确认后执行前会再次核对车辆 ID，操作不可撤销。
- 枪支厢型车会先检查可调参数缓存，缓存未完成时不会写入空地址。

## 数据来源与验证边界

初始数据锁定到 lonelybud/YimMenuV2 的 `9d8fb0d` 快照，参考其 [Recovery 界面](https://github.com/lonelybud/YimMenuV2/blob/9d8fb0d1852e1dd4b4b78dd3e5dc821a9762b81c/src/game/frontend/submenus/Recovery.hpp)、[Simple 功能](https://github.com/lonelybud/YimMenuV2/blob/9d8fb0d1852e1dd4b4b78dd3e5dc821a9762b81c/src/game/features/recovery/Simple.hpp)和[合约功能](https://github.com/lonelybud/YimMenuV2/blob/9d8fb0d1852e1dd4b4b78dd3e5dc821a9762b81c/src/game/frontend/submenus/Recovery/RecoveryHeistCategory.hpp)。中文维护版没有复制其旧式直接绘制架构，而是重新拆分为命令、菜单组和带前置检查的执行逻辑。

当前静态检查和本地交叉编译不能证明 Rockstar 服务器保存行为、任务面板刷新或个人载具永久删除可用。发布前至少需要验证：

1. Windows 原生 Clang 全量构建。
2. 每个按钮在无产业、无任务或无当前载具时的失败行为。
3. 写入后切换战局并重新登录，确认进度状态一致。
4. 删除载具使用测试车辆验证，确认请求不会作用于其他车位。
