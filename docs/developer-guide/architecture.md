# Architecture

This document describes the high-level architecture of Steam Media Pack and how its main components interact.

Steam Media Pack integrates streaming services into the Steam Library by creating non-Steam shortcuts, configuring launch commands, installing library artwork, and providing maintenance tools.

---

## Architecture Overview

Steam Media Pack is organized into four main layers:

```text
User
  │
  ▼
Steam Library
  │
  ▼
Service Launchers
  │
  ▼
Web Browser
  │
  ▼
Streaming Service
```

Supporting tools manage:

- installation;
- configuration;
- Steam shortcuts;
- library artwork;
- backups;
- diagnostics;
- launch modes.

---

## Main Components

```text
Steam Media Pack
│
├── Service Launchers
├── Steam Integration
├── Browser Integration
├── Library Artwork
├── Configuration
├── Backup and Restore
└── Diagnostic Tools
```

---

## Service Launchers

Each supported streaming service has its own launcher.

The launcher is responsible for:

1. loading the project configuration;
2. detecting the selected launch mode;
3. locating a compatible browser;
4. applying browser arguments;
5. opening the streaming service;
6. writing diagnostic information when necessary.

Launchers should remain small and focused on starting the service.

Shared behavior should be implemented through reusable functions or centralized configuration rather than duplicated across launchers.

---

## Launch Modes

Steam Media Pack supports three launch modes:

- Auto Mode
- App Mode
- Kiosk Mode

The selected mode determines which browser arguments are used when starting a service.

### Auto Mode

Auto Mode selects the most appropriate behavior based on the current environment and available configuration.

### App Mode

App Mode opens the streaming service in a standalone browser window with minimal browser interface.

### Kiosk Mode

Kiosk Mode opens the streaming service in fullscreen kiosk mode.

Launch mode selection is managed by:

```text
select-mode.sh
```

For user-facing information, see:

- [Launch Modes](../user-guide/launch-modes.md)

---

## Browser Integration

Steam Media Pack uses Chromium-based browsers to access streaming services.

Supported browsers may include:

- Chromium
- Google Chrome
- Microsoft Edge
- Brave

The launcher detects an available browser and starts it with the arguments required by the selected launch mode.

The browser layer is responsible only for presenting the streaming service. Authentication, playback availability, DRM support, and service restrictions remain controlled by the browser and streaming provider.

---

## Steam Integration

Steam integration is handled by tools responsible for creating and maintaining non-Steam shortcuts.

Main scripts:

```text
tools/install-steam-shortcuts.py
tools/install-steam-shortcuts.sh
tools/restore-steam-shortcuts.sh
```

The integration process includes:

1. locating the Steam installation;
2. identifying the Steam user data directory;
3. locating the shortcut configuration;
4. creating a backup;
5. adding or updating service shortcuts;
6. preserving unrelated existing shortcuts;
7. saving the updated configuration.

Steam must normally be closed before shortcut files are modified.

For user-facing information, see:

- [Steam Integration](../user-guide/steam-integration.md)

---

## Library Artwork

Steam Media Pack installs artwork for each streaming service so that shortcuts appear consistently inside the Steam Library.

Artwork management is handled by:

```text
tools/update-steam-arts.py
tools/update-steam-arts.sh
```

Supported artwork categories may include:

```text
Grid
Hero
Logo
Icon
```

The artwork tool associates image files with the corresponding Steam shortcut identifiers and installs them in the appropriate Steam grid directory.

For user-facing information, see:

- [Library Artwork](../user-guide/library-artwork.md)

---

## Configuration

Configuration controls project behavior without requiring changes to individual launchers.

Configuration responsibilities include:

- launch mode selection;
- browser selection;
- browser arguments;
- display behavior;
- service URLs;
- log locations;
- backup locations.

Configuration should be centralized whenever possible.

Launchers should read configuration values rather than contain duplicated hard-coded settings.

---

## Backup and Restore

Steam Media Pack creates backups before modifying Steam shortcut data.

The backup layer protects the user's existing Steam configuration and enables recovery when:

- shortcut installation fails;
- generated entries are incorrect;
- Steam rewrites the shortcuts file;
- the user wants to return to a previous state.

The restore process is handled by:

```text
tools/restore-steam-shortcuts.sh
```

Backup operations must be completed before any destructive or irreversible modification.

For user-facing information, see:

