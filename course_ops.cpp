#include "course_ops.h"

string nextEnrollmentId()
{
    int total = countRows("enrollments.txt") + 1;
    string id = "E";

    if (total < 10) id += "000";
    else if (total < 100) id += "00";
    else if (total < 1000) id += "0";

    id += to_string(total);
    return id;
}

void printCourses()
{
    vector<vector<string> > courses = readTXT("courses.txt");

    cout << left << setw(10) << "Code"
         << setw(35) << "Course"
         << setw(10) << "Credit"
         << setw(10) << "Seats"
         << setw(10) << "Filled"
         << setw(12) << "Prereq" << endl;

    cout << "--------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)courses.size(); i++)
    {
        if (courses[i].size() >= 7)
        {
            cout << left << setw(10) << courses[i][0]
                 << setw(35) << courses[i][1]
                 << setw(10) << courses[i][2]
                 << setw(10) << courses[i][4]
                 << setw(10) << courses[i][5]
                 << setw(12) << courses[i][6] << endl;
        }
    }
}

int getCreditLoad(string roll, string semester)
{
    vector<vector<string> > enrollments = readTXT("enrollments.txt");
    vector<vector<string> > courses = readTXT("courses.txt");

    int totalCredits = 0;

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if (enrollments[i].size() >= 6 &&
            enrollments[i][1] == roll &&
            enrollments[i][3] == semester &&
            enrollments[i][5] == "active")
        {
            string code = enrollments[i][2];

            for (int j = 0; j < (int)courses.size(); j++)
            {
                if (courses[j].size() >= 3 && courses[j][0] == code)
                {
                    totalCredits += atoi(courses[j][2].c_str());
                }
            }
        }
    }

    return totalCredits;
}

bool checkPrerequisite(string roll, string courseCode)
{
    vector<string> course = findRow("courses.txt", 0, courseCode);

    if (course.size() < 7)
    {
        return false;
    }

    string prereq = course[6];

    if (prereq == "NONE")
    {
        return true;
    }

    // Basic PF version: prerequisite is considered completed
    // if student has already been enrolled in that prerequisite course.
    vector<vector<string> > enrollments = readTXT("enrollments.txt");

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if (enrollments[i].size() >= 6 &&
            enrollments[i][1] == roll &&
            enrollments[i][2] == prereq)
        {
            return true;
        }
    }

    return false;
}

EnrollResult enrollStudent(string roll, string courseCode, string semester, string date)
{
    vector<string> student = searchByRoll(roll);

    if (student.size() == 0 || student.size() < 6 || student[5] != "active")
    {
        return STUDENT_NOT_FOUND;
    }

    vector<string> course = findRow("courses.txt", 0, courseCode);

    if (course.size() == 0 || course.size() < 7)
    {
        return COURSE_NOT_FOUND;
    }

    vector<vector<string> > enrollments = readTXT("enrollments.txt");

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if (enrollments[i].size() >= 6 &&
            enrollments[i][1] == roll &&
            enrollments[i][2] == courseCode &&
            enrollments[i][3] == semester &&
            enrollments[i][5] == "active")
        {
            return ALREADY_ENROLLED;
        }
    }

    int capacity = atoi(course[4].c_str());
    int enrolled = atoi(course[5].c_str());

    if (enrolled >= capacity)
    {
        return SEAT_FULL;
    }

    int newLoad = getCreditLoad(roll, semester) + atoi(course[2].c_str());

    if (newLoad > 21)
    {
        return CREDIT_LIMIT;
    }

    if (!checkPrerequisite(roll, courseCode))
    {
        return PREREQ_MISSING;
    }

    vector<string> row;
    row.push_back(nextEnrollmentId());
    row.push_back(roll);
    row.push_back(courseCode);
    row.push_back(semester);
    row.push_back(date);
    row.push_back("active");

    appendTXT("enrollments.txt", row);

    // update course enrolled count
    vector<string> header = readHeader("courses.txt");
    vector<vector<string> > courses = readTXT("courses.txt");

    for (int i = 0; i < (int)courses.size(); i++)
    {
        if (courses[i].size() >= 6 && courses[i][0] == courseCode)
        {
            int count = atoi(courses[i][5].c_str());
            count++;
            courses[i][5] = to_string(count);
        }
    }

    writeTXT("courses.txt", header, courses);

    return ENROLL_SUCCESS;
}

