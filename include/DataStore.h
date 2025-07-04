#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <vector>
#include <map>
#include <Arduino.h>

struct Student
{
    String uid;
    String name;
    String matric;
    bool present;
};

class DataStore
{
public:
    std::vector<String> faculties;
    std::map<String, std::vector<String>> departments;
    std::vector<Student> attendanceSheet;

    DataStore();
    void loadMockData();
    void markPresent(const String &uid);
    Student *findStudent(const String &uid);
};

#endif