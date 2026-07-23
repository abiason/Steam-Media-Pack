# Steam Integration

Steam Media Pack integrates streaming services directly into your Steam Library while preserving your existing non-Steam shortcuts.

---

# Overview

During installation, Steam Media Pack automatically:

- Detects your Steam installation
- Locates the `shortcuts.vdf` file
- Creates a backup when necessary
- Adds new streaming shortcuts
- Preserves existing user shortcuts
- Refreshes Steam artwork

No manual editing of Steam files is required.

---

# Steam Detection

The installer automatically searches for the current Steam installation.

If Steam cannot be found, the installation process is interrupted and an error message is displayed.

---

# Steam Shortcuts

Steam stores non-Steam shortcuts in the following file:

```text
userdata/<SteamID>/config/shortcuts.vdf
```

Steam Media Pack updates this file automatically while preserving existing entries.

---

# Backup

Before modifying `shortcuts.vdf`, a backup is created automatically.

This allows the original shortcuts to be restored if necessary.

---

# Automatic Shortcut Creation

Each supported streaming service is added as a separate Steam shortcut.

Depending on the selected configuration, shortcuts can be launched using:

- App Mode
- Kiosk Mode
- Auto Mode

---

# Library Artwork

After creating the shortcuts, Steam Media Pack installs the corresponding artwork automatically.

The following artwork types are supported:

- Grid
- Hero
- Logo
- Icon

This ensures every shortcut matches the visual style of the Steam Library.

---

# Existing Shortcuts

Steam Media Pack never removes existing user shortcuts during installation.

Only the required entries are added or updated.

---

# Updating Steam

After installation:

1. Close Steam completely.
2. Run the installation or shortcut update script if necessary.
3. Launch Steam again.

The new shortcuts should appear automatically.

---

# Troubleshooting

If the shortcuts do not appear:

- Verify that Steam is completely closed before installation.
- Confirm that the correct Steam account is being used.
- Run the diagnostics tool.
- Check the generated log files.

See:

- Troubleshooting

---

# Related Documentation

- Installation
- Configuration
- Library Artwork
- Backup and Restore
- Troubleshooting
