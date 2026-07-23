# Coding Standards

This document defines the coding standards adopted by Steam Media Pack.

Following consistent standards improves readability, maintainability, and collaboration.

---

# General Principles

- Write clear and readable code.
- Prefer simplicity over cleverness.
- Avoid duplicated logic.
- Keep functions small and focused.
- Use descriptive names.

---

# Shell Scripts

- Use `bash`.
- Start scripts with:

```bash
#!/usr/bin/env bash
set -euo pipefail
```

- Quote variable expansions.
- Use meaningful function names.
- Validate external dependencies before using them.
- Return non-zero exit codes on failure.

---

# Python

- Follow PEP 8.
- Use type hints whenever practical.
- Keep functions focused on a single responsibility.
- Catch expected exceptions and report meaningful errors.
- Avoid broad `except:` blocks.

---

# File Naming

Use lowercase with hyphens.

Examples:

```text
install-steam-shortcuts.sh
restore-steam-shortcuts.sh
update-steam-arts.py
```

---

# Documentation

- Write documentation in English.
- Keep documentation concise.
- Update documentation whenever behavior changes.

---

# Comments

Write comments only when they explain *why*, not *what*.

Avoid obvious comments.

---

# Logging

Log meaningful events:

- operation started;
- operation completed;
- warnings;
- errors.

Never log sensitive information.

---

# Error Handling

Always:

- validate inputs;
- check required files;
- fail fast on unrecoverable errors;
- provide actionable error messages.

---

# Formatting

- Use 4 spaces for Python.
- Use consistent indentation in shell scripts.
- End files with a newline.
- Remove trailing whitespace.
