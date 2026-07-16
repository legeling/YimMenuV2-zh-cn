# GTA 1.73 / 1158.13 兼容性

本文档记录中文维护版针对 GTA 5: Enhanced `1.73 / 1158.13` 的适配状态。它描述的是当前代码和验证边界，不代表所有菜单功能已经通过游戏实测。

## 状态摘要

| 范围 | 来源 | 中文提交 | 状态 |
| --- | --- | --- | --- |
| b1158.13 全量地址、特征码、车辆、脚本及四类抢劫数据 | 上游 PR [#988](https://github.com/YimMenu/YimMenuV2/pull/988)，锁定 `e65380a` | `213eebd` | 已合入；443 个 Windows 目标编译步骤通过，未实机验证 |
| `GameSkeletonUpdate` 公开/好友战局崩溃防护和诊断 | 下游白盒适配 | `3d30df7` | 已完成静态和编译验证，等待游戏实测 |
| PR #970、#973、#979 的先期适配 | 上游 PR [#970](https://github.com/YimMenu/YimMenuV2/pull/970)、[#973](https://github.com/YimMenu/YimMenuV2/pull/973)、[#979](https://github.com/YimMenu/YimMenuV2/pull/979) | `a28e89e`、`6ed8d61`、`d297bdc`、`703bc45`、`907dd05` | 历史记录保留，内容已由 #988 的更新版本覆盖 |

完整的锁定上游提交和吸收方式见[下游合入记录](../maintenance/downstream-integrations.md)。

## 已完成检查

- `GPBD_FM`、`GPBD_FM_2`、`GPBD_FM_3` 已通过本机 Clang C++23 语法检查和结构尺寸 `static_assert`。
- PR #973 的新增特征码已完成 token 和十六进制格式检查。
- PR #988 更新的 33 个文件已完成冲突复核；与既有中文翻译及先期适配重叠的内容均已保留或采用 #988 的新值。
- 仓库内 Zig 0.16 工具链完成 443 个 Windows 目标 C/C++ 编译步骤，包括 `GameSkeletonUpdate`、异常处理、全部抢劫模块、Globals、Locals 和 Pointers。
- 合入时全仓库已通过 `git diff --check`，不存在冲突标记。
- 普通 `main` 推送不会触发 CI/CD；只有与 `VERSION` 一致的 `vMAJOR.MINOR.PATCH-zh-cn.REVISION` 标签会触发并通过构建发布。

## 崩溃定位与防护

PR #988 和 issue [#990](https://github.com/YimMenu/YimMenuV2/issues/990) 的两份独立日志都记录了相同故障：`EXCEPTION_ILLEGAL_INSTRUCTION`，游戏偏移为 `GTA5_Enhanced.exe + 0x1487620`，调用栈返回到旧版 `GameSkeletonUpdate.cpp:22` 的 `node->Run()`。结合“仅 YimMenu 玩家战局可用、公开或好友战局崩溃”的反馈，当前最强证据指向新版战局激活了未被旧名单拦截的完整性检查更新节点，而不是 #988 更新的抢劫数据本身。

下游适配做了两项处理：

- 除原有 `0xA0F39FB6` 和 `TamperActions` 外，同时拦截已知安全模块名称及伪装名称 `rageSecEngine`、`rageSecGamePluginManager`、`fwClothMeshing`、`CCreditsText`；组节点和 `Common Main` 子节点统一检查。
- 在每次调用更新节点前写入线程局部异常上下文。若仍然崩溃，日志会额外给出 update type、节点地址、组哈希、节点哈希和 `Run` 虚函数地址，便于锁定新版遗漏节点。

这是一项基于两份一致崩溃日志和静态逆向信息的保守修复，不是实机结论。若游戏日志仍出现相同异常，应优先检查新增的 `Active exception context [GameSkeletonUpdate(...)]` 行，而不是继续按旧偏移猜测。

## 尚未确认

- 尚未在 Windows/MSVC 或 GitHub Actions 上生成并验证有效 DLL。本机 Zig/CMake 虽完成全部编译和链接命令，但最终 `libYimMenuV2.dll` 被错误生成为 `ar archive`，不能算有效 Windows DLL。
- 尚未在 GTA `1.73 / 1158.13` 中完成注入、菜单打开和功能回归。
- PR #988 已更新公寓、末日、赌场和佩里科岛抢劫模块中的硬编码 `ScriptGlobal(...)` / `ScriptLocal(...)` 值，但没有游戏实测，仍不能确认分红、任务配置、收益修改或瞬间完成全部可用。
- PR #988 暂时禁用了失效的 `BattlEyeStatusUpdatePatch` 扫描和补丁；相关绕过能力不能视为已恢复。
- 豪宅相关车辆存储数据不等于豪宅抢劫支持；当前没有对应的任务配置、分红、收益或流程修改模块。

## 版本验收清单

- [ ] Windows `RelWithDebInfo` 全量构建成功。
- [ ] DLL 可在 GTA 5: Enhanced `1.73 / 1158.13` 主菜单注入。
- [ ] 菜单可打开、卸载且无崩溃。
- [ ] 公开战局和好友战局保持运行至少 15 分钟，日志中没有新的 `GameSkeletonUpdate` 异常上下文。
- [ ] 网络和加入战局相关更新通过实机验证。
- [ ] 车辆、衣柜和赌场相关更新通过实机验证。
- [ ] 四类现有抢劫功能逐项回归。
- [ ] 确认豪宅抢劫所需 stats、globals、locals、脚本和任务流程。
