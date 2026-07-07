#include "student_ops.h"

bool validRollNo(string roll)
{
    // Required format: BSAI-YY-XXX
    if (roll.length() != 11)
    {
        return false;
    }

    if (roll.substr(0, 5) != "BSAI-")
    {
        return false;
    }

    if (roll[7] != '-')
    {
        return false;
    }

    if (roll[5] < '0' || roll[5] > '9') return false;
    if (roll[6] < '0' || roll[6] > '9') return false;
    if (roll[8] < '0' || roll[8] > '9') return false;
    if (roll[9] < '0' || roll[9] > '9') return false;
    if (roll[10] < '0' || roll[10] > '9') return false;

    return true;
}

bool validName(string name)
{
    if (name.length() == 0)
    {
        return false;
    }

    for (int i = 0; i < (int)name.length(); i++)
    {
        if (name[i] >= '0' && name[i] <= '9')
        {
            return false;
        }
    }

    return true;
}

bool addStudent(string roll, string name, string dept, string semester, string cgpa)
{
    if (!validRollNo(roll))
    {
        cout << "Invalid roll number format. Use BSAI-YY-XXX." << endl;
        return false;
    }

    if (!validName(name))
    {
        cout << "Invalid name. Name cannot be empty or contain digits." << endl;
        return false;
    }

    if (rowExists("students.txt", 0, roll))
    {
        cout << "Student already exists." << endl;
        return false;
    }

    double value = atof(cgpa.c_str());

    if (value < 0.0 || value > 4.0)
    {
        cout << "CGPA must be between 0.0 and 4.0." << endl;
        return false;
    }

    vector<string> row;
    row.push_back(roll);
    row.push_back(name);
    row.push_back(dept);
    row.push_back(semester);
    row.push_back(cgpa);
    row.push_back("active");

    appendTXT("students.txt", row);
    return true;
}

vector<string> searchByRoll(string roll)
{
    return findRow("students.txt", 0, roll);
}

vector<vector<string> > searchByName(string namePart)
{
    vector<vector<string> > result;
    vector<vector<string> > rows = readTXT("students.txt");

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if ((int)rows[i].size() > 1)
        {
            string name = rows[i][1];

            // Simple substring search without find()
            for (int j = 0; j <= (int)name.length() - (int)namePart.length(); j++)
            {
                if (name.substr(j, namePart.length()) == namePart)
                {
                    result.push_back(rows[i]);
                    break;
                }
            }
        }
    }

    return result;
}

bool updateStudent(string roll, int fieldNo, string newValue)
{
    if (fieldNo == 0)
    {
        cout << "Roll number cannot be updated." << endl;
        return false;
    }

    vector<string> header = readHeader("students.txt");
    vector<vector<string> > rows = readTXT("students.txt");
    bool found = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() > 0 && rows[i][0] == roll)
        {
            if (fieldNo >= 0 && fieldNo < (int)rows[i].size())
            {
                if (fieldNo == 1 && !validName(newValue))
                {
                    cout << "Invalid name." << endl;
                    return false;
                }

                if (fieldNo == 4)
                {
                    double g = atof(newValue.c_str());
                    if (g < 0.0 || g > 4.0)
                    {
                        cout << "Invalid CGPA." << endl;
                        return false;
                    }
                }

                rows[i][fieldNo] = newValue;
                found = true;
            }
        }
    }

    if (found)
    {
        writeTXT("students.txt", header, rows);
    }

    return found;
}

bool softDelete(string roll)
{
    vector<string> header = readHeader("students.txt");
    vector<vector<string> > rows = readTXT("students.txt");
    bool found = false;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 6 && rows[i][0] == roll)
        {
            rows[i][5] = "inactive";
            found = true;
        }
    }

    if (found)
    {
        writeTXT("students.txt", header, rows);
    }

    return found;
}

vector<vector<string> > listActiveStudents()
{
    vector<vector<string> > rows = readTXT("students.txt");
    vector<vector<string> > active;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 6 && rows[i][5] == "active")
        {
            active.push_back(rows[i]);
        }
    }

    // Selection sort by roll number
    for (int i = 0; i < (int)active.size() - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < (int)active.size(); j++)
        {
            if (active[j][0] < active[minIndex][0])
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            vector<string> temp = active[i];
            active[i] = active[minIndex];
            active[minIndex] = temp;
        }
    }

    return active;
}

