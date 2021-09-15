#include <iostream>
#include <cstring>

#define STUDENT_MAX_NUM 500

typedef enum {
	ADD = 1,
	FIND,
	SEE_ALL,
	CLOSE
} command;

typedef struct{
	int id;
	char name[24];
	char phone[16];
	int grade;
} Student;

int fileOpen(FILE*& fp, const char* filename, const char* mode){
	fp = fopen(filename, mode);
	if (fp == NULL){
		perror("open file : ");
		return -1;
		exit(1);
	}
	return 0;
}

int AddStudent(FILE* fp, Student* student_in){

	std::cout << "Enter Student's ID" << std::endl;
	std::cin >> student_in->id; getchar();
	std::cout << "Enter Student's Name" << std::endl;
	std::cin >> student_in->name; getchar();
	std::cout << "Enter Student's Phone Number(ex 010-1234-5678)" << std::endl;
	std::cin >> student_in->phone; getchar();
	std::cout << "Enter Student's Grade" << std::endl;
	std::cin >> student_in->grade; getchar();

	fseek(fp, 0, SEEK_END);
	// fp의 파일포인터를 whenece SEEK_END로 부터 0만큼 이동한 곳으로 옮긴다.
	fwrite(student_in, sizeof(Student), 1, fp);
	
	return 0;
}

int FindStudent(FILE* fp, Student* student_in){
	// 해당하는 학생을 찾았을 경우에는 해당하는 파일포인터의 위치를 알려주고,
	// 찾지 못하였을 경우에는 -1를 반환한다.
	char name_buf[10];
	std::cout << " Enter Student's Name" << std::endl;
	std::cin >> name_buf;
	getchar();
	
	fseek(fp, 0, SEEK_SET);

	while(!feof(fp)){
		fread(student_in, sizeof(Student), 1, fp);
		if (strcmp(student_in->name, name_buf) == 0){
			fseek(fp, -sizeof(Student), SEEK_CUR);
			return ftell(fp);
		}
	}
	return -1;
}

int SeeAll(FILE* fp, Student* student_in){
	int num;
	fseek(fp, 0, SEEK_SET);
	std::cout << "ID \t" << "NAME \t" << "PHONE \t" << "GRADE" << std::endl;
	while(!feof(fp)){
		if((num = fread(student_in, sizeof(Student), 1, fp)) != 1){
			perror("read student data : ");
			return -1;
		}
		std::cout << student_in->id << "\t" << student_in->name << "\t" << student_in->phone << "\t" << student_in->grade << "\n";
	}
	return 0;
}


int makeAction(){
	std::cout << "ADD : 1 / FIND : 2 / SEE ALL DATA : 3 / CLOSE : 4" << std::endl;
	int sel;
	std::cin >> sel;
	return sel;
}



int main(){
	FILE *fp = nullptr;
	Student student;
	fileOpen(fp, "StudentDB.bin", "ab+");

	while(true){
		switch (makeAction()){
		case ADD:
			AddStudent(fp, &student);
			continue;
		case FIND:
			if (FindStudent(fp, &student) < 0){
				std::cout << "Can't find the data" << std::endl;
			}
			else{
				std::cout << student.id << "\t" << student.name << "\t" << student.phone << "\t" << student.grade << "\n";
			}
			break;
		case SEE_ALL:
			if(SeeAll(fp, &student) < 0){
				std::cout << "Error with Reading File" << std::endl;	
			}
		case CLOSE:
			exit(0);
		}
	}
}

