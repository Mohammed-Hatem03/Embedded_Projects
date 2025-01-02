#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define ADMIN_MODE 1
#define USER_MODE 2
#define EXIT 3
#define GENDER_SIZE 10
#define NAME_SIZE 50
#define PASSWORD_SIZE 5
#define MAX_PATIENTS 100
#define MAX_SLOTS 5

// Time slots available
const char *time_slots[MAX_SLOTS] = {
		"2:00 PM - 2:30 PM",
		"2:30 PM - 3:00 PM",
		"3:00 PM - 3:30 PM",
		"4:00 PM - 4:30 PM",
		"4:30 PM - 5:00 PM"
};

// Patient structure
typedef struct {
	unsigned int id;
	unsigned int age;
	unsigned char name[NAME_SIZE];
	unsigned char gender[GENDER_SIZE];
} Information;

// Reservation structure
typedef struct {
	unsigned int patient_id;
	int slot_index; // Index of the reserved slot
} Reservation;

unsigned char default_password[PASSWORD_SIZE] = "1234";
unsigned char entered_password_by_admin[PASSWORD_SIZE];
unsigned char matched_password = 0;
unsigned int attempts = 3;

Information patients[MAX_PATIENTS];
Reservation reservations[MAX_SLOTS] = {{0, -1}}; // Initialize reservations as empty

unsigned int patient_count = 0;

void enteringPassword(unsigned char *entered_password);
unsigned char checkPasswords(unsigned char *default_password, unsigned char *entered_password);
void addPatient(void);
void editPatientRecord(void);
void reserveSlot(void);
void cancelReservation(void);
void viewPatientRecord(void);
void viewReservations(void);
void initializeReservations(void);
int findPatientByID(unsigned int id);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	unsigned int mode = 1;


	while (mode != EXIT) {
		printf("1. ADMIN\n");
		printf("2. USER\n");
		printf("3. EXIT\n");
		printf("----------------------------\n");
		printf("Enter MODE: ");
		scanf("%d", &mode);
		printf("----------------------------\n");
		switch (mode) {
		case ADMIN_MODE:
			printf("Hello ADMIN\n\n");
			while (attempts > 0 && !matched_password) {
				enteringPassword(entered_password_by_admin);
				matched_password = checkPasswords(default_password, entered_password_by_admin);
			}

			if (attempts == 0) {
				printf("Too many incorrect attempts. Exiting...\n\n");
				mode = EXIT;
				break;
			}

			printf("Access Granted.\n\n");
			int admin_choice;
			do {
				printf("1. Add New Patient Record\n");
				printf("2. Edit Patient Record\n");
				printf("3. Reserve a Slot with the Doctor\n");
				printf("4. Cancel Reservation\n");
				printf("5. Exit Admin Mode\n");
				printf("Enter your choice: ");
				scanf("%d", &admin_choice);

				switch (admin_choice) {
				case 1:
					addPatient();
					break;
				case 2:
					editPatientRecord();
					break;
				case 3:
					reserveSlot();
					break;
				case 4:
					cancelReservation();
					break;
				case 5:
					printf("Exiting Admin Mode...\n\n");
					matched_password = 0;
					break;
				default:
					printf("Invalid choice. Try again.\n\n");
				}
			} while (admin_choice != 5);
			break;

		case USER_MODE:
			printf("Hello USER\n\n");
			int user_choice;
			do {
				printf("1. View Patient Record\n");
				printf("2. View Today's Reservations\n");
				printf("3. Exit\n");
				printf("Enter your choice: ");
				scanf("%d", &user_choice);

				switch (user_choice) {
				case 1:
					viewPatientRecord();
					break;
				case 2:
					viewReservations();
					break;
				case 3:
					printf("Exiting User Mode...\n\n");
					break;
				default:
					printf("Invalid choice. Try again.\n\n");
				}
			} while (user_choice != 3);
			break;
		case EXIT:
			printf("EXITING THE PROGRAM\n");
		}
	}

	return 0;
}

void enteringPassword(unsigned char *entered_password) {
	printf("Please Enter Your Password: ");
	scanf("%s", entered_password);
}

unsigned char checkPasswords(unsigned char *default_password, unsigned char *entered_password) {
	if (strcmp((char *)default_password, (char *)entered_password) == 0) {
		return TRUE; // Passwords match
	} else {
		attempts--;
		printf("Incorrect password. Remaining attempts: %d\n\n", attempts);
		return FALSE; // Passwords do not match
	}
}

