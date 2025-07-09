#include "ArtnetETH.h"
#include <string.h>

ArtnetETH::ArtnetETH() {}

void ArtnetETH::begin() {
    Udp.begin(ART_NET_PORT);
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

    // Clear previous DMX
    memset(dmxFrame, 0, sizeof(dmxFrame));

    // Only copy what fits
    if ((18 + length) <= len) {
        memcpy(dmxFrame, &packetBuffer[18], length);
    } else {
        return false; // Malformed packet
    }

    return true;
}


uint8_t* ArtnetETH::getDmxFrame() {
    return dmxFrame;
}

uint16_t ArtnetETH::getUniverse() {
    return universe;
}

uint8_t ArtnetETH::getSequence() {
    return sequence;
}
uint16_t ArtnetETH::getLength() {
    return length;
}