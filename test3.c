#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "header/types.h"
#define STRING_SIZE 100 // 문자열 길이

typedef struct {
    char gubun[10];
    char date[20];
    char totalScore[10];
    char result[10];
} TestResult;

void testResult(User * loginUser);

int main(void)
{
    User loginInfo = {
        .id = "jh123",
        .name = "김지현",
    };

    testResult(&loginInfo);
}

void testResult(User * loginUser)
{
    char line[STRING_SIZE];
    char filePath[100] = "";
    TestResult tr;

    strcat(filePath, "/home/lms/source/cteam/dataFile/");
    strcat(filePath, loginUser -> id);
    strcat(filePath, "/TestResult.txt");
    
    printf("%s\n", filePath);

    FILE * fp = fopen(filePath, "rt");
    if (fp == NULL)
    {
        printf("없음");
        return;
    }

    printf("현재 로그인된 아이디 : %s\n", loginUser->id);
	printf("+-------------------- 시 험 합 격 여 부 ---------------------+\n");
	printf("  구분  |      일시      |  점수  |  합격여부  \n");

    // fgets(line, sizeof(line), fp);
    // printf("%s", line);

    while (fgets(line, sizeof(line), fp))
    {
        strcpy(tr.gubun, strtok(line, ","));
        printf("  %s    ", tr.gubun);
        strcpy(tr.date, strtok(NULL, ","));
        printf("%14s", tr.date);
        strcpy(tr.totalScore, strtok(NULL, ","));
        printf("    %4s   ", tr.totalScore);
        strcpy(tr.result, strtok(NULL, "\n"));
        printf("   %s\n", tr.result);
    }
	printf("+------------------------------------------------------------+\n");
    fclose(fp);
}