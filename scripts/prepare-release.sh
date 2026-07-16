#!/usr/bin/env bash

set -euo pipefail

usage() {
	echo "Usage: $0 <major|minor|patch>" >&2
	exit 2
}

[[ $# -eq 1 ]] || usage
case "$1" in
	major|minor|patch) ;;
	*) usage ;;
esac

repo_root="$(git rev-parse --show-toplevel)"
cd "$repo_root"

if [[ "$(git branch --show-current)" != "main" ]]; then
	echo "Releases must be prepared from main." >&2
	exit 1
fi

if [[ -n "$(git status --porcelain)" ]]; then
	echo "The worktree must be clean before preparing a release." >&2
	exit 1
fi

if git rev-parse -q --verify refs/remotes/origin/main >/dev/null \
	&& ! git merge-base --is-ancestor origin/main HEAD; then
	echo "main is behind or has diverged from origin/main; synchronize it first." >&2
	exit 1
fi

current="$(tr -d '[:space:]' < VERSION)"
if [[ ! "$current" =~ ^([0-9]+)\.([0-9]+)\.([0-9]+)$ ]]; then
	echo "VERSION is not a semantic version: $current" >&2
	exit 1
fi

major="${BASH_REMATCH[1]}"
minor="${BASH_REMATCH[2]}"
patch="${BASH_REMATCH[3]}"

case "$1" in
	major) major=$((major + 1)); minor=0; patch=0 ;;
	minor) minor=$((minor + 1)); patch=0 ;;
	patch) patch=$((patch + 1)) ;;
esac

next="${major}.${minor}.${patch}"
tag="v${next}-zh-cn"

if git rev-parse -q --verify "refs/tags/${tag}" >/dev/null; then
	echo "Tag already exists: $tag" >&2
	exit 1
fi

printf '%s\n' "$next" > VERSION
git add VERSION
git commit -m "Bump version to ${next}"
git tag -a "$tag" -m "YimMenuV2 Chinese release ${next}"

echo "Prepared ${tag}. Review it, then publish with:"
echo "  git push origin main"
echo "  git push origin ${tag}"
