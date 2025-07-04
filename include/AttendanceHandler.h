#ifndef ATTENDANCE_HANDLER_H
#define ATTENDANCE_HANDLER_H

#include "DisplayManager.h"
#include "RFIDManager.h"
#include "NetworkManager.h"
#include "DataStore.h"
#include "KeypadManager.h"

enum class AttendanceState
{
    SELECT_FACULTY,
    SELECT_LEVEL,
    SELECT_DEPARTMENT,
    READY,
    TAKING_ATTENDANCE
};

class AttendanceHandler
{
private:
    DisplayManager &display;
    RFIDManager &rfid;
    NetworkManager &network;
    KeypadManager &keypad;
    DataStore &data;

    AttendanceState state;
    int page;
    int selectedFaculty;
    int selectedLevel;
    int selectedDepartment;

public:
    AttendanceHandler(DisplayManager &d, RFIDManager &r, NetworkManager &n, DataStore &ds, KeypadManager &kpm);
    void selectCourseFilters(char key);
    void checkCard();
    void displayFilters();
};

#endif