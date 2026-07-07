# Campus Analytics Engine

This is a basic C++ Programming Fundamentals project for a fictional university campus.

The system manages:

- Student records
- Course enrollments
- Attendance
- Grades
- Fee records
- Reports

## Important Rules Followed

- C++ only
- No OOP classes
- No STL algorithms
- No `<algorithm>`
- No `<map>`, `<set>`, or `<unordered_map>`
- No `<ctime>`
- TXT file based storage
- Multi-file project structure

## Project Files

```txt
main.cpp
filehandler.h / filehandler.cpp
student_ops.h / student_ops.cpp
course_ops.h / course_ops.cpp
attendance.h / attendance.cpp
grades.h / grades.cpp
fee_tracker.h / fee_tracker.cpp
reports.h / reports.cpp
```

## Data Files

```txt
students.txt
courses.txt
enrollments.txt
attendance_log.txt
fees.txt
```

## How to Compile

Use this command:

```bash
g++ main.cpp filehandler.cpp student_ops.cpp course_ops.cpp attendance.cpp grades.cpp fee_tracker.cpp reports.cpp -o campus
```

## How to Run

Linux / GitHub Codespaces:

```bash
./campus
```

Windows:

```bash
campus.exe
```

## Sample Menu

```txt
==============================================
          CAMPUS ANALYTICS ENGINE
          Programming Fundamentals
==============================================

Main Menu
1. Student Management
2. Course Management
3. Attendance Management
4. Grades Management
5. Fee Tracker
6. Reports
0. Exit
```

## Suggested Git Commit Plan

```bash
git init
git add .
git commit -m "Create project structure and data files"

git add filehandler.*
git commit -m "Add file handling module"

git add student_ops.*
git commit -m "Add student management module"

git add course_ops.*
git commit -m "Add course enrollment module"

git add attendance.*
git commit -m "Add attendance module"

git add grades.*
git commit -m "Add grades module"

git add fee_tracker.*
git commit -m "Add fee tracker module"

git add reports.*
git commit -m "Add reports module"

git add main.cpp README.md
git commit -m "Integrate main menu and documentation"
```

## Notes for Viva

You should be able to explain:

- How TXT files are read and written
- How validation works
- How selection sort / bubble sort is used
- How attendance percentage is calculated
- How fee balance and late fine are calculated
- How reports are generated
