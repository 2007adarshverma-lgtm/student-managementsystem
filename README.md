# Student Management System (C)

A console-based Student Management System written in pure C.
First-semester CSE mini project (solo).

**Author:** <YOUR NAME> | **Roll No:** <YOUR ROLL NO> | **College:** <YOUR COLLEGE>

## Features
- Add, display, search, update and delete student records
- Sort students by average marks
- Count students above a given average (recursion)
- Duplicate roll number check and marks validation (0-100)
- PASS/FAIL result (pass mark 40)
- Data saved to `students.dat` automatically, loaded on start

## How to run
```bash
gcc -Wall -o sms main.c
./sms
```
Or with make: `make` then `./sms` (Windows: `sms.exe`).

## Syllabus concepts used
| Unit | Concepts | Where used |
|---|---|---|
| 1 | Data types, type conversion, header files, storage classes | `(float)sum / n`, `static int count`, global `head`, `stdio.h` `stdlib.h` `string.h` |
| 2 | Preprocessors, conditionals, loops | `#define`, `#include`, `switch` menu, `do-while`, `for`, `while`, `if-else` |
| 3 | Arrays, strings, functions, call by value/reference, recursion | `marks[]`, `char name[]`, `calcAverage(int marks[], int n)`, `countAbove()` |
| 4 | Pointers, self-referential structure, dynamic memory, structure, union, file handling | linked list node with `next`, `malloc`/`free`, `struct Student`, `union Contact`, `fwrite`/`fread` |

## Algorithm (add student)
1. Read roll number; if it already exists, stop
2. Allocate a new node with `malloc`
3. Read name, marks of 3 subjects, contact
4. Calculate average
5. Append node to the end of the linked list
6. Save the list to file

## Sample output
```
Roll: 1 | Name: Aman | Marks: 80 70 90 | Avg: 80.00 | PASS | Phone: 9999999999
Roll: 2 | Name: Riya | Marks: 30 35 40 | Avg: 35.00 | FAIL | Email: r@x.com
Total students: 2
```

## Limitations and future scope
- Fixed 3 subjects; could be made dynamic
- No login system; could add admin password
- Could add a GUI or database
