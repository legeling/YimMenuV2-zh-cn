# 下游合入记录

本文档记录中文维护版已经吸收的官方提交和上游尚未合并的 PR。Git 历史负责保存代码，本文档负责说明来源、目的、验证边界和后续同步状态。

维护者在向 `main` 合入新的上游提交或提前吸收官方 PR 时，必须同步更新本文档。

游戏版本兼容性结论见 [`../compatibility/`](../compatibility/)，用户功能的实际支持范围见 [`../features/`](../features/)。PR 已合入不等于受影响功能已经通过实机验证。

## 当前官方基线

- 官方仓库：[YimMenu/YimMenuV2](https://github.com/YimMenu/YimMenuV2)
- 官方分支：`enhanced`
- 已同步官方提交：`ef1a4df2162a9c87d8232285c88147fadbad8be3`
- 中文仓库合并提交：`ba758c09368eafba285a72285c325f4e35e1fe81`
- 同步日期：2026-07-15

本次官方基线包含：

| 官方 PR | 官方提交 | 内容 | 状态 |
| --- | --- | --- | --- |
| [#949](https://github.com/YimMenu/YimMenuV2/pull/949) | `017904b` | 扩展 Lua 菜单、ImGui、文件、内存、指针、命令、玩家、实体、事件和脚本相关 API，并补充 Lua 文档与类型提示 | 已由官方合并，已随 `upstream/enhanced` 同步 |
| [#952](https://github.com/YimMenu/YimMenuV2/pull/952) | `628f2fd` | 更新 `GetAvatars` 特征码 | 已由官方合并，已随 `upstream/enhanced` 同步 |
| [#963](https://github.com/YimMenu/YimMenuV2/pull/963) | `ef1a4df` | 修复 Lua Pattern 扫描错误指向旧版 `GTA5.exe` 的问题 | 已由官方合并，已随 `upstream/enhanced` 同步 |

## 提前吸收的官方 PR

以下 PR 在合入中文仓库时仍处于官方未合并状态。为避免 PR 后续更新被无意带入，中文仓库按审查时锁定的上游提交进行吸收。

| 官方 PR | 锁定上游提交 | 中文提交 | 内容 | 吸收方式 | 当前状态 |
| --- | --- | --- | --- | --- | --- |
| [#988](https://github.com/YimMenu/YimMenuV2/pull/988) | `e65380a3b8cab46a20d18ee6bf89d3e0ca40f9f2` | `213eebd`、`3d30df7` | 全量适配 b1158.13 的网络、脚本、车辆、Globals、Locals、日常活动及四类抢劫硬编码数据；下游追加 `GameSkeletonUpdate` 安全节点拦截和异常上下文，处理公开/好友战局非法指令崩溃 | `cherry-pick -x` 后解决与 #970、#973、#979 及中文翻译的冲突，再追加审查修复 | 已提前吸收，等待实机验证和官方处理 |
| [#985](https://github.com/YimMenu/YimMenuV2/pull/985) | `3037a5e` | `29bfba5`、`c8124bb`、`13756c0`、`a56e02d` | 修复通知并发、Hook 状态、渲染帧同步、Lua 资源循环、PatternScanner 性能和启动错误处理；下游同时完善近地址跳板分配、完整镜像安全扫描、Call Site/IAT 内存保护、异常上下文、宏安全及 Hook 失败回滚 | `cherry-pick -x` 后追加审查修复 | 已提前吸收，等待官方处理 |
| [#973](https://github.com/YimMenu/YimMenuV2/pull/973) | `187b910` | `703bc45` | 更新游戏升级后失效的网络伤害、事件确认、遥测、BattlEye 状态、加入战局和战局池相关特征码与偏移 | `cherry-pick -x` | 历史先期适配；已由 #988 的新值覆盖 |
| [#970](https://github.com/YimMenu/YimMenuV2/pull/970) | `24219c5`、`b57c52b`、`5030953` | `a28e89e`、`6ed8d61`、`d297bdc` | 适配 GTA 1.73 / 1158.13：更新车辆列表、Script Global 基址、`GPBD_FM_2` 结构尺寸和衣柜脚本签名 | `cherry-pick -x` | 历史先期适配；已由 #988 的新值覆盖 |
| [#979](https://github.com/YimMenu/YimMenuV2/pull/979) | `e075bd4` | `907dd05` | 更新赌场老虎机结果表和旋转状态的 script local 索引 | `cherry-pick -x` | 历史先期适配；已由 #988 的新值覆盖 |
| [#967](https://github.com/YimMenu/YimMenuV2/pull/967) | `f47abeb` | `8e9491f` | 将 FSL 文件名说明从 `version.dll` 更新为 `WINMM.dll` | 手工适配中文 README | 已提前吸收，等待官方处理 |

## 验证记录

2026-07-15 至 2026-07-16 对上述提前吸收的 PR 完成了以下检查：

- 核对 PR 基线、头提交和改动文件，确认审查期间提交未漂移。
- `GPBD_FM`、`GPBD_FM_2`、`GPBD_FM_3` 头文件通过本机 Clang C++23 语法检查和结构尺寸 `static_assert`。
- PR #973 的 6 组新增特征码通过 token 和十六进制格式检查。
- PR #985 按文件复审，并处理了官方 Copilot 指出的 5 项问题：检查 `VirtualProtect` 结果、避免无效的任意地址跳板、保持异常上下文一致，以及将 `REL` 宏改为语句安全形式。
- PR #985 的 `APPDATA` 失败路径改为直接卸载，避免 `goto` 跨越 `std::filesystem::path` 初始化。
- PatternScanner 保留 `SizeOfImage` 完整镜像范围，但改为只扫描已提交且可读的内存区域；缓存命中前会重新校验地址、保护属性和签名字节。
- Call Hook 跳板改为围绕实际调用点搜索空闲区域，并对每个跳板验证 `rel32` 位移和容量，不再依赖固定地址或不受约束的回退分配。
- 后续白盒复审修复了 Hook 在 `MH_ApplyQueued` 前提前提交状态、批量应用失败后未回滚、构造失败遗留悬空注册指针和初始化失败仍继续启动的问题。
- PatternScanner 现在会收集全部异步任务结果，手动映射模式也会扫描全部待查模式；缓存和内存区域边界计算增加了防越界及防整数溢出检查。
- 异常恢复逻辑改为线程局部去重，并修正 `REX.R` 对 `R8` 至 `R15` 寄存器编号的扩展计算；同时清除了 Windows `ERROR` 宏导致的日志级别编译错误。
- PR #988 锁定单提交 `e65380a` 合入；33 个变更文件与既有 #970、#973、#979 补丁逐项解决冲突，并保留中文翻译。
- PR #988 与 issue #990 的两份独立日志均为 `EXCEPTION_ILLEGAL_INSTRUCTION`、`GTA5_Enhanced.exe + 0x1487620`，且调用栈返回旧版 `GameSkeletonUpdate.cpp:22`。下游据此扩展完整性检查节点名单，并记录每次 `Run()` 的节点哈希和虚函数地址。
- 2026-07-17 的中文维护版实机日志进一步锁定 `Common Main / CNetwork` 节点，并确认发生在无 FSL 的 `Vanilla` 路径。下游恢复上游历史中的短特征 BattlEye 状态补丁、移除易漂移的固定偏移，并增加非法指令机器码日志；该修复仍需公共及好友战局复测。
- `v1.0.2-zh-cn` 实测证明短特征状态补丁已经应用但仍在同一节点崩溃；机器码和栈回溯进一步锁定 `+0x1487620` 陷阱及 `+0x1485806` 返回点。后续工作区修复只在异常类型、节点上下文、RIP、16 字节机器码和栈顶返回地址全部吻合时模拟叶子返回，避免吞掉其他异常或屏蔽整个 `CNetwork`。
- PR #988 及下游崩溃防护使用 Zig 0.16 完成 443 个 Windows 目标编译步骤；本次涉及的全部 C/C++ 编译单元均成功。
- 全仓库通过 `git diff --check`，不存在冲突标记。
- 普通 `main` 推送未触发 CI/CD；只有与 `VERSION` 一致的 `vMAJOR.MINOR.PATCH-zh-cn` 标签会触发并通过构建发布。
- `v1.0.2-zh-cn` 已通过 GitHub Actions 的 Windows 原生 Clang 构建并成功生成中文 Release；这证明源码可编译并能产出 PE DLL，但不能替代游戏注入和进战局测试。
- `v1.0.3-zh-cn` 已通过同一 Windows 原生构建和 PE 校验并生成版本化 DLL；原生编译任务耗时 12 分 44 秒，其中 CMake 配置约 69 秒、449 个编译单元及链接约 11 分 5 秒。
- 发布流水线只保留一次 Windows 原生编译，Release 直接复用同一份已验证产物；移除重复发布构建和不能证明 DLL 格式有效的 Zig CI 构建。
- 后续发布接入 `sccache` 的 GitHub Actions 内容缓存，并使用嵌入式调试信息提高 Clang/MSVC ABI 对象的可缓存性；首次运行负责填充缓存，后续版本通过缓存统计和总耗时复核收益，不缓存整个构建目录以避免按时间戳误用旧对象。

本机 macOS 没有完成有效 Windows DLL 构建，GTA 1.73 / 1158.13 实机验证也仍未完成。PR #988 已更新抢劫分红及任务功能中单独硬编码的 `ScriptGlobal(...)` / `ScriptLocal(...)` 地址，但静态更新和 GitHub 原生编译都不能替代游戏回归，因此仍不能确认这些功能已经适配。

PR #985 与 #988 已使用仓库内 Zig 工具链完成全部 C/C++ 编译单元检查，并据此发现和修复日志级别错误；但本机 macOS 的 CMake 4.3 / Zig 0.16 组合把最终 `.dll` 链接成了归档文件，因此该结果不能视为有效 Windows DLL。Hook、Renderer、异常处理、游戏地址和抢劫数据改动仍必须在 Windows 构建及游戏注入环境中继续验证。

## 维护规则

1. 提前吸收官方 PR 前，记录 PR URL、审查时的完整 head SHA，并使用锁定 SHA 合入。
2. 合入 `main` 前，在本文件中记录中文提交、改动目的、吸收方式、验证结果和已知风险。
3. 官方 PR 后续更新时，不自动跟随；重新审查新增提交后再决定是否补充吸收。
4. 官方 PR 合并或关闭后，更新表格状态。下次同步 `upstream/enhanced` 时检查重复补丁和冲突，再将记录标记为“已由官方基线覆盖”或“下游继续保留”。
5. 任何影响游戏版本兼容性的硬编码地址、特征码或 script local/global 变更，都必须单独列项，不能只写在提交信息里。
