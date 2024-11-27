#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>


//#include "../header/types.h"
//#define FILE_DATA "../dataFile/test.csv"
//#define FILE_LOGIN "../dataFile/UserLogin.txt" // 회원정보
//#define FILE_WRONG "../dataFile/WrongAnswerNote.txt" // 오답노트
//#define FILE_MAP "../dataFile/map.txt"

#include "/home/lms/CLionProjects/cteam/header/types.h"
#define FILE_DATA "/home/lms/CLionProjects/cteam/dataFile/test.txt"
#define FILE_LOGIN "/home/lms/CLionProjects/cteam/dataFile/UserLogin.txt" // 회원정보
#define FILE_MAP "/home/lms/CLionProjects/cteam/dataFile/map.txt"

#define STRING_SIZE 100 // 문자열 길이
#define USER_SIZE 20 // 문자 길이
#define QUESTIONS_NUM 40 // 문제수



#define COL 101
#define ROW 100

#define PER_SEC 1000
#define COUNTTIME 10

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

static struct termios orig_termios;

/** 자동입력 */
void echoOff();

/** 엔터 입력 */
void echoOn();

/** 버퍼 비우기 */
void clearBuffer();

/** 시간가져오기\n
  * 파라메터
  *  - 담을 문자열 변수 주소
  *  - 시간 형식(년, 월, 일, 시, 분)
  *    ex) "%d.%d.%d %d:%d" >> 24.11.27 14:39
  */
void getTime(char *, char [15]);

/** 모의고사 시험체크(0초부터 증가)\n
  * 파라메터
  *  - 시험 시작시각 정보
  */
int checkMockTime(time_t);

/** 필기시험 시험체크(2400초(40분)부터 감소)\n
  * 파라메터
  *  - 시험 시작시각 정보
  */
int checkWrittenTime(time_t);

/** 메인메뉴 불러오기 (로그인, 회원가입, 아이디 비번찾기 등) */
void mainMenu();

//메인메뉴
/** 서브메뉴 불러오기 (모의테스트, 오답노트, 실기시험 등)\n
  * 파라메터
  *  - 로그인된 사용자 정보
 */
int subMenu(User *);

/** 아이디/비밀번호 찾기 */
void userInfoFind();

/** 회원가입 */
void userInfoAdd();

/** 로그인\n
  * 파라메터
  *  - 로그인된 사용자 정보
  */
void userLogin(User *);

//서브메뉴
/** 모의테스트\n
  * 파라메터
  *  - 로그인된 사용자 정보
  */
void mockTest(User *);

/** 모의테스트 결과 출력\n
  * 파라메터
  *  - 시험 점수
  *  - 로그인된 사용자 정보
  *  - 시험 시작시각 정보
  */
void mockTestResult(double, User *, time_t);

/** 필기시험\n
  * 파라메터
  *  - 로그인된 사용자 정보
 */
void writtenTest(User *);

/** 실기시험\n
  * 파라메터
  *  - 로그인된 사용자 정보
  */
void practicalTest(User *);

/** 오답노트\n
  * 파라메터
  *  - 로그인된 사용자 정보
  */
void wrongAnswerNote(User *);

/** 시험 결과 출력\n
  * 파라메터
  *  - 로그인된 사용자 정보
  */
void testResult(User *);

// 필기문제 관련
/** 문제 불러오기\n
  * 파라메터
  *  - 불러올 문제의 번호
  *  - 문제 번호와 답이 담길 구조체의 주소값
  */
void selQuestion(char [], Question *);

/** 필기시험 합격/불합격시 파일에 내용 저장\n
  * 파라메터
  *  - 시험 결과
  *  - 로그인된 사용자 정보
  *  - 시험 시작 시간
  */
void printWrittenResultFile(double, User *, time_t);

// 실기시험 관련
/** 초기 맵 셋팅\n
  * 파라메터
  *  - 맵 정보를 담을 100 * 101 문자형 배열
  */
void setMap(char [ROW][COL]);

/** 초기 횡단보도, 신호등, 사람, 사용자가 아닌 다른 자동차 셋팅\n
  * 파라메터
  *  - 횡단보도와 신호등 정보가 담길 CrossWolk 배열
  *  - 사람 정보가 담길 PersonAndCar 배열
  *  - 다른 자동차 정보가 담길 PersonAndCar 배열
  */
void setObstacle(CrossWolk *, PersonAndCar *, PersonAndCar *);

/** 사용자 자동차 위치 기본셋팅\n
  * 파라메터
  *  - 맵 정보가 셋팅된 100 * 101 문자형 배열
  *  - 사용자 자동차 정보가 담길 구조체의 주소
  */
void setCarPosition(char [ROW][COL], Car *);

/** 사용자 자동차 움직임 동작\n
  * 파라메터
  *  - 맵 정보가 셋팅된 100 * 101 문자형 배열
  *  - 사용자 자동차 정보가 담긴 구조체의 주소
  */
void moveUserCar(char [ROW][COL], Car *);

/** 맵에 셋팅된 사람 움직임\n
  * 파라메터
  *  - 맵 정보가 셋팅된 100 * 101 문자형 배열,\n
  *  - 사람 정보가 담긴 PersonAndCar 배열
  */
void movePerson(char [ROW][COL], PersonAndCar *, int);

/** 맵에 셋팅된 다른 자동차 움직임\n
  * 파라메터
  *  - 맵 정보가 셋팅된 100 * 101 문자형 배열,\n
  *  - 다른 자동차 정보가 담긴 PersonAndCar 배열
  */
void moveCar(char [ROW][COL], PersonAndCar *, int);

/** 사용자 신호위반 확인\n
  * 파라메터
  *  - 횡단보도와 신호등 정보가 담긴 CrossWolk 배열,\n
  *  - 사용자 자동차 정보가 담긴 구조체의 주소
  */
void checkCrosswalk(CrossWolk *, Car *);

/** 매 턴 셋팅된 맵 출력\n
  * 파라메터
  *  - 맵 정보가 셋팅된 100 * 101 문자형 배열,\n
  *  - 다른 자동차 정보가 담긴 PersonAndCar 배열
  */
void printMap(char [ROW][COL], Car *);

/** 매 턴 사용자의 자동차 정보 출력\n
  * 파라메터
  *  - 사용자 자동차 정보가 담긴 구조체
  */
void printStatus(Car, char);

/** 실기시험 합격/불합격시 파일에 내용 저장\n
  * 파라메터
  *  - 사용자 자동차 정보가 담긴 구조체 주소\n
  *  - 로그인된 사용자 정보
  */
void printPracticalResultFile(Car *, User *);