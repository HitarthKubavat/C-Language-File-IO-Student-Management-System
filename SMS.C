// ===================================================
// C Project: Student Management System (sms.c)
// Demonstrates structs, file I/O, and basic algorithms
// ===================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a Student record
typedef struct {
    int roll_number;
    char name[50];
    char major[50];
    float gpa;
} Student;

// Global constant for the filename
#define FILENAME "students.dat"

// --- Function Prototypes ---
void cleanup_input_buffer();
void display_menu();
void add_student();
void view_all_students();
void search_student();
void update_student();


// -----------------------------------------------------------------
//               HELPER FUNCTIONS
// -----------------------------------------------------------------

// Helper function to clear the input buffer
void cleanup_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void display_menu() {
    printf("\n===================================\n");
    printf("  STUDENT MANAGEMENT SYSTEM (C)\n");
    printf("===================================\n");
    printf("1. Add New Student Record\n");
    printf("2. View All Student Records\n");
    printf("3. Search Student by Roll Number\n");
    printf("4. Update Student Record\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}


// -----------------------------------------------------------------
//               CORE CRUD FUNCTIONS
// -----------------------------------------------------------------

void add_student() {
    Student s;
    FILE *fp = fopen(FILENAME, "ab"); // Open for append binary

    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }

    printf("\n--- Add New Student ---\n");
    printf("Enter Roll Number: ");
    if (scanf("%d", &s.roll_number) != 1) {
        printf("Invalid input. Please enter a number.\n");
        cleanup_input_buffer();
        fclose(fp);
        return;
    }
    
    // Clear buffer and read strings (allows spaces)
    cleanup_input_buffer();
    printf("Enter Name: ");
    if (fgets(s.name, 50, stdin) == NULL) return;
    s.name[strcspn(s.name, "\n")] = 0; // Remove newline
    
    printf("Enter Major: ");
    if (fgets(s.major, 50, stdin) == NULL) return;
    s.major[strcspn(s.major, "\n")] = 0; // Remove newline

    printf("Enter GPA: ");
    if (scanf("%f", &s.gpa) != 1) {
        printf("Invalid input. Please enter a valid GPA.\n");
        cleanup_input_buffer();
        fclose(fp);
        return;
    }

    // Write the struct (one record) to the file
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student record added successfully!\n");
    cleanup_input_buffer(); // Clear any remaining newline/input
}

void view_all_students() {
    Student s;
    int count = 0;
    FILE *fp = fopen(FILENAME, "rb"); // Open for read binary

    if (fp == NULL) {
	printf("\n No student records found. File does not exist or is empty.\n");
	return;
    }

    printf("\n--- All Student Records ---\n");
    printf("%-15s %-30s %-20s %s\n", "Roll No", "Name", "Major", "GPA");
    printf("------------------------------------------------------------------\n");

    // Read records one by one until the end of the file (EOF)
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("%-15d %-30s %-20s %.2f\n", s.roll_number, s.name, s.major, s.gpa);
        count++;
    }

    if (count == 0) {
        printf("The file is empty.\n");
    }
    
    fclose(fp);
}

void search_student() {
    int search_roll;
    int found = 0;
    Student s;
    FILE *fp = fopen(FILENAME, "rb");

    if (fp == NULL) {
        printf("\n No student records found.\n");
        return;
    }

    printf("\n--- Search Student ---\n");
    printf("Enter Roll Number to search: ");
    if (scanf("%d", &search_roll) != 1) {
        printf("Invalid input. Please enter a number.\n");
        cleanup_input_buffer();
        fclose(fp);
        return;
    }
    cleanup_input_buffer(); // Clear buffer after reading the roll number

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll_number == search_roll) {
            printf("\n--- Student Found! ---\n");
            printf("Roll No: %d\n", s.roll_number);
            printf("Name:    %s\n", s.name);
            printf("Major:   %s\n", s.major);
            printf("GPA:     %.2f\n", s.gpa);
            found = 1;
            break; // Stop searching once found (Linear Search)
        }
    }

    if (!found) {
        printf("\n Student with Roll Number %d not found.\n", search_roll);
    }
    
    fclose(fp);
}

void update_student() {
    int search_roll;
    int found = 0;
    Student s;
    // Open for read/write binary to allow modifying data in place
    FILE *fp = fopen(FILENAME, "rb+"); 

    if (fp == NULL) {
        printf("\n No student records found or file error.\n");
        return;
    }

    printf("\n--- Update Student Record ---\n");
    printf("Enter Roll Number of student to update: ");
    if (scanf("%d", &search_roll) != 1) {
        printf("Invalid input. Please enter a number.\n");
        cleanup_input_buffer();
        fclose(fp);
        return;
    }
    cleanup_input_buffer(); // Clear buffer after reading the roll number

    // Loop through the file record by record
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll_number == search_roll) {
            found = 1;
            
            printf("\n Record found for Roll No %d. Enter NEW details:\n", search_roll);
            
            // Read Name
            printf("Enter NEW Name (%s): ", s.name);
            if (fgets(s.name, 50, stdin) == NULL) return;
            s.name[strcspn(s.name, "\n")] = 0; 
            
            // Read Major
            printf("Enter NEW Major (%s): ", s.major);
            if (fgets(s.major, 50, stdin) == NULL) return;
            s.major[strcspn(s.major, "\n")] = 0;
            
            // Read GPA
            printf("Enter NEW GPA (%.2f): ", s.gpa);
            if (scanf("%f", &s.gpa) != 1) {
                printf("Invalid GPA input. Update failed.\n");
                cleanup_input_buffer();
                fclose(fp);
                return;
            }
            cleanup_input_buffer();
            
            // --- Update the File Record ---
            
            // Move file pointer back by one struct size to overwrite the old record
            fseek(fp, -(long)sizeof(Student), SEEK_CUR);
            
            // Overwrite the old record with the modified struct data
            fwrite(&s, sizeof(Student), 1, fp);
            
            printf("\n Student record updated successfully!\n");
            
            break; // Exit the loop
        }
    }

    if (!found) {
        printf("\n Student with Roll Number %d not found.\n", search_roll);
    }
    
    fclose(fp);
}


// -----------------------------------------------------------------
//               MAIN FUNCTION (The Program Entry Point)
// -----------------------------------------------------------------

int main() {
    int choice;
    clrscr();
    do {
	display_menu();

	// Read the user's choice
	if (scanf("%d", &choice) != 1) {
	    // Handle non-integer input gracefully
	    printf("\n Invalid input. Please enter a number from the menu.\n");
	    cleanup_input_buffer();
	    choice = 0; // Reset choice to loop again
	    continue;
	}

	switch (choice) {
	    case 1:
		add_student();
		break;
	    case 2:
		view_all_students();
		break;
	    case 3:
		search_student();
		break;
	    case 4:
		update_student();
		break;
	    case 5:
		printf("\n Exiting Student Management System. Goodbye!\n");
		break;
	    default:
		printf("\n Invalid choice. Please select an option between 1 and 5.\n");
	}

    } while (choice != 5);

    getch();
    return 0;
}