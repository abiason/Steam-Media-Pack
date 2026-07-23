# Backup and Restore

Steam Media Pack automatically creates backups before modifying your Steam shortcuts.

This ensures that your existing Steam Library can always be restored if necessary.

---

# Automatic Backup

Before updating the Steam shortcut database (`shortcuts.vdf`), Steam Media Pack automatically creates a backup.

No user interaction is required.

---

# What Is Backed Up?

The following files are backed up when necessary:

- Steam shortcuts (`shortcuts.vdf`)
- Steam Media Pack configuration (when applicable)

This guarantees that your previous configuration can be restored.

---

# When Is a Backup Created?

A backup is automatically created:

- During installation
- Before updating Steam shortcuts
- Before replacing existing Steam Media Pack entries

---

# Restoring a Backup

To restore the latest backup, run:

```bash
./tools/restaurar-atalhos-steam.sh
```

The restoration process will:

- Replace the current `shortcuts.vdf`
- Restore your previous Steam shortcuts
- Remove any changes made after the backup

---

# Best Practices

For maximum safety:

- Close Steam before restoring a backup.
- Do not manually edit `shortcuts.vdf`.
- Keep backup files until you confirm everything is working correctly.

---

# If Restoration Fails

If the automatic restoration does not solve the problem:

1. Verify that Steam is completely closed.
2. Confirm that the backup file still exists.
3. Run the diagnostics tool.
4. Review the generated log files.

---

# Related Documentation

- Installation
- Configuration
- Steam Integration
- Troubleshooting
