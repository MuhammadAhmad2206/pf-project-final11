#include "filehandler.h"

vector<string> splitCSVLine(string line)
{
    vector<string> row;
    string value = "";
    bool insideQuotes = false;

    for (int i = 0; i < (int)line.length(); i++)
    {
        char ch = line[i];

        if (ch == '"')
        {
            insideQuotes = !insideQuotes;
        }
        else if (ch == ',' && insideQuotes == false)
        {
            row.push_back(value);
            value = "";
        }
        else
        {
            value += ch;
        }
    }

    row.push_back(value);
    return row;
}

vector<string> readHeader(string fileName)
{
    vector<string> header;
    ifstream file(fileName.c_str());

    if (!file)
    {
        return header;
    }

    string line;
    if (getline(file, line))
    {
        header = splitCSVLine(line);
    }

    file.close();
    return header;
}

vector<vector<string> > readTXT(string fileName)
{
    vector<vector<string> > rows;
    ifstream file(fileName.c_str());

    if (!file)
    {
        cout << "Unable to open file: " << fileName << endl;
        return rows;
    }

    string line;
    bool firstLine = true;

    while (getline(file, line))
    {
        if (firstLine)
        {
            firstLine = false;
            continue;
        }

        if (line.length() > 0)
        {
            rows.push_back(splitCSVLine(line));
        }
    }

    file.close();
    return rows;
}

void writeTXT(string fileName, vector<string> header, vector<vector<string> > rows)
{
    ofstream file(fileName.c_str());

    if (!file)
    {
        cout << "Unable to write file: " << fileName << endl;
        return;
    }

    for (int i = 0; i < (int)header.size(); i++)
    {
        file << header[i];
        if (i != (int)header.size() - 1)
        {
            file << ",";
        }
    }
    file << endl;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        for (int j = 0; j < (int)rows[i].size(); j++)
        {
            string value = rows[i][j];

            bool hasComma = false;
            for (int k = 0; k < (int)value.length(); k++)
            {
                if (value[k] == ',')
                {
                    hasComma = true;
                }
            }

            if (hasComma)
            {
                file << "\"" << value << "\"";
            }
            else
            {
                file << value;
            }

            if (j != (int)rows[i].size() - 1)
            {
                file << ",";
            }
        }
        file << endl;
    }

    file.close();
}

void appendTXT(string fileName, vector<string> row)
{
    ofstream file(fileName.c_str(), ios::app);

    if (!file)
    {
        cout << "Unable to append file: " << fileName << endl;
        return;
    }

    for (int i = 0; i < (int)row.size(); i++)
    {
        file << row[i];
        if (i != (int)row.size() - 1)
        {
            file << ",";
        }
    }

    file << endl;
    file.close();
}

vector<string> findRow(string fileName, int colIndex, string value)
{
    vector<vector<string> > rows = readTXT(fileName);

    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (colIndex >= 0 && colIndex < (int)rows[i].size())
        {
            if (rows[i][colIndex] == value)
            {
                return rows[i];
            }
        }
    }

    vector<string> empty;
    return empty;
}

bool rowExists(string fileName, int colIndex, string value)
{
    vector<string> row = findRow(fileName, colIndex, value);

    if (row.size() > 0)
    {
        return true;
    }

    return false;
}

int countRows(string fileName)
{
    vector<vector<string> > rows = readTXT(fileName);
    return rows.size();
}
