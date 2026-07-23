# Release Process

This document describes the release process adopted by Steam Media Pack.

---

# Release Flow

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

# Development

New work starts in a feature branch created from `develop`.

Example:

```text
feature/browser-detection
feature/developer-guide
feature/netflix-launcher
```

After review, changes are merged into `develop`.

---

# Homologation

The `homolog` branch is used for release validation.

Typical activities include:

- functional testing;
- documentation review;
- packaging validation;
- regression testing.

Only validated changes should be promoted to production.

---

# Production

The `main` branch contains stable releases.

Every release should:

- pass validation;
- include updated documentation;
- include release notes;
- be tagged.

---

# Versioning

The project follows Semantic Versioning.

```text
MAJOR.MINOR.PATCH
```

Examples:

```text
11.0.0
11.1.0
11.1.1
12.0.0
```

---

# Release Checklist

Before creating a release:

- Update documentation.
- Update the roadmap if necessary.
- Verify all CI workflows.
- Validate installation.
- Validate Steam integration.
- Validate launchers.
- Validate library artwork.

---

# Git Tags

Each production release should receive a Git tag.

Example:

```text
v11.1.0
v11.2.0
v12.0.0
```

---

# General Guidelines

- Keep releases small and focused.
- Avoid mixing unrelated features.
- Ensure documentation matches the released version.
- Never release untested changes.
