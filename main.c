#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define SUBJECTS 3
#define PASS_MARKS 40
#define FILE_NAME "students.dat"

union Contact {
    long phone;
    char email[40];
};

struct Student {
    int roll;
    char name[MAX_NAME];
    int marks[SUBJECTS];
    float average;
    int contactType;            /* 1 = phone, 2 = email */
    union Contact contact;
    struct Student *next;       /* self-referential */
};

static int count = 0;           /* storage class: static */
struct Student *head = NULL;    /* global head pointer */

/* ---------- helpers ---------- */

int readInt(const char *prompt) {
    char buf[64];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (sscanf(buf, "%d", &val) == 1) return val;
        printf("Invalid number, try again.\n");
    }
}

void readString(const char *prompt, char *dest, int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) { dest[0] = '\0'; return; }
    dest[strcspn(dest, "\n")] = '\0';
}

float calcAverage(int marks[], int n) {     /* array passed to function */
    int sum = 0;
    for (int i = 0; i < n; i++) sum += marks[i];
    return (float)sum / n;                  /* type conversion */
}

struct Student *findByRoll(int roll) {
    for (struct Student *p = head; p != NULL; p = p->next)
        if (p->roll == roll) return p;
    return NULL;
}

void printStudent(const struct Student *p) {
    printf("Roll: %d | Name: %s | Marks: ", p->roll, p->name);
    for (int i = 0; i < SUBJECTS; i++) printf("%d ", p->marks[i]);
    printf("| Avg: %.2f | %s | ", p->average,
           p->average >= PASS_MARKS ? "PASS" : "FAIL");
    if (p->contactType == 1) printf("Phone: %ld\n", p->contact.phone);
    else                     printf("Email: %s\n", p->contact.email);
}

/* ---------- recursion ---------- */

int countAbove(struct Student *p, float limit) {
    if (p == NULL) return 0;                         /* base case */
    return (p->average > limit) + countAbove(p->next, limit);
}

/* ---------- file handling ---------- */

void saveToFile(void) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) { printf("Cannot open file for saving.\n"); return; }
    for (struct Student *p = head; p != NULL; p = p->next)
        fwrite(p, sizeof(struct Student), 1, fp);
    fclose(fp);
}

void loadFromFile(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return;                          /* no file yet: fine */
    struct Student temp, *tail = NULL;
    while (fread(&temp, sizeof(struct Student), 1, fp) == 1) {
        struct Student *s = (struct Student *)malloc(sizeof(struct Student));
        if (s == NULL) break;
        *s = temp;
        s->next = NULL;
        if (head == NULL) head = s; else tail->next = s;
        tail = s;
        count++;
    }
    fclose(fp);
}

/* ---------- features ---------- */

void addStudent(void) {
    int roll = readInt("Roll: ");
    if (findByRoll(roll) != NULL) {
        printf("Roll number already exists.\n");
        return;
    }
    struct Student *s = (struct Student *)malloc(sizeof(struct Student));
    if (s == NULL) { printf("Memory error.\n"); return; }

    s->roll = roll;
    readString("Name: ", s->name, MAX_NAME);
    for (int i = 0; i < SUBJECTS; i++) {
        int m;
        do {
            printf("Marks in subject %d ", i + 1);
            m = readInt("(0-100): ");
        } while (m < 0 || m > 100);
        s->marks[i] = m;
    }
    s->average = calcAverage(s->marks, SUBJECTS);

    s->contactType = readInt("Contact type (1 = phone, 2 = email): ");
    if (s->contactType == 1) {
        char buf[32];
        readString("Phone: ", buf, sizeof(buf));
        s->contact.phone = atol(buf);
    } else {
        s->contactType = 2;
        readString("Email: ", s->contact.email, sizeof(s->contact.email));
    }

    s->next = NULL;                                  /* add at end */
    if (head == NULL) head = s;
    else {
        struct Student *p = head;
        while (p->next != NULL) p = p->next;
        p->next = s;
    }
    count++;
    saveToFile();
    printf("Student added.\n");
}

void displayAll(void) {
    if (head == NULL) { printf("No records.\n"); return; }
    for (struct Student *p = head; p != NULL; p = p->next) printStudent(p);
    printf("Total students: %d\n", count);
}

void searchStudent(void) {
    int roll = readInt("Enter roll to search: ");
    struct Student *p = findByRoll(roll);
    if (p) printStudent(p); else printf("Not found.\n");
}

void updateStudent(void) {
    int roll = readInt("Enter roll to update: ");
    struct Student *p = findByRoll(roll);
    if (p == NULL) { printf("Not found.\n"); return; }
    readString("New name: ", p->name, MAX_NAME);
    for (int i = 0; i < SUBJECTS; i++) {
        int m;
        do {
            printf("New marks in subject %d ", i + 1);
            m = readInt("(0-100): ");
        } while (m < 0 || m > 100);
        p->marks[i] = m;
    }
    p->average = calcAverage(p->marks, SUBJECTS);
    saveToFile();
    printf("Updated.\n");
}

void deleteStudent(void) {
    int roll = readInt("Enter roll to delete: ");
    struct Student *p = head, *prev = NULL;
    while (p != NULL && p->roll != roll) { prev = p; p = p->next; }
    if (p == NULL) { printf("Not found.\n"); return; }
    if (prev == NULL) head = p->next; else prev->next = p->next;
    free(p);                                         /* release memory */
    count--;
    saveToFile();
    printf("Deleted.\n");
}

void sortByAverage(void) {                           /* bubble sort on list */
    if (head == NULL || head->next == NULL) return;
    int swapped;
    do {
        swapped = 0;
        for (struct Student *p = head; p->next != NULL; p = p->next) {
            if (p->average < p->next->average) {
                struct Student *n = p->next;
                struct Student tmp = *p;
                struct Student *pn = p->next, *nn = n->next;
                *p = *n;  p->next = pn;              /* swap data, keep links */
                *n = tmp; n->next = nn;
                swapped = 1;
            }
        }
    } while (swapped);
    saveToFile();
    printf("Sorted by average (highest first).\n");
}

void freeAll(void) {
    struct Student *p = head;
    while (p != NULL) {
        struct Student *n = p->next;
        free(p);
        p = n;
    }
    head = NULL;
}

/* ---------- main ---------- */

int main(void) {
    int choice;
    loadFromFile();
    do {
        printf("\n===== STUDENT MANAGEMENT =====\n");
        printf("1.Add  2.Display  3.Search  4.Update  5.Delete\n");
        printf("6.Sort by average  7.Count above average  0.Exit\n");
        choice = readInt("Enter choice: ");
        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortByAverage(); break;
            case 7: {
                int lim = readInt("Limit: ");
                printf("Students above %d: %d\n", lim, countAbove(head, (float)lim));
                break;
            }
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    freeAll();
    return 0;
}
