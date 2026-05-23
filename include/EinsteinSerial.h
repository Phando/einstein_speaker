#ifndef EINSTEIN_SERIAL_H
#define EINSTEIN_SERIAL_H

#include <Arduino.h>

#define EINSTEIN_RXD 4
#define EINSTEIN_TXD 0
#define EINSTEIN_BAUD 115200

class EinsteinSerial {
public:
    void begin() {
        Serial1.begin(EINSTEIN_BAUD, SERIAL_8N1, EINSTEIN_RXD, EINSTEIN_TXD);
    }

    void sendCommand(int cmd) {
        sendRaw("DEMO+" + String(cmd) + "\r");
    }

    void sendCommand(int cmd, String param) {
        sendRaw("DEMO+" + String(cmd) + "-" + param + "\r");
    }

    void setColor(String hexRGB) {
        sendCommand(20, hexRGB);
    }

    void setText(String text) {
        sendCommand(22, text);
    }

    void allOff() {
        sendCommand(21);
    }

    void whiteListen() {
        sendCommand(2);
        sendCommand(11);
    }

    void colorListen(String color) {
        setColor(color);
        sendCommand(8);
        sendCommand(11);
    }

    void whiteSpeak() {
        sendCommand(4);
        sendCommand(13);
    }

    void colorSpeak(String color) {
        setColor(color);
        sendCommand(10);
        sendCommand(13);
    }

    void crazyHair() {
        sendCommand(5);
    }

    void micOn() {
        sendCommand(16);
        sendCommand(1);
    }

    void micOff() {
        sendCommand(17);
        sendCommand(1);
    }

private:
    void sendRaw(String data) {
        Serial1.print(data);
        Serial1.flush();
    }
};

#endif
