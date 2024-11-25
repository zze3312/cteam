#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"types.h"

#define FILE_LOGIN "UserLogin.txt" // 회원정보
#define FILE_TEST "CarTest_Questions.txt" // 문제
#define FILE_ANSWER "CarTest_Answers.txt" // 정답
#define FILE_WRONG "Wrong_AnswerNote.txt" // 오답노트
#define STRING_SIZE 100 // 문자열 길이
#define QUESTIONS_NUM 40 // 문제수


void mainComment(); // 메인화면
void loginComment(); // 로그인 전 메뉴
void userLogin(char * loginID); // 로그인
void userInfoAdd(); // 회원가입
void userInfoFind(); // 가입내역조회
void subComment(); // 로그인 후 메뉴

int main(void)
{
	int mainMenuNum;
	while(1)
	{
		mainComment();
		loginComment(); 
		printf(" 진행하실 번호를 입력해주세요: ");
		scanf("%d", &mainMenuNum);
		
		switch(mainMenuNum)
		{
			case 1: // 로그인
                userLogin();
				// if (userLogin)
				// {
				// 	printf("님의 접속을 환영합니다. \n"); // loginID
					
					// int subMenuNum;
					// while(1)
					// {
					// 	subComment();
					// 	printf(" 진행하실 번호를 입력해주세요: ");
					// 	scanf("%d", &subMenuNum);
					// }
					// if (subMenuNum == 1) // 모의테스트
					// {
						
					// }
					// else if (subMenuNum == 2) // 실전테스트(필기)
					// {
						
					// }
					// else if (subMenuNum == 3) // 실기시험 - 도로주행시험
					// {
						
					// }
					// else if (subMenuNum == 4) // 시험합격여부
					// {
						
					// }
					// elsf if (subMenuNum == 5) // 지난테스트
					// {
						
					// }
					// else if (subMenuNum == 6) // 오답노트
					// {
						
					// }
					// else if (subMenuNum == 7) // 종료
					// {
					// 	printf(" 브고트 운전면허 프로그램을 종료합니다. \n");
					// 	exit(0); // 프로그램 종료
					// }
					// else
					// {
					// 	printf(" 잘못된 입력입니다. 다시 선택해주세요. \n");
					// }
				// }
				// else
				// {
				// 	printf(" ID 또는 PWD의 정보를 잘못 입력하였습니다. 다시 입력해주세요. \n");
				// }
				break;
			case 2: // 회원가입
				userInfoAdd();
				break;
			case 3: // 가입내역조회
				userInfoFind();
				break;
			case 4: // 종료
				printf(" 브고트 운전면허 프로그램을 종료합니다. \n");
				exit(0); // 프로그램 종료
			default:
				printf(" 잘못된 입력입니다. 다시 선택해주세요. \n");
				break;
		}
	}
	return 0;	
}

void mainComment()
{
	printf("+-----------------------------------------------------------+ \n");
	printf("|                  브고트 운전면허 프로그램                 |\n");
	printf("+-----------------------------------------------------------+ \n");
}

void loginComment()
{
	printf(" 회원이십니까? \n");
	printf(" 1. 로그인(회원이신 경우) \n");
	printf(" 2. 회원가입(비회원이신 경우) \n");
	printf(" 3. 가입내역 조회(로그인 정보를 잊어버리신 경우) \n");	
	printf(" 4. 종료 \n");
	printf("+-----------------------------------------------------------+ \n");
}

void userLogin(char * loginID) // 사용자 입력 요청 - 로그인 : 아이디 id, 비번 pw
{	
	char userID[STRING_SIZE];
	char userPWD[STRING_SIZE];
	char line[STRING_SIZE * 3];	
	char * name;
	char * id;
	char * pwd;
	
	printf("+------------------------ 로 그 인 -------------------------+ \n");
	fputs(" ID : \n", stdout); fgets(userID, sizeof(userID), stdin);
    fputs(" Password : \n", stdout); fgets(userPWD, sizeof(userPWD), stdin);

	FILE * fp_log = fopen(FILE_LOGIN, "rt");
	if (fp_log == NULL)
	{
		printf(" 가입된 정보 내역이 없습니다. 회원가입을 진행해주세요. \n");
		return 0;
	}	

	while(fgets(line, sizeof(line), fp_log)) // 파일 내 ID/PWD 정보 비교
	{		
		name = strtok(line, ","); // 데이터 구분
		id = strtok(NULL, ",");
		pwd = strtok(NULL, "\n");
			
		if (strcmp(id, userID) == 0 && strcmp(pwd, userPWD) == 0)
		{
			fclose(fp_log);
			return 1; // 로그인 성공
		}
	}
	fclose(fp_log);
	return 0; // 로그인 실패
}

