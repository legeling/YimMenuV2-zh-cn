# GTA 1.73 / 1158.13 兼容性

本文档记录中文维护版针对 GTA 5: Enhanced `1.73 / 1158.13` 的适配状态。它描述的是当前代码和验证边界，不代表所有菜单功能已经通过游戏实测。

## 状态摘要

| 范围 | 来源 | 中文提交 | 状态 |
| --- | --- | --- | --- |
| 网络、遥测、BattlEye 状态、加入战局相关特征码 | 上游 PR [#973](https://github.com/YimMenu/YimMenuV2/pull/973) | `703bc45` | 已合入，未实机验证 |
| 新增车辆列表 | 上游 PR [#970](https://github.com/YimMenu/YimMenuV2/pull/970) | `a28e89e` | 已合入，未实机验证 |
| Script Global 基址和 `GPBD_FM_2` 结构尺寸 | 上游 PR [#970](https://github.com/YimMenu/YimMenuV2/pull/970) | `6ed8d61` | 已合入，通过本机语法和结构尺寸检查 |
| 衣柜脚本签名 | 上游 PR [#970](https://github.com/YimMenu/YimMenuV2/pull/970) | `d297bdc` | 已合入，未实机验证 |
| 赌场老虎机 script local 索引 | 上游 PR [#979](https://github.com/YimMenu/YimMenuV2/pull/979) | `907dd05` | 已合入，未实机验证 |

完整的锁定上游提交和吸收方式见[下游合入记录](../maintenance/downstream-integrations.md)。

## 已完成检查

- `GPBD_FM`、`GPBD_FM_2`、`GPBD_FM_3` 已通过本机 Clang C++23 语法检查和结构尺寸 `static_assert`。
- PR #973 的新增特征码已完成 token 和十六进制格式检查。
- 合入时全仓库已通过 `git diff --check`，不存在冲突标记。
- 普通 `main` 推送不会触发 CI/CD；只有 `v*-zh-cn.*` 标签会触发构建和发布。

## 尚未确认

- 尚未在 Windows 上完成全量构建。
- 尚未在 GTA `1.73 / 1158.13` 中完成注入、菜单打开和功能回归。
- 抢劫模块仍包含独立硬编码的 `ScriptGlobal(...)` 和 `ScriptLocal(...)` 地址。PR #970 更新 typed global 基址，并不能证明分红、任务配置、收益修改或瞬间完成已经兼容。
- 豪宅相关车辆存储数据不等于豪宅抢劫支持；当前没有对应的任务配置、分红、收益或流程修改模块。

## 版本验收清单

- [ ] Windows `RelWithDebInfo` 全量构建成功。
- [ ] DLL 可在 GTA 5: Enhanced `1.73 / 1158.13` 主菜单注入。
- [ ] 菜单可打开、卸载且无崩溃。
- [ ] 网络和加入战局相关更新通过实机验证。
- [ ] 车辆、衣柜和赌场相关更新通过实机验证。
- [ ] 四类现有抢劫功能逐项回归。
- [ ] 确认豪宅抢劫所需 stats、globals、locals、脚本和任务流程。
