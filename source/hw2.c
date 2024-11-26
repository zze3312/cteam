#include <stdio.h>
#include <string.h>
#include <time.h>

// #include "../header/types.h"
// #define FILE_DATA "../dataFile/test.csv"
// #define FILE_WRONG "../dataFile/WrongAnswerNote.txt" // 오답노트

#include "/home/lms/CLionProjects/cteam/header/types.h"
#define FILE_DATA "/home/lms/CLionProjects/cteam/dataFile/test.csv"
#define FILE_WRONG "/home/lms/CLionProjects/cteam/dataFile/WrongAnswerNote.txt" // 오답노트

void selQuestion(char num[], Question *qst);
void main() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char selNum[5];
    printf("출력할 문제번호를 작성해주세요 : ");
    scanf("%s", selNum);

    Question qst;
    selQuestion(selNum, &qst);

    char result[2] = "";
    printf("답 입력 : ");
    fscanf(stdin, "%s", result);
    char passYn = 'N';
    for (int i = 0; i < strlen(qst.correct); i++) {
        if (qst.correct[i] == result[0]) {
            passYn = 'Y';
            break;
        }
    }

    if (passYn == 'Y') printf("정답!\n");
    else {
        char buffer[15];
        printf("오답!\n");
        sprintf(buffer, "%d.%d.%d",  tm.tm_year - 100, tm.tm_mon + 1, tm.tm_mday);

        FILE *fp = fopen(FILE_WRONG, "wt");
        //23,2,4,jh123,24.11.26

        fprintf(fp, "%s,%s,%s,%s\n", qst.questionNumber, qst.correct, result, buffer);
    }
}

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