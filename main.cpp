#include <iostream>
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"

using namespace std;

void printTitle()
{
    cout << "\n==============================================" << endl;
    cout << "          CAMPUS ANALYTICS ENGINE" << endl;
    cout << "          Programming Fundamentals" << endl;
    cout << "==============================================" << endl;
}

void showMainMenu()
{
    cout << "\nMain Menu" << endl;
    cout << "1. Student Management" << endl;
    cout << "2. Course Management" << endl;
    cout << "3. Attendance Management" << endl;
    cout << "4. Grades Management" << endl;
    cout << "5. Fee Tracker" << endl;
    cout << "6. Reports" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter choice: ";
}

int main()
{
    int choice = -1;

    printTitle();

    while (choice != 0)
    {
        showMainMenu();
        cin >> choice;

        if (choice == 1)
        {
            studentMenu();
        }
        else if (choice == 2)
        {
            courseMenu();
        }
        else if (choice == 3)
        {
            attendanceMenu();
        }
        else if (choice == 4)
        {
            gradesMenu();
        }
        else if (choice == 5)
        {
            feeMenu();
        }
        else if (choice == 6)
        {
            reportsMenu();
        }
        else if (choice == 0)
        {
            cout << "Program ended. Thank you." << endl;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
