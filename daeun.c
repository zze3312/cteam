#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include"types.h"

#define FILE_LOGIN "UserLogin.txt" // 회원정보
#define FILE_TEST "CarTest_Questions.txt" // 문제
#define FILE_ANSWER "CarTest_Answers.txt" // 정답
#define FILE_WRONG "Wrong_AnswerNote.txt" // 오답노트

#define STRING_SIZE 100 // 문자열 길이
#define USER_SIZE 20
#define QUESTIONS_NUM 40 // 문제수


void mainComment(); // 메인화면
void userLogin(User *); // 로그인
void userInfoAdd(); // 회원가입
void userInfoFind(); // 가입내역조회
void subComment(); // 로그인 후 메뉴
int subMenu(User *);
void clearBuffer(); // 입력값 비우기

int main(void)
{
	int mainMenuNum;
	User loginUser;
	while(1)
	{
		mainComment();
		printf(" 진행하실 번호를 입력해주세요: ");
		scanf("%d", &mainMenuNum);

		switch(mainMenuNum)
		{
			case 1: // 로그인
				system("clear");
                userLogin(&loginUser);
				if (strlen(loginUser.id) != 0)
				{
					printf("%s님의 접속을 환영합니다.\n", loginUser.id); // loginID
					sleep(1);
					system("clear");
					int result = subMenu(&loginUser);
					if (result == 0) {
						break;
					}
				}else {
					printf("가입되지 않은 정보입니다.\n");
				}
				break;
			case 2: // 회원가입
				userInfoAdd();
				break;
			case 3: // 가입내역조회
				userInfoFind();
				break;
			case 4: // 종료
				printf(" 브고트 운전면허 프로그램을 종료합니다.\n");
				exit(0); // 프로그램 종료
			default:
				printf(" 잘못된 입력입니다. 다시 선택해주세요.\n");
				break;
		}
	}
	return 0;	
}

void mainComment()
{
	printf("+-----------------------------------------------------------+\n");
	printf("|                  브고트 운전면허 프로그램                 |\n");
	printf("+-----------------------------------------------------------+\n");
	printf(" 1. 로그인\n");
	printf(" 2. 회원가입\n");
	printf(" 3. 아이디/비밀번호 찾기\n");
	printf(" 4. 종료\n");
	printf("+-----------------------------------------------------------+\n");
}


void userLogin(User *loginUser) // 사용자 입력 요청 - 로그인 : 아이디 id, 비번 pw
{
    char userId[USER_SIZE];
	char userPwd[USER_SIZE];
	char line[STRING_SIZE];
	User tempUser;
	
	printf("+------------------------ 로 그 인 -------------------------+\n"); clearBuffer();
	fputs(" ID : ", stdout); fgets(userId, sizeof(userId), stdin);
	strcpy(strstr(userId, "\n"),"\0");
    fputs(" Password : ", stdout); fgets(userPwd, sizeof(userPwd), stdin);
	strcpy(strstr(userPwd, "\n"),"\0");

	FILE * fp = fopen(FILE_LOGIN, "rt");
	if (fp == NULL)
	{
		printf(" 회원정보 파일이 없습니다. 관리자에게 문의해주세요.\n");
	}
	while(fgets(line, sizeof(line), fp)) // 파일 내 ID/PWD 정보 비교
	{
		strcpy(tempUser.name, strtok(line, ",")); // 데이터 구분
		strcpy(tempUser.birth, strtok(NULL, ","));
		strcpy(tempUser.tel, strtok(NULL, ","));
		strcpy(tempUser.id, strtok(NULL, ","));
		strcpy(tempUser.pwd, strtok(NULL, "\n"));

		if (strcmp(tempUser.id, userId) == 0 && strcmp(tempUser.pwd, userPwd) == 0)
		{
			strcpy(loginUser -> name, tempUser.name);
			strcpy(loginUser -> birth, tempUser.birth);
			strcpy(loginUser -> tel, tempUser.tel);
			strcpy(loginUser -> id, tempUser.id);
			strcpy(loginUser -> pwd, tempUser.pwd);

			printf("%s : 로그인에 성공하셨습니다\n", loginUser -> id);
            break;
		}
	}
	fclose(fp);
}

