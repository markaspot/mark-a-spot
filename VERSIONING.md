# Version Management

> **Full documentation**: See [markaspot-docs/development/versioning.md](./markaspot-docs/development/versioning.md)

## Quick Reference

```bash
cd frontend

# Interactive release (recommended)
pnpm release

# Prerelease versions
pnpm release:alpha    # 2.0.0-alpha.1 → 2.0.0-alpha.2
pnpm release:beta     # → 2.0.0-beta.0
pnpm release:rc       # → 2.0.0-rc.0

# Preview changes without releasing
pnpm release:dry
```

## What It Does

1. Runs lint and i18n checks
2. Prompts for version bump
3. Updates package.json and CHANGELOG.md
4. Creates commit and tag
5. Pushes to GitHub
6. Creates GitHub Release

## Branch Strategy

| Branch | Version Track |
|--------|---------------|
| dev | v1.x.x (legacy) |
| dev-2.x | v2.x.x (active) |
| client/* | clientname-vX.X.X |

## Client Deployment

```bash
git checkout client/bonn
git merge dev-2.x
git tag bonn-v2.1.0
git push origin client/bonn --tags
```
