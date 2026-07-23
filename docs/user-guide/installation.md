# Installation

This guide explains how to install Steam Media Pack on a supported Linux distribution.

---

# Requirements

Before installing, ensure you have:

- Linux
- Steam installed
- A supported Chromium-based browser
- Git
- Bash

---

# Clone the Repository

```bash
git clone https://github.com/abiason/Steam-Media-Pack.git

cd Steam-Media-Pack
```

---

# Grant Execution Permissions

```bash
chmod +x install.sh uninstall.sh diagnostics.sh select-mode.sh

chmod +x launchers/*.sh

chmod +x tools/*.sh

chmod +x tools/*.py
```

---

# Run the Installer

```bash
./install.sh
```

The installer will:

- Detect your Steam installation
- Install launchers
- Install Steam artwork
- Create backups when necessary
- Configure the selected launch mode

---

# Verify the Installation

Open Steam.

Your streaming services should appear in the Library.

---

# Next Steps

After installation you may wish to:

- Change the launch mode
- Update artwork
- Restore a backup
- Run diagnostics

These topics are covered in the User Guide.

---

# Next Steps

After installation you may wish to:

- Configure launch modes
- Update library artwork
- Restore a backup
- Run diagnostics

See the User Guide for details.
