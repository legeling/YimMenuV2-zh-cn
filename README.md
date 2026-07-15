# YimMenuV2 中文维护版

这是 [YimMenu/YimMenuV2](https://github.com/YimMenu/YimMenuV2) 的非官方简体中文维护版本，面向 GTA 5: Enhanced。

- 中文仓库：[legeling/YimMenuV2-zh-cn](https://github.com/legeling/YimMenuV2-zh-cn)
- 官方上游：[YimMenu/YimMenuV2](https://github.com/YimMenu/YimMenuV2)
- 上游分支：`enhanced`
- 中文主分支：`main`
- 中文版本下载：[GitHub Releases](https://github.com/legeling/YimMenuV2-zh-cn/releases)

本仓库会在保留简体中文本地化和中文发行配置的同时，定期同步官方上游的功能与修复。它不是 YimMenu 官方发布渠道；上游项目的功能说明、风险提示和许可证仍然适用。

## 项目文档

维护流程、上游合入台账、游戏版本兼容性和功能支持状态统一收录在[项目文档](docs/README.md)中。README 只保留使用者需要的仓库说明和安装信息。

## 使用方法

1. 下载最新版 [FSL](https://www.unknowncheats.me/forum/grand-theft-auto-v/616977-fsl-local-gtao-saves.html)，将 `WINMM.dll` 放入 GTA V 目录。FSL 现在是可选组件，但出于账号安全考虑仍强烈建议使用。
2. 从[中文版本 Releases](https://github.com/legeling/YimMenuV2-zh-cn/releases)下载 `YimMenuV2.dll`。
3. 下载注入器，例如 [Xenos](https://www.unknowncheats.me/forum/general-programming-and-reversing/124013-xenos-injector-v2-3-2-a.html)。
4. 打开 Rockstar Launcher，选择 Grand Theft Auto V Enhanced，在设置中禁用 BattlEye。通过 Steam 或 Epic Games 启动时，可能还需要添加 `-nobattleye` 启动参数。
5. 启动 GTA V，在主菜单中使用注入器注入 `YimMenuV2.dll`。

## 打开菜单

按 `INSERT` 或 `Ctrl+\` 打开菜单。

## 常见问题

### 每隔五分钟就与公共战局失去同步

目前项目不包含 BattlEye 绕过。由于心跳检测失败，正常主机会最终将你移出战局；除使用真正的私有绕过方案外，目前没有其他解决方法。

### 移除 FSL 后游戏进度消失

FSL 会将账号存档数据重定向到本地磁盘，因此使用 FSL 产生的进度只会在启用 FSL 时显示。你也可以不使用 FSL 运行 YimMenuV2，但不建议这样做。

### 移除 FSL 后游戏无法启动

这是一个已知问题。删除 `Documents/GTAV Enhanced/Profiles` 后重试。

## 许可证

本项目沿用上游的 [GPL-2.0 许可证](LICENSE)。
