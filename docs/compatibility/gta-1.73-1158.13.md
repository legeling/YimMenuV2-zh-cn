# GTA 1.73 / 1158.13 兼容性

本文档记录中文维护版针对 GTA 5: Enhanced `1.73 / 1158.13` 的适配状态。它描述的是当前代码和验证边界，不代表所有菜单功能已经通过游戏实测。

## 状态摘要

| 范围 | 来源 | 中文提交 | 状态 |
| --- | --- | --- | --- |
| b1158.13 全量地址、特征码、车辆、脚本及四类抢劫数据 | 上游 PR [#988](https://github.com/YimMenu/YimMenuV2/pull/988)，锁定 `e65380a` | `213eebd` | 已合入；443 个 Windows 目标编译步骤通过，未实机验证 |
| 无 FSL 公开/好友战局非法指令修复 | 上游 PR [#1019](https://github.com/YimMenu/YimMenuV2/pull/1019)，提交 `a9e9e0a` | `f1181c8` | 已按官方同步新版状态特征和 213 处函数恢复；下游临时崩溃绕过已撤销，等待实机复测 |
| 名钻赌场豪劫单人气闸 | 上游 PR [#1018](https://github.com/YimMenu/YimMenuV2/pull/1018)，提交 `da012b1` | `244757e` | `fm_mission_controller` 局部变量已由 `63640` 更新为 `64655`；静态检查通过，待终章实机验证 |
| 科兹中心豪劫准备配置 | 上游开放 PR [#1014](https://github.com/YimMenu/YimMenuV2/pull/1014)，锁定 `40c22688` | `f1181c8` | 已新增目标、采购、准备和侦察状态；PR 无上游检查，未实机验证 |
| PR #970、#973、#979 的先期适配 | 上游 PR [#970](https://github.com/YimMenu/YimMenuV2/pull/970)、[#973](https://github.com/YimMenu/YimMenuV2/pull/973)、[#979](https://github.com/YimMenu/YimMenuV2/pull/979) | `a28e89e`、`6ed8d61`、`d297bdc`、`703bc45`、`907dd05` | 历史记录保留，内容已由 #988 的更新版本覆盖 |

完整的锁定上游提交和吸收方式见[下游合入记录](../maintenance/downstream-integrations.md)。

## 已完成检查

- `GPBD_FM`、`GPBD_FM_2`、`GPBD_FM_3` 已通过本机 Clang C++23 语法检查和结构尺寸 `static_assert`。
- PR #973 的新增特征码已完成 token 和十六进制格式检查。
- PR #988 更新的 33 个文件已完成冲突复核；与既有中文翻译及先期适配重叠的内容均已保留或采用 #988 的新值。
- 仓库内 Zig 0.16 工具链完成 443 个 Windows 目标 C/C++ 编译步骤，包括 `GameSkeletonUpdate`、异常处理、全部抢劫模块、Globals、Locals 和 Pointers。
- `v1.0.2-zh-cn` 已通过 GitHub Actions 的 Windows 原生 Clang 构建并成功生成带版本号的 DLL Release。
- 合入时全仓库已通过 `git diff --check`，不存在冲突标记。
- 普通 `main` 推送不会触发 CI/CD；只有与 `VERSION` 一致的 `vMAJOR.MINOR.PATCH-zh-cn` 标签会触发并通过构建发布。
- PR #1019 的 20 个改动文件已逐项解决与中文本地化及下游 #985 加固的冲突；`v1.1.0-zh-cn` 已通过 Windows 原生 Clang 构建、PE 校验和版本化产物上传。
- PR #1018 只更新名钻赌场豪劫单人气闸的一处 `ScriptLocal` 常量；已核对官方提交差异并通过 `git diff --check`，未运行 GTA 实机流程。

## 崩溃定位与防护

PR #988 和 issue [#990](https://github.com/YimMenu/YimMenuV2/issues/990) 的两份独立日志都记录了相同故障：`EXCEPTION_ILLEGAL_INSTRUCTION`，游戏偏移为 `GTA5_Enhanced.exe + 0x1487620`，调用栈返回到旧版 `GameSkeletonUpdate.cpp:22` 的 `node->Run()`。结合“仅 YimMenu 玩家战局可用、公开或好友战局崩溃”的反馈，当前最强证据指向新版战局激活了未被旧名单拦截的完整性检查更新节点，而不是 #988 更新的抢劫数据本身。

2026-07-17 收到的中文维护版 `v1.0.1-zh-cn` 实机日志再次记录了完全相同的游戏偏移，并由新增异常上下文进一步锁定为 `type=1`、`group=0xCA49E244 (Common Main)`、`node=0xB6331929 (CNetwork)`。日志同时显示 `Anticheat bypass mode: Vanilla`，证明问题发生在预期的无 FSL 路径。`CNetwork` 是必要网络更新节点，不能通过整节点屏蔽规避崩溃。

`v1.0.2-zh-cn` 的后续实测显示短特征 `BattlEyeStatusUpdatePatch` 已成功找到并应用，但 32 秒后仍在同一 `CNetwork` 节点和 `+0x1487620` 崩溃。新增机器码为 `1E 4C 69 3B 26 66 66 66 66 66 2E 0F 1F 84 00 00`，调用栈下一帧为 `+0x1485806`。这证明状态补丁不是完整修复，也排除了“补丁没有命中”的解释。上游 issue [#1003](https://github.com/YimMenu/YimMenuV2/issues/1003) 也确认当前官方构建仍无法加入好友或公开战局。

当前正式修复包含：

- 使用 PR #1019 的新版 `BattlEyeStatusUpdatePatch` 特征，解析实际目标函数并在入口返回，不再使用已证明不足的旧七字节栈帧补丁。
- 按 PR #1019 恢复 213 处被加密或破坏的函数，包括将 `+0x1487620` 恢复为跳转到有效实现。
- 撤销下游此前加入的精确异常模拟返回、额外 `GameSkeletonUpdate` 节点屏蔽和异常上下文；这些方案仅用于官方修复出现前的诊断，不再参与运行。

官方函数恢复是当前唯一针对该陷阱的正式修复。若仍在 `+0x1487620` 崩溃，应保留完整日志并重新核对游戏构建和官方偏移，不能再通过吞掉异常来掩盖问题。

## 尚未确认

- 本机 Zig/CMake 虽完成全部编译和链接命令，但最终 `libYimMenuV2.dll` 被错误生成为 `ar archive`；发布只采用 GitHub Actions 的 Windows 原生构建产物，不采用这份交叉编译输出。
- `v1.0.2-zh-cn` 已完成注入并运行到加入战局阶段，但公开/好友战局仍在 `CNetwork` 更新中崩溃；PR #1019 组合修复尚未实测。
- PR #988 已更新公寓、末日、赌场和佩里科岛抢劫模块中的硬编码 `ScriptGlobal(...)` / `ScriptLocal(...)` 值，但没有游戏实测，仍不能确认分红、任务配置、收益修改或瞬间完成全部可用。
- PR #1014 的科兹中心豪劫模块直接写入六组 `MPX_K26_*` stats；目标编号、位掩码和写入后的任务面板刷新均未经过 GTA 实机验证，也不包含分红、收益或瞬间完成能力。
- 旧无 FSL 状态补丁已经实测命中但不足以单独阻止崩溃；新版状态补丁和完整函数恢复仍需共同实机验证。
- 豪宅资产和车库存储数据不等于抢劫支持；当前新增的是“科兹中心豪劫”准备配置，仍没有分红、收益或终章流程修改模块。

## 版本验收清单

- [x] Windows `RelWithDebInfo` 全量构建成功。
- [x] DLL 可在 GTA 5: Enhanced `1.73 / 1158.13` 主菜单注入。
- [ ] 菜单可打开、卸载且无崩溃。
- [ ] 公开战局和好友战局保持运行至少 15 分钟，日志中没有新的 `GameSkeletonUpdate` 异常上下文。
- [ ] 网络和加入战局相关更新通过实机验证。
- [ ] 车辆、衣柜和赌场相关更新通过实机验证。
- [ ] 公寓、赌场、佩里科岛和末日四类既有抢劫功能逐项回归。
- [ ] 科兹中心豪劫的 27 个目标、采购、准备任务和侦察状态逐项回归。
