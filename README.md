# ArtnetETH

**Version**: 1.0.0\
**Author**: Anoof Chappangathil [anoof7262@gmail.com](mailto\:anoof7262@gmail.com)

Arduino library to receive Art-Net DMX (ArtDMX) over Ethernet, tailored for ESP32 boards using the LAN8720 PHY (e.g., LILYGO T-Ethernet Lite).

## Features

- Receive Art-Net DMX (ArtDMX) packets on UDP port 6454.
- Retrieve DMX frame data (up to 512 channels), sequence, universe, and packet length.
- **Direct NeoPixel (WS2812) control via Art-Net DMX.**
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
| PWR\_EN    | 12              |

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
/*
 - This assumes your board uses LAN8720 Ethernet PHY. Change the ETH_TYPE, ETH_ADDR, MDC/MDIO pins based on your hardware.
 - The sketch reads ArtDMX packets and prints the universe, data length, and first DMX channel value.
 - You can easily hook this up to control LEDs, relays, servos, or other DMX-reactive elements.
*/

#include <ETH.h>
#include <WiFiUdp.h>
#include <ArtnetETH.h>

// Replace with your board-specific PHY settings if needed
#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_ADDR       1
#define ETH_POWER_PIN  -1
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18

ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ArtnetETH Example");

  // Initialize Ethernet
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);

  // Wait for Ethernet link
  Serial.println("Waiting for Ethernet link...");
  while (!ETH.linkUp()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(ETH.localIP());

  artnet.begin();
}

void loop() {
  if (artnet.read()) {
    Serial.print("DMX Received - Universe: ");
    Serial.print(artnet.getUniverse());
    Serial.print(" | Length: ");
    Serial.print(artnet.getLength());
    Serial.print(" | First Channel Value: ");
    Serial.println(artnet.getDmxFrame()[0]);

    // Example: turn on an LED if first DMX value is above threshold
    if (artnet.getDmxFrame()[0] > 127) {
      // digitalWrite(LED_BUILTIN, HIGH);
    } else {
      // digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
```

### NeoPixel Example

This example shows how to control a WS2812/NeoPixel LED strip using Art-Net DMX data. Each pixel uses 3 DMX channels (R, G, B).

```cpp
#include <ETH.h>
#include <WiFiUdp.h>
#include <ArtnetETH.h>

#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_ADDR       1
#define ETH_POWER_PIN  -1
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18

#define NEOPIXEL_PIN   13        // GPIO for NeoPixel data
#define NUM_PIXELS     16        // Number of LEDs in the strip
#define DMX_START_CH   0         // Start at DMX channel 0

ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  delay(1000);

  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);
  while (!ETH.linkUp()) delay(100);

  Serial.println("Starting ArtnetETH NeoPixel Example");
  Serial.print("IP address: ");
  Serial.println(ETH.localIP());

  artnet.begin();

  // Initialize NeoPixel (pin, num pixels, start DMX channel)
  artnet.initNeoPixel(NEOPIXEL_PIN, NUM_PIXELS, DMX_START_CH);
}

void loop() {
  artnet.read();  // Art-Net DMX received frames update NeoPixels automatically
}
```

### Examples

- **DataDump**: Prints raw DMX channels to Serial. See `Examples/DataDump/DataDump.ino`.
- **BasicRead**: Reads and prints specific channel values. See `Examples/BasicRead/BasicRead.ino`.
- **Neopixel**: Controls an addressable NeoPixel (WS2812) LED strip using Art-Net DMX. See `Examples/Neopixel/Neopixel.ino`.


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

