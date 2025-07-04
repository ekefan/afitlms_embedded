#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : u8g2(U8G2_R0, U8X8_PIN_NONE) {}

void DisplayManager::begin()
{
    u8g2.begin();
}

const char *modeToString(SystemMode mode)
{
    switch (mode)
    {
    case SystemMode::ATTENDANCE:
        return "ATTENDANCE";
    case SystemMode::ENROLLMENT:
        return "ENROLLMENT";
    default:
        return "UNKNOWN";
    }
}

void DisplayManager::showMode(SystemMode mode, bool prompt)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    if (mode == SystemMode::ATTENDANCE)
    {
        u8g2.drawStr(120, 10, "A");
    }
    else
    {
        u8g2.drawStr(120, 10, "E");
    }

    if (prompt)
    {
        u8g2.drawStr(10, 30, "Change mode?");
        u8g2.drawStr(10, 45, "1: Yes  0: No");
    }

    u8g2.sendBuffer();
}

void DisplayManager::showMessage(const char *msg)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    u8g2.drawStr(10, 30, msg);

    u8g2.sendBuffer();
}