void userInfoAdd()
{
	User userInput; // 구조체 호출
	User tempUser;
	char flag = 'Y';
	char line[STRING_SIZE];



	while(1){
		char quit;
		clearBuffer();
		if (quit == 'q') {
			break;
		}

		printf("+----------------------- 회 원 가 입 -----------------------+\n");
		printf(" 회원가입을 위해 아래의 정보를 입력해주세요.\n");
	    fputs(" 1.이름 : ", stdout); fgets(userInput.name, sizeof(userInput.name), stdin);
	    strcpy(strstr(userInput.name, "\n"),"\0");
	    fputs(" 2.생년월일(yymmdd) : ", stdout); fgets(userInput.birth, sizeof(userInput.birth), stdin);
		strcpy(strstr(userInput.birth, "\n"),"\0");
	    fputs(" 3.휴대폰번호(숫자만 입력해주세요) : ", stdout); fgets(userInput.tel, sizeof(userInput.tel), stdin);
		strcpy(strstr(userInput.tel, "\n"),"\0");
	    fputs(" 4.ID : ", stdout); fgets(userInput.id, sizeof(userInput.id), stdin);
		strcpy(strstr(userInput.id, "\n"),"\0");
	    fputs(" 5.Password : ", stdout); fgets(userInput.pwd, sizeof(userInput.pwd), stdin);
		strcpy(strstr(userInput.pwd, "\n"),"\0");

		FILE * fp = fopen(FILE_LOGIN, "rt");
		while (fgets(line, sizeof(line), fp)) {
			strcpy(tempUser.name,strtok(line, ","));
			strcpy(tempUser.birth, strtok(NULL, ","));
			strcpy(tempUser.tel, strtok(NULL, ","));
			strcpy(tempUser.id, strtok(NULL, ","));
			strcpy(tempUser.pwd, strtok(NULL, "\n"));

			if (strcmp(tempUser.tel, userInput.tel) == 0 || strcmp(tempUser.id, userInput.id) == 0) {
				flag = 'N';
				break;
			}
		}
		fclose(fp);

		if (flag == 'N') {
			printf("이미 가입된 정보입니다.\n 다시 입력해 주세요.\n 뒤로 가시려면 q를 입력하세요 : ");
			scanf("%c",&quit);
		} else {
			fp = fopen(FILE_LOGIN, "at");

			if (fp == NULL)
			{
				printf(" 파일을 불러올 수 없습니다.\n");
				return;
			}

			fprintf(fp, "%s,%s,%s,%s,%s\n", userInput.name, userInput.birth, userInput.tel, userInput.id, userInput.pwd);
			printf(" %s님 회원가입이 완료되었습니다. 가입을 축하드립니다.\n", userInput.name);

			fclose(fp);
			break;
		}
	}
}

void userInfoFind()
{
	char userName[USER_SIZE];
	char userTel[USER_SIZE];
	char line[STRING_SIZE];
	User userInfo;
	int found = 0;

	clearBuffer();
	printf("+---------------------- 아이디/비밀번호 찾기 ----------------------+\n");
	printf(" 가입하신 이름과 생년월일을 입력해주세요.\n");
	fputs(" 이름 : ", stdout); fgets(userName, sizeof(userName), stdin);
	strcpy(strstr(userName, "\n"),"\0");

	fputs(" 연락처 : ", stdout); fgets(userTel, sizeof(userTel), stdin);
	strcpy(strstr(userTel, "\n"),"\0");
	FILE * fp = fopen(FILE_LOGIN, "rt");
	if (fp == NULL)
	{
		printf(" 가입된 내역이 없습니다. 회원가입을 진행해주세요.\n");
		return;
	}
	
	while (fgets(line, sizeof(line), fp)){
		strcpy(userInfo.name,strtok(line, ","));
		strcpy(userInfo.birth, strtok(NULL, ","));
		strcpy(userInfo.tel, strtok(NULL, ","));
		strcpy(userInfo.id, strtok(NULL, ","));
		strcpy(userInfo.pwd, strtok(NULL, "\n"));
		
		if (strcmp(userInfo.name, userName) == 0 && strcmp(userInfo.tel, userTel) == 0)
		{
			printf("+------------------- 회원 가입 정보 --------------------+\n");
			printf(" 이름 : %s\n", userInfo.name);
			printf(" 생년월일 : %s\n", userInfo.birth);
			printf(" 휴대폰번호 : %s\n", userInfo.tel);
			printf(" ID : %s\n", userInfo.id);
			printf(" PWD : %s\n", userInfo.pwd);
			found = 1;
		}
	}
	if (!found)
	{
		printf(" 해당 정보로 가입된 사용자를 찾을 수 없습니다.\n");
	}
	fclose(fp);
}

