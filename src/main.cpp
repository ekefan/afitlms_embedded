#include <Arduino.h>
#include "ModeManager.h"
#include "KeypadManager.h"
#include "DisplayManager.h"

ModeManager modeManager;
KeypadManager keypadManager;
DisplayManager displayManager;

void setup()
{
  displayManager.begin();
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
}

void loop()
{
  keypadManager.update(modeManager);
  displayManager.showMode(modeManager.getMode(), modeManager.isPrompting());
  delay(100);
}