#include <stdio.h>
#include <stdlib.h>

struct student {
	int id;
	char name[50];
	int age;
	float gpa;
};

struct node {
	struct student data;
	struct node *next;
};

struct node *head = NULL;

void addStudent(const struct student *const ptr);
void displayStudents(void);
void searchStudentByID(int id);
void updateStudents(int id);
float calculateAverageGPA(void);
void searchHighestGPA(void);
void deleteStudent(int id);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	struct student Student;
	unsigned int choice = 0;
	unsigned int id = 0;
	while (choice < 8 && choice >= 0) {
		printf("1.Add Student\n");
		printf("2.Display Students\n");
		printf("3.Search Student By ID\n");
		printf("4.Update Student Information\n");
		printf("5.Delete Student\n");
		printf("6.Calculate Average GPA\n");
		printf("7.Search for Student with Highest GPA\n");
		printf("8.Exit\n");
		printf("----------------------------\n");
		printf("Enter Choice: ");
		scanf("%d", &choice);
		printf("----------------------------\n");
		switch (choice) {
		case 1:
			printf("Enter Name: ");
			scanf("%s", Student.name);

			printf("Enter Age: ");
			scanf("%d", &Student.age);

			printf("Enter ID: ");
			scanf("%d", &Student.id);

			printf("Enter GPA: ");
			scanf("%f", &Student.gpa);
			addStudent(&Student);
			break;
		case 2:
			displayStudents();
			break;
		case 3:

			printf("Enter Student ID to Search: ");
			scanf("%d", &id);
			searchStudentByID(id);
			break;
		case 4:

			printf("Enter Student ID to Update it's Information: ");
			scanf("%d", &id);
			printf("----------------------------\n");
			updateStudents(id);
			break;
		case 5:

			printf("Enter Student ID to Delete it's Data: ");
			scanf("%d", &id);
			deleteStudent(id);
			break;
		case 6:
			printf("Average GPA = %f\n", calculateAverageGPA());
			printf("----------------------------\n");
			break;
		case 7:
			searchHighestGPA();
			break;
		case 8:
			printf("End Of Program... Good Bye!\n");
		}
	}
}
void addStudent(const struct student *const ptr) {
	printf("----------------------------\n");
	struct node *current = NULL;
	struct node *search = NULL;
	struct node *link = (struct node*) malloc(sizeof(struct node));

	if (ptr == NULL) {
		printf("ERROR!\n");
		printf("----------------------------\n");
		return;
	}

	link->data = *ptr;
	link->next = NULL;
	if (head == NULL) {
		head = link;
		return;
	}

	current = head;
	search = head;
	while (search != NULL) {
		if (search->data.id == ptr->id) {
			printf(
					"Error! The ID '%d' You Entered exists\nPlease, Enter another ID\n",
					ptr->id);
			printf("----------------------------\n");
			return;
		}
		search = search->next;
	}
	while (current->next != NULL) {
		current = current->next;
	}

	current->next = link;
}

void displayStudents(void) {

	struct node *current = head;

	if (head == NULL) {
		printf("The List is Empty\n");
		printf("----------------------------\n");
		return;
	}

	while (current != NULL) {
		printf("[ ID: %d, Name: %s, Age: %d, GPA: %.2f ]\n", current->data.id,
				current->data.name, current->data.age, current->data.gpa);
		current = current->next;
	}

	printf("----------------------------\n");
}
void searchStudentByID(int id) {

	struct node *current = NULL;
	unsigned char found = 0;
	if (head == NULL) {
		printf("List is Empty\n");
		printf("----------------------------\n");
		return;
	}
	current = head;
	while (current != NULL) {
		if (current->data.id == id) {
			printf("[ ID: %d, Name: %s, Age: %d, GPA: %.2f ]\n", current->data.id,
					current->data.name, current->data.age, current->data.gpa);
			found = 1;
		}
		current = current->next;

	}
	if (!(found)) {
		printf("There is no Student with this ID '%d'\n", id);
	}
	printf("----------------------------\n");
}
void updateStudents(int id) {
	struct node *current = NULL;
	unsigned char found = 0;
	if (head == NULL) {
		printf("List is Empty\n");
		printf("----------------------------\n");
		return;
	}
	current = head;
	printf("Enter New Details for the Student with ID %d\n", id);
	while (current != NULL) {
		if (current->data.id == id) {
			printf("Enter New Name: ");
			scanf("%s", current->data.name);
			printf("Enter New Age: ");
			scanf("%d", &current->data.age);
			printf("Enter New GPA: ");
			scanf("%f", &current->data.gpa);
			found = 1;
		}
		current = current->next;
	}
	if (!(found)) {
		printf("There is no Student with this ID '%d'\n", id);
	} else {
		printf("Student Details Updated Successfully\n");
	}
	printf("----------------------------\n");
}
float calculateAverageGPA(void) {
	struct node *current = NULL;
	float sum = 0;
	unsigned int numOfStudents = 0;
	if (head == NULL) {
		return 0;
	}
	current = head;
	while (current != NULL) {
		sum += current->data.gpa;
		current = current->next;
		numOfStudents++;
	}
	return sum / (float) numOfStudents;
}
void searchHighestGPA(void) {
	struct node *current = NULL;
	struct node *maxGPA_Student = NULL;
	if (head == NULL) {
		printf("List is Empty\n");
		printf("----------------------------\n");
		return;
	}
	current = head;
	maxGPA_Student = head;
	while (current != NULL) {
		if (current->data.gpa > maxGPA_Student->data.gpa) {
			maxGPA_Student = current;
		}
		current = current->next;
	}
	printf(
			"Highest GPA = %.2f\nStudent with the Highest GPA:\n[ ID: %d, Name: %s, Age: %d, GPA: %.2f ]\n",
			maxGPA_Student->data.gpa, maxGPA_Student->data.id,
			maxGPA_Student->data.name, maxGPA_Student->data.age,
			maxGPA_Student->data.gpa);
	printf("----------------------------\n");
}
void deleteStudent(int id) {

	struct node *current = NULL;
	struct node *prev = NULL;
	struct node *delete_node = NULL;

	if (head == NULL) {
		printf("List is Empty\n");
		printf("----------------------------\n");
		return;
	}

	if (head->data.id == id) {
		delete_node = head;
		head = head->next;
		free(delete_node);
		printf("Student with ID '%d' Deleted Successfully\n", id);
		printf("----------------------------\n");
		return;
	}

	current = head->next;
	prev = head;
	while (current != NULL) {
		if (current->data.id == id) {
			delete_node = current;
			prev->next = current->next;
			free(delete_node);
			printf("Student with ID '%d' Deleted Successfully\n", id);
			printf("----------------------------\n");
			return;
		}
		prev = current;
		current = current->next;
	}

	if (delete_node == NULL) {
		printf("There is no Student with this ID '%d'\n", id);
		printf("----------------------------\n");
	}

}

