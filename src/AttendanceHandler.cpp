#include "AttendanceHandler.h"

AttendanceHandler::AttendanceHandler(DisplayManager &d, RFIDManager &r, NetworkManager &n, DataStore &ds, KeypadManager &kpm)
    : display(d), rfid(r), network(n), data(ds), keypad(kpm), state(AttendanceState::SELECT_FACULTY),
      page(0), selectedFaculty(-1), selectedLevel(-1), selectedDepartment(-1) {}

void AttendanceHandler::selectCourseFilters(char key)
{
    switch (state)
    {
    case AttendanceState::SELECT_FACULTY:
        if (key == '9')
        {
            page++;
        }
        else if (key >= '1' && key <= '4')
        {
            int index = (key - '1') + page * 4;
            if (index < data.faculties.size())
            {
                selectedFaculty = index;
                page = 0;
                state = AttendanceState::SELECT_LEVEL;
            }
        }
        break;

    case AttendanceState::SELECT_LEVEL:
        if (key >= '1' && key <= '5')
        {
            selectedLevel = key - '0';
            page = 0;
            state = AttendanceState::SELECT_DEPARTMENT;
        }
        break;

    case AttendanceState::SELECT_DEPARTMENT:
        if (key == '9')
        {
            page++;
        }
        else if (key >= '1' && key <= '4')
        {
            String faculty = data.faculties[selectedFaculty];
            auto &depts = data.departments[faculty];
            int index = (key - '1') + page * 4;
            if (index < depts.size())
            {
                selectedDepartment = index;
                state = AttendanceState::READY;

                // Simulate MQTT request
                network.publish("attendance/request", "Requesting sheet...");
                display.showMessage("Ready for attendance");
            }
        }
        break;

    case AttendanceState::READY:
        state = AttendanceState::TAKING_ATTENDANCE;
        break;

    default:
        break;
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
    switch (state)
    {
    case AttendanceState::SELECT_FACULTY:
    {
        display.drawTitle("Select Faculty");
        int start = page * 4;
        for (int i = 0; i < 4; i++)
        {
            int idx = start + i;
            if (idx < data.faculties.size())
            {
                String label = String(i + 1) + ": " + data.faculties[idx];
                display.drawLine(i, label);
            }
        }
        display.drawFooter("9: Next");
        selectCourseFilters(keypad.getKeyFromKeypad());
        break;
    }
    }
    if (display.isScreen(DisplayScreen::ATTENDANCE_FILTERS))
        display.render();
    else
        display.clear();
}