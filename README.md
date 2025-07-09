# ArtnetETH

**Version**: 1.0.0\
**Author**: Anoof Chappangathil [anoof7262@gmail.com](mailto\:anoof7262@gmail.com)

Arduino library to receive Art-Net DMX (ArtDMX) over Ethernet, tailored for ESP32 boards using the LAN8720 PHY (e.g., LILYGO T-Ethernet Lite).

## Features

- Receive Art-Net DMX (ArtDMX) packets on UDP port 6454.
- Retrieve DMX frame data (up to 512 channels), sequence, universe, and packet length.
- Configurable Ethernet initialization for LAN8720-based ESP32 boards.
- Minimal dependencies and lightweight implementation.

## Hardware Requirements

- ESP32 development board with LAN8720 Ethernet PHY (e.g., LILYGO T-Ethernet Lite).
- LAN8720 module or onboard PHY chip.
- Ethernet cable and network switch or router within the same subnet as your Art-Net controller.

> **Note:** This library uses the built-in `ETH.h` driver with PHY type `ETH_PHY_LAN8720` by default. If your board uses a different PHY, you may need to adjust the `ETH.begin()` parameters accordingly.

### LAN8720 Pin Configuration (LILYGO T-Ethernet Lite Default)

| PHY Signal | ESP32 Pin       |
| ---------- | --------------- |
| RX0        | 25              |
| RX1        | 26              |
| TX0        | 21              |
| TX1        | 22              |
| TX\_EN     | 23              |
| MDC        | 23              |
| MDIO       | 18              |
| CLK\_MODE  | GPIO0 (pull-up) |
| PWR\_EN    | 5               |

*Verify pin mappings for your specific board.*

## Installation

### Arduino Library Manager

1. Open the Arduino IDE.
2. Go to **Sketch > Include Library > Manage Libraries…**.
3. Search for **ArtnetETH** and install.

### Manual Installation

1. Download the latest release ZIP from [GitHub](https://github.com/anoofc/ArtnetETH).
2. Extract and rename the folder to `ArtnetETH`.
3. Move it into your Arduino libraries directory (`~/Arduino/libraries/ArtnetETH`).
4. Restart the Arduino IDE.

## Usage

### Basic Setup for LAN8720 ESP32

```cpp
#include <ETH.h>
#include <ArtnetETH.h>

ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  // Initialize Ethernet with LAN8720 PHY
  // Arguments: powerPin, mdcPin, mdioPin, clkMode, phyAddress, phyType
  ETH.begin(5, 23, 18, ETH_CLK_MODE_GPIO0_IN, 0, ETH_PHY_LAN8720);
  delay(1000); // wait for link

  if (ETH.linkUp()) {
    Serial.printf("ETHIP: %s\n", ETH.localIP().toString().c_str());
  } else {
    Serial.println("Ethernet link failed");
  }

  artnet.begin();
}

void loop() {
  if (artnet.read()) {
    Serial.printf("Universe: %u, Sequence: %u, Length: %u\n",
                  artnet.getUniverse(), artnet.getSequence(), artnet.getLength());

    uint8_t* dmx = artnet.getDmxFrame();
    for (uint16_t i = 0; i < artnet.getLength(); i++) {
      Serial.print(dmx[i]);
      Serial.print(' ');
    }
    Serial.println();
  }
}
```

### Examples

- **DataDump**: Prints raw DMX channels to Serial. See `examples/DataDump/DataDump.ino`.
- **BasicRead**: Reads and prints specific channel values. See `examples/BasicRead/BasicRead.ino`.

## API Reference

| Method                   | Description                                                                          |
| ------------------------ | ------------------------------------------------------------------------------------ |
| `void begin()`           | Start listening on Art-Net port (6454).                                              |
| `bool read()`            | Parse incoming Art-Net packet. Returns `true` if a valid ArtDMX packet was received. |
| `uint8_t* getDmxFrame()` | Pointer to the DMX frame data (up to 512 bytes).                                     |
| `uint16_t getUniverse()` | Universe number of the last packet.                                                  |
| `uint8_t getSequence()`  | Sequence number of the last packet.                                                  |
| `uint16_t getLength()`   | Number of channels in the last packet.                                               |

## Troubleshooting

- **No Ethernet Link:** Check LAN8720 power pin and cable connection.
- **Wrong Pin Mapping:** Verify your board’s PHY pins and adjust `ETH.begin()` accordingly.
- **DMX Data Missing:** Ensure your Art-Net controller is on the same network and universe matches.

## Contributing

Contributions, issues, and feature requests are welcome! Please open an issue or pull request on the GitHub repository.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

