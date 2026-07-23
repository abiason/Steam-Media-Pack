# Git Workflow

This document describes the Git workflow adopted by Steam Media Pack.

The project follows a Git Flow–inspired workflow to keep development organized and maintain stable release branches.

---

# Branch Strategy

```text
feature/*
        │
        ▼
develop
        │
        ▼
homolog
        │
        ▼
main
```

---

# Branches

| Branch | Purpose |
|---------|---------|
| `feature/*` | New features, improvements, and documentation. |
| `develop` | Integration branch for ongoing development. |
| `homolog` | Pre-release validation and testing. |
| `main` | Stable production releases. |

---

# Development Flow

1. Create a feature branch from `develop`.
2. Implement the changes.
3. Open a Pull Request to `develop`.
4. After review, merge into `develop`.
5. Promote tested changes to `homolog`.
6. Merge validated releases into `main`.

---

# Branch Naming

Use descriptive branch names.

Examples:

```text
feature/developer-guide
feature/library-artwork
feature/browser-detection
feature/netflix-launcher
feature/gui-installer
```

---

# Commit Messages

Follow the Conventional Commits specification.

Examples:

```text
feat: add Disney+ launcher

fix: detect Brave browser correctly

docs: update developer guide

refactor: simplify browser detection

chore: update project assets
```

---

# Pull Requests

Every Pull Request should:

- have a clear title;
- describe the implemented changes;
- remain focused on a single objective;
- be reviewed before merging.

---

# General Guidelines

- Never commit directly to `main`.
- Never commit directly to `homolog`.
- Keep feature branches small.
- Prefer multiple small Pull Requests over one large Pull Request.
- Keep `develop` stable whenever possible.
