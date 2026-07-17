# GTA 1.73 / 1158.13 兼容性

本文档记录中文维护版针对 GTA 5: Enhanced `1.73 / 1158.13` 的适配状态。它描述的是当前代码和验证边界，不代表所有菜单功能已经通过游戏实测。

## 状态摘要

| 范围 | 来源 | 中文提交 | 状态 |
| --- | --- | --- | --- |
| b1158.13 全量地址、特征码、车辆、脚本及四类抢劫数据 | 上游 PR [#988](https://github.com/YimMenu/YimMenuV2/pull/988)，锁定 `e65380a` | `213eebd` | 已合入；443 个 Windows 目标编译步骤通过，未实机验证 |
| `GameSkeletonUpdate` 公开/好友战局崩溃防护和诊断 | 下游白盒适配 | `3d30df7`、`v1.0.2-zh-cn` 及后续工作区修复 | `v1.0.2` 证明状态补丁不足；已增加精确陷阱恢复，等待复测 |
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

## 崩溃定位与防护

PR #988 和 issue [#990](https://github.com/YimMenu/YimMenuV2/issues/990) 的两份独立日志都记录了相同故障：`EXCEPTION_ILLEGAL_INSTRUCTION`，游戏偏移为 `GTA5_Enhanced.exe + 0x1487620`，调用栈返回到旧版 `GameSkeletonUpdate.cpp:22` 的 `node->Run()`。结合“仅 YimMenu 玩家战局可用、公开或好友战局崩溃”的反馈，当前最强证据指向新版战局激活了未被旧名单拦截的完整性检查更新节点，而不是 #988 更新的抢劫数据本身。

2026-07-17 收到的中文维护版 `v1.0.1-zh-cn` 实机日志再次记录了完全相同的游戏偏移，并由新增异常上下文进一步锁定为 `type=1`、`group=0xCA49E244 (Common Main)`、`node=0xB6331929 (CNetwork)`。日志同时显示 `Anticheat bypass mode: Vanilla`，证明问题发生在预期的无 FSL 路径。`CNetwork` 是必要网络更新节点，不能通过整节点屏蔽规避崩溃。

`v1.0.2-zh-cn` 的后续实测显示短特征 `BattlEyeStatusUpdatePatch` 已成功找到并应用，但 32 秒后仍在同一 `CNetwork` 节点和 `+0x1487620` 崩溃。新增机器码为 `1E 4C 69 3B 26 66 66 66 66 66 2E 0F 1F 84 00 00`，调用栈下一帧为 `+0x1485806`。这证明状态补丁不是完整修复，也排除了“补丁没有命中”的解释。上游 issue [#1003](https://github.com/YimMenu/YimMenuV2/issues/1003) 也确认当前官方构建仍无法加入好友或公开战局。

下游适配当前包含：

- 除原有 `0xA0F39FB6` 和 `TamperActions` 外，同时拦截已知安全模块名称及伪装名称 `rageSecEngine`、`rageSecGamePluginManager`、`fwClothMeshing`、`CCreditsText`；组节点和 `Common Main` 子节点统一检查。
- 在每次调用更新节点前写入线程局部异常上下文。若仍然崩溃，日志会额外给出 update type、节点地址、组哈希、节点哈希和 `Run` 虚函数地址，便于锁定新版遗漏节点。
- 恢复无 FSL 路径的 `BattlEyeStatusUpdatePatch`：使用上游历史修复中的稳定七字节状态比较特征，在 Arxan 混淆例程的已知栈帧内提前返回；不再依赖 #988 中失效的长特征和易漂移的固定偏移。
- 非法指令日志额外记录 RIP 处的 16 字节机器码。若恢复状态补丁后仍崩溃，下一份实机日志可以直接区分固定 `UD2` 陷阱和新的混淆指令序列。
- 针对实测确认的 b1158.13 陷阱增加窄范围恢复：仅当异常类型、`Common Main / CNetwork` 上下文、RIP、完整 16 字节机器码和栈顶返回地址同时吻合时，模拟该叶子调用的 `ret`，让 `CNetwork` 继续执行；任一条件不符仍按原异常流程记录并终止。

这是一项基于两份一致崩溃日志和静态逆向信息的保守修复，不是实机结论。若游戏日志仍出现相同异常，应优先检查新增的 `Active exception context [GameSkeletonUpdate(...)]` 行，而不是继续按旧偏移猜测。

## 尚未确认

- 本机 Zig/CMake 虽完成全部编译和链接命令，但最终 `libYimMenuV2.dll` 被错误生成为 `ar archive`；发布只采用 GitHub Actions 的 Windows 原生构建产物，不采用这份交叉编译输出。
- `v1.0.2-zh-cn` 已完成注入并运行到加入战局阶段，但公开/好友战局仍在 `CNetwork` 更新中崩溃；新增精确陷阱恢复尚未实测。
- PR #988 已更新公寓、末日、赌场和佩里科岛抢劫模块中的硬编码 `ScriptGlobal(...)` / `ScriptLocal(...)` 值，但没有游戏实测，仍不能确认分红、任务配置、收益修改或瞬间完成全部可用。
- 无 FSL 状态补丁已经实测命中但不足以单独阻止崩溃；必须继续验证新增精确陷阱恢复，不能视为已经实机验收。
- 豪宅相关车辆存储数据不等于豪宅抢劫支持；当前没有对应的任务配置、分红、收益或流程修改模块。

## 版本验收清单

- [x] Windows `RelWithDebInfo` 全量构建成功。
- [x] DLL 可在 GTA 5: Enhanced `1.73 / 1158.13` 主菜单注入。
- [ ] 菜单可打开、卸载且无崩溃。
- [ ] 公开战局和好友战局保持运行至少 15 分钟，日志中没有新的 `GameSkeletonUpdate` 异常上下文。
- [ ] 网络和加入战局相关更新通过实机验证。
- [ ] 车辆、衣柜和赌场相关更新通过实机验证。
- [ ] 四类现有抢劫功能逐项回归。
- [ ] 确认豪宅抢劫所需 stats、globals、locals、脚本和任务流程。
