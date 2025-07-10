/*
  ArtnetETH NeoPixel Example

  Controls a WS2812 (NeoPixel) LED strip from Art-Net DMX.
  - Each LED uses 3 DMX channels (R, G, B)
  - Set universe and DMX start channel as needed in your lighting software.

  Connections:
    - Connect your NeoPixel data line to NEOPIXEL_PIN (default: GPIO13).
    - Provide sufficient power to the LED strip.
    - Connect your ESP32 with LAN8720 to your Art-Net network.

  Requires: ArtnetETH library, Adafruit_NeoPixel library.

  Author: Anoof Chappangathil
*/

#include <ETH.h>
#include <WiFiUdp.h>
#include <ArtnetETH.h>

// LAN8720 Ethernet settings
#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_ADDR       1
#define ETH_POWER_PIN  -1
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18

#define NEOPIXEL_PIN   13      // GPIO13 (or your data pin)
#define NUM_PIXELS     16      // Number of NeoPixels
#define DMX_START_CH   0       // DMX start channel (0–509 for RGB)

// Art-Net object
ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ArtnetETH NeoPixel Example");

  // Ethernet init
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);

  Serial.print("Waiting for Ethernet...");
  while (!ETH.linkUp()) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP: ");
  Serial.println(ETH.localIP());

  artnet.begin();

  // Initialize NeoPixel on NEOPIXEL_PIN, NUM_PIXELS, starting at DMX channel DMX_START_CH
  artnet.initNeoPixel(NEOPIXEL_PIN, NUM_PIXELS, DMX_START_CH);
}

void loop() {
  // Each valid Art-Net packet will automatically update the NeoPixel LEDs.
  artnet.read();
}
