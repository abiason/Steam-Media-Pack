# Troubleshooting

This guide covers the most common issues encountered while using Steam Media Pack.

---

# Steam Does Not Detect the Shortcuts

Possible causes:

- Steam was still running during installation.
- The wrong Steam account was used.
- The installation was interrupted.

Solution:

1. Completely close Steam.
2. Run the installer again.
3. Launch Steam.

---

# Artwork Does Not Appear

Possible causes:

- Steam cache has not been refreshed.
- Artwork update was interrupted.

Solution:

1. Close Steam.
2. Run:

```bash
./tools/atualizar-artes-steam.sh
```

3. Start Steam again.

---

# Streaming Service Does Not Launch

Check:

- Browser installation
- Browser path
- Internet connection
- Service availability

If necessary, review the log file for the affected service.

---

# Installer Cannot Find Steam

Verify that:

- Steam is installed.
- Steam has been launched at least once.
- The current user owns the Steam installation.

---

# Diagnostics

Run:

```bash
./diagnostico.sh
```

The diagnostics tool checks:

- Steam installation
- Browser availability
- Required directories
- Launchers
- Artwork
- Configuration

---

# Logs

Each streaming service generates its own log file.

Review these logs before reporting an issue.

---

# Reporting a Bug

When opening an issue, include:

- Linux distribution
- Desktop environment
- Steam version
- Browser used
- Log files
- Steps to reproduce the problem

This information helps us reproduce and resolve the issue more quickly.
