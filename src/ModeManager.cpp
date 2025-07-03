#include "ModeManager.h"

ModeManager::ModeManager()
{
    currentMode = SystemMode::ATTENDANCE;
    promptActive = false;
}

void ModeManager::requestModeChange(SystemMode newMode)
{
    requestedMode = newMode;
    promptActive = true;
}

void ModeManager::confirmChange(bool accept)
{
    if (accept)
        currentMode = requestedMode;
    promptActive = false;
}

SystemMode ModeManager::getMode() const
{
    return currentMode;
}

bool ModeManager::isPrompting() const
{
    return promptActive;
}