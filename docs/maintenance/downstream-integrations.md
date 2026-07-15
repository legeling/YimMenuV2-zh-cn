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
| [#973](https://github.com/YimMenu/YimMenuV2/pull/973) | `187b910` | `703bc45` | 更新游戏升级后失效的网络伤害、事件确认、遥测、BattlEye 状态、加入战局和战局池相关特征码与偏移 | `cherry-pick -x` | 已提前吸收，等待官方处理 |
| [#970](https://github.com/YimMenu/YimMenuV2/pull/970) | `24219c5`、`b57c52b`、`5030953` | `a28e89e`、`6ed8d61`、`d297bdc` | 适配 GTA 1.73 / 1158.13：更新车辆列表、Script Global 基址、`GPBD_FM_2` 结构尺寸和衣柜脚本签名 | `cherry-pick -x` | 已提前吸收，等待官方处理 |
| [#979](https://github.com/YimMenu/YimMenuV2/pull/979) | `e075bd4` | `907dd05` | 更新赌场老虎机结果表和旋转状态的 script local 索引 | `cherry-pick -x` | 已提前吸收，等待官方处理 |
| [#967](https://github.com/YimMenu/YimMenuV2/pull/967) | `f47abeb` | `8e9491f` | 将 FSL 文件名说明从 `version.dll` 更新为 `WINMM.dll` | 手工适配中文 README | 已提前吸收，等待官方处理 |

## 验证记录

2026-07-15 对上述四个提前吸收的 PR 完成了以下检查：

- 核对 PR 基线、头提交和改动文件，确认审查期间提交未漂移。
- `GPBD_FM`、`GPBD_FM_2`、`GPBD_FM_3` 头文件通过本机 Clang C++23 语法检查和结构尺寸 `static_assert`。
- PR #973 的 6 组新增特征码通过 token 和十六进制格式检查。
- 全仓库通过 `git diff --check`，不存在冲突标记。
- 普通 `main` 推送未触发 CI/CD；只有 `v*-zh-cn.*` 标签会触发构建和发布。

本机没有完成 Windows 全量构建，也没有完成 GTA 1.73 / 1158.13 实机验证。PR #970 更新的是 typed global 基址，不包含抢劫分红功能中单独硬编码的 `ScriptGlobal(...)` 地址，因此不能据此确认分红功能已经适配该游戏版本。

## 维护规则

1. 提前吸收官方 PR 前，记录 PR URL、审查时的完整 head SHA，并使用锁定 SHA 合入。
2. 合入 `main` 前，在本文件中记录中文提交、改动目的、吸收方式、验证结果和已知风险。
3. 官方 PR 后续更新时，不自动跟随；重新审查新增提交后再决定是否补充吸收。
4. 官方 PR 合并或关闭后，更新表格状态。下次同步 `upstream/enhanced` 时检查重复补丁和冲突，再将记录标记为“已由官方基线覆盖”或“下游继续保留”。
5. 任何影响游戏版本兼容性的硬编码地址、特征码或 script local/global 变更，都必须单独列项，不能只写在提交信息里。