- [Backup and Restore](../user-guide/backup-and-restore.md)

---

## Diagnostics

Diagnostics are handled by:

```text
diagnostics.sh
```

The diagnostic process may inspect:

- operating system information;
- desktop session;
- available browsers;
- Steam installation paths;
- required commands;
- project files;
- file permissions;
- current launch mode;
- generated logs.

Diagnostics should avoid changing the system. Their primary responsibility is to collect information that helps identify installation and runtime problems.

---

## Data Flow

The normal launch flow is:

```text
User selects a service in Steam
              │
              ▼
Steam executes the service launcher
              │
              ▼
Launcher reads the current configuration
              │
              ▼
Launcher detects an available browser
              │
              ▼
Launch mode arguments are applied
              │
              ▼
Browser opens the streaming service
```

The installation flow is:

```text
Installation command
        │
        ▼
Validate project files
        │
        ▼
Locate Steam installation
        │
        ▼
Create shortcut backup
        │
        ▼
Install or update shortcuts
        │
        ▼
Install library artwork
        │
        ▼
Validate the result
```

---

## Directory Responsibilities

The project directories follow these responsibilities:

```text
docs/
```

Project documentation for users, contributors, and maintainers.

```text
planning/
```

Internal product planning, milestones, risks, and future ideas.

```text
tools/
```

Installation, Steam integration, artwork, backup, and maintenance utilities.

```text
docs/assets/
```

Documentation images, diagrams, screenshots, and branding resources.

Service-specific launcher directories should contain only files directly related to launching that service.

---

## Design Principles

### Single Responsibility

Each script or module should have one clearly defined responsibility.

Examples:

- launchers start services;
- shortcut tools manage Steam shortcuts;
- artwork tools manage library artwork;
- diagnostic tools collect system information.

### Minimal Duplication

Shared logic should not be duplicated across multiple service launchers.

Reusable behavior should be centralized.

### Safe Modification

Steam configuration must be backed up before modification.

Existing user shortcuts must be preserved whenever possible.

### Explicit Failure

Scripts should return clear errors when an operation cannot be completed.

Failures should not be silently ignored.

### Portability

Paths and environment details should be detected dynamically instead of assuming a fixed installation layout.

### Idempotency

Running installation or update tools multiple times should not create duplicate Steam shortcuts or corrupt existing configuration.

### Separation of Concerns

User documentation, developer documentation, planning files, scripts, artwork, and runtime configuration must remain separated.

---

## Error Handling

Scripts should:

- validate required files before execution;
- verify external commands before using them;
- quote filesystem paths;
- stop on critical failures;
- provide actionable error messages;
- avoid partial configuration changes;
- restore or preserve backups after failure.

Shell scripts should use strict execution rules where appropriate:

```bash
set -euo pipefail
```

Exceptions must be documented when a script cannot use strict mode.

Python tools should:

- validate input;
- catch expected filesystem and parsing errors;
- avoid broad exception handling without reporting details;
- return a non-zero exit code when execution fails.

---

## Logging

Logs should contain enough information to reproduce a failure without exposing sensitive user information.

Recommended log information:

- script name;
- timestamp;
- detected browser;
- selected launch mode;
- detected Steam path;
- executed operation;
- success or failure;
- relevant error messages.

Logs should not contain:

- passwords;
- authentication tokens;
- browser cookies;
- session credentials;
- private account data.

---

## Security Boundaries

Steam Media Pack does not manage streaming service credentials.

Authentication is performed directly through the selected browser.

The project should never:

- collect passwords;
- store authentication tokens;
- copy browser profiles;
- modify browser credential databases;
- bypass DRM or service access restrictions.

---

## Extension Points

The architecture should allow future additions without redesigning the entire project.

Expected extension points include:

- additional streaming services;
- additional Chromium-based browsers;
- new launch modes;
- YAML-based configuration;
- graphical installer;
- plugin system;
- Flatpak packaging;
- AppImage packaging;
- automatic updates.

New components should follow the same separation of responsibilities described in this document.

---

## Related Documentation

- [Project Structure](project-structure.md)
- [Git Workflow](git-workflow.md)
- [Coding Standards](coding-standards.md)
- [Launcher Development](launcher-development.md)
- [Release Process](release-process.md)
- [Product Specification](../reference/product-specification.md)
- [Architecture Decision Records](../adr/)
