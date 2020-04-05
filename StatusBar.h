#pragma once

#include <Display.h>
#include "OperatingMode.h"
#if defined(ESP8266)
#include <avr/pgmspace.h>
#elif defined (ESP32)
#include <pgmspace.h>
#endif

struct StatusBar
{
    StatusBar(const OperatingMode &operating_mode, Display &display);

    void update();

protected:

    const OperatingMode &operating_mode;
    Display &display;

    void printWifiStatus();

    Dimension bar_dimension{0, 0};
    Rectangle print_area{0, 0, 0, 0};

    void reset();
};
