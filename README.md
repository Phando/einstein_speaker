# Einstein Remote

A WiFi-based remote control for the Salesforce Einstein Smart Speaker. Runs on a TTGO LoRa32 ESP32 board wired inside the speaker via serial. Any phone can connect and control the speaker's hair LEDs, belly display, and microphone — no app install required.

## How It Works

### Captive Portal (First Boot / No WiFi)

On first boot (or when saved WiFi is unavailable), the ESP32 creates an open WiFi access point named `Einstein-XXXX` (where XXXX is derived from the device MAC address). When a phone connects to this network, the captive portal automatically opens the control page in the phone's browser.

The captive portal works by:
1. Running a DNS server that redirects all domain lookups to the ESP32
2. Responding to OS-specific captive portal detection URLs (`/generate_204`, `/hotspot-detect.html`, etc.)
3. Redirecting any unknown request to the main control page

### WiFi (Station Mode)

Once WiFi credentials are configured via the web UI, the ESP32 will:
1. Attempt to connect to the saved network on boot (10-second timeout)
2. If successful, serve the same control page on its local IP address
3. Display its IP address on Einstein's belly every 10 minutes so users can find it
4. If the network becomes unavailable, fall back to AP/captive portal mode

To configure WiFi: expand the "WiFi Setup" accordion at the bottom of the page, enter your network name and password, and tap "Save & Reboot." To clear saved credentials, tap "Forget WiFi."

### OTA (Over-the-Air Updates)

Since the ESP32 is sealed inside the speaker, firmware updates are done over WiFi:
1. Build the project with `pio run`
2. Open the control page in a browser
3. Expand the "Firmware Update" accordion at the bottom
4. Select the `.bin` file (located at `.pio/build/ttgo-lora32-v1/firmware.bin`)
5. Tap "Upload & Reboot"
6. The upload progress is displayed; the device reboots automatically on success

OTA works in both AP mode and station mode.

## Control UI

The web interface is a mobile-first touch-optimized page served directly from the ESP32's flash memory. It communicates with the device via WebSocket for low-latency button response.

A green dot in the header indicates an active WebSocket connection. If it turns red, the page will automatically reconnect.

### Button Sections

**Einstein (White)** — Listen and Speak buttons with white hair LEDs. Press and hold to activate; release to turn off.

**Alexa (Blue)** — Listen and Speak buttons with blue color LEDs. Press and hold to activate; release to turn off.

**Speak with Color** — A 3x3 grid of colored buttons plus a rainbow button. Each activates the hair LEDs in that color along with the belly speak animation. Press and hold; release to turn off.

Colors (arranged warm to cool, then soft pastels):
- Coral, Amber, Lime
- Cyan, Turquoise, Purple
- Magenta, Pink, Lavender
- Rainbow (animated crazy hair mode)

**Microphone** — Mic On and Mic Off are click-to-enable buttons (not hold). Clicking one deactivates the other. These control the belly mute/unmute indicator.

**All Off** — Click to turn off all LEDs, animations, and the microphone indicator. Clears both mic buttons.

### Button Behavior

| Type | Interaction | Examples |
|------|-------------|----------|
| Hold | Press and hold to activate, release to deactivate | Einstein, Alexa, Speak with Color |
| Radio | Click to enable, click again to disable. Mutually exclusive within group | Mic On, Mic Off |
| Tap | Single click to fire | All Off |

## Hardware

- **Board:** TTGO LoRa32 v1 (ESP32)
- **Serial TX to speaker MCU:** GPIO 0
- **Serial RX from speaker MCU:** GPIO 4
- **Baud rate:** 115200 (8N1)
- **GPIO 34/35:** Button port available on the board (original remote lost, unused in this project)

The speaker's internal MCU accepts commands in the format `DEMO+<number>\r` over serial. This project translates web UI interactions into those serial commands.

## Building & Flashing

Requires [PlatformIO](https://platformio.org/).

```bash
cd einstein_remote

# Build
pio run

# Flash via USB (first time only)
pio run -t upload

# Subsequent updates via OTA from the web UI
```

## Project Structure

```
einstein_remote/
├── platformio.ini          # Board config and library dependencies
├── src/
│   └── main.cpp            # WiFi, WebSocket, OTA, and command routing
└── include/
    ├── EinsteinSerial.h    # Serial command interface to speaker MCU
    └── WebUI.h             # Complete web page as PROGMEM string
```
