#include "DataStore.h"

DataStore::DataStore()
{
    loadMockData();
}

void DataStore::loadMockData()
{
    faculties = {"ENG", "SCI", "ART", "MED", "LAW", "SOC"};
    departments["ENG"] = {"EEE", "MEC", "CIV", "CHE"};
    departments["SCI"] = {"PHY", "BIO", "CHM", "CSC"};

    attendanceSheet = {
        {"a1b2c3", "John Doe", "ENG123", false},
        {"d4e5f6", "Jane Smith", "SCI456", false}};
}

void DataStore::markPresent(const String &uid)
{
    for (auto &student : attendanceSheet)
    {
        if (student.uid == uid)
        {
            student.present = true;
            break;
        }
    }
}

Student *DataStore::findStudent(const String &uid)
{
    for (auto &student : attendanceSheet)
    {
        if (student.uid == uid)
        {
            return &student;
        }
    }
    return nullptr;
}