void userInfoAdd()
{
	FILE * fp = fopen(FILE_LOGIN, "a+t");
	if (fp == NULL)
	{
		printf(" 파일을 불러올 수 없습니다. \n");
		return;
	}
	User person; // 구조체 호출
	printf("+----------------------- 회 원 가 입 -----------------------+ \n");
	printf(" 회원가입을 위해 아래의 정보를 입력해주세요. \n");
    fputs(" 1.이름 : ", stdout); fgets(person.name, sizeof(person.name), stdin);
    fputs(" 2.생년월일(yymmdd) : ", stdout); fgets(person.birth, sizeof(person.birth), stdin);
    fputs(" 3.휴대폰번호 : ", stdout); fgets(person.tel, sizeof(person.tel), stdin);
    fputs(" 4.ID : ", stdout); fgets(person.id, sizeof(person.id), stdin);
    fputs(" 5.Password : ", stdout); fgets(person.pwd, sizeof(person.pwd), stdin);
    
	fprintf(fp, "%s, %s, %s, %s, %s \n", person.name, person.birth, person.tel, person.id, person.pwd);
    fclose(fp);
	
	printf(" 회원가입이 완료되었습니다. 가입을 축하드립니다. \n");
}

void userInfoFind()
{
	char userfind[STRING_SIZE];
	char line[STRING_SIZE * 3];
	char * name;
	char * birth;
	char * tel;
	char * id;
	char * pwd;
	int found = 0;

	printf("+-----------------------------------------------------------+ \n");
	printf("+-------------------- 가 입 내 역 조 회 --------------------+ \n");
	printf(" 회원정보에 등록된 이름 또는 생년월일을 입력해주세요. \n");
	printf(" 이름/생년월일 : ");
	scanf("%s", userfind);
	
	FILE * fp = fopen(FILE_LOGIN, "rt");
	if (fp == NULL)
	{
		printf(" 가입된 내역이 없습니다. 회원가입을 진행해주세요. \n");
		return;
	}
	
	while (fgets(line, sizeof(line), fp))
	{
		name = strtok(line, ",");
		birth = strtok(NULL, ",");
		tel = strtok(NULL, ",");
		id = strtok(NULL, ",");
		pwd = strtok(NULL, "\n");
		
		if (strcmp(name, userfind) == 0 || strcmp(birth, userfind) == 0)
		{
			printf("+------------------- 사용자의 가입 정보 --------------------+ \n");
			printf(" 이름 : %s \n", name);
			printf(" 생년월일 : %s \n", birth);
			printf(" 휴대폰번호 : %s \n", tel);
			printf(" ID : %s \n", id);	
			printf(" PWD : %s \n", pwd);
			found = 1;
			break;
		}
	}
	if (!found)
	{
		printf(" 해당 정보로 가입된 사용자를 찾을 수 없습니다. \n");
	}
	fclose(fp);
}

// void subComment()
// {
// 	printf("+--------------------- 사 용 자 메 뉴 ----------------------+ \n");
// 	printf(" 1. 모의테스트 \n");
// 	printf(" 2. 실전테스트(필기) \n");
// 	printf(" 3. 실기시험 - 도로주행시험 \n");	
// 	printf(" 4. 시험합격여부 \n");
// 	printf(" 5. 지난테스트 \n");
// 	printf(" 6. 오답노트 \n");
// 	printf(" 7. 종료 \n");
// 	printf("+-----------------------------------------------------------+ \n");
// }

// void mcokTest(Question * Questions)
// {
// 	char line[STRING_SIZE * 3];
// 	int * questionNumber;
// //	char * answer1;
// //	char * answer2;
// //	char * answer3;
// //	char * answer4;
// 	char * correct;
// 	int score, totalQuestion = 0;
// 	int userCorrect;
	
// 	FILE * fp_test = fopen(FILE_TEST, "rt");
// 	FILE * fp_answer = fopen(FILE_ANSWER, "rt");
// 	if (fp_test == NULL || fp_answer == NULL)
// 	{
// 		printf(" 파일을 불러올 수 없습니다. \n");
// 		return 0;
// 	}
	
// 	printf("+--------------------- 모의테스트 시작 ---------------------+ \n");
// 	while(fgets(line, sizeof(line), fp))
// 	{
		
// 		totalQuestion++;
		
// 	}
// }