#include "attendance.h"

vector<vector<string> > backupAttendance;

string nextLogId()
{
    int total = countRows("attendance_log.txt") + 1;
    string id = "L";

    if (total < 10) id += "0000";
    else if (total < 100) id += "000";
    else if (total < 1000) id += "00";
    else if (total < 10000) id += "0";

    id += to_string(total);
    return id;
}

void markAttendance(string courseCode, string semester, string date)
{
    backupAttendance = readTXT("attendance_log.txt");

    vector<vector<string> > students = listEnrolledStudents(courseCode, semester);

    if (students.size() == 0)
    {
        cout << "No enrolled students found." << endl;
        return;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        char status;
        cout << students[i][0] << " - " << students[i][1] << " (P/A/L): ";
        cin >> status;

        while (status != 'P' && status != 'A' && status != 'L')
        {
            cout << "Enter only P, A, or L: ";
            cin >> status;
        }

        vector<string> row;
        row.push_back(nextLogId());
        row.push_back(students[i][0]);
        row.push_back(courseCode);
        row.push_back(date);
        string s = "";
        s += status;
        row.push_back(s);

        appendTXT("attendance_log.txt", row);
    }

    cout << "Attendance saved." << endl;
}

double getAttendancePct(string roll, string courseCode)
{
    vector<vector<string> > rows = readTXT("attendance_log.txt");

    int total = 0;
    double presentValue = 0.0;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 5 &&
            rows[i][1] == roll &&
            rows[i][2] == courseCode)
        {
            total++;

            if (rows[i][4] == "P")
            {
                presentValue += 1.0;
            }
            else if (rows[i][4] == "L")
            {
                presentValue += 0.5;
            }
        }
    }

    if (total == 0)
    {
        return 0.0;
    }

    return (presentValue / total) * 100.0;
}

vector<vector<string> > getShortageList(string courseCode, string semester)
{
    vector<vector<string> > students = listEnrolledStudents(courseCode, semester);
    vector<vector<string> > shortage;

    for (int i = 0; i < (int)students.size(); i++)
    {
        double pct = getAttendancePct(students[i][0], courseCode);

        if (pct < 75.0)
        {
            students[i].push_back(to_string(pct));
            shortage.push_back(students[i]);
        }
    }

    return shortage;
}

bool undoLastSession()
{
    if (backupAttendance.size() == 0)
    {
        return false;
    }

    vector<string> header = readHeader("attendance_log.txt");
    writeTXT("attendance_log.txt", header, backupAttendance);
    backupAttendance.clear();
    return true;
}

void printDailySheet(string courseCode, string date)
{
    vector<vector<string> > rows = readTXT("attendance_log.txt");

    cout << left << setw(15) << "Roll"
         << setw(12) << "Course"
         << setw(15) << "Date"
         << setw(10) << "Status" << endl;

    cout << "---------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 5 &&
            rows[i][2] == courseCode &&
            rows[i][3] == date)
        {
            cout << left << setw(15) << rows[i][1]
                 << setw(12) << rows[i][2]
                 << setw(15) << rows[i][3]
                 << setw(10) << rows[i][4] << endl;
        }
    }
}

void attendanceMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== ATTENDANCE MANAGEMENT ======" << endl;
        cout << "1. Mark attendance" << endl;
        cout << "2. Attendance percentage" << endl;
        cout << "3. Shortage list" << endl;
        cout << "4. Undo last session" << endl;
        cout << "5. Print daily sheet" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string code, sem, date;
            cout << "Course Code: ";
            cin >> code;
            cout << "Semester: ";
            cin >> sem;
            cout << "Date DD-MM-YYYY: ";
            cin >> date;
            markAttendance(code, sem, date);
        }
        else if (choice == 2)
        {
            string roll, code;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Course Code: ";
            cin >> code;
            cout << "Attendance: " << getAttendancePct(roll, code) << "%" << endl;
        }
        else if (choice == 3)
        {
            string code, sem;
            cout << "Course Code: ";
            cin >> code;
            cout << "Semester: ";
            cin >> sem;

            vector<vector<string> > list = getShortageList(code, sem);
            printStudentTable(list);
        }
        else if (choice == 4)
        {
            if (undoLastSession())
                cout << "Last session undone." << endl;
            else
                cout << "No backup available." << endl;
        }
        else if (choice == 5)
        {
            string code, date;
            cout << "Course Code: ";
            cin >> code;
            cout << "Date: ";
            cin >> date;
            printDailySheet(code, date);
        }
    }
}
