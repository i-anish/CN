#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#define cap 5

typedef struct DOB{

	int day;
	int month;
	int year;
}DOB;
typedef struct student{
	int roll;
	char name[60];
	char address[60];
	char branch[10];
	float cgpa;
	DOB dob;
}STUDENT;

int size=0;
STUDENT std[cap];

bool checkRoll(int roll){

	for(int i=0;i<size;i++){
		if(roll==std[i].roll)
			return false;
	}

	return true;
}
void addStudent(){
	if(size==cap-1){
		printf("Capacity is full!");
		return;
	}

	int roll;
	printf("\nEnter roll number: ");
	scanf("%d",&roll);
	if(!checkRoll(roll) && size!=0){
		printf("Please enter a unique roll number!");
		return;
	}
	std[size].roll=roll;

	printf("Enter name of student: ");
	scanf("%s",std[size].name);

	printf("Enter address: ");
	scanf("%s",std[size].address);

	printf("Enter branch: ");
	scanf("%s",std[size].branch);

	printf("Enter cgap: ");
	scanf("%f",&std[size].cgpa);

	printf("Enter day of birth: ");
	scanf("%d",&std[size].dob.day);

	printf("Enter month of birth: ");
	scanf("%d",&std[size].dob.month);

	printf("Enter year of birth: ");
	scanf("%d",&std[size].dob.year);

	size++;
}

void deleteStudent(int roll){
	int i=0;
	bool found=false;
	for(;i<=size;){
		if(std[i].roll==roll){
			found=true;
			break;
		}
		i++;
	}
	if(found){
		for(int j=i;j<size;j++){
			std[j] = std[j+1];
		}

		printf("Student successfully deleted!");
		size--;
	}
	else{
		printf("Sorry, student doesn't exists!");
	}
}

void showDetails(){

	if(size==0){
		printf("Sorry, their are no details of any student!");
		return;
	}
	for(int i=0;i<size;i++){
		printf("Details of %d student:\n",i+1);
		printf("Name: %s\n",std[i].name);
		printf("Roll: %d\n",std[i].roll);
		printf("CGPA: %f\n",std[i].cgpa);
		printf("Address: %s\n",std[i].address);
		printf("Branch: %s\n",std[i].branch);
		printf("DOB: %d/%d/%d",std[i].dob.day,std[i].dob.month,std[i].dob.year);
		printf("\n\n");
	}
}

void highestCGPA(){

	int highCGPA=0;
	int indWithHighCGPA=-1;

	for(int i=0;i<size;i++){
		if(highCGPA<std[i].cgpa){
			highCGPA = std[i].cgpa;
			indWithHighCGPA = i;
		}
	}

	printf("Details of student with highest cgpa:\n");
	printf("Name: %s\n",std[indWithHighCGPA].name);
	printf("Roll: %d\n",std[indWithHighCGPA].roll);
	printf("CGPA: %f\n",std[indWithHighCGPA].cgpa);
	printf("Address: %s\n",std[indWithHighCGPA].address);
	printf("Branch: %s\n",std[indWithHighCGPA].branch);
	printf("DOB: %d/%d/%d",std[indWithHighCGPA].dob.day,std[indWithHighCGPA].dob.month,std[indWithHighCGPA].dob.year);
	printf("\n\n");
}


int main(){
	int roll;

	while(true){
		printf("\nEnter\n1 to add student\n2 to delete a student\n3 to see the details of student\n4 to fetch the details of student with highest CGPA\n");
		int choice;
		scanf("%d",&choice);
		switch(choice){
			case 1:	addStudent();
				break;
			case 2:	printf("Enter roll of student to delete: ");
			       	scanf("%d",&roll);
				deleteStudent(roll);
				break;
			case 3:	showDetails();
				break;
			case 4:	highestCGPA();
				break;
			default: exit(0);
		}
	}

     	return 0;
}

