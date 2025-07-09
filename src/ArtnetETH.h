#ifndef ARTNETETH_LILYGO_H
#define ARTNETETH_LILYGO_H

#include <ETH.h>
#include <WiFiUdp.h>

class ArtnetETH_LILYGO {
public:
    ArtnetETH_LILYGO();
    void begin();
    bool read();
    uint8_t* getDmxFrame();
    uint16_t getUniverse();
    uint8_t getSequence();
    uint16_t getLength();  // add this

private:
    WiFiUDP Udp;
    static const int ART_NET_PORT = 6454;
    static const int MAX_BUFFER_ARTNET = 530;
    uint8_t packetBuffer[MAX_BUFFER_ARTNET];

    uint8_t sequence;
    uint16_t universe;
    uint16_t length;  // instead of uint8_t
    uint8_t dmxFrame[512];
    
};

#endif
