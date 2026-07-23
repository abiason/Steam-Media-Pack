<p align="center">
  <img src="docs/assets/branding/banner.png" alt="Steam Media Pack Banner" width="100%">
</p>

<p align="center">
  <img src="docs/assets/branding/logo.png" alt="Steam Media Pack Logo" width="180">
</p>

<h1 align="center">Steam Media Pack</h1>

<p align="center">
Turn Steam into the ultimate media center for Linux.
</p>

<p align="center">

![Release](https://img.shields.io/github/v/release/abiason/Steam-Media-Pack?style=for-the-badge)
![Build](https://img.shields.io/github/actions/workflow/status/abiason/Steam-Media-Pack/build.yml?branch=develop&label=Build&style=for-the-badge)
![CI](https://img.shields.io/github/actions/workflow/status/abiason/Steam-Media-Pack/ci.yml?branch=develop&label=CI&style=for-the-badge)
![License](https://img.shields.io/github/license/abiason/Steam-Media-Pack?style=for-the-badge)
![Platform](https://img.shields.io/badge/Linux-CachyOS%20%7C%20SteamOS%20%7C%20Bazzite-blue?style=for-the-badge)

</p>

---

# 🎮 What is Steam Media Pack?

**Steam Media Pack** is an Open Source project developed by **Tech Dual 4.0** that transforms your Steam library into a complete entertainment hub.

With just a few clicks, you can add your favorite streaming services directly to Steam, featuring:

- Desktop Mode
- Gaming Mode
- Big Picture
- Steam Deck
- Automatic artwork installation
- Automatic shortcut creation
- Safe backups
- Automatic restore

Everything seamlessly integrated into the Steam experience.

---

# ✨ Features

- 🚀 Automatic Steam integration
- 🎨 Automatic library artwork installation
- 🎮 Automatic Steam shortcut creation
- 💾 Automatic backup of `shortcuts.vdf`
- 🔄 Backup restoration
- 🖥️ Desktop Mode support
- 🎮 Gaming Mode support
- 🖥️ Kiosk Mode
- 📱 App Mode
- ⚙️ Auto Mode
- 📺 Streaming-optimized launchers
- 🌐 Chromium-based browser support
- 📐 Automatic resolution detection
- 🧹 Smart Steam environment cleanup
- 📋 Individual logs for each service
- 🛠️ Fully Open Source

---

# 📺 Supported Services

| Service | Status |
|----------|:------:|
| Netflix | ✅ |
| Prime Video | ✅ |
| Disney+ | ✅ |
| Max | ✅ |
| Spotify | ✅ |
| YouTube TV | ✅ |

---

# 🖥️ Compatibility

| Platform | Status |
|----------|:------:|
| CachyOS Handheld | ✅ Official |
| SteamOS | 🟡 Compatible (not officially validated) |
| Bazzite | 🟡 Compatible (not officially validated) |
| Arch Linux | 🟡 Compatible |
| Nobara | 🟡 Compatible |

---

# 📸 Screenshots

## Steam Library

> *(Coming soon)*

---

## Gaming Mode

> *(Coming soon)*

---

## Desktop Mode

> *(Coming soon)*

---

# 🚀 Quick Start

```bash
git clone https://github.com/abiason/Steam-Media-Pack.git

cd Steam-Media-Pack

./install.sh
```

For detailed installation instructions, see the documentation in `docs/`.

---

# 📦 Installation

See the complete installation guide in:

```
docs/user-guide/installation.md
```

---

# 🎨 Install Steam Shortcuts Automatically

Completely close Steam.

Then run:

```bash
./tools/instalar-atalhos-steam.sh
```

Launch Steam again.

Your existing shortcuts will be preserved.

---

# 🖼️ Update Artwork Only

```bash
./tools/atualizar-artes-steam.sh
```

---

# 🔄 Restore Backup

```bash
./tools/restaurar-atalhos-steam.sh
```

---

# ⚙️ Change Launch Mode

```bash
~/.local/share/steam-media-pack/selecionar-modo.sh
```

Available modes:

- Auto
- Kiosk
- App

---

# 🔍 Diagnostics

```bash
./diagnostico.sh
```

---

# 🗑️ Uninstallation

```bash
./uninstall.sh
```

---

# 📁 Project Structure

```text
Steam-Media-Pack
│
├── desktop
├── docs
├── icons
├── launchers
├── steam-art
├── tools
│
├── install.sh
├── uninstall.sh
├── selecionar-modo.sh
├── diagnostico.sh
│
└── README.md
```

---

# 📚 Documentation

Documentation includes:

- Installation
- Configuration
- Steam Integration
- Library Artwork
- FAQ
- Troubleshooting
- Architecture

All documentation is available in the **docs/** directory.

---

# 🛣️ Roadmap

## v11

- ✅ Steam Integration
- ✅ Automatic Artwork
- ✅ Backup
- ✅ Restore
- ✅ Steam Shortcuts

## v11.1 — Foundation

- Git Flow
- CI/CD
- Release Automation
- Documentation
- Branding
- ADR

## v11.2 — Experience

- GitHub Pages
- Technical Diagrams
- User Guide
- Developer Guide

## v12.0 — Product

- GUI Installer
- Plugin System
- YAML Configuration
- Flatpak
- AppImage
- Automatic Updates

- ⏳ New user interface
- ⏳ Apple TV+
- ⏳ Jellyfin
- ⏳ Plex
- ⏳ GloboPlay
- ⏳ Installer improvements
- ⏳ File-based configuration
- ⏳ Automatic updater

---

# 🤝 Contributing

Contributions are welcome!

Before submitting a Pull Request, please read:

- CONTRIBUTING.md
- CODE_OF_CONDUCT.md
- SECURITY.md

---

# ⚠️ Disclaimer

Steam Media Pack is an independent Open Source project.

It is **not affiliated with or endorsed by**:

- Valve
- Netflix
- Amazon
- Disney
- Warner Bros. Discovery
- Google
- Spotify

All trademarks are the property of their respective owners.

---

# 👨‍💻 Developed by

## Tech Dual 4.0

Created and maintained by **Alberto Biason**.

---

# 📄 License

Distributed under the MIT License.

See **LICENSE** for more information.

# 🔗 Useful Links

- Documentation → `docs/`
- Roadmap → `ROADMAP.md`
- Changelog → `CHANGELOG.md`
- Releases → GitHub Releases
