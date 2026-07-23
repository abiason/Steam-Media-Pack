# Frequently Asked Questions (FAQ)

This page answers the most common questions about Steam Media Pack.

---

# Which Linux distributions are supported?

Officially supported:

- CachyOS Handheld

Compatible:

- SteamOS
- Bazzite
- Arch Linux
- Nobara

Additional distributions may work but are not officially tested.

---

# Which browsers are supported?

Steam Media Pack currently supports Chromium-based browsers, including:

- Chromium
- Google Chrome
- Microsoft Edge
- Brave

Firefox support is planned for a future release.

---

# Will my existing Steam shortcuts be removed?

No.

Steam Media Pack preserves your existing non-Steam shortcuts and only adds or updates the entries required for supported streaming services.

---

# Does Steam need to be closed during installation?

Yes.

Steam must be completely closed before:

- Installing Steam Media Pack
- Updating shortcuts
- Updating artwork
- Restoring backups

---

# Can I update the artwork without reinstalling?

Yes.

Run:

```bash
./tools/atualizar-artes-steam.sh
```

---

# Can I restore my previous Steam shortcuts?

Yes.

Run:

```bash
./tools/restaurar-atalhos-steam.sh
```

---

# Does Steam Media Pack modify Steam itself?

No.

Steam Media Pack only manages non-Steam shortcuts and their associated artwork.

It does not modify Steam binaries or internal functionality.

---

# Is an internet connection required?

Yes.

An internet connection is required to access streaming services.

The installation itself may also require downloading project updates when applicable.

---

# Is Steam Deck supported?

Yes.

Steam Media Pack is designed to work with Steam Deck and other Linux gaming devices.

---

# Is Steam Media Pack affiliated with Valve?

No.

Steam Media Pack is an independent Open Source project and is not affiliated with or endorsed by Valve or any streaming service.

---

# Where can I report bugs?

Please open an Issue on GitHub and include:

- Linux distribution
- Steam version
- Browser used
- Log files
- Steps to reproduce the issue
