# DAMI - Door Access Manager Interface

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-00979D)
![Build Tool](https://img.shields.io/badge/build-PlatformIO-orange)

## 📖 Overview

DAMI (Door Access Manager Interface) is a smart door access control system developed at **Universidade de Pernambuco** (Brazil) to manage student access to restricted university laboratories. The system uses QR code-based authentication to provide secure and convenient access control.

### How It Works

1. The DAMI device displays a dynamically generated QR code on its screen
2. Students scan the QR code with their mobile devices
3. The authentication system validates the student's credentials
4. If authorized, the device unlocks the door
5. Access is logged for security and auditing purposes

## ✨ Features

- **🔐 OTP-based Authentication**: Time-based One-Time Password (TOTP) generation for enhanced security
- **📱 QR Code Display**: Real-time QR code generation and display on device screen
- **🌐 WiFi Connectivity**: Supports WPA2, WPA, and PEAP enterprise networks
- **☁️ MQTT Integration**: Real-time communication with backend services
- **🕒 NTP Time Synchronization**: Accurate time keeping for OTP generation
- **🎨 Modern UI**: Built with LVGL graphics library
- **🔊 Audio/Visual Feedback**: Alerts for access granted/denied events
- **⚙️ Configurable**: File-based configuration system
- **🖥️ Serial Console**: Debug and configuration interface

## 🛠️ Hardware Requirements

- **Microcontroller**: ESP32 WROVER Kit
- **Display**: Compatible with LovyanGFX library
- **Flash**: Minimum 4MB (configured with min_spiffs partition)
- **PSRAM**: Required for graphics and UI operations
- **Additional Components**:
  - Electronic lock mechanism
  - Buzzer/speaker (for audio alerts)
  - Power supply

## 📋 Software Architecture

### Core Modules

```
src/
├── main.cpp              # Application entry point
├── config/               # Configuration management
│   ├── config_data.h     # Configuration data structures
│   └── config_loader.cpp # Configuration file parser
└── modules/
    ├── wifi/             # WiFi connection management
    ├── mqtt/             # MQTT client and messaging
    ├── ntp/              # Network time protocol sync
    ├── otp/              # OTP (TOTP) generation
    ├── screen/           # Display and UI management
    └── lock/             # Lock control and alerts
```

### Libraries Used

- **[LVGL 8.3.7](https://lvgl.io/)**: Graphics and UI framework
- **[LovyanGFX 0.4.18](https://github.com/lovyan03/LovyanGFX)**: Hardware-accelerated graphics driver
- **[PubSubClient 2.8](https://github.com/knolleary/pubsubclient)**: MQTT client library
- **[ESP32-PsRamFS](https://github.com/tobozo/ESP32-PsRamFS)**: PSRAM file system
- **[ESP32Ping](https://github.com/marian-craciunescu/ESP32Ping)**: Network diagnostics

## 🚀 Getting Started

### Prerequisites

1. **PlatformIO**: Install [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) or PlatformIO Core
2. **Python 3**: Required for build scripts
3. **Git**: For cloning the repository

### Installation

1. Clone the repository:
```bash
git clone https://github.com/dami-pie/dami-firmware.git
cd dami-firmware
```

2. Create environment configuration files:
```bash
# Create .env for production
touch .env

# Create .env.development for development
touch .env.development
```

3. The build system uses configuration files from the `conf/` directory to generate build flags:
   - `broker.conf`: MQTT broker settings
   - `ntp.conf`: NTP server configuration
   - Additional `.env` files for custom environment variables

### Building

#### Development Build
```bash
pio run -e dev
```

#### Production Build
```bash
pio run -e prod
```

### Flashing

```bash
# Flash to device
pio run -t upload

# Flash and monitor serial output
pio run -t upload && pio device monitor
```

## ⚙️ Configuration

### WiFi Configuration

WiFi credentials are stored in `data/configs/wifi` (binary format). Configure via the serial console or update the configuration through the device interface.

### MQTT Broker

Configure MQTT settings in `conf/broker.conf`:
```properties
CONFIG_BROKER_PORT=8883
CONFIG_BROKER_URL="your_mqtt_broker_ip"
CONFIG_BASE_CLIENT_URL="http://your_backend_server:3000"
```

### NTP Server

Configure NTP servers in `conf/ntp.conf`:
```properties
NTP_SERVER_1="a.st1.ntp.br"
NTP_SERVER_2="b.st1.ntp.br"
NTP_SERVER_3="time.google.com"
NTP_TIMESHIFT=-10800  # GMT offset in seconds (-3h for Brazil)
NTP_TIMEOUT=500
```

### Device Configuration

Configure device-specific settings in `conf/` directory (loaded during build via `load_env.py`):
- `bsp.conf`: Board support package settings
- `colors.conf`: UI color scheme
- `keys.conf`: OTP secret keys
- `tones.conf`: Audio alert tones
- `files.conf`: File system paths

These configuration files use the format `CONFIG_KEY="value"` and are converted to build flags.

## 🔧 Development

### Build Environments

- **dev**: Development environment with verbose logging (DEBUG_LEVEL=5)
- **prod**: Production environment with minimal logging (DEBUG_LEVEL=2)

### Debug Console

The firmware includes a serial console for debugging and configuration. Available commands:
- `wifi`: WiFi management and diagnostics
- `mqtt`: MQTT connection status and testing
- `ntp`: Time synchronization status
- `config`: View and modify configuration

### Serial Monitor

Connect to serial monitor at 115200 baud:
```bash
pio device monitor -b 115200
```

## 📁 Project Structure

```
dami-firmware/
├── platformio.ini        # PlatformIO configuration
├── load_env.py          # Environment loader script
├── conf/                # Configuration templates
├── data/                # File system data
│   └── configs/         # Runtime configurations
├── include/             # Header files
├── lib/                 # Custom libraries
│   ├── console/         # Serial console implementation
│   ├── utils/           # Utility functions
│   └── lvgl/            # LVGL configuration
├── src/                 # Source code
│   ├── main.cpp         # Main application
│   ├── config/          # Configuration loader
│   └── modules/         # Functional modules
├── tools/               # Build and development tools
└── doc/                 # Documentation and schematics
```

## 🤝 Contributing

This is an academic project developed at Universidade de Pernambuco. Contributions, suggestions, and improvements are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

This project was developed for academic purposes at Universidade de Pernambuco.

## 👥 Authors

Developed by students at **Universidade de Pernambuco** (UPE), Brazil.

## 🙏 Acknowledgments

- Universidade de Pernambuco for supporting this project
- The ESP32 and Arduino communities
- All open-source library contributors

## 📞 Support

For questions, issues, or suggestions, please open an issue in the GitHub repository.

---

**Made with ❤️ at Universidade de Pernambuco**
