#include <Arduino.h>

#define LORA_DEFAULT_SS_PIN     18
#define LORA_DEFAULT_RESET_PIN  14
#define LORA_DEFAULT_DIO0_PIN   26
#define LORA_DEFAULT_DIO1_PIN   33
#define LORA_DEFAULT_DIO2_PIN   32

#define MAX_BYTE_LORA 64

#include "heltec.h"
#include "images.hpp"

#include <RadioLib.h>

unsigned int counter = 0;

SX1276 radio = new Module(LORA_DEFAULT_SS_PIN, LORA_DEFAULT_RESET_PIN, LORA_DEFAULT_DIO0_PIN, LORA_DEFAULT_DIO1_PIN);
uint8_t new_package [MAX_BYTE_LORA];

void logo() {
    Heltec.display->clear();
    Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
    Heltec.display->display();
}

// flag to indicate that a packet was received
volatile bool receivedFlag = true;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif

void setFlag() {
    // check if the interrupt is enabled
    if (!enableInterrupt) {
        return;
    }

    // we got a packet, set the flag
    receivedFlag = true;
}

void InitLoRa() {
    SPI.begin(SCK, MISO, MOSI, SS);

    // initialize SX1278 with default settings
    Serial.print(F("[SX1276] Initializing ... "));

    int state = radio.begin(868.0, 125.0, 7, 5, 0x12);
    radio.setCRC(true);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

    radio.setDio0Action(setFlag, 0);

    // set the function that will be called
    // when new packet is received
    // radio.setDio0Action(setFlag);

    // start listening for LoRa packets
    Serial.print(F("[SX1276] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }
}

void setup() {
    //WIFI Kit series V1 not support Vext control
    Heltec.begin(true /*DisplayEnable Enable*/,
                 false /*Heltec.Heltec.Heltec.LoRa Disable*/,
                 true /*Serial Enable*/,
                 true /*PABOOST Enable*/);

    Heltec.display->init();
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_10);
    logo();
    delay(1500);
    Heltec.display->clear();

    InitLoRa();
    delay(1000);

    Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
    Heltec.display->display();
    delay(1000);
}

void loop() {

    // check if the flag is set
    if (receivedFlag) {
        // disable the interrupt service routine while
        // processing the data
        enableInterrupt = false;

        // reset flag
        //receivedFlag = false;

        Heltec.display->clear();
        Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
        Heltec.display->setFont(ArialMT_Plain_10);

        Heltec.display->drawString(0, 0, "Received packages:");
        Heltec.display->drawString(100, 0, String(counter));
        Heltec.display->display();

        uint8_t buf[RADIOLIB_SX127X_MAX_PACKET_LENGTH];
        size_t len = sizeof(buf);
        // you can receive data as an Arduino String
        // NOTE: receive() is a blocking method!
        //       See example ReceiveInterrupt for details
        //       on non-blocking reception method.

        int state = radio.receive(buf, len);

        if (state == RADIOLIB_ERR_NONE) {
            counter++;
            // packet was successfully received

            // print the data of the packet
            for(int i=0; i<MAX_BYTE_LORA; i++){
                new_package[i] = buf[i];
            }

            int16_t RSSI_LoRa = radio.getRSSI();

            // sends data via USB
            Serial.write(new_package, sizeof(new_package));
            Serial.write(RSSI_LoRa & 0xFF);
            Serial.write(RSSI_LoRa >> 8);

            Serial.println();

        } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
            // timeout occurred while waiting for a packet
            Serial.println(F("timeout!"));

        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));

        } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);

        }

        radio.startReceive();

        enableInterrupt = true;
    }
}
