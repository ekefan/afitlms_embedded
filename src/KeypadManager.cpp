#include "KeypadManager.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};

KeypadManager::KeypadManager()
    : keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS), typedText("") {}

void KeypadManager::handleModeChange(ModeManager &modeManager)
{
    char key = keypad.getKey();

    if (key)
    {
        if (!modeManager.isPrompting())
        {
            if (key == '0')
            {
                typedText = "0";
            }
            else if (key == '#' && typedText == "0")
            {

                SystemMode current = modeManager.getMode();
                if (current == SystemMode::ATTENDANCE)
                    modeManager.requestModeChange(SystemMode::ENROLLMENT);
                else
                    modeManager.requestModeChange(SystemMode::ATTENDANCE);

                typedText = "";
            }
            else
            {
                typedText = "";
            }
        }
        else
        {
            if (key == '1')
            {
                modeManager.confirmChange(true);
            }
            else if (key == '0')
            {
                modeManager.confirmChange(false);
            }
        }
    }
}

char KeypadManager::getKeyFromKeypad()
{
    return keypad.getKey();
}