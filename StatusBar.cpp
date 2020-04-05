#include "StatusBar.h"
#if defined(ESP8266)
#include <include/wl_definitions.h>
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFiType.h>
#include <WiFi.h>
#endif

namespace
{
    // ☐
    constexpr const uint8_t symbol_wifi_disconnected[] PROGMEM {
            0b11111111,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b10000001,
            0b11111111};
    // ✗
    constexpr const uint8_t symbol_wifi_disabled[] PROGMEM {
            0b01000001,
            0b00100010,
            0b00010100,
            0b00001000,
            0b00010100,
            0b00100010,
            0b01000001};
    // 📶
    constexpr const uint8_t symbol_wifi_connected_strong[] PROGMEM  {
            0b00000001,
            0b00000001,
            0b00000101,
            0b00000101,
            0b00010101,
            0b00010101,
            0b01010101};

    // 📶
    constexpr const uint8_t symbol_wifi_connected_moderate[] PROGMEM  {
            0b00000000,
            0b00000000,
            0b00000100,
            0b00000100,
            0b00010100,
            0b00010100,
            0b01010100};

    // 📶
    constexpr const uint8_t symbol_wifi_connected_weak[] PROGMEM  {
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00010000,
            0b00010000,
            0b01010000};
    // 📶
    constexpr const uint8_t symbol_wifi_connected_barely[] PROGMEM  {
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b00000000,
            0b01000000};
    // 📶
    constexpr const uint8_t symbol_wifi_ap[] PROGMEM  {
            0b01001001,
            0b00101010,
            0b00010100,
            0b00001000,
            0b00001000,
            0b00001000,
            0b00001000};

    struct SymbolWifiConnectedStrong : public SymbolBitmap<8, 7, WHITE, symbol_wifi_connected_strong>
    {
    };
    struct SymbolWifiConnectedModerate : public SymbolBitmap<8, 7, WHITE, symbol_wifi_connected_moderate>
    {
    };
    struct SymbolWifiConnectedWeak : public SymbolBitmap<8, 7, WHITE, symbol_wifi_connected_weak>
    {
    };
    struct SymbolWifiConnectedBarely : public SymbolBitmap<8, 7, WHITE, symbol_wifi_connected_barely>
    {
    };
    struct SymbolWifiDisconnected : public SymbolBitmap<8, 7, WHITE, symbol_wifi_disconnected>
    {
    };
    struct SymbolWifiDisabled : public SymbolBitmap<8, 7, WHITE, symbol_wifi_disabled>
    {
    };
    struct SymbolWifiAp : public SymbolBitmap<8, 7, WHITE, symbol_wifi_ap>
    {
    };
}

StatusBar::StatusBar(const OperatingMode &operating_mode, Display &display) : operating_mode(operating_mode),
                                                                              display(display)
{
    bar_dimension = Dimension{display.width(), 8};
    print_area = Rectangle{0, 0, bar_dimension.width, bar_dimension.height};
}

void StatusBar::update()
{
    reset();
    printWifiStatus();
    display.display();
}

void StatusBar::printWifiStatus()
{
    bool is_sta_enabled = operating_mode == OperatingMode::Mode::Wifi && WiFi.getMode() != WIFI_OFF;
    bool is_sta_connected =
            WiFi.status() == WL_CONNECTED && (WiFi.getMode() == WIFI_STA || WiFi.getMode() == WIFI_AP_STA);
    bool is_ap_enabled = WiFi.getMode() != WIFI_OFF && (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA);
    Rectangle &b = print_area;

    // TODO: investigate how to calculate the signal strength.
    const int8_t assumed_noise_floor = -120;

    if (is_ap_enabled)
    {
        display.drawBitmap<SymbolWifiAp>(b.width - 1 - SymbolWifiAp::width, 0);
    } else if (is_sta_enabled)
    {
        if (is_sta_connected)
        {
            int32_t snr = (-assumed_noise_floor) - (-WiFi.RSSI());
            if (snr > 50)
            {
                display.drawBitmap<SymbolWifiConnectedStrong>(b.width - 1 - SymbolWifiConnectedStrong::width);
            } else if (snr > 20)
            {
                display.drawBitmap<SymbolWifiConnectedModerate>(
                        display.width() - 1 - SymbolWifiConnectedModerate::width,
                        0);
            } else if (snr > 10)
            {
                display.drawBitmap<SymbolWifiConnectedWeak>(b.width - 1 - SymbolWifiConnectedWeak::width);
            } else
            {
                display.drawBitmap<SymbolWifiConnectedBarely>(b.width - 1 - SymbolWifiConnectedBarely::width);
            }
        } else
        {
            display.drawBitmap<SymbolWifiDisconnected>(b.width - 1 - SymbolWifiDisconnected::width);
        }
    } else
    {
        display.drawBitmap<SymbolWifiDisabled>(b.width - 1 - SymbolWifiDisabled::width);
    }
}

void StatusBar::reset()
{
    Rectangle &b = print_area;
    display.fillRect(b.x, b.y, b.width, b.height, BLACK);
    display.setCursorPosition(b.x, b.y);
}
