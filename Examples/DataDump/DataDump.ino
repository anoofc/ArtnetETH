#include <ETH.h>
#include <WiFiUdp.h>
#include <ArtnetETH.h>

#define ETH_TYPE       ETH_PHY_LAN8720
#define ETH_ADDR       1
#define ETH_POWER_PIN  -1
#define ETH_MDC_PIN    23
#define ETH_MDIO_PIN   18

#define ARTNET_UNIVERSES  3
#define BASE_UNIVERSE     0

ArtnetETH artnet;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ArtnetETH DataDump Example");

  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);
  while (!ETH.linkUp()) delay(100);

  Serial.print("Connected, IP: ");
  Serial.println(ETH.localIP());

  artnet.begin(ARTNET_UNIVERSES, BASE_UNIVERSE);
}

void loop() {
  if (artnet.read()) {
    Serial.print("U");
    Serial.print(artnet.getUniverse());
    Serial.print(" | Len: ");
    Serial.print(artnet.getLength());
    Serial.print(" | Data: ");

    uint8_t* dmx = artnet.getDmxFrame();
    for (int i = 0; i < artnet.getLength(); i++) {
      Serial.print(dmx[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
