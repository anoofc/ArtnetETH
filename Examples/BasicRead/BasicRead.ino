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
