#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/*
    This module handles all TXT file work.
    It uses simple loops and vectors only.
*/

vector<string> splitCSVLine(string line);
vector<vector<string> > readTXT(string fileName);
vector<string> readHeader(string fileName);
void writeTXT(string fileName, vector<string> header, vector<vector<string> > rows);
void appendTXT(string fileName, vector<string> row);
vector<string> findRow(string fileName, int colIndex, string value);
bool rowExists(string fileName, int colIndex, string value);
int countRows(string fileName);

#endif