bool dropCourse(string roll, string courseCode, string semester)
{
    vector<vector<string> > att = readTXT("attendance_log.txt");

    for (int i = 0; i < (int)att.size(); i++)
    {
        if (att[i].size() >= 5 &&
            att[i][1] == roll &&
            att[i][2] == courseCode)
        {
            cout << "Cannot drop. Attendance record already exists." << endl;
            return false;
        }
    }

    vector<string> header = readHeader("enrollments.txt");
    vector<vector<string> > rows = readTXT("enrollments.txt");
    bool changed = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 6 &&
            rows[i][1] == roll &&
            rows[i][2] == courseCode &&
            rows[i][3] == semester &&
            rows[i][5] == "active")
        {
            rows[i][5] = "dropped";
            changed = true;
        }
    }

    if (changed)
    {
        writeTXT("enrollments.txt", header, rows);
    }

    return changed;
}

vector<vector<string> > listEnrolledStudents(string courseCode, string semester)
{
    vector<vector<string> > result;
    vector<vector<string> > enrollments = readTXT("enrollments.txt");

    for (int i = 0; i < (int)enrollments.size(); i++)
    {
        if (enrollments[i].size() >= 6 &&
            enrollments[i][2] == courseCode &&
            enrollments[i][3] == semester &&
            enrollments[i][5] == "active")
        {
            vector<string> student = searchByRoll(enrollments[i][1]);
            if (student.size() > 0)
            {
                result.push_back(student);
            }
        }
    }

    return result;
}

void printEnrollResult(EnrollResult result)
{
    if (result == ENROLL_SUCCESS) cout << "Enrollment successful." << endl;
    else if (result == STUDENT_NOT_FOUND) cout << "Student not found or inactive." << endl;
    else if (result == COURSE_NOT_FOUND) cout << "Course not found." << endl;
    else if (result == ALREADY_ENROLLED) cout << "Student already enrolled." << endl;
    else if (result == SEAT_FULL) cout << "Course seats are full." << endl;
    else if (result == CREDIT_LIMIT) cout << "Credit limit exceeded." << endl;
    else if (result == PREREQ_MISSING) cout << "Prerequisite not completed." << endl;
}

void courseMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== COURSE MANAGEMENT ======" << endl;
        cout << "1. View courses" << endl;
        cout << "2. Enroll student" << endl;
        cout << "3. Drop course" << endl;
        cout << "4. Check credit load" << endl;
        cout << "5. List enrolled students" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            printCourses();
        }
        else if (choice == 2)
        {
            string roll, code, sem, date;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Course Code: ";
            cin >> code;
            cout << "Semester: ";
            cin >> sem;
            cout << "Date DD-MM-YYYY: ";
            cin >> date;

            printEnrollResult(enrollStudent(roll, code, sem, date));
        }
        else if (choice == 3)
        {
            string roll, code, sem;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Course Code: ";
            cin >> code;
            cout << "Semester: ";
            cin >> sem;

            if (dropCourse(roll, code, sem))
                cout << "Course dropped." << endl;
            else
                cout << "Drop failed." << endl;
        }
        else if (choice == 4)
        {
            string roll, sem;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Semester: ";
            cin >> sem;
            cout << "Credit Load: " << getCreditLoad(roll, sem) << endl;
        }
        else if (choice == 5)
        {
            string code, sem;
            cout << "Course Code: ";
            cin >> code;
            cout << "Semester: ";
            cin >> sem;
            printStudentTable(listEnrolledStudents(code, sem));
        }
    }
}
