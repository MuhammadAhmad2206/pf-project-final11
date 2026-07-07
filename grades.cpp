#include "grades.h"

double bestThreeOfFive(double quiz[], int n)
{
    if (n <= 0)
    {
        return 0.0;
    }

    if (n < 3)
    {
        double sumSmall = 0.0;
        for (int i = 0; i < n; i++)
        {
            sumSmall += quiz[i];
        }
        return sumSmall / n;
    }

    // Find two lowest values without sort
    int low1 = 0;
    int low2 = 1;

    if (quiz[low2] < quiz[low1])
    {
        int temp = low1;
        low1 = low2;
        low2 = temp;
    }

    for (int i = 2; i < n; i++)
    {
        if (quiz[i] < quiz[low1])
        {
            low2 = low1;
            low1 = i;
        }
        else if (quiz[i] < quiz[low2])
        {
            low2 = i;
        }
    }

    double sum = 0.0;
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        if (i != low1 && i != low2)
        {
            sum += quiz[i];
            count++;
        }
    }

    if (count == 0)
    {
        return 0.0;
    }

    return sum / count;
}

double computeWeightedTotal(double quizAvg, double asgnAvg, double mid, double finalMarks)
{
    double total = 0.0;

    total += quizAvg * 0.10;
    total += asgnAvg * 0.10;
    total += mid * 0.30;
    total += finalMarks * 0.50;

    return total;
}

string getLetterGrade(double total)
{
    if (total >= 85) return "A";
    if (total >= 80) return "B+";
    if (total >= 70) return "B";
    if (total >= 65) return "C+";
    if (total >= 60) return "C";
    if (total >= 50) return "D";
    return "F";
}

double gradePoint(string grade)
{
    if (grade == "A") return 4.0;
    if (grade == "B+") return 3.5;
    if (grade == "B") return 3.0;
    if (grade == "C+") return 2.5;
    if (grade == "C") return 2.0;
    if (grade == "D") return 1.0;
    return 0.0;
}

double computeGPA(string roll, string semester)
{
    // Basic PF-level GPA for this project:
    // existing CGPA from students.txt is used as semester GPA.
    vector<string> s = searchByRoll(roll);

    if (s.size() >= 5)
    {
        return atof(s[4].c_str());
    }

    return 0.0;
}

Stats computeClassState(double arr[], int n)
{
    Stats st;
    st.highest = 0;
    st.lowest = 0;
    st.mean = 0;
    st.median = 0;

    if (n <= 0)
    {
        return st;
    }

    st.highest = arr[0];
    st.lowest = arr[0];

    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        if (arr[i] > st.highest) st.highest = arr[i];
        if (arr[i] < st.lowest) st.lowest = arr[i];
        sum += arr[i];
    }

    st.mean = sum / n;

    // manual bubble sort for median
    double tempArr[200];
    for (int i = 0; i < n; i++)
    {
        tempArr[i] = arr[i];
    }

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (tempArr[j] > tempArr[j + 1])
            {
                double temp = tempArr[j];
                tempArr[j] = tempArr[j + 1];
                tempArr[j + 1] = temp;
            }
        }
    }

    if (n % 2 == 1)
    {
        st.median = tempArr[n / 2];
    }
    else
    {
        st.median = (tempArr[(n / 2) - 1] + tempArr[n / 2]) / 2.0;
    }

    return st;
}

string applyAttendancePenalty(string roll, string courseCode, string grade)
{
    double pct = getAttendancePct(roll, courseCode);

    if (pct < 75.0)
    {
        return "F";
    }

    return grade;
}

void enterMarks()
{
    string roll, courseCode;
    double quizzes[5];
    double asgn[3];
    double mid, finalMarks;

    cout << "Roll No: ";
    cin >> roll;
    cout << "Course Code: ";
    cin >> courseCode;

    for (int i = 0; i < 5; i++)
    {
        cout << "Quiz " << i + 1 << " marks out of 10: ";
        cin >> quizzes[i];

        while (quizzes[i] < 0 || quizzes[i] > 10)
        {
            cout << "Enter valid marks 0-10: ";
            cin >> quizzes[i];
        }

        quizzes[i] = quizzes[i] * 10; // convert to percentage
    }

    double asgnSum = 0.0;
    for (int i = 0; i < 3; i++)
    {
        cout << "Assignment " << i + 1 << " marks out of 10: ";
        cin >> asgn[i];

        while (asgn[i] < 0 || asgn[i] > 10)
        {
            cout << "Enter valid marks 0-10: ";
            cin >> asgn[i];
        }

        asgnSum += asgn[i] * 10;
    }

    cout << "Mid marks out of 100: ";
    cin >> mid;
    cout << "Final marks out of 100: ";
    cin >> finalMarks;

    double quizAvg = bestThreeOfFive(quizzes, 5);
    double asgnAvg = asgnSum / 3.0;
    double total = computeWeightedTotal(quizAvg, asgnAvg, mid, finalMarks);
    string grade = getLetterGrade(total);
    grade = applyAttendancePenalty(roll, courseCode, grade);

    cout << "\nTotal Marks : " << total << endl;
    cout << "Grade       : " << grade << endl;
    cout << "Grade Point : " << gradePoint(grade) << endl;
}

void gradesMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== GRADES MANAGEMENT ======" << endl;
        cout << "1. Enter marks and calculate grade" << endl;
        cout << "2. Compute GPA from student record" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            enterMarks();
        }
        else if (choice == 2)
        {
            string roll, sem;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Semester: ";
            cin >> sem;
            cout << "GPA/CGPA: " << computeGPA(roll, sem) << endl;
        }
    }
}
