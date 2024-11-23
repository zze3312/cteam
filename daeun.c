#include<stdio.h>
#include<stdlib.h>
#include"types.h"

#define file_name "LogInfo.txt"

void mainComment();
void userJoin(User * sptr);
void userLogin(int * userID, int * userPWD);

int main(void)
{
    mainComment();

    FILE * fp = fopen(file_name, "wt"); // 회원가입 정보 저장
    User person1;
    printf("회원가입을 위해 아래의 정보를 입력해주세요.\n");
    fputs("1.이름 : ", stdout); fgets(person1.name, sizeof(person1.name), stdin);
    fputs("2.생년월일(yymmdd) : ", stdout); fgets(person1.birth, sizeof(person1.birth), stdin);
    fputs("3.휴대폰번호 : ", stdout); fgets(person1.tel, sizeof(person1.tel), stdin);
    fputs("4.ID : ", stdout); fgets(person1.id, sizeof(person1.id), stdin);
    fputs("5.Password : ", stdout); fgets(person1.pwd, sizeof(person1.pwd), stdin);
    fprintf(fp, "%s %s %s %s %s", person1.name, person1.birth, person1.tel, person1.id, person1.pwd);
    fclose(fp);

    FILE * fp = fopen("LogInfo.txt", "rt");

    return 0;
}

void mainComment() // 메인 COMMENT 출력 
{
    printf("+-------------------------------------------------+ \n");
    printf("|            브고트 운전면허 프로그램             |\n");
    printf("+-------------------------------------------------+ \n");
}

// void userMain() // 로그인 정보 소유 여부 확인 ('0'일경우 회원가입, '1'일경우 로그인)
// {
//     int num;
//     printf("회원이십니까? \n");
//     printf("회원이신 경우 '1'입력 : 로그인 \n");
//     printf("비회원이신 경우 '2'입력 : 회원가입 \n");
//     printf("로그인 정보를 잊어버리신 경우 '3'입력 : 가입내역 조회\n");
//     scanf("%d", &num);

//     switch(num)
//     {
//         case 1:
            
//             break;
//         case 2:

//             break;
//         case 3:

//     }
// }

void userJoin(User * sptr) // 사용자 입력 요청 - 회원가입: 이름 name, 생년월일 birth, 전화번호 tel, 아이디 id, 비번 pwd
{
    printf("학생 이름 : %s \n", sptr -> name);
    printf("생년월일 : %s \n", sptr -> birth);
    printf("전화번호 : %s \n", sptr -> tel);
    printf("ID : %s \n", sptr -> id);
    printf("Password : %s \n", sptr -> pwd);
}

void userCheck(User * sptr, int * userID) // 가입내역조회
{
    if (sptr -> id == userID)
    {
        User person1;
        char nameCheck, birthCheck;
        printf("이름과 생년월일을 입력해주세요: \n");
        fputs("1.이름 : ", stdout); fgets(nameCheck, sizeof(nameCheck), stdin);
        fputs("2.생년월일(yymmdd) : ", stdout); fgets(birthCheck, sizeof(birthCheck), stdin);
        if (person1.name == nameCheck && person1.birth == birthCheck) 
        {
            // logInfor.txt 내 동일정보 확인
            FILE * fp = fopen("logInfor.txt", "rt");
            
            printf("가입한 내역이 있습니다.");    
        }
        else 
        {
            printf("가입한 정보가 없습니다. 회원가입을 진행해주세요.");
        }

    }
}

// void userLogin(int * userID, int * userPWD) // 사용자 입력 요청 - 로그인 : 아이디 id, 비번 pwd
// {
//     printf("로그인하세요.\n");
//     fputs("ID : ", stdout); fgets(userID, sizeof(userID), stdin);
//     fputs("Password : ", stdout); fgets(userPWD, sizeof(userPWD), stdin);
// }

// void userCheck(User * sptr, int * userID, int * userPWD) // 로그인 정보 일치 여부 확인
// {
//     if (sptr -> id == userID && sptr -> pwd == userPWD)
//     {
//         printf("로그인 중입니다.\n");
//     }
//     else if (sptr -> id != userID && sptr -> pwd != userPWD)
//     {
//         printf("정보를 잘못 입력하셨습니다.");
//     }
// }



// 과거 테스트 진행내역 출력
// 모의테스트 문제풀이 진행내역(언제, 어떤 문제를 풀었고, 맞은 문제/틀린 문제, 선택한 답, 목록 내 문제 삭제여부)


// 테스트 진행 목록 내 문제 삭제 여부 확인 후 삭제 or 유지


// 실전 테스트 합격여부 출력 - 총 점수, 합격/불합격 결과
