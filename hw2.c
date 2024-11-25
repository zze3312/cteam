#include <stdio.h>
#include <string.h>

#include "types.h"
void selQuestion(char num[], Question *qst);
void main() {
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
    else printf("오답!\n");
}

void selQuestion(char num[], Question *qst) {
    char line[1000];
    Question question = {.questionNumber = "", .question = "", .answer1 = "", .answer2 = "", .answer3 = "", .answer4 = "", .correct = ""};
    int i = 0;

    FILE *fp = fopen("test.csv", "rt");
    while(fgets(line, sizeof(line), fp)) {

        char token = 'F';
        for (int j = 0; j < strlen(line); j++) {
            if (line[j] == '"') {
                token = 'T';
                break;
            }
        }
        if (token == 'F'){
            strcpy(question.questionNumber,strtok(line, "."));
            strcpy(question.question, strtok(NULL, ","));
            strcpy(question.answer1, strtok(NULL, ","));
            strcpy(question.answer2, strtok(NULL, ","));
            strcpy(question.answer3, strtok(NULL, ","));
            strcpy(question.answer4, strtok(NULL, ","));
            strcpy(question.correct, strtok(NULL, "\n\r"));
            i++;
        } else {
            int cnt = 0;
            int idx = 0;
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

                idx += strlen(temp) - 1;
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
                idx++;
                if (splitYn == 'Y') {

                    if (gubun == 0) {
                        gubun++;
                        strcpy(strstr(buffer, "."),"");
                        strcpy(question.questionNumber, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 1) {
                        gubun++;
                        strcpy(question.question, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 2) {
                        gubun++;
                        strcpy(question.answer1, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 3) {
                        gubun++;
                        strcpy(question.answer2, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 4) {
                        gubun++;
                        strcpy(question.answer3, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 5) {
                        gubun++;
                        strcpy(question.answer4, buffer);
                        strcpy(buffer, "");
                    }else if (gubun == 6) {
                        gubun++;
                        strcpy(question.correct, buffer);
                        strcpy(buffer, "");
                        idx = 0;
                    }
                }

                if (cnt == 2) {
                    cnt = 0;
                }

                if (gubun == 7) {
                    gubun = 0;
                    i++;
                    break;
                }
            }
        }
        if (strcmp(question.questionNumber, num) == 0) {
            break;
        }
    }
    fclose(fp);

    strcpy(qst->questionNumber, question.questionNumber);
    strcpy(qst->question, question.question);
    strcpy(qst->answer1, question.answer1);
    strcpy(qst->answer2, question.answer2);
    strcpy(qst->answer3, question.answer3);
    strcpy(qst->answer4, question.answer4);
    strcpy(qst->correct, question.correct);

    printf("%s :", qst -> questionNumber);
    printf("%s\n", qst -> question);
    printf("%s\n", qst -> answer1);
    printf("%s\n", qst -> answer2);
    printf("%s\n", qst -> answer3);
    printf("%s\n", qst -> answer4);
    printf("답 : %s\n", qst -> correct);
    printf("\n");
}