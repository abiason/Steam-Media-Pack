# Architecture

This document describes the high-level architecture of Steam Media Pack.

Its purpose is to define the project's main components, their responsibilities, and how they interact.

This document is intended for contributors and maintainers.

---

# Goals

The architecture is designed to:

- keep the project modular;
- minimize code duplication;
- simplify maintenance;
- isolate responsibilities;
- support new streaming services;
- remain portable across supported platforms.

---

# High-Level Architecture

```text
                    Steam Media Pack
                           │
     ┌─────────────────────┼─────────────────────┐
     │                     │                     │
     ▼                     ▼                     ▼
Service Launchers   Steam Integration   Browser Integration
     │                     │                     │
     └──────────────┬──────┴──────────────┬──────┘
                    ▼                     ▼
             Configuration         Library Artwork
                    │
                    ▼
              Maintenance Tools
```

---

# Components

| Component | Responsibility |
|-----------|----------------|
| Service Launchers | Launch supported streaming services. |
| Steam Integration | Create and maintain Steam shortcuts. |
| Browser Integration | Detect and configure supported browsers. |
| Configuration | Centralize project settings. |
| Library Artwork | Install and update Steam Library artwork. |
| Maintenance Tools | Installation, backup, restore, diagnostics, and maintenance utilities. |

---

# Data Flow

## Service Launch

```text
User
   │
   ▼
Steam Library
   │
   ▼
Service Launcher
   │
   ▼
Configuration
   │
   ▼
Browser Detection
   │
   ▼
Launch Mode
   │
   ▼
Streaming Service
```

## Installation

```text
Installation
      │
      ▼
Locate Steam
      │
      ▼
Create Backup
      │
      ▼
Install Shortcuts
      │
      ▼
Install Library Artwork
      │
      ▼
Validation
```

---

# Directory Responsibilities

```text
docs/
```

Project documentation.

```text
planning/
```

Internal planning and product management.

```text
launchers/
```

Streaming service launchers.

```text
tools/
```

Installation, Steam integration, artwork, backup, restore, diagnostics, and maintenance scripts.

```text
assets/
```

Project assets.

---

# Design Principles

- **Single Responsibility** – Each component has one responsibility.
- **Modularity** – Components should remain independent.
- **Reusability** – Shared logic should be centralized.
- **Safe Modification** – Always back up Steam data before modifying it.
- **Idempotency** – Repeated executions must not create duplicate shortcuts.
- **Portability** – Detect system paths dynamically.
- **Separation of Concerns** – Documentation, launchers, tools, assets, and planning remain separated.

---

# Related Documentation

- Project Structure
- Git Workflow
- Coding Standards
- Launcher Development
- Release Process
- Product Specification
- Architecture Decision Records
