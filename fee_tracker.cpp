#include "fee_tracker.h"

bool validDate(string date)
{
    if (date.length() != 10)
    {
        return false;
    }

    if (date[2] != '-' || date[5] != '-')
    {
        return false;
    }

    for (int i = 0; i < (int)date.length(); i++)
    {
        if (i == 2 || i == 5)
        {
            continue;
        }

        if (date[i] < '0' || date[i] > '9')
        {
            return false;
        }
    }

    return true;
}

int toDayNumber(string date)
{
    if (!validDate(date))
    {
        return 0;
    }

    int day = atoi(date.substr(0, 2).c_str());
    int month = atoi(date.substr(3, 2).c_str());
    int year = atoi(date.substr(6, 4).c_str());

    int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int total = year * 365;

    for (int i = 0; i < month - 1; i++)
    {
        total += monthDays[i];
    }

    total += day;
    return total;
}

int daysBetween(string d1, string d2)
{
    int first = toDayNumber(d1);
    int second = toDayNumber(d2);

    return second - first;
}

bool recordPayment(string roll, string semester, int amount, string paymentDate, string method)
{
    if (!validDate(paymentDate))
    {
        cout << "Invalid date format." << endl;
        return false;
    }

    vector<string> header = readHeader("fees.txt");
    vector<vector<string> > fees = readTXT("fees.txt");
    bool found = false;

    for (int i = 0; i < (int)fees.size(); i++)
    {
        if (fees[i].size() >= 9 &&
            fees[i][1] == roll &&
            fees[i][2] == semester)
        {
            int paid = atoi(fees[i][4].c_str());
            int total = atoi(fees[i][3].c_str());

            paid += amount;
            fees[i][4] = to_string(paid);
            fees[i][6] = paymentDate;
            fees[i][7] = method;

            if (paid >= total)
            {
                int late = daysBetween(fees[i][5], paymentDate);
                if (late > 0)
                    fees[i][8] = "paid_late";
                else
                    fees[i][8] = "paid";
            }
            else
            {
                fees[i][8] = "partial";
            }

            found = true;
        }
    }

    if (found)
    {
        writeTXT("fees.txt", header, fees);
    }

    return found;
}

double computeLateFine(string roll, string semester)
{
    vector<vector<string> > fees = readTXT("fees.txt");

    for (int i = 0; i < (int)fees.size(); i++)
    {
        if (fees[i].size() >= 9 &&
            fees[i][1] == roll &&
            fees[i][2] == semester)
        {
            string due = fees[i][5];
            string paid = fees[i][6];

            if (paid == "00-00-0000")
            {
                paid = "30-06-2026"; // basic fixed current date for report
            }

            int lateDays = daysBetween(due, paid);

            if (lateDays <= 0)
            {
                return 0.0;
            }

            int weeks = lateDays / 7;
            int totalFee = atoi(fees[i][3].c_str());

            return totalFee * 0.02 * weeks;
        }
    }

    return 0.0;
}

void generateReceipt(string roll, string semester)
{
    vector<vector<string> > fees = readTXT("fees.txt");
    vector<string> student = searchByRoll(roll);

    for (int i = 0; i < (int)fees.size(); i++)
    {
        if (fees[i].size() >= 9 &&
            fees[i][1] == roll &&
            fees[i][2] == semester)
        {
            int total = atoi(fees[i][3].c_str());
            int paid = atoi(fees[i][4].c_str());
            double fine = computeLateFine(roll, semester);
            double balance = total + fine - paid;

            cout << "\n========== FEE RECEIPT ==========" << endl;
            if (student.size() >= 2)
            {
                cout << "Student Name : " << student[1] << endl;
            }
            cout << "Roll No      : " << roll << endl;
            cout << "Semester     : " << semester << endl;
            cout << "Total Fee    : " << total << endl;
            cout << "Late Fine    : " << fine << endl;
            cout << "Amount Paid  : " << paid << endl;
            cout << "Balance      : " << balance << endl;
            cout << "Status       : " << fees[i][8] << endl;
            cout << "=================================" << endl;
            return;
        }
    }

    cout << "Fee record not found." << endl;
}

vector<vector<string> > getDefaulters()
{
    vector<vector<string> > result;
    vector<vector<string> > fees = readTXT("fees.txt");

    for (int i = 0; i < (int)fees.size(); i++)
    {
        if (fees[i].size() >= 9)
        {
            int total = atoi(fees[i][3].c_str());
            int paid = atoi(fees[i][4].c_str());

            if (paid < total)
            {
                fees[i].push_back(to_string(total - paid));
                result.push_back(fees[i]);
            }
        }
    }

    // Bubble sort by balance descending
    for (int i = 0; i < (int)result.size() - 1; i++)
    {
        for (int j = 0; j < (int)result.size() - i - 1; j++)
        {
            int bal1 = atoi(result[j][9].c_str());
            int bal2 = atoi(result[j + 1][9].c_str());

            if (bal1 < bal2)
            {
                vector<string> temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    return result;
}

void feeMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n====== FEE TRACKER ======" << endl;
        cout << "1. Record payment" << endl;
        cout << "2. Compute late fine" << endl;
        cout << "3. Generate receipt" << endl;
        cout << "4. Show fee defaulters" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string roll, sem, date, method;
            int amount;

            cout << "Roll No: ";
            cin >> roll;
            cout << "Semester: ";
            cin >> sem;
            cout << "Amount: ";
            cin >> amount;
            cout << "Payment Date DD-MM-YYYY: ";
            cin >> date;
            cout << "Method: ";
            cin >> method;

            if (recordPayment(roll, sem, amount, date, method))
                cout << "Payment recorded." << endl;
            else
                cout << "Payment failed." << endl;
        }
        else if (choice == 2)
        {
            string roll, sem;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Semester: ";
            cin >> sem;
            cout << "Late fine: " << computeLateFine(roll, sem) << endl;
        }
        else if (choice == 3)
        {
            string roll, sem;
            cout << "Roll No: ";
            cin >> roll;
            cout << "Semester: ";
            cin >> sem;
            generateReceipt(roll, sem);
        }
        else if (choice == 4)
        {
            vector<vector<string> > def = getDefaulters();

            cout << left << setw(15) << "Roll"
                 << setw(12) << "Semester"
                 << setw(12) << "Total"
                 << setw(12) << "Paid"
                 << setw(12) << "Balance" << endl;

            for (int i = 0; i < (int)def.size(); i++)
            {
                if (def[i].size() >= 10)
                {
                    cout << left << setw(15) << def[i][1]
                         << setw(12) << def[i][2]
                         << setw(12) << def[i][3]
                         << setw(12) << def[i][4]
                         << setw(12) << def[i][9] << endl;
                }
            }
        }
    }
}
