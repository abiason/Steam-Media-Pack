# Launcher Development

This document describes how to add a new streaming service to Steam Media Pack.

---

# Responsibilities

Each launcher should:

- launch a single streaming service;
- load the project configuration;
- detect the selected launch mode;
- locate a supported browser;
- apply browser arguments;
- open the streaming service.

Launchers should not modify Steam shortcuts or artwork.

---

# Directory

Each launcher belongs in:

```text
launchers/
```

---

# Naming

Use lowercase with hyphens.

Example:

```text
netflix.sh
disney-plus.sh
prime-video.sh
```

---

# Browser Support

Launchers should support all browsers officially supported by the project.

Browser detection should reuse existing project logic.

---

# Configuration

Never hard-code:

- browser paths;
- Steam paths;
- user directories.

Always use centralized configuration.

---

# Error Handling

Launchers should:

- validate required commands;
- provide clear error messages;
- exit with a non-zero status on failure.

---

# Testing

Before submitting a Pull Request, verify:

- launcher starts correctly;
- supported browsers are detected;
- all launch modes work correctly;
- Steam shortcut launches successfully.

---

# Guidelines

- Keep launchers simple.
- Reuse existing code whenever possible.
- Avoid duplicated logic.
- Follow the project's coding standards.
