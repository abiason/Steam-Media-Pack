# Launch Modes

Steam Media Pack supports multiple launch modes to provide the best experience across different devices and usage scenarios.

---

# Available Modes

## Auto Mode

Automatically selects the most appropriate launch mode for your environment.

This is the recommended option for most users.

---

## App Mode

Launches the streaming service using the browser's application mode.

Characteristics:

- Minimal browser interface
- Dedicated application window
- Ideal for Desktop Mode

---

## Kiosk Mode

Launches the browser in full-screen kiosk mode.

Characteristics:

- Full-screen experience
- No browser interface
- Optimized for Gaming Mode
- Recommended for Steam Deck

---

# Changing the Launch Mode

Run:

```bash
~/.local/share/steam-media-pack/selecionar-modo.sh
```

Choose one of the available modes:

- Auto
- App
- Kiosk

The selected mode will be applied to all supported streaming services.

---

# Recommended Usage

| Environment | Recommended Mode |
|--------------|------------------|
| Desktop PC | App |
| HTPC | Kiosk |
| Steam Deck | Kiosk |
| General Use | Auto |

---

# Notes

Changing the launch mode does not modify your Steam shortcuts.

Only the browser launch parameters are updated.

---

# Related Documentation

- Installation
- Configuration
- Steam Integration