void printStudent(vector<string> s)
{
    if (s.size() == 0)
    {
        cout << "Student not found." << endl;
        return;
    }

    cout << "\nRoll No    : " << s[0] << endl;
    cout << "Name       : " << s[1] << endl;
    cout << "Department : " << s[2] << endl;
    cout << "Semester   : " << s[3] << endl;
    cout << "CGPA       : " << s[4] << endl;
    cout << "Status     : " << s[5] << endl;
}

void printStudentTable(vector<vector<string> > rows)
{
    cout << left << setw(15) << "Roll"
         << setw(25) << "Name"
         << setw(25) << "Department"
         << setw(10) << "Sem"
         << setw(10) << "CGPA"
         << setw(12) << "Status" << endl;

    cout << "--------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].size() >= 6)
        {
            cout << left << setw(15) << rows[i][0]
                 << setw(25) << rows[i][1]
                 << setw(25) << rows[i][2]
                 << setw(10) << rows[i][3]
                 << setw(10) << rows[i][4]
                 << setw(12) << rows[i][5] << endl;
        }
    }
}

void searchAsYouType()
{
    string keyword = "";
    char ch;

    cout << "\nSearch as you type demo." << endl;
    cout << "Enter characters one by one. Enter # to stop." << endl;

    while (true)
    {
        cout << "\nCurrent search: " << keyword << endl;
        cout << "Next character: ";
        cin >> ch;

        if (ch == '#')
        {
            break;
        }

        keyword += ch;

        vector<vector<string> > rows = readTXT("students.txt");
        vector<vector<string> > matched;

        for (int i = 0; i < (int)rows.size(); i++)
        {
            if (rows[i].size() >= 2)
            {
                string name = rows[i][1];
                if (name.length() >= keyword.length())
                {
                    if (name.substr(0, keyword.length()) == keyword)
                    {
                        matched.push_back(rows[i]);
                    }
                }
            }
        }

        printStudentTable(matched);
    }
}

void studentMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== STUDENT MANAGEMENT ======" << endl;
        cout << "1. Add student" << endl;
        cout << "2. Search by roll" << endl;
        cout << "3. Search by name" << endl;
        cout << "4. Update student" << endl;
        cout << "5. Soft delete student" << endl;
        cout << "6. List active students" << endl;
        cout << "7. Search as you type" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string roll, name, dept, sem, cgpa;
            cin.ignore();
            cout << "Roll No: ";
            getline(cin, roll);
            cout << "Name: ";
            getline(cin, name);
            cout << "Department: ";
            getline(cin, dept);
            cout << "Semester: ";
            getline(cin, sem);
            cout << "CGPA: ";
            getline(cin, cgpa);

            if (addStudent(roll, name, dept, sem, cgpa))
            {
                cout << "Student added successfully." << endl;
            }
        }
        else if (choice == 2)
        {
            string roll;
            cout << "Enter roll no: ";
            cin >> roll;
            printStudent(searchByRoll(roll));
        }
        else if (choice == 3)
        {
            string name;
            cin.ignore();
            cout << "Enter name part: ";
            getline(cin, name);
            printStudentTable(searchByName(name));
        }
        else if (choice == 4)
        {
            string roll, value;
            int fieldNo;
            cout << "Enter roll no: ";
            cin >> roll;
            cout << "Field number (1 name, 2 dept, 3 semester, 4 cgpa, 5 status): ";
            cin >> fieldNo;
            cin.ignore();
            cout << "New value: ";
            getline(cin, value);

            if (updateStudent(roll, fieldNo, value))
                cout << "Student updated." << endl;
            else
                cout << "Update failed." << endl;
        }
        else if (choice == 5)
        {
            string roll;
            cout << "Enter roll no: ";
            cin >> roll;

            if (softDelete(roll))
                cout << "Student marked inactive." << endl;
            else
                cout << "Student not found." << endl;
        }
        else if (choice == 6)
        {
            printStudentTable(listActiveStudents());
        }
        else if (choice == 7)
        {
            searchAsYouType();
        }
    }
}
