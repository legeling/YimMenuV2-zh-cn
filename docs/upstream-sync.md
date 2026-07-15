# 上游同步指南

本文档说明中文维护仓库与官方 YimMenuV2 的关系，以及维护者同步上游改动时应遵循的流程。

## 仓库关系

- `origin`：`git@github.com:legeling/YimMenuV2-zh-cn.git`
- `upstream`：`git@github.com:YimMenu/YimMenuV2.git`
- 中文主线：`main`
- 官方同步来源：`upstream/enhanced`

首次克隆中文仓库后，添加官方上游：

```bash
git clone git@github.com:legeling/YimMenuV2-zh-cn.git
cd YimMenuV2-zh-cn
git remote add upstream git@github.com:YimMenu/YimMenuV2.git
git fetch upstream --prune --no-tags
```

使用以下命令检查配置：

```bash
git remote -v
git branch -vv
```

## 同步流程

1. 确认工作区干净，并刷新两个远端。

```bash
git status --short --branch
git fetch origin --prune --no-tags
git fetch upstream --prune --no-tags
```

2. 查看官方新增提交和双方分叉情况。

```bash
git log --oneline main..upstream/enhanced
git rev-list --left-right --count main...upstream/enhanced
```

3. 从最新中文主线创建同步分支，再合并官方分支。

```bash
git switch main
git pull --ff-only origin main
git switch -c sync/upstream-YYYYMMDD
git merge --no-ff upstream/enhanced
```

4. 处理冲突并完成验证。解决冲突时应同时保留：

- 上游新增的功能、接口、指针和兼容性修复。
- 中文菜单文本、本地化资源和中文发行配置。
- `README.md` 中的中文仓库身份与下载入口。
- `.github/workflows/nightly.yml` 中的中文版本发布规则。
- `docs/downstream-integrations.md` 中的下游合入来源和状态记录。

不要直接用 `--ours` 或 `--theirs` 覆盖整个冲突文件。生成文件与其生成脚本应保持一致。

5. 更新合入记录。官方基线变化和提前吸收的官方 PR 都必须写入 `docs/downstream-integrations.md`，至少记录来源链接、锁定 SHA、中文提交、验证边界和当前状态。

6. 将同步分支推送到中文仓库，完成代码审查和可用的本地验证后合入 `main`。普通分支推送不会触发 CI；完整 CI/CD 只在正式中文版本标签上运行。

```bash
git push -u origin sync/upstream-YYYYMMDD
```

## 标签与发布

官方上游和中文仓库都可能使用 `nightly` 等同名标签。刷新分支时使用 `--no-tags`，避免远端标签覆盖本地同名标签。中文正式版本使用 `v*-zh-cn.*` 格式的标签。只有推送这种格式的标签时才会执行 CI 构建和中文 Release 发布；普通分支推送不会触发 CI/CD。

同步完成后，应确认 `main` 同时包含 `upstream/enhanced` 的最新提交和中文维护提交：

```bash
git merge-base --is-ancestor upstream/enhanced main
git status --short --branch
```