void subComment(char userId[])
{
	printf("현재 로그인된 아이디 : %s\n", userId);
	printf("+--------------------- 사 용 자 메 뉴 ----------------------+\n");
	printf(" 1. 모의테스트\n");
	printf(" 2. 실전테스트(필기)\n");
	printf(" 3. 실기시험 - 도로주행시험\n");	
	printf(" 4. 시험합격여부\n");
	printf(" 5. 지난테스트\n");
	printf(" 6. 오답노트\n");
	printf(" 7. 로그아웃\n");
	printf("+-----------------------------------------------------------+\n");
}

int subMenu(User *loginUser){
	if (strlen(loginUser -> id) == 0) {
		printf("로그인된 정보가 없습니다.\n");
		sleep(1);
		return 0;
	}

	while(1)
	{
		subComment(loginUser->id);
		printf(" 진행하실 번호를 입력해주세요: ");
		int subMenuNum;
		scanf("%d", &subMenuNum);
		clearBuffer();
		if (subMenuNum == 1) // 모의테스트
		{
			printf("모의테스트 실행\n");
		}
		else if (subMenuNum == 2) // 실전테스트(필기)
		{
			printf("실전테스트 실행\n");
		}
		else if (subMenuNum == 3) // 실기시험 - 도로주행시험
		{
			printf("실기시험 실행\n");
		}
		else if (subMenuNum == 4) // 시험합격여부
		{
			printf("시험합격여부 실행\n");
		}
		else if (subMenuNum == 5) // 지난테스트
		{
			printf("지난테스트 실행\n");
		}
		else if (subMenuNum == 6) // 오답노트
		{
			printf("오답노트 실행\n");
		}
		else if (subMenuNum == 7) // 종료
		{
			strcpy(loginUser -> name, "\0");
			strcpy(loginUser -> birth, "\0");
			strcpy(loginUser -> tel, "\0");
			strcpy(loginUser -> id, "\0");
			strcpy(loginUser -> pwd, "\0");
			printf(" 로그아웃 합니다.\n");

			break; // 프로그램 종료
		}
		else
		{
			printf(" 잘못된 입력입니다. 다시 선택해주세요.\n");
		}
	}
}

// void mcokTest(Question * Questions)
// {
// 	char line[STRING_SIZE];
// 	int * questionNumber;
// 	char * answer1;
// 	char * answer2;
// 	char * answer3;
// 	char * answer4;
// 	char * correct;
// 	int score, totalQuestion = 0;
// 	int userCorrect;
	
// 	FILE * fp_test = fopen(FILE_TEST, "rt");
// 	FILE * fp_answer = fopen(FILE_ANSWER, "rt");
// 	if (fp_test == NULL || fp_answer == NULL)
// 	{
// 		printf(" 파일을 불러올 수 없습니다.\n");
// 		return 0;
// 	}
	
// 	printf("+--------------------- 모의테스트 시작 ---------------------+\n");
// 	while(fgets(line, sizeof(line), fp))
// 	{
		
// 		totalQuestion++;
		
// 	}
// }

// void wrongAnswer()
// {
//     FILE * fp_wrong = fopen(FILE_WRONG, "rt");
// }

void clearBuffer()
{
    while (getchar() != '\n');
}