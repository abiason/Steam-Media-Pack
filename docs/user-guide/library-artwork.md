# Library Artwork

Steam Media Pack automatically installs custom artwork for every supported streaming service.

This provides a consistent and native-looking experience inside the Steam Library.

---

# Supported Artwork

The following artwork types are supported:

| Artwork | Description |
|----------|-------------|
| Grid | Portrait image displayed in the Library grid |
| Hero | Background image shown on the game page |
| Logo | Transparent logo displayed over the Hero image |
| Icon | Small icon used by Steam |

---

# Automatic Installation

Artwork is installed automatically during the installation process.

No manual configuration is required.

If artwork already exists, Steam Media Pack updates it when necessary.

---

# Artwork Directory

Artwork files are stored in the project directory:

```text
steam-art/
```

Each streaming service has its own artwork set.

Example:

```text
steam-art/
├── netflix/
├── disney-plus/
├── max/
├── prime-video/
├── spotify/
└── youtube-tv/
```

---

# Updating Artwork

If you only want to refresh the Steam artwork without reinstalling the project, run:

```bash
./tools/atualizar-artes-steam.sh
```

Steam Media Pack will update every artwork while preserving your existing shortcuts.

---

# Adding New Artwork

When adding support for a new streaming service:

1. Create a new directory inside `steam-art/`.
2. Add all required artwork files.
3. Use the correct naming convention.
4. Test the artwork in Steam.

---

# Best Practices

For the best visual experience:

- Use high-resolution images.
- Keep transparent backgrounds where appropriate.
- Use PNG format whenever possible.
- Ensure artwork follows Steam's visual style.

---

# Troubleshooting

If artwork does not appear:

- Completely close Steam.
- Run the artwork update script again.
- Restart Steam.
- Verify that the artwork files exist.
- Check the generated log files.

---

# Related Documentation

- Installation
- Steam Integration
- Troubleshooting
