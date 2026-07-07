#include "reports.h"

void printMeritList()
{
    vector<vector<string> > students = listActiveStudents();

    // Bubble sort by CGPA descending
    for (int i = 0; i < (int)students.size() - 1; i++)
    {
        for (int j = 0; j < (int)students.size() - i - 1; j++)
        {
            double cgpa1 = atof(students[j][4].c_str());
            double cgpa2 = atof(students[j + 1][4].c_str());

            if (cgpa1 < cgpa2)
            {
                vector<string> temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    cout << "\n================ MERIT LIST ================" << endl;
    cout << left << setw(8) << "Rank"
         << setw(15) << "Roll"
         << setw(25) << "Name"
         << setw(10) << "CGPA" << endl;

    cout << "-------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)students.size(); i++)
    {
        cout << left << setw(8) << i + 1
             << setw(15) << students[i][0]
             << setw(25) << students[i][1]
             << setw(10) << students[i][4] << endl;
    }
}

void printAttendanceDefaulters()
{
    vector<vector<string> > courses = readTXT("courses.txt");

    cout << "\n============= ATTENDANCE DEFAULTERS =============" << endl;
    cout << left << setw(15) << "Roll"
         << setw(25) << "Name"
         << setw(12) << "Course"
         << setw(12) << "Percent" << endl;

    cout << "-------------------------------------------------------------" << endl;

    for (int c = 0; c < (int)courses.size(); c++)
    {
        if (courses[c].size() >= 1)
        {
            vector<vector<string> > enrollments = readTXT("enrollments.txt");

            for (int i = 0; i < (int)enrollments.size(); i++)
            {
                if (enrollments[i].size() >= 6 &&
                    enrollments[i][2] == courses[c][0] &&
                    enrollments[i][5] == "active")
                {
                    double pct = getAttendancePct(enrollments[i][1], courses[c][0]);

                    if (pct < 75.0)
                    {
                        vector<string> s = searchByRoll(enrollments[i][1]);

                        if (s.size() >= 2)
                        {
                            cout << left << setw(15) << s[0]
                                 << setw(25) << s[1]
                                 << setw(12) << courses[c][0]
                                 << setw(12) << pct << endl;
                        }
                    }
                }
            }
        }
    }
}

void printFeeDefaulters()
{
    vector<vector<string> > def = getDefaulters();

    cout << "\n================ FEE DEFAULTERS ================" << endl;
    cout << left << setw(15) << "Roll"
         << setw(25) << "Name"
         << setw(12) << "Semester"
         << setw(12) << "Balance" << endl;

    cout << "-------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)def.size(); i++)
    {
        if (def[i].size() >= 10)
        {
            vector<string> s = searchByRoll(def[i][1]);
            string name = "Unknown";

            if (s.size() >= 2)
            {
                name = s[1];
            }

            cout << left << setw(15) << def[i][1]
                 << setw(25) << name
                 << setw(12) << def[i][2]
                 << setw(12) << def[i][9] << endl;
        }
    }
}

void printSemesterResult(string semester)
{
    vector<vector<string> > students = listActiveStudents();

    cout << "\n================ SEMESTER RESULT ================" << endl;
    cout << left << setw(15) << "Roll"
         << setw(25) << "Name"
         << setw(12) << "Semester"
         << setw(12) << "GPA/CGPA"
         << setw(15) << "Status" << endl;

    cout << "-------------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)students.size(); i++)
    {
        if (students[i].size() >= 5)
        {
            double gpa = computeGPA(students[i][0], semester);
            string status = "Pass";

            if (gpa < 2.0)
            {
                status = "Warning";
            }

            cout << left << setw(15) << students[i][0]
                 << setw(25) << students[i][1]
                 << setw(12) << semester
                 << setw(12) << gpa
                 << setw(15) << status << endl;
        }
    }
}

void printDepartmentSummary()
{
    vector<vector<string> > students = listActiveStudents();

    string departments[50];
    int counts[50];
    double totalCgpa[50];
    int deptCount = 0;

    for (int i = 0; i < 50; i++)
    {
        counts[i] = 0;
        totalCgpa[i] = 0.0;
    }

    for (int i = 0; i < (int)students.size(); i++)
    {
        if (students[i].size() >= 5)
        {
            string dept = students[i][2];
            int index = -1;

            for (int j = 0; j < deptCount; j++)
            {
                if (departments[j] == dept)
                {
                    index = j;
                }
            }

            if (index == -1)
            {
                departments[deptCount] = dept;
                index = deptCount;
                deptCount++;
            }

            counts[index]++;
            totalCgpa[index] += atof(students[i][4].c_str());
        }
    }

    cout << "\n================ DEPARTMENT SUMMARY ================" << endl;
    cout << left << setw(30) << "Department"
         << setw(10) << "Count"
         << setw(15) << "Avg CGPA"
         << setw(12) << "Pass Rate" << endl;

    cout << "----------------------------------------------------------------" << endl;

    for (int i = 0; i < deptCount; i++)
    {
        double avg = 0.0;
        if (counts[i] > 0)
        {
            avg = totalCgpa[i] / counts[i];
        }

        cout << left << setw(30) << departments[i]
             << setw(10) << counts[i]
             << setw(15) << avg
             << setw(12) << "100%" << endl;
    }
}

void exportReportToFile(int reportChoice)
{
    ofstream out("exported_report.txt");

    if (!out)
    {
        cout << "Unable to create report file." << endl;
        return;
    }

    streambuf* oldBuffer = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    if (reportChoice == 1)
    {
        printMeritList();
    }
    else if (reportChoice == 2)
    {
        printAttendanceDefaulters();
    }
    else if (reportChoice == 3)
    {
        printFeeDefaulters();
    }
    else if (reportChoice == 4)
    {
        printDepartmentSummary();
    }

    cout.rdbuf(oldBuffer);
    out.close();

    cout << "Report exported to exported_report.txt" << endl;
}

void reportsMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== REPORTS ======" << endl;
        cout << "1. Merit list" << endl;
        cout << "2. Attendance defaulters" << endl;
        cout << "3. Fee defaulters" << endl;
        cout << "4. Semester result" << endl;
        cout << "5. Department summary" << endl;
        cout << "6. Export report to file" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            printMeritList();
        }
        else if (choice == 2)
        {
            printAttendanceDefaulters();
        }
        else if (choice == 3)
        {
            printFeeDefaulters();
        }
        else if (choice == 4)
        {
            string sem;
            cout << "Semester: ";
            cin >> sem;
            printSemesterResult(sem);
        }
        else if (choice == 5)
        {
            printDepartmentSummary();
        }
        else if (choice == 6)
        {
            int r;
            cout << "Export which report? 1 Merit, 2 Attendance, 3 Fee, 4 Department: ";
            cin >> r;
            exportReportToFile(r);
        }
    }
}
