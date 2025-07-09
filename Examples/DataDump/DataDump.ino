#include <ETH.h>
#include <WiFiUdp.h>
#include <ArtnetETH.h>

// Replace with your board-specific PHY settings
#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_ADDR       1
#define ETH_POWER_PIN  -1
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18

ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ArtnetETH - Print All 512 Channels");

  // Initialize Ethernet
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);

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
    uint16_t universe = artnet.getUniverse();
    uint16_t length = artnet.getLength();
    uint8_t* dmx = artnet.getDmxFrame();

    Serial.print("DMX Packet - Universe: ");
    Serial.print(universe);
    Serial.print(" | Length: ");
    Serial.println(length);

    for (int i = 0; i < 512; i++) {
      Serial.print("CH ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(dmx[i]);
      Serial.print((i % 16 == 15) ? "\n" : "\t");  // Print 16 values per line
    }

    Serial.println("--------------------------------------------------");
  }
}
