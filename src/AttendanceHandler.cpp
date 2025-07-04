#include "AttendanceHandler.h"

AttendanceHandler::AttendanceHandler(DisplayManager &d, RFIDManager &r, NetworkManager &n, DataStore &ds, KeypadManager &kpm)
    : display(d), rfid(r), network(n), data(ds), keypad(kpm), state(AttendanceState::SELECT_FACULTY), selectedCourseCodes(-1) {}

void AttendanceHandler::selectCourseFilters(char key)
{
    if (key >= '1' && key <= '4')
    {
        int index = key - '1';
        if (index < data.courseCodes.size())
        {
            selectedCourseCodes = index;
        }
        String courseCode = data.courseCodes[selectedCourseCodes];
        display.setScreen(DisplayScreen::MESSAGE);
        display.showMessage("Ready to take attendance for lecture");
    }
}

void AttendanceHandler::checkCard()
{
    if (state == AttendanceState::TAKING_ATTENDANCE && rfid.isCardPresent())
    {
        String uid = rfid.readUID();
        Student *student = data.findStudent(uid);
        if (student)
        {
            data.markPresent(uid);
            display.showMessage((student->name + " " + student->matric).c_str());
            delay(1500);
        }
        else
        {
            display.showMessage("Unknown card");
            delay(1500);
        }
    }
}

void AttendanceHandler::displayFilters()
{
    data.loadMockData();
    display.drawTitle("Select Course Code");
    for (int i = 0; i < 4; i++)
    {
        if (i < data.courseCodes.size())
        {
            String label = String(i + 1) + ": " + data.courseCodes[i];
            display.drawLine(i, label);
        }
    }
    selectCourseFilters(keypad.getKeyFromKeypad());
}