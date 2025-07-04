#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include "ModeManager.h"

class DisplayManager
{
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

public:
    DisplayManager();
    void begin();
    void showMode(SystemMode mode, bool prompt);
};

#endif