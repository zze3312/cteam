#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include <wctype.h>

#include "/home/lms/CLionProjects/cteam/header/types.h"

#define FILE_LOGIN "/home/lms/CLionProjects/cteam/dataFile/UserLogin.txt" // 회원정보
#define FILE_DATA "/home/lms/CLionProjects/cteam/dataFile/test.csv"
#define FILE_WRONG "/home/lms/CLionProjects/cteam/dataFile/WrongAnswerNote.txt" // 오답노트
#define STRING_SIZE 100 // 문자열 길이
#define USER_SIZE 20 // 문자 길이
#define QUESTIONS_NUM 40 // 문제수


void mainComment(); // 메인화면
void userLogin(User *); // 로그인
void userInfoAdd(); // 회원가입
void userInfoFind(); // 가입내역조회
void subComment(); // 로그인 후 메뉴
int subMenu(User *); // 메뉴 선택
void clearBuffer(); // 입력값 비우기


void wrongAnswerNote();


void selQuestion(char num[], Question *qst);

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
					printf(" %s님의 접속을 환영합니다.\n", loginUser.id);
					sleep(1);
					system("clear");
					int result = subMenu(&loginUser);
					if (result == 0) 
					{
						break;
					}
				}
				else
				{
					printf(" 가입되지 않은 정보입니다.\n");
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
	while(fgets(line, sizeof(line), fp)) 
	{
		strcpy(tempUser.name, strtok(line, ",")); // 데이터 구분
		strcpy(tempUser.birth, strtok(NULL, ","));
		strcpy(tempUser.tel, strtok(NULL, ","));
		strcpy(tempUser.id, strtok(NULL, ","));
		strcpy(tempUser.pwd, strtok(NULL, "\n"));

		if (strcmp(tempUser.id, userId) == 0 && strcmp(tempUser.pwd, userPwd) == 0) // tempUser(작성한 id/pwd)와 userId(회원가입 id/pwd)가 동일할 경우
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

	while(1)
	{
		char quit;
		clearBuffer();
		if (quit == 'q') // 잘못 선택했을 경우 메인 출력
		{
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
		while (fgets(line, sizeof(line), fp)) 
		{
			strcpy(tempUser.name,strtok(line, ","));
			strcpy(tempUser.birth, strtok(NULL, ","));
			strcpy(tempUser.tel, strtok(NULL, ","));
			strcpy(tempUser.id, strtok(NULL, ","));
			strcpy(tempUser.pwd, strtok(NULL, "\n"));

			if (strcmp(tempUser.tel, userInput.tel) == 0 || strcmp(tempUser.id, userInput.id) == 0)
			{
				flag = 'N';
				break;
			}
		}
		fclose(fp);

		if (flag == 'N') 
		{
			printf("이미 가입된 정보입니다.\n 다시 입력해 주세요.\n 뒤로 가시려면 q를 입력하세요 : ");
			scanf("%c",&quit);
		} 
		else 
		{
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
	printf("+------------------- 아이디/비밀번호 찾기 --------------------+\n");
	printf(" 가입하신 이름과 휴대폰번호를 입력해주세요.\n");
	fputs(" 이름 : ", stdout); fgets(userName, sizeof(userName), stdin);
	strcpy(strstr(userName, "\n"),"\0");

	fputs(" 휴대폰번호 : ", stdout); fgets(userTel, sizeof(userTel), stdin);
	strcpy(strstr(userTel, "\n"),"\0");
	FILE * fp = fopen(FILE_LOGIN, "rt");
	if (fp == NULL)
	{
		printf(" 가입된 내역이 없습니다. 회원가입을 진행해주세요.\n");
		return;
	}
	
	while (fgets(line, sizeof(line), fp))
	{
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
	printf(" 5. 오답노트\n");
	printf(" 6. 로그아웃\n");
	printf("+-----------------------------------------------------------+\n");
}

int subMenu(User *loginUser)
{
	if (strlen(loginUser -> id) == 0) 
	{
		printf(" 로그인된 정보가 없습니다.\n");
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
			printf(" 모의테스트 실행\n");
		}
		else if (subMenuNum == 2) // 실전테스트(필기)
		{
			printf(" 실전테스트 실행\n");
		}
		else if (subMenuNum == 3) // 실기시험 - 도로주행시험
		{
			printf(" 실기시험 실행\n");
		}
		else if (subMenuNum == 4) // 시험합격여부
		{
			printf(" 시험합격여부 실행\n");
		}
		else if (subMenuNum == 5) // 오답노트
		{
			wrongAnswerNote(loginUser);
		}
		else if (subMenuNum == 6) // 로그아웃
		{
			strcpy(loginUser -> name, "\0");
			strcpy(loginUser -> birth, "\0");
			strcpy(loginUser -> tel, "\0");
			strcpy(loginUser -> id, "\0");
			strcpy(loginUser -> pwd, "\0");
			printf(" 로그아웃 합니다.\n");

			break;
		}
		else
		{
			printf(" 잘못된 입력입니다. 다시 선택해주세요.\n");
		}
	}
}

void clearBuffer()
{
    while (getchar() != '\n');
}

void wrongAnswerNote(User *loginUser){
	Result temp;
	Result temp2;
	Result list[10];

	FILE * fp = fopen(FILE_WRONG, "rt");

	if (fp == NULL){
		printf("오답노트 파일이 없습니다.\n");
	}
	printf("+----------------------- 오 답 노 트 ------------------------+\n");
	printf("  회차  |        일시        |   제출자   \n");
	char line[STRING_SIZE];
	int i = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		strcpy(temp.question.questionNumber, strtok(line, ","));
		strcpy(temp.date, strtok(NULL, ","));
		strcpy(temp.userId, strtok(NULL, ","));
		strcpy(temp.useYn, strtok(NULL, "\n"));
		// printf("[t1.date : %s / t2.date : %s]", temp.date, temp2.date);
		// printf("[t1.userId : %s / t2.userId : %s]\n", temp.userId, temp2.userId);
		if (strcmp(temp.useYn, "Y") == 0
			&& (strcmp(temp.date, temp2.date) != 0 || strcmp(temp.userId, temp2.userId) != 0)
			&& strcmp(loginUser -> id, temp.userId) == 0){
			strcpy(temp2.date, temp.date);
			strcpy(temp2.userId, temp.userId);

			printf("%5d      ", i);
			printf("%s     ",temp.date); //일시
			printf("%s\n",temp.userId); //제출자

			strcpy(list[i].date, temp.date);
			strcpy(list[i].userId, temp.userId);

			i++;
		}
	}
	fclose(fp);

	printf("+----------------------------------------------------------+\n");
	printf("풀이하실 회차를 선택해주세요 : ");
	int userInput = fgetc(stdin) - '0';

	fp = fopen(FILE_WRONG, "rt");
	if (fp == NULL) {
		printf("오답노트 파일이 없습니다.\n");
	}
	Result wrong[40];
	i = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		strcpy(temp.question.questionNumber, strtok(line, ","));
		strcpy(temp.date, strtok(NULL, ","));
		strcpy(temp.userId, strtok(NULL, ","));
		strcpy(temp.useYn, strtok(NULL, "\n"));
		if (strcmp(temp.useYn, "Y") == 0
			&& strcmp(list[userInput - 1].date, temp.date) == 0
			&& strcmp(list[userInput - 1].userId, temp.userId) == 0) {
			strcpy(wrong[i].question.questionNumber, temp.question.questionNumber);
			strcpy(wrong[i].date, temp.date);
			strcpy(wrong[i].userId, temp.userId);
			strcpy(wrong[i].useYn, temp.useYn);
			i++;
		}
	}
	fclose(fp);

	i = 0;
	while (wrong[i].question.questionNumber != NULL) {
		//문제불러오기
		selQuestion(wrong[i].question.questionNumber, &(wrong[i].question));

		char result[2] = "";
		printf("답 입력 : ");
		fscanf(stdin, "%s", result);
		char passYn = 'N';
		for (int i = 0; i < strlen(wrong[i].question.correct); i++) {
			if (wrong[i].question.correct[i] == result[0]) {
				passYn = 'Y';
				break;
			}
		}

		if (passYn == 'Y') {
			printf("정답!\n");
			printf("삭제하시겠습니까?\n");
			fgets(result, sizeof(result), stdin);
			strcpy(strstr(result, "\n"), "\0" );
			if (result == "Y" || result == "y") {
				towupper(result);
				strcpy(wrong[i].useYn, "N");
			}
		} else {
			printf("오답!\n");
		}
		i++;
	}


//	char userId[USER_SIZE];
//	char line[STRING_SIZE];
//	int episode;
//
//	Result wrong;
//	User user;
//
//	selQuestion(wrong.question.questionNumber, &wrong.question);
//	while (fgets(line, sizeof(line), fp))
//	{
//		strcpy(wrong.question.questionNumber,strtok(line, ","));
//		strcpy(wrong.date,strtok(line, ","));
//		strcpy(user.id,strtok(line, "\n"));
//
//		printf("+----------------------- 오 답 노 트 ------------------------+\n");
//		printf("  회차  |    일시    |   제출자   ");
//		if (strcmp(userId, user.id) == 0)
//		{
//			// 회차 증가?
//			printf("   %d         %s          %d", wrong.gubun, wrong.date, user.id);
//		}
//		printf("+-----------------------------------------------------------+\n");
//		printf("오답노트를 진행하실 회차를 선택해주세요.\n");
//		scanf("%d", &episode);
//	}
//	// 회차마다 오답노트 진행? 회차 선택시 출력?
//	if (1);
//	if (wrong.userInput == wrong.question.correct)
//	{
//		int choice;
//		printf("정답입니다! 이 문제를 오답노트에서 삭제하시겠습니까? '1' 삭제 / '0' 유지):");
//		scanf("%d", &choice);
//
//		if (choice == 0)
//		{
//			fprintf("%s\n 정답: %d\n 오답: %d\n", wrong.question.questionNumber, wrong.question.correct, wrong.userInput);
//		}
//	}
//	else
//	{
//		printf("틀렸습니다. 추가 학습이 필요합니다.\n");
//		fprintf(fp, "%s\n 정답: %d\n 오답: %d\n", wrong.question.questionNumber, wrong.question.correct, wrong.userInput);
//	}
//	fclose(fp);
}

// void realTestResult()
// {
// 	int count, totalScore = 0; // 구분, 점수
// 	char date, testResult; // 날짜, 합격여부
// 	char ch

// 	printf("+-------------------- 시 험 합 격 여 부 ---------------------+\n");
// 	if (totalScore >= 70)
// {
// 	printf("합격");
// }
// 	while(1)
// 	{
// 		if ()
// 		{
// 			printf("  구분  |    날짜    |   점수   |  합격여부  ");
// 			printf("   %d         %s          %d          %s", count, date, testTime, totalScore, testResult);
// 		}
// 	}
// }

// void backMenu()
// {
// 	printf("메뉴로 돌아가시겠습니까? '0', 계속 진행하시겠습니까? '1'");
// 	scanf("%d", &backNum);
	
// 	if (backMenu == 0)
// 	{
// 		break;
// 	}
// }

// 오답노트 시작
// 모의테스트에서 틀린 문제 파일 출력
// 오답노트 내용을 표로 출력 ex) 회차, 날짜, 시간, 점수
// 회차 선택시 오답노트 복습 진행
// 오답노트
// 로그인한 ID와 파일 내 ID가 동일한 경우


void selQuestion(char num[], Question *qst) {
    char line[1000];
    Question question = {.questionNumber = "", .correct = ""};
    int i = 0;

    FILE *fp = fopen(FILE_DATA, "rt");
    while(fgets(line, sizeof(line), fp)) {

        if (strcmp(question.questionNumber, num) == 0) {
            break;
        }

        char token = 'F';
        for (int j = 0; j < strlen(line); j++) {
            if (line[j] == '"') {
                token = 'T';
                break;
            }
        }
        if (token == 'F'){
            strcpy(question.questionNumber,strtok(line, ".")); //문제번호

            if (strcmp(question.questionNumber, num) == 0){
                printf("%s. ", question.questionNumber);
                printf("%s\n", strtok(NULL, ","));//문제
                printf("%s\n", strtok(NULL, ","));//1문항
                printf("%s\n", strtok(NULL, ","));//2문항
                printf("%s\n", strtok(NULL, ","));//3문항
                printf("%s\n", strtok(NULL, ","));//4문항
                //printf("%s\n", strtok(NULL, "\n\r"));//답
            }
            strcpy(question.correct, strtok(line, "."));
            i++;
        } else {
            int cnt = 0;
            int gubun = 0;
            char splitYn = 'N';
            char buffer[500] = "";
            char temp[30] = "";

            while (1){
                splitYn = 'N';
                cnt = 0;
                if (gubun == 0) {
                    strcpy(temp, strtok(line, " "));
                }else if (gubun == 6) {
                    strcpy(temp, strtok(NULL, "\r"));
                } else {
                    strcpy(temp, strtok(NULL, " "));
                }

                strcat(buffer, temp);
                strcat(buffer, " ");

                for (int j = 0; j < strlen(buffer); j++) {
                    if (buffer[j] == '"') {
                        cnt++;
                    }
                }

                for (int j = 0; j < strlen(temp); j++) {
                    if (temp[j] == '\0') break;
                    if (temp[j] == '.' || (temp[j] == ',' && (cnt == 0 || cnt == 2)) || temp[j] == '\n') {
                        splitYn = 'Y';
                        break;
                    }
                }
                if (splitYn == 'Y') {
                    if (gubun == 0) {
                        strcpy(strstr(buffer, "."),"");
                        strcpy(question.questionNumber, buffer);
                        printf("%s. ", question.questionNumber);
                    }else if (gubun == 6) {
                        strcpy(question.correct, buffer);
                    }

                    gubun++;

                    if (gubun != 1 && gubun != 7 && strcmp(question.questionNumber, num) == 0) {
                        printf("%s\n", buffer);
                    }

                    strcpy(buffer, "");
                }

                if (gubun == 7) {
                    gubun = 0;
                    i++;
                    break;
                }
            }
        }

    }
    fclose(fp);

    strcpy(qst->questionNumber, question.questionNumber);
    strcpy(qst->correct, question.correct);

    printf("\n");
}