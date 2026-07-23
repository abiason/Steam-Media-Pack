# Project Structure

This document describes the official directory structure of Steam Media Pack.

Maintaining a consistent project structure improves maintainability, simplifies navigation, and helps contributors understand where new files belong.

---

# Repository Layout

```text
Steam-Media-Pack/
│
├── assets/
├── docs/
│   ├── adr/
│   ├── assets/
│   ├── developer-guide/
│   ├── reference/
│   └── user-guide/
│
├── launchers/
├── planning/
├── tools/
│
├── README.md
├── README.pt-BR.md
├── ROADMAP.md
└── LICENSE
```

---

# Directory Overview

| Directory | Purpose |
|-----------|---------|
| assets/ | Project assets used by the application. |
| docs/ | User and developer documentation. |
| launchers/ | Streaming service launchers. |
| planning/ | Internal planning documents. |
| tools/ | Installation, maintenance, and Steam integration scripts. |

---

# Documentation

```text
docs/
├── adr/
├── assets/
├── developer-guide/
├── reference/
└── user-guide/
```

This directory contains all project documentation.

---

# Launchers

```text
launchers/
```

Contains one launcher for each supported streaming service.

Launchers should only contain service-specific logic.

---

# Tools

```text
tools/
```

Contains utility scripts responsible for:

- installation;
- Steam shortcuts;
- library artwork;
- backup and restore;
- diagnostics;
- maintenance.

---

# Assets

```text
assets/
```

Contains project assets used during installation and runtime.

Documentation images belong in:

```text
docs/assets/
```

---

# Planning

```text
planning/
```

Contains internal project planning, including:

- product planning;
- milestones;
- backlog;
- architecture decisions;
- release planning.

These documents are intended for maintainers.

---

# Root Files

| File | Purpose |
|------|---------|
| README.md | Official project documentation (English). |
| README.pt-BR.md | Portuguese translation. |
| ROADMAP.md | Public roadmap. |
| LICENSE | Project license. |

---

# Guidelines

- Keep related files together.
- Avoid duplicate documentation.
- Prefer reusable tools over duplicated scripts.
- Keep user documentation separate from developer documentation.
- Keep planning documents separate from public documentation.
