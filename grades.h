#ifndef GRADES_H
#define GRADES_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"
#include "attendance.h"

using namespace std;

struct Stats
{
    double highest;
    double lowest;
    double mean;
    double median;
};

double bestThreeOfFive(double quiz[], int n);
double computeWeightedTotal(double quizAvg, double asgnAvg, double mid, double finalMarks);
string getLetterGrade(double total);
double gradePoint(string grade);
double computeGPA(string roll, string semester);
Stats computeClassState(double arr[], int n);
string applyAttendancePenalty(string roll, string courseCode, string grade);
void enterMarks();
void gradesMenu();

#endif
