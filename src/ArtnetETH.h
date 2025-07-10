#ifndef ARTNETETH_H
#define ARTNETETH_H

#include <ETH.h>                        // Include Ethernet library for ESP32   
#include <WiFiUdp.h>                    // Include WiFi UDP library for receiving Art-Net packets over Ethernet 
#include <Adafruit_NeoPixel.h>          // Include Adafruit NeoPixel library for controlling LED strips

class ArtnetETH {
public:
    ArtnetETH();                    
    void begin();                   // Initialize the Art-Net receiver
    bool read();                    // Read the next Art-Net packet, returns true if a valid packet was received 
    uint8_t* getDmxFrame();         // Returns a pointer to the DMX frame data (512 bytes)
    uint16_t getUniverse();         // Returns the universe number of the DMX frame (0-65535)
    uint8_t getSequence();          // Returns the sequence number of the DMX frame (0-255)
    uint16_t getLength();           // Returns the length of the DMX frame (0-512)

    /** 
     * Initialize a NeoPixel strip.
     * @param pin         Data pin (e.g. D2)
     * @param numPixels   Number of LEDs in the strip
     * @param startChan   First DMX channel to map (0–511). Defaults to 0.
     * @param type        Pixel color order/type. Defaults to NEO_GRB+NEO_KHZ800.
     */
    void initNeoPixel(uint8_t pin,
        uint16_t numPixels,
        uint16_t startChan = 0,
        neoPixelType type = NEO_GRB + NEO_KHZ800);

    /**
    * Push the current DMX frame out to the NeoPixel strip.
    * Reads 3 channels per pixel (R,G,B).
    */
    void updateNeoPixel();

private:
    WiFiUDP Udp;                                // UDP object for receiving Art-Net packets
    static const int ART_NET_PORT = 6454;       // Default Art-Net port
    static const int MAX_BUFFER_ARTNET = 530;   // Maximum buffer size for Art-Net packets (530 bytes)
    uint8_t packetBuffer[MAX_BUFFER_ARTNET];    // Buffer to store incoming Art-Net packets

    uint8_t sequence;                   // Sequence number (0-255)
    uint16_t universe;                  // Universe number (0-65535)    
    uint16_t length;                    // Length of the DMX frame (0-512)
    uint8_t dmxFrame[512];              // DMX frame data (512 bytes)

    Adafruit_NeoPixel* pixels = nullptr;    // Pointer to the NeoPixel strip object
    uint16_t pixelCount = 0;                // Number of pixels in the strip
    uint16_t pixelStartChannel = 0;         // First DMX channel to map to the NeoPixel strip
    
};

#endif
