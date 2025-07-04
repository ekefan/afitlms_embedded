#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

enum class SystemMode
{
    ATTENDANCE,
    ENROLLMENT,
};

class ModeManager
{
private:
    SystemMode currentMode;
    bool promptActive;
    bool attendanceActive;
    SystemMode requestedMode;

public:
    ModeManager();

    void requestModeChange(SystemMode newMode);
    void confirmChange(bool accept);
    SystemMode getMode() const;
    bool isPrompting() const;
    bool isTakingAttendance() const;
    void setAttendanceModeChange(SystemMode newMode);
};

#endif