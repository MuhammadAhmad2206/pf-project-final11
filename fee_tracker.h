#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include "filehandler.h"
#include "student_ops.h"

using namespace std;

/*
    fees.txt columns:
    0 fee_id, 1 roll_no, 2 semester, 3 total_fee, 4 amount_paid,
    5 due_date, 6 payment_date, 7 payment_method, 8 status
*/

bool validDate(string date);
int daysBetween(string d1, string d2);
bool recordPayment(string roll, string semester, int amount, string paymentDate, string method);
double computeLateFine(string roll, string semester);
void generateReceipt(string roll, string semester);
vector<vector<string> > getDefaulters();
void feeMenu();

#endif
