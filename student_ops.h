#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include "filehandler.h"

using namespace std;

/*
    students.txt columns:
    0 roll_no, 1 name, 2 department, 3 semester, 4 cgpa, 5 status
*/

bool validRollNo(string roll);
bool validName(string name);
bool addStudent(string roll, string name, string dept, string semester, string cgpa);
vector<string> searchByRoll(string roll);
vector<vector<string> > searchByName(string namePart);
bool updateStudent(string roll, int fieldNo, string newValue);
bool softDelete(string roll);
vector<vector<string> > listActiveStudents();
void printStudent(vector<string> student);
void printStudentTable(vector<vector<string> > rows);
void studentMenu();
void searchAsYouType();

#endif
