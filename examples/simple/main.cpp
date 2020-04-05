#include <OperatingMode.h>
#include <Display.h>
#include <StatusBar.h>

OperatingMode operating_mode;
Display display;
StatusBar status_bar{operating_mode, display};

void processNormalMode()
{
    status_bar.update();
}

void processSwitchToWifi()
{
    status_bar.update();
}

void processWifi(){}
{
    status_bar.update();
}

void setup()
{
      display.setup();
}

void loop()
{
    if (operating_mode == OperatingMode::Mode::Normal)
        processNormalMode();
    else if (operating_mode == OperatingMode::Mode::SwitchToWifi)
        processSwitchToWifi();
    else if (operating_mode == OperatingMode::Mode::Wifi)
        processWifi();
}