void addPatient(void) {
	if (patient_count >= MAX_PATIENTS) {
		printf("Patient list is full. Cannot add more patients.\n\n");
		return;
	}

	Information new_patient;
	printf("Enter Patient ID: ");
	scanf("%u", &new_patient.id);

	if (findPatientByID(new_patient.id) != -1) {
		printf("Patient ID already exists.\n\n");
		return;
	}

	printf("Enter Patient Name: ");
	scanf("%s", new_patient.name);
	printf("Enter Patient Age: ");
	scanf("%u", &new_patient.age);
	printf("Enter Patient Gender: ");
	scanf("%s", new_patient.gender);

	patients[patient_count++] = new_patient;
	printf("Patient added successfully.\n\n");
}

int findPatientByID(unsigned int id) {
	for (int i = 0; i < patient_count; i++) {
		if (patients[i].id == id) {
			return i; // Return the index of the patient
		}
	}
	return -1; // Patient not found
}

void viewPatientRecord(void) {
	unsigned int id;
	printf("Enter Patient ID to view record: ");
	scanf("%u", &id);

	int index = findPatientByID(id);
	if (index == -1) {
		printf("Patient ID not found.\n\n");
		return;
	}

	// Display patient information
	printf("Patient ID: %u\n", patients[index].id);
	printf("Name: %s\n", patients[index].name);
	printf("Age: %u\n", patients[index].age);
	printf("Gender: %s\n\n", patients[index].gender);
}

void viewReservations(void) {
    printf("Today's Reservations:\n");

    int has_reservations = 0; // Flag to check if there are reservations
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (reservations[i].slot_index != -1) { // Check if slot is reserved
            printf("Slot: %s, Patient ID: %u\n", time_slots[i], reservations[i].patient_id);
            has_reservations = 1;
        }
    }

    if (!has_reservations) {
        printf("No reservations for today.\n");
    }
    printf("\n");
}


void editPatientRecord(void) {
	unsigned int id;
	printf("Enter Patient ID to edit: ");
	scanf("%u", &id);

	int index = findPatientByID(id);
	if (index == -1) {
		printf("Patient ID not found.\n\n");
		return;
	}

	printf("Enter New Name: ");
	scanf("%s", patients[index].name);
	printf("Enter New Age: ");
	scanf("%u", &patients[index].age);
	printf("Enter New Gender: ");
	scanf("%s", patients[index].gender);

	printf("Patient record updated successfully.\n\n");
}

void reserveSlot(void) {

	initializeReservations();
    unsigned int id;
    printf("Enter Patient ID to reserve a slot: ");
    scanf("%u", &id);

    // Check if the patient ID exists
    int index = findPatientByID(id);
    if (index == -1) {
        printf("Patient ID not found.\n\n");
        return;
    }

    // Display available slots
    printf("Available Slots:\n");
    int has_available_slots = 0;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (reservations[i].slot_index == -1) { // Slot is available
            printf("%d. %s\n", i + 1, time_slots[i]);
            has_available_slots = 1;
        }
    }

    // If no slots are available, display a message
    if (!has_available_slots) {
        printf("No available slots. All are reserved.\n\n");
        return;
    }

    // Get the user's choice for the slot
    int slot_choice;
    printf("Enter slot number to reserve: ");
    scanf("%d", &slot_choice);

    // Validate slot choice
    if (slot_choice < 1 || slot_choice > MAX_SLOTS) {
        printf("Invalid slot selection.\n\n");
        return;
    }

    // Check if the chosen slot is already reserved
    if (reservations[slot_choice - 1].slot_index != -1) {
        printf("Selected slot is already reserved. Choose a different slot.\n\n");
        return;
    }

    // Reserve the selected slot
    reservations[slot_choice - 1].patient_id = id;
    reservations[slot_choice - 1].slot_index = slot_choice - 1;
    printf("Slot reserved successfully for Patient ID: %u, Time: %s\n\n", id, time_slots[slot_choice - 1]);
}


void cancelReservation(void) {
	unsigned int id;
	printf("Enter Patient ID to cancel reservation: ");
	if (scanf("%u", &id) != 1) {
		printf("Invalid input. Try again.\n");
		return;
	}

	for (int i = 0; i < MAX_SLOTS; i++) {
		if (reservations[i].patient_id == id) {
			reservations[i].patient_id = 0;
			reservations[i].slot_index = -1;
			printf("Reservation canceled successfully.\n");
			return;
		}
	}
	printf("No reservation found for this ID.\n");
}

void initializeReservations(void) {
    for (int i = 0; i < MAX_SLOTS; i++) {
        reservations[i].patient_id = 0;
        reservations[i].slot_index = -1;
    }
}

