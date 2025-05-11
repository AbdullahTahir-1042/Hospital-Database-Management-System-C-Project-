#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define MAX_STRING_LENGTH 50

int loadPatients(FILE *file, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    int count = 0;
    rewind(file);
    while (fscanf(file, "%d %s %d %s", &ids[count], names[count], &ages[count], diseases[count]) == 4) {
        count++;
        if (count >= MAX_RECORDS) break;
    }
    return count;
}

void saveAllPatients(FILE *file, int recordCount, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    file = freopen("hospital_database.txt", "w", file); // Open in write mode to clear file
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%d %s %d %s\n", ids[i], names[i], ages[i], diseases[i]);
    }
    fflush(file);
}

void addPatient(FILE *file, int *recordCount, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    if (*recordCount >= MAX_RECORDS) {
        printf("Database is full. Cannot add more patients.\n");
        return;
    }

    int tempId, tempAge;
    char tempName[MAX_STRING_LENGTH];
    char tempDisease[MAX_STRING_LENGTH];

    printf("Enter patient ID: ");
    scanf("%d", &tempId);
    while ((getchar()) != '\n');

    printf("Enter patient name: ");
    fgets(tempName, MAX_STRING_LENGTH, stdin);
    tempName[strcspn(tempName, "\n")] = '\0';

    printf("Enter patient age: ");
    scanf("%d", &tempAge);
    while ((getchar()) != '\n');

    printf("Enter patient disease: ");
    fgets(tempDisease, MAX_STRING_LENGTH, stdin);
    tempDisease[strcspn(tempDisease, "\n")] = '\0';

    ids[*recordCount] = tempId;
    ages[*recordCount] = tempAge;
    strcpy(names[*recordCount], tempName);
    strcpy(diseases[*recordCount], tempDisease);

    fprintf(file, "%d %s %d %s\n", tempId, tempName, tempAge, tempDisease);
    fflush(file);

    (*recordCount)++;
    printf("Patient record added successfully!\n");
}

void displayPatients(int recordCount, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    printf("\nPatient Records:\n");
    printf("ID:\tName:\t\tAge:\tDisease:\n");
    for (int i = 0; i < recordCount; ++i) {
        printf("%d\t%s\t\t%d\t%s\n", ids[i], names[i], ages[i], diseases[i]);
    }
    printf("\n");
}

void searchPatient(int recordCount, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    int searchID;
    printf("\nEnter patient ID to search: ");
    scanf("%d", &searchID);

    for (int i = 0; i < recordCount; ++i) {
        if (ids[i] == searchID) {
            printf("\nPatient found!\n");
            printf("ID: %d, Name: %s, Age: %d, Disease: %s\n\n", ids[i], names[i], ages[i], diseases[i]);
            return;
        }
    }
    printf("Patient with ID %d not found.\n\n", searchID);
}

void deletePatient(FILE *file, int *recordCount, int ids[], char names[][MAX_STRING_LENGTH], int ages[], char diseases[][MAX_STRING_LENGTH]) {
    int delID;
    printf("\nEnter patient ID to delete: ");
    scanf("%d", &delID);

    int found = 0;
    for (int i = 0; i < *recordCount; ++i) {
        if (ids[i] == delID) {
            found = 1;
            for (int j = i; j < *recordCount - 1; ++j) {
                ids[j] = ids[j + 1];
                ages[j] = ages[j + 1];
                strcpy(names[j], names[j + 1]);
                strcpy(diseases[j], diseases[j + 1]);
            }
            (*recordCount)--;
            break;
        }
    }

    if (found) {
        saveAllPatients(file, *recordCount, ids, names, ages, diseases);
        printf("Patient with ID %d deleted successfully.\n\n", delID);
    } else {
        printf("Patient with ID %d not found.\n\n", delID);
    }
}

int main() {
    FILE *file = fopen("hospital_database.txt", "a+");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    int ids[MAX_RECORDS];
    char names[MAX_RECORDS][MAX_STRING_LENGTH];
    int ages[MAX_RECORDS];
    char diseases[MAX_RECORDS][MAX_STRING_LENGTH];

    int recordCount = loadPatients(file, ids, names, ages, diseases);

    int choice;
    do {
        printf("\nHospital Database Management\n");
        printf("1. Add Patient\t\t2. Display Patients\n");
        printf("3. Search Patient\t4. Delete Patient\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while ((getchar()) != '\n');

        switch (choice) {
            case 1:
                addPatient(file, &recordCount, ids, names, ages, diseases);
                break;
            case 2:
                if (recordCount > 0)
                    displayPatients(recordCount, ids, names, ages, diseases);
                else
                    printf("No patient records to display.\n");
                break;
            case 3:
                if (recordCount > 0)
                    searchPatient(recordCount, ids, names, ages, diseases);
                else
                    printf("No patient records to search.\n");
                break;
            case 4:
                if (recordCount > 0)
                    deletePatient(file, &recordCount, ids, names, ages, diseases);
                else
                    printf("No patient records to delete.\n");
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);

    fclose(file);
    return 0;
}
