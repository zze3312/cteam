#include "types.h"
#include <stdio.h>
int main(void)
{
    
    Question    list;
    Question    *ptr;
    
    list.question[500] = 0;
    FILE* fp;
    fp = fopen("list.txt", "r");
    if (fp == NULL)
    {
        printf("fall");
    }
    fclose(fp);
    
    return 0;


}

// 문제 출력 1000문제 중 40문항

// 한문제 풀고 한문제 정답 확인

// 실시간 총 점수 확인

// 틀린문제는 오답노트로 

// 문제 풀기 완료시 푼 시간과 점수 공개

// 모의테스트 결과 파일에 저장

// typedef struct{
//     int questionNumber;
//     char question[500];
//     char answer1[200];
//     char answer2[200];
//     char answer3[200];
//     char answer4[200];
//     int correct;
// } Question;

// typedef struct{
//     char gubun;
//     int userInput;
//     char useYn;
//     char date[8];
//     Question question;
// } Result;

// typedef struct{
//     char name[20];
//     char tel[20];
//     char id[20];
//     char pwd[20];
//     char birth[8];
//     Result userResult;
// } User;

// typedef struct{
//     char leftLight;
//     char rightLight;
//     double kph;
//     int score;
//     char failYn;
// } Car;