#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "filehandler.h"
#include "course_ops.h"

using namespace std;

/*
    attendance_log.txt columns:
    0 log_id, 1 roll_no, 2 course_code, 3 session_date, 4 status
*/

string nextLogId();
void markAttendance(string courseCode, string semester, string date);
double getAttendancePct(string roll, string courseCode);
vector<vector<string> > getShortageList(string courseCode, string semester);
bool undoLastSession();
void printDailySheet(string courseCode, string date);
void attendanceMenu();

#endif
