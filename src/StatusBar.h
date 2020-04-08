#pragma once

#include <Display.h>
#include "OperatingMode.h"
#if defined(ESP8266)
#include <avr/pgmspace.h>
#elif defined (ESP32)
#include <pgmspace.h>
#endif

struct StatusBarData {
    // enable printing wifi symbol
    bool enable_wifi_status {true};

    // enable printing temperature
    bool enable_temperature {false};
    float temperature_celsius{0};
};

struct StatusBar
{
    StatusBar(const OperatingMode &operating_mode, Display &display);

    void update();

    StatusBarData data;

protected:

    const OperatingMode &operating_mode;
    Display &display;

    void printWifiStatus();

    Dimension bar_dimension{0, 0};
    Rectangle print_area{0, 0, 0, 0};

    void reset();
};
