#ifndef KEYPAD_MANAGER_H
#define KEYPAD_MANAGER_H

#include <Keypad.h>
#include "ModeManager.h"

class KeypadManager
{
private:
    Keypad keypad;
    String typedText;

public:
    KeypadManager();
    void update(ModeManager &modeManager);
};

#endif