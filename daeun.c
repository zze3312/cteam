#pragma warning(disable: 4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"types.h"

#define FILE_LOGIN "LogInfo.txt"
#define FILE_WRONG "WrongAnwser.txt"
#define STRING_SIZE 1001
#define CHAR_SIZE 20

void mainComment();
void loginComment();
void userLogin(User * sptr, int * userID, int * userPWD);
void userJoin(User * sptr);
void userInfoAdd(User * sptr);
void userInfor(User * userInfor);
void userInforFind(User * userInfor, int size);
void userPage(User * ptr);

int main(void)
{
    mainComment();
    loginComment();

    int menuNum1;
    while(1)
    {
        scanf("%d", &menuNum1);        
        switch(menuNum1) // 메인 메뉴 선택
        {
            case 1: // 1번 로그인
                userLogin();
                break;
            case 2: // 2번 회원가입
                userInfoAdd();
                break;
            case 3: // 3번 가입내역조회
                userInfor();
                userInforFind();
                break;
            default:
                break;
        }
    }

    userPage();
    int menuNum2;
    while(1)
    {
        scanf("%d", &menuNum2);
        switch(menuNum2) // 로그인 완료시 메뉴 선택
        {
            case 1: // 1번 모의 테스트

                break;
            case 2: // 2번 실전테스트(필기)

                break;
            case 3: // 3번 실기시험 - 도로주행시험
            
                break;
            case 4: // 4번 시험합격여부

                break;
            case 5: // 5번 지난테스트

                break;
            case 6: // 6번 오답노트

                break;
            default:
                break;
        }
    }
    return 0;
}

void mainComment() // main 화면 comment 출력
{
    printf("+-----------------------------------------------------------+ \n");
    printf("|                  브고트 운전면허 프로그램                 |\n");
    printf("+-----------------------------------------------------------+ \n");
}

void loginComment() // login 화면 comment 출력
{
    printf("회원이십니까? \n");
    printf("회원이신 경우 '1'입력 : 로그인 \n");
    printf("비회원이신 경우 '2'입력 : 회원가입 \n");
    printf("로그인 정보를 잊어버리신 경우 '3'입력 : 가입내역 조회\n");
}

void userLogin(User * sptr, int * userID, int * userPWD) // 사용자 입력 요청 - 로그인 : 아이디 id, 비번 pw
{
    printf("로그인하세요.\n");
    fputs("ID : ", stdout); fgets(userID, sizeof(userID), stdin);
    fputs("Password : ", stdout); fgets(userPWD, sizeof(userPWD), stdin);

    if (sptr -> id == userID && sptr -> pwd == userPWD)
    {
        printf("로그인 중입니다.\n");
    }
    else if (sptr -> id != userID && sptr -> pwd != userPWD)
    {
        printf("정보를 잘못 입력하셨습니다.");
        return -1;
    }
}

void userJoin(User * sptr) // 사용자 입력 요청 - 회원가입: 이름 name, 생년월일 birth, 전화번호 tel, 아이디 id, 비번 pwd
{
    printf("학생 이름 : %s \n", sptr -> name);
    printf("생년월일 : %s \n", sptr -> birth);
    printf("전화번호 : %s \n", sptr -> tel);
    printf("ID : %s \n", sptr -> id);
    printf("Password : %s \n", sptr -> pwd);
}

void userInfoAdd(User * sptr) // 회원가입 진행
{
    FILE * fp = fopen(FILE_LOGIN, "a+t"); 
    if(fp == NULL)    
    {
        puts("파일 열기에 실패하였습니다.");
        return -1;
    }

    User person1;
    printf("회원가입을 위해 아래의 정보를 입력해주세요.\n");
    fputs("1.이름 : ", stdout); fgets(person1.name, sizeof(person1.name), stdin);
    fputs("2.생년월일(yymmdd) : ", stdout); fgets(person1.birth, sizeof(person1.birth), stdin);
    fputs("3.휴대폰번호 : ", stdout); fgets(person1.tel, sizeof(person1.tel), stdin);
    fputs("4.ID : ", stdout); fgets(person1.id, sizeof(person1.id), stdin);
    fputs("5.Password : ", stdout); fgets(person1.pwd, sizeof(person1.pwd), stdin);
    fprintf(fp, "%s %s %s %s %s", person1.name, person1.birth, person1.tel, person1.id, person1.pwd);
    fclose(fp);
}

void userInfor(User * userInfor) // 가입여부조회
{
    User userInfor[CHAR_SIZE];
    FILE * fp = fopen(FILE_LOGIN, "rt");
    if (fp == NULL)
    {
        printf("가입된 내역이 없습니다. 회원가입을 진행해주세요.\n");
        return 0;
    }
    char buffer[STRING_SIZE], *token;
    int i, idx = 0;
    
    while(!feof(fp))
    {
        i = 0;
        fgets(buffer, STRING_SIZE, fp);
        token = strtok(buffer, " ");
        while(token != NULL)
        {
            if(i == 0)
            {
                strcpy(userInfor[idx].name, token);
            }
            else if(i == 1)
            {
                strcpy(userInfor[idx].id, token);
                strcpy(userInfor[idx].pwd, token);
            }
            i++;
            token = strtok(NULL, " ");
        }
        idx++;
    }
    for (int i = 0; i < idx; i++)
    {
        printf("%s %s %s \n", userInfor[i].name, userInfor[i].id, userInfor[i].pwd);
    }
    fclose(fp);
}

void userInforFind(User * userInfor, int size) // 가입내역조회 출력
{
    printf("이름을 입력해주세요: ");
    char find_name[CHAR_SIZE];
    scanf("%s %s", find_name);
    char * ptr;
    User * u;
    int line = 0;
    
    for (u = userInfor; u < userInfor + size; u++, line++)
    {
        ptr = strstr(u -> name, find_name);
        if (ptr != NULL)
        {
            printf("%s 님의 ID는 [ %s ]이며, 비밀번호는 [ %s ]입니다. \n", u -> name, u -> id, u -> pwd);
        }
        else
        {
            printf("가입된 내역이 없습니다. 회원가입을 진행해주세요.\n");
        }
    }
}

void userPage(User * ptr) // 로그인 완료시 메뉴 선택
{
    User person1;
    printf("%s님의 접속을 환영합니다.\n", person1.name);
    printf("'1'입력 : 모의테스트         '2'입력 : 실전테스트(필기) \n");
    printf("'3'입력 : 도로주행시험       '4'입력 : 시험합격여부\n");
    printf("'5'입력 : 지난테스트         '6'입력 : 오답노트");
}

// 필기/실기 시험합격여부 조회 (진행날짜, 점수, 합격여부)

// 지난테스트 선택시 (시험진행회차, 점수, 진행시간, 합격여부)

// 오답노트 선택시 지금까지 풀었던 문제 중 틀린 것들 출력
void WrongAnwser()
{
    FILE * fp = fopen(FILE_WRONG, "wt");
    
}

