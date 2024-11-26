#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <time.h>
#include<unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>

//#include "../header/types.h"
//#define FILE_DATA "../dataFile/test.csv"
//#define FILE_LOGIN "../dataFile/UserLogin.txt" // 회원정보
//#define FILE_WRONG "../dataFile/WrongAnswerNote.txt" // 오답노트
//#define FILE_MAP "../dataFile/map.txt"

#include "/home/lms/CLionProjects/cteam/header/types.h"
#define FILE_DATA "/home/lms/CLionProjects/cteam/dataFile/test.csv"
#define FILE_LOGIN "/home/lms/CLionProjects/cteam/dataFile/UserLogin.txt" // 회원정보
#define FILE_WRONG "/home/lms/CLionProjects/cteam/dataFile/WrongAnswerNote.txt" // 오답노트
#define FILE_MAP "/home/lms/CLionProjects/cteam/dataFile/map.txt"

#define STRING_SIZE 100 // 문자열 길이
#define USER_SIZE 20 // 문자 길이
#define QUESTIONS_NUM 40 // 문제수



#define COL 101
#define ROW 100

#define MAP_ICON1 "⬛"
#define MAP_ICON2 "🟫"
#define MAP_ICON3 "🟨"
#define MAP_ICON4 "🟦"
#define MAP_ICON5 "🟩"
#define USER_CAR_ICON "🚖"
#define CAR_LIGHT_ON_ICON "🟧"
#define CAR_LIGHT_OFF_ICON "⬛"
#define CROSSWALK_ICON "◽"
#define BLINKER_ICON_RED "🟥"
#define BLINKER_ICON_ORG "🟧"
#define BLINKER_ICON_GRN "🟩"
#define STOP_LINE_ICON "⬜"
#define PERSON_ICON "🙋"
#define CAR_ICON "🚗"
#define MAP_FINISH "🟪"

#define KEY_GO 'w'
#define KEY_BREAK 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_LEFT_LIGHT 'q'
#define KEY_RIGHT_LIGHT 'e'
#define KEY_CHANGE_LINE_LEFT 'z'
#define KEY_CHANGE_LINE_RIGHT 'c'
#define KEY_ACCEL 'x'
#define KEY_OFF 'b'

enum mapIcon{
    MAP_ICON1_NUM = '0',
    MAP_ICON2_NUM = '1',
    MAP_ICON3_NUM = '2',
    MAP_ICON4_NUM = '3',
    MAP_ICON5_NUM = '4',
    USER_CAR_ICON_NUM = '5',
    CAR_LIGHT_ON_NUM = '6',
    CAR_LIGHT_OFF_NUM = '7',
    CROSSWALK_NUM = '8',
    BLINKER_NUM = '9',
    STOP_LINE_NUM = 'S',
    PERSON_NUM = 'P',
    CAR_NUM = 'C',
    MAP_FINISH_NUM = 'F',
};

enum color {
    BLINKER_NUM_RED = 0,
    BLINKER_NUM_ORG = 1,
    BLINKER_NUM_GRN = 2,
};

enum onOff {
    OFF = 'N',
    ON = 'Y'
};

enum direction {
    WEST = 'W',
    EAST = 'E',
    SOUTH = 'S',
    NORTH = 'N'
};

enum failReason {
    FAIL_REASON_1 = 1, // 좌회전 깜빡이 없음
    FAIL_REASON_2 = 2, // 우회전 깜빡이 없음
    FAIL_REASON_3 = 3, // 좌측 차선변경 깜빡이 없음
    FAIL_REASON_4 = 4, // 우측 차선변경 깜빡이 없음
    FAIL_REASON_5 = 5, // 코스이탈
    FAIL_REASON_6 = 6, // 역주행
    FAIL_REASON_7 = 7, // 신호위반
    FALL_REASON_8 = 8, // 사고
    FALL_REASON_9 = 9, // 감속없이 시동 끄기
    FALL_REASON_10 = 10, // 중앙선 침범
};

void mainMenu(User *);
void userLogin(User *); // 로그인
void userInfoAdd(); // 회원가입
void userInfoFind(); // 가입내역조회
int subMenu(User *); // 메뉴 선택
void clearBuffer(); // 입력값 비우기
void selQuestion(char num[], Question *qst);

void setMap(char [ROW][COL]);
void setObstacle(CrossWolk *, PersonAndCar *, PersonAndCar *);
void setCarPosition(char [ROW][COL], Car *);

void moveUserCar(char [ROW][COL], Car *);
void movePerson(char [ROW][COL], PersonAndCar *, int);
void moveCar(char [ROW][COL], PersonAndCar *, int);

void checkCrosswalk(CrossWolk *, Car *);

void printMap(char [ROW][COL], Car *);
void printStatus(Car, char);
void printFailResult(Car *);
void printSuccResult(Car *);

void startTest();
void mockTest();