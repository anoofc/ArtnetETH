#include "ArtnetETH.h"
#include <string.h>             // for memcpy, memset
#include <Arduino.h>            // ensure pinMode, etc.
#include <Adafruit_NeoPixel.h>  // NeoPixel library for LED control

ArtnetETH::ArtnetETH() {}

void ArtnetETH::begin() {
    Udp.begin(ART_NET_PORT);            // Start listening on the Art-Net port
}

bool ArtnetETH::read() {
    int packetSize = Udp.parsePacket();
    if (packetSize <= 0 || packetSize > MAX_BUFFER_ARTNET) return false;

    int len = Udp.read(packetBuffer, MAX_BUFFER_ARTNET);
    if (len < 18) return false;  // Not enough data for ArtDMX

    // Check ID
    if (memcmp(packetBuffer, "Art-Net", 7) != 0) return false;

    // Check OpCode == 0x5000 (ArtDMX)
    uint16_t opcode = packetBuffer[8] | (packetBuffer[9] << 8);
    if (opcode != 0x5000) return false;

    sequence = packetBuffer[12];
    universe = packetBuffer[14] | (packetBuffer[15] << 8);

    // DMX data length
    length = packetBuffer[16] << 8 | packetBuffer[17];
    if (length > 512) length = 512;

    // Copy DMX data
    memcpy(dmxFrame, &packetBuffer[18], length);

    // After reading, immediately update the LEDs
    updateNeoPixel();

    return true;
}

// bool ArtnetETH::read() {
//     int packetSize = Udp.parsePacket();
//     if (packetSize <= 0 || packetSize > MAX_BUFFER_ARTNET) return false;

//     int len = Udp.read(packetBuffer, MAX_BUFFER_ARTNET);
//     if (len < 18) return false;  // Not enough data for ArtDMX

//     // Check ID
//     if (memcmp(packetBuffer, "Art-Net", 7) != 0) return false;

//     // Check OpCode == 0x5000 (ArtDMX)
//     uint16_t opcode = packetBuffer[8] | (packetBuffer[9] << 8);
//     if (opcode != 0x5000) return false;

//     sequence = packetBuffer[12];
//     universe = packetBuffer[14] | (packetBuffer[15] << 8);

//     // DMX data length
//     length = packetBuffer[16] << 8 | packetBuffer[17];
//     if (length > 512) length = 512;

//     // Clear previous DMX
//     memset(dmxFrame, 0, sizeof(dmxFrame));

//     // Only copy what fits
//     if ((18 + length) <= len) {
//         memcpy(dmxFrame, &packetBuffer[18], length);
//     } else {
//         return false; // Malformed packet
//     }

//     return true;
// }


uint8_t* ArtnetETH::getDmxFrame() { return dmxFrame; }      // Returns a pointer to the DMX frame data (512 bytes)
uint16_t ArtnetETH::getUniverse() { return universe; }      // Returns the universe number of the DMX frame (0-65535)
uint8_t  ArtnetETH::getSequence() { return sequence; }      // Returns the sequence number of the DMX frame (0-255)
uint16_t ArtnetETH::getLength()   { return length; }        // Returns the length of the DMX frame (0-512)

void ArtnetETH::initNeoPixel(uint8_t pin,
    uint16_t numPixels,
    uint16_t startChan,
    neoPixelType type) {
    pixelCount        = numPixels;
    pixelStartChannel = startChan;
    // Allocate and configure
    pixels = new Adafruit_NeoPixel(numPixels, pin, type);
    pixels->begin();
    pixels->show();  // clear
}

void ArtnetETH::updateNeoPixel() {
    if (!pixels) return;  // not configured

    // For each LED, read 3 DMX channels (R, G, B)
    for (uint16_t i = 0; i < pixelCount; i++) {
        uint16_t chan = pixelStartChannel + i * 3;
        uint8_t r = 0, g = 0, b = 0;
        if (chan + 2 < length) {
            r = dmxFrame[chan];
            g = dmxFrame[chan + 1];
            b = dmxFrame[chan + 2];
        }
        pixels->setPixelColor(i, pixels->Color(r, g, b));
    }
    pixels->show();
}