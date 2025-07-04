#include "AttendanceHandler.h"

const char *ATTENDANCE_REQUEST_TOPIC = "attendance/request";
String ATTENDANCE_RESPONSE_TOPIC_PREFIX = "attendance/response/";

AttendanceHandler::AttendanceHandler(DisplayManager &d, RFIDManager &r, NetworkManager &n, DataStore &ds, ModeManager &mm)
    : display(d), rfid(r), network(n), data(ds), mode(mm), state(AttendanceState::SELECT_COURSE_CODE), selectedCourseCodes(-1) {}

void AttendanceHandler::selectCourseFilters(char key)
{
    if (key >= '1' && key <= '4' && state == AttendanceState::TAKING_ATTENDANCE)
    {
        int index = key - '1';
        if (index < data.courseCodes.size())
        {
            selectedCourseCodes = index;
        }
        String courseCode = data.courseCodes[selectedCourseCodes];
        display.setScreen(DisplayScreen::MESSAGE);
        display.showMessage("Ready for attendance");
        delay(2000);
        display.showMessageAtPos(30, 45, courseCode.c_str());
        fetchAttendanceData();
    }
}

void AttendanceHandler::checkCard()
{
    if (state == AttendanceState::TAKING_ATTENDANCE && rfid.isCardPresent())
    {
        String uid = rfid.readUID();
        LectureParticipants *student = data.findStudent(uid);
        if (student)
        {
            data.markPresent(uid);
            display.clear();
            display.showMessage((student->name + " " + student->uniqueId).c_str());
            delay(1500);
        }
        else
        {
            display.showMessage("Unknown card");
            delay(3000);
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
    state = AttendanceState::TAKING_ATTENDANCE;
}

AttendanceState AttendanceHandler::getState()
{
    return state;
}

void AttendanceHandler::fetchAttendanceData()
{
    String courseCode = data.courseCodes[selectedCourseCodes];
    String responseTopic = ATTENDANCE_RESPONSE_TOPIC_PREFIX + courseCode;

    network.publish(ATTENDANCE_REQUEST_TOPIC, courseCode);

    network.subscribe(responseTopic.c_str());

    display.setScreen(DisplayScreen::MESSAGE);
    display.showMessageAtPos(30, 45, "Fetching data...");
    delay(5000);

    state = AttendanceState::WAITING_FOR_ATTENDANCE_DATA;
}

void AttendanceHandler::loop()
{
    if (state == AttendanceState::WAITING_FOR_ATTENDANCE_DATA && data.ready)
    {
        display.clear();
        display.showMessageAtPos(30, 45, "Scan your cards");
        delay(2000);
        state = AttendanceState::TAKING_ATTENDANCE;
    }

    if (state == AttendanceState::TAKING_ATTENDANCE)
    {
        checkCard();
    }
}
