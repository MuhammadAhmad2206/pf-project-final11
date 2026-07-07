#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include "filehandler.h"
#include "student_ops.h"

using namespace std;

/*
    courses.txt columns:
    0 course_code, 1 course_name, 2 credit_hours, 3 instructor, 4 capacity, 5 enrolled, 6 prerequisite

    enrollments.txt columns:
    0 enrollment_id, 1 roll_no, 2 course_code, 3 semester, 4 enrollment_date, 5 status
*/

enum EnrollResult
{
    ENROLL_SUCCESS,
    STUDENT_NOT_FOUND,
    COURSE_NOT_FOUND,
    ALREADY_ENROLLED,
    SEAT_FULL,
    CREDIT_LIMIT,
    PREREQ_MISSING
};

string nextEnrollmentId();
EnrollResult enrollStudent(string roll, string courseCode, string semester, string date);
bool dropCourse(string roll, string courseCode, string semester);
int getCreditLoad(string roll, string semester);
bool checkPrerequisite(string roll, string courseCode);
vector<vector<string> > listEnrolledStudents(string courseCode, string semester);
void printCourses();
void printEnrollResult(EnrollResult result);
void courseMenu();

#endif
