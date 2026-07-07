#ifndef REPORTS_H
#define REPORTS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"

using namespace std;

void printMeritList();
void printAttendanceDefaulters();
void printFeeDefaulters();
void printSemesterResult(string semester);
void printDepartmentSummary();
void exportReportToFile(int reportChoice);
void reportsMenu();

#endif
