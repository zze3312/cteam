#include <stdio.h>
#include "../header/types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

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
#define FILE_MAP "../dataFile/map.txt"

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


void printResultFile(Car *, User *);








void getTime(char *date, char format[15]);

void main() {
    srand(time(NULL));
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;    // non-canonical input 설정
    term.c_lflag &= ~ECHO;      // 입력 시 터미널에 보이지 않게
    term.c_cc[VMIN] = 1;        // 최소 입력 버퍼 크기
    term.c_cc[VTIME] = 0;       //버퍼 비우는 시간 (timeout)
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    char map[ROW][COL] = {};
    char course = rand() % 4 + 'A';

    Car userCar = {OFF, OFF, 0.0, 100, OFF, OFF, 'W'};
    userCar.turn = 1;
    CrossWolk cws[4];
    PersonAndCar psn[4];
    PersonAndCar car[4];

    setMap(map);
    setObstacle(cws, psn, car);
    setCarPosition(map, &userCar);

    while (1) {
        system("clear");
        printMap(map, &userCar);
        printStatus(userCar, course);
        moveUserCar(map, &userCar);
        if ( userCar.inputKey == KEY_GO || userCar.inputKey == KEY_ACCEL || userCar.inputKey == KEY_BREAK ) {
            moveCar(map, car, userCar.turn);
            movePerson(map, psn, userCar.turn);
            for (int i = 0; i < sizeof(car) / sizeof(car[0]); i++) {
                if (car[i].beforeBlock == USER_CAR_ICON_NUM){
                    userCar.failYn = ON;
                    userCar.failLog[userCar.failCnt] = FALL_REASON_8;
                    userCar.failCnt++;
                }
            }
        }
        checkCrosswalk(cws, &userCar);
        if (userCar.score < 70 || userCar.failYn == ON) {
            userCar.failYn = ON;
            break;
        }
        if (userCar.beforeBlock == 'F'){
            break;
        }
    }

    if (userCar.failYn == ON) {
        system("clear");
        printFailResult(&userCar);
    }else {
        system("clear");
        printSuccResult(&userCar);
    }
    //printResultFile(&userCar, loginUser);


}

void setMap(char map[ROW][COL]) {
    FILE *fp = fopen(FILE_MAP, "rt");

    if (fp == NULL) {
        printf("파일에 문제가 있습니다.\n");
    }

    // '\n'문자 때문에 작성한 길이보다 조금더 불러와야됨
    fread(map, 1, 10100, fp);
    fclose(fp);

}

void setObstacle(CrossWolk *cws, PersonAndCar *psn, PersonAndCar *cars) {
    // 신호등과 횡단보도 좌표
    CrossWolk tempCws[4] = {
        {26,  43, 27, 43, 27, 44, 28, 43, 28, 44, 29, 43, 29, 44, 30, 43, 30, 44, 31, 43, 31, 44},
        {54, 55, 54, 56, 54, 57, 54, 58, 54, 59, 54, 60, 54, 61, 54, 62, 54, 63, 54, 64, 55, 56, 55, 57, 55, 58, 55, 59, 55, 60, 55, 61, 55, 62, 55, 63, 55, 64},
        {63, 90, 63, 91, 63, 92, 63, 93, 63, 94, 63, 95, 63, 96, 63, 97, 64, 91, 64, 92, 64, 93, 64, 94, 64, 95, 64, 96, 64, 97},
        {88, 25, 88, 26, 88, 27, 88, 28, 88, 29, 88, 30, 88, 31, 88, 32, 88, 33, 88, 34, 89, 26, 89, 27, 89, 28, 89, 29, 89, 30, 89, 31, 89, 32, 89, 33, 89, 34}
    };

    PersonAndCar tempPsn[4] = {
        {26, 44, SOUTH, PERSON_NUM, OFF, MAP_ICON2_NUM, OFF},
        {55, 55, EAST, PERSON_NUM, OFF, MAP_ICON2_NUM, OFF},
        {64, 90, EAST, PERSON_NUM, OFF, MAP_ICON2_NUM, OFF},
        {89, 25, EAST, PERSON_NUM, OFF, MAP_ICON2_NUM, OFF}
    };

    PersonAndCar tempCars[4] = {
        {17, 31, EAST, CAR_NUM, ON, MAP_ICON1_NUM, OFF},
        {28, 55, WEST, CAR_NUM, ON, MAP_ICON1_NUM, OFF},
        {40, 21, EAST, CAR_NUM, ON, MAP_ICON1_NUM, OFF},
        {45, 61, NORTH, CAR_NUM, ON, MAP_ICON1_NUM, OFF}
    };

    for (int i = 0; i < 4; i++) {
        cws[i] = tempCws[i];
        psn[i] = tempPsn[i];
        cars[i] = tempCars[i];
    }

}

void setCarPosition(char map[ROW][COL], Car *cptr) {
    cptr -> now.row = 28;
    cptr -> now.col = 58;

    cptr -> before.row = 28;
    cptr -> before.col = 58;

    cptr -> beforeBlock = MAP_ICON1_NUM;

    map[cptr -> now.row][cptr -> now.col] = USER_CAR_ICON_NUM;
}

void moveUserCar(char map[ROW][COL], Car *car) {
    char move;
    // 이동키 받기
    read(0, &move, sizeof(move));
    car -> inputKey = move;

    // 시동안걸려있으면 아무것도 동작안함
    if ((car -> startupYn) == ON) {
        // 이전위치 확인 -> 가속붙었을 때 신호위반이나 사고 확인용
        car -> before.row = car -> now.row;
        car -> before.col = car -> now.col;

        map[car -> now.row][car -> now.col] = car -> beforeBlock;
        if ( car -> inputKey == KEY_GO || car -> inputKey == KEY_ACCEL || car -> inputKey == KEY_BREAK ) {
            car -> turn ++; //차가 움직일 때만 턴수 증가

            if (car -> inputKey == KEY_ACCEL) {
                car -> kph += 20.0;
            }

            if (car -> inputKey == KEY_BREAK) {
                if (car -> kph > 0){
                    car -> kph -= 20.0;
                }
            }

            // 속도 20km/h당 1칸
            if (car -> direction == EAST) {
                (car -> now.col) += (int)((car -> kph) / 20.0);
            }else if (car -> direction == WEST) {
                (car -> now.col) -= (int)((car -> kph) / 20.0);
            }else if (car -> direction == SOUTH) {
                (car -> now.row) += (int)((car -> kph) / 20.0);
            }else if (car -> direction == NORTH) {
                (car -> now.row) -= (int)((car -> kph) / 20.0);
            }
        }else if (car -> inputKey == KEY_LEFT) {
            // 좌회전하는데 깜빡이를 깜빡...^^...
            if (car -> leftLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_1;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            // 아무튼 깜빡이들 끔
            car -> leftLight = OFF;
            car -> rightLight = OFF;

            // 방향 전환
            if (car -> direction == NORTH) {
                car -> direction = WEST;
            }else if (car -> direction == WEST) {
                car -> direction = SOUTH;
            }else if (car -> direction == SOUTH) {
                car -> direction = EAST;
            }else if (car -> direction == EAST) {
                car -> direction = NORTH;
            }

        }else if (car -> inputKey == KEY_RIGHT) {
            if (car -> rightLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_2;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                car -> direction = EAST;
            }else if (car -> direction == WEST) {
                car -> direction = NORTH;
            }else if (car -> direction == SOUTH) {
                car -> direction = WEST;
            }else if (car -> direction == EAST) {
                car -> direction = SOUTH;
            }
        } else if (car -> inputKey == KEY_LEFT_LIGHT) {
            // 왼깜빡 키면 오른깜빡 끄기
            if (car -> rightLight == ON) {
                car -> rightLight = OFF;
            }
            // 깜빡이가 켜져있으면 끄고 꺼져있으면 켜기
            if (car -> leftLight == ON) {
                car -> leftLight = OFF;
            }else {
                car -> leftLight = ON;
            }
        } else if (car -> inputKey == KEY_RIGHT_LIGHT) {
            if (car -> leftLight == ON) {
                car -> leftLight = OFF;
            }

            if (car -> rightLight == ON) {
                car -> rightLight = OFF;
            }else {
                car -> rightLight = ON;
            }
        } else if (car -> inputKey == KEY_CHANGE_LINE_LEFT) {
            // 차선변경 깜빡이를 깜빡쓰..
            if (car -> leftLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_3;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            // 여기도 아무튼 깜빡이를 꺼줌
            car -> leftLight = OFF;
            car -> rightLight = OFF;

            // 차가 바라보고 있는 방향별로 차선변경 동작
            if (car -> direction == NORTH) {
                (car -> now.col)--;
            }else if (car -> direction == WEST) {
                (car -> now.row)++;
            }else if (car -> direction == SOUTH) {
                (car -> now.col)++;
            }else if (car -> direction == EAST) {
                (car -> now.row)--;
            }
        } else if (car -> inputKey == KEY_CHANGE_LINE_RIGHT) {
            if (car -> rightLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_4;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                (car -> now.col)++;
            }else if (car -> direction == WEST) {
                (car -> now.row)--;
            }else if (car -> direction == SOUTH) {
                (car -> now.col)--;
            }else if (car -> direction == EAST) {
                (car -> now.row)++;
            }
        } else if (car -> inputKey == KEY_OFF) {
            if ( car -> kph != 0.0) {
                car -> failLog[car -> failCnt] = FALL_REASON_9;
                (car -> failCnt)++;
                car -> score -= 10;
            }
            car -> kph = 0.0;
            car -> startupYn = OFF;
        }

        // 장애물 부딪히면 실격
        if (map[car -> now.row][car -> now.col] == PERSON_NUM
            || map[car -> now.row][car -> now.col] == CAR_NUM
            || map[car -> now.row][car -> now.col] == MAP_ICON2_NUM
            || map[car -> now.row][car -> now.col] == MAP_ICON3_NUM) {
            if (map[car -> now.row][car -> now.col] == MAP_ICON3_NUM){
                car -> failLog[car -> failCnt] = FALL_REASON_10;

            }else {
                car -> failLog[car -> failCnt] = FALL_REASON_8;
            }
            car -> failYn = ON;
            (car -> failCnt)++;
            car -> score -= 50;
        }

        // 자동차 도착전 아이콘 저장
        car -> beforeBlock = map[car -> now.row][car -> now.col];
        // 변경된 위치값에 자동차 아이콘 넣어주기
        map[car -> now.row][car -> now.col] = USER_CAR_ICON_NUM;
    }
    // 시동이 꺼져있으면...
    else {
        // w키가 시동키
        if (car -> inputKey == KEY_GO ) {
            car -> startupYn = ON;
            car -> kph = 0.0;
        }
    }
}

void movePerson(char map[ROW][COL], PersonAndCar *psn, int turn) {
    for (int i = 0; i < 4; i++) {
        if (turn % 10 > 5 && psn[i].crossYn == ON) {
            continue;
        }else if (turn % 10 != 0){
            psn[i].crossYn = OFF;
        }
        map[psn[i].pos.row][psn[i].pos.col] = psn[i].beforeBlock;
        //횡단보도 아닌곳에서 움직임 > 횡단보도 앞에서 멈춤
        if (psn[i].moveYn == ON) {
            if (psn[i].direction == NORTH && map[psn[i].pos.row - 1][psn[i].pos.col] != CROSSWALK_NUM) {
                (psn[i].pos.row)--;
                if (map[psn[i].pos.row - 1][psn[i].pos.col] == CROSSWALK_NUM) {
                    psn[i].moveYn = OFF;
                }
            } else if (psn[i].direction == WEST && map[psn[i].pos.row][psn[i].pos.col - 1] != CROSSWALK_NUM) {
                (psn[i].pos.col)--;
                if (map[psn[i].pos.row][psn[i].pos.col - 1] == CROSSWALK_NUM) {
                    psn[i].moveYn = OFF;
                }
            } else if (psn[i].direction == SOUTH && map[psn[i].pos.row + 1][psn[i].pos.col] != CROSSWALK_NUM) {
                (psn[i].pos.row)++;
                if (map[psn[i].pos.row + 1][psn[i].pos.col] == CROSSWALK_NUM) {
                    psn[i].moveYn = OFF;
                }
            } else if (psn[i].direction == EAST && map[psn[i].pos.row][psn[i].pos.col + 1] != CROSSWALK_NUM) {
                (psn[i].pos.col)++;
                if (map[psn[i].pos.row][psn[i].pos.col + 1] == CROSSWALK_NUM) {
                    psn[i].moveYn = OFF;
                }
            }
        }
        // 자동차 신호 빨강/주황색일 때 움직임
        if (turn % 10 >= 5 && psn[i].crossYn == OFF){
            psn[i].moveYn = ON;
            if (psn[i].direction == NORTH) {
                psn[i].pos.row -= 2;
                if (map[psn[i].pos.row][psn[i].pos.col] != CROSSWALK_NUM) {
                    psn[i].direction = SOUTH;
                    psn[i].crossYn = ON;
                }
            }else if (psn[i].direction == EAST) {
                psn[i].pos.col += 2;
                if (map[psn[i].pos.row][psn[i].pos.col] != CROSSWALK_NUM) {
                    psn[i].direction = WEST;
                    psn[i].crossYn = ON;
                }
            }else if (psn[i].direction == SOUTH) {
                psn[i].pos.row += 2;
                if (map[psn[i].pos.row][psn[i].pos.col] != CROSSWALK_NUM) {
                    psn[i].direction = NORTH;
                    psn[i].crossYn = ON;
                }
            }else if (psn[i].direction == WEST) {
                psn[i].pos.col -= 2;
                if (map[psn[i].pos.row][psn[i].pos.col] != CROSSWALK_NUM) {
                    psn[i].direction = EAST;
                    psn[i].crossYn = ON;
                }
            }
        }
        psn[i].beforeBlock = map[psn[i].pos.row][psn[i].pos.col];
        map[psn[i].pos.row][psn[i].pos.col] = PERSON_NUM;
    }
}

void moveCar(char map[ROW][COL], PersonAndCar *car, int turn) {
    for (int i = 0; i < 4; i++) {
        map[car[i].pos.row][car[i].pos.col] = car[i].beforeBlock;

        if (turn % 10 > 4) {
            if (   (car[i].direction == NORTH && map[car[i].pos.row - 1][car[i].pos.col] == CROSSWALK_NUM)
                || (car[i].direction == EAST && map[car[i].pos.row][car[i].pos.col + 1] == CROSSWALK_NUM)
                || (car[i].direction == SOUTH && map[car[i].pos.row + 1][car[i].pos.col] == CROSSWALK_NUM)
                || (car[i].direction == WEST && map[car[i].pos.row][car[i].pos.col - 1] == CROSSWALK_NUM) ){
                car[i].moveYn = OFF;
            }
        }else {
            car[i].moveYn = ON;
        }

        if (car[i].moveYn == ON) {
            if (car[i].direction == NORTH) {
                if (map[car[i].pos.row][car[i].pos.col - 1] == MAP_ICON1_NUM && car[i].crossYn == OFF) {
                    car[i].pos.col -= 2;
                    car[i].direction = SOUTH;
                    car[i].crossYn = ON;
                }else {
                    (car[i].pos.row)--;
                    car[i].crossYn = OFF;
                }
            }else if (car[i].direction == EAST) {
                if (map[car[i].pos.row - 1][car[i].pos.col] == MAP_ICON1_NUM && car[i].crossYn == OFF) {
                    car[i].pos.row -= 2;
                    car[i].direction = WEST;
                    car[i].crossYn = ON;
                }else {
                    (car[i].pos.col)++;
                    car[i].crossYn = OFF;
                }
            }else if (car[i].direction == SOUTH) {
                if (map[car[i].pos.row][car[i].pos.col + 1] == MAP_ICON1_NUM && car[i].crossYn == OFF) {
                    car[i].pos.col += 2;
                    car[i].direction = NORTH;
                    car[i].crossYn = ON;
                }else {
                    (car[i].pos.row)++;
                    car[i].crossYn = OFF;
                }
            }else if (car[i].direction == WEST) {
                if (map[car[i].pos.row + 1][car[i].pos.col] == MAP_ICON1_NUM && car[i].crossYn == OFF) {
                    car[i].pos.row += 2;
                    car[i].direction = EAST;
                    car[i].crossYn = ON;
                }else {
                    (car[i].pos.col)--;
                    car[i].crossYn = OFF;
                }
            }
        }
        car[i].beforeBlock = map[car[i].pos.row][car[i].pos.col];
        map[car[i].pos.row][car[i].pos.col] = CAR_NUM;
    }
}

void checkCrosswalk(CrossWolk *cws, Car *car) {
    int sign = 0;
    int startRow, endRow;
    int startCol, endCol;

    if (car->turn % 5 == 0) {
        sign = BLINKER_NUM_ORG;
    }else if (car->turn % 10 < 5 && car->turn % 10 != 0) {
        sign = BLINKER_NUM_GRN;
    }else {
        sign = BLINKER_NUM_RED;
    }
    if (car -> before.row > car -> now.row) {
        startRow = car -> now.row;
        endRow = car -> before.row;
    }else {
        startRow = car -> before.row;
        endRow = car -> now.row;
    }
    if (car -> before.col > car -> now.col) {
        startCol = car -> now.col;
        endCol = car -> before.col;
    }else {
        startCol = car -> before.col;
        endCol = car -> now.col;
    }

    if (sign == BLINKER_NUM_RED){
        bool check = false;
        // 차가 이동한 거리안에
        for (int i = startRow; i <= endRow; i++) {
            for (int j = startCol; j <= endCol; j++) {
                // 횡단보도 블럭이 있다면
                for (int k = 0; k < 4; k++) {
                    for (int l = 0; l < 20; l++) {
                        if (cws[k].crossWolkBlock[l].row == i && cws[k].crossWolkBlock[l].col == j) {
                            //printf("crossWolkBlock : [%d, %d] / car : [%d, %d]", cws[k].crossWolkBlock[l].row, cws[k].crossWolkBlock[l].col, i, j);
                            check = true;
                        }
                    }
                }
            }
        }

        if (check) {
            car -> failLog[car -> failCnt] = FAIL_REASON_7;
            car -> failCnt++;
            car -> failYn = ON;
            car -> score -= 50;
        }
    }
}

void printMap(char map[ROW][COL], Car *car) {
    // 화면이 너무 커서 20 * 30, 차가 중심에 오도록 설정.. (맵끝에 붙으면 시야가 좁아짐)
    int col_min = car -> now.col - 15 <= 0 ? 0 : car -> now.col - 15;
    int col_max = car -> now.col + 15 >= 100 ? 100 : car -> now.col + 15;
    int row_min = car -> now.row - 10 <= 0 ? 0 : car -> now.row - 10;
    int row_max = car -> now.row + 10 >= 100 ? 100 : car -> now.row + 10;

    // for (int i = 0; i < ROW; i++) {
    //     for (int j = 0; j < COL; j++) {
    //         if (map[i][j] == 'C') {
    //             printf("%d, %d, ", i, j);
    //         }
    //     }
    // }

    // 배열에 들어간대로 아이콘 출력
    for (int i = row_min; i < row_max; i++) {
        for (int j = col_min; j < col_max; j++) {
            //printf("%c", '0');
            if (map[i][j] == MAP_ICON1_NUM) {
                printf("%5s", MAP_ICON1);
            } else if (map[i][j] == MAP_ICON2_NUM) {
                printf("%6s", MAP_ICON2);
            } else if (map[i][j] == MAP_ICON3_NUM) {
                printf("%6s", MAP_ICON3);
            } else if (map[i][j] == MAP_ICON4_NUM) {
                printf("%6s", MAP_ICON4);
            } else if (map[i][j] == MAP_ICON5_NUM) {
                printf("%6s", MAP_ICON5);
            } else if (map[i][j] == USER_CAR_ICON_NUM) {
                printf("%6s", USER_CAR_ICON);
            } else if (map[i][j] == CROSSWALK_NUM) {
                printf("%5s", CROSSWALK_ICON);
            } else if (map[i][j] == BLINKER_NUM) {
                // 신호등 : 초*4 주 빨*4 주 ~~ 이런순서로 바뀜 (10턴이 1사이클)
                if (car->turn % 5 == 0) {
                    printf("%6s", BLINKER_ICON_ORG);
                }else if (car->turn % 10 < 5 && car->turn % 10 != 0) {
                    printf("%6s", BLINKER_ICON_GRN);
                }else {
                    printf("%6s", BLINKER_ICON_RED);
                }
            } else if (map[i][j] == STOP_LINE_NUM){
                printf("%5s", STOP_LINE_ICON);
            } else if (map[i][j] == PERSON_NUM) {
                printf("%6s", PERSON_ICON);
            } else if (map[i][j] == CAR_NUM) {
                printf("%6s", CAR_ICON);
            } else if (map[i][j] == MAP_FINISH_NUM) {
                printf("%5s", MAP_FINISH);
            } else {
                printf("?");
            }
        }
        printf("\n");
    }
}

void printStatus(Car car, char course) {
    printf("===================================================================================================================================================================\n");
    printf("시험을 진행할 코스는 %c 코스입니다\n", course);
    printf("현재 턴 : [ %d ]\n", car.turn);
    printf("이전위치 : [ %d, %d ]\n", car.before.row, car.before.col);
    printf("현재위치 : [ %d, %d ]\n", car.now.row, car.now.col);
    printf("방향 : [ %c ]\n", car.direction);
    printf("시동 : [ %s ]\n", car.startupYn == ON ? "켜짐" : "꺼짐");
    printf("시속 : [ %.1f ] k/h \n", car.kph);
    printf("방향지시등 : [%5s %5s  ]\n", car.leftLight == ON ? CAR_LIGHT_ON_ICON : CAR_LIGHT_OFF_ICON,
           car.rightLight == ON ? CAR_LIGHT_ON_ICON : CAR_LIGHT_OFF_ICON);
    printf("===================================================================================================================================================================\n");
    printf("[ 감점로그 (현재점수 : %d) ] \n", car.score);
    for (int i = 0; i < sizeof(car.failLog) / sizeof(car.failLog[0]); i++) {
        if (car.failLog[i] == 0) { break; }
        printf("%d. ", i + 1);
        switch (car.failLog[i]) {
            case FAIL_REASON_1:
                printf("좌회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_2:
                printf("우회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_3:
                printf("좌측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_4:
                printf("우측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_5:
                printf("코스이탈 : 실격\n");
            break;
            case FAIL_REASON_6:
                printf("역주행 : 실격\n");
            break;
            case FAIL_REASON_7:
                printf("신호위반 : 실격\n");
            break;
            case FALL_REASON_8:
                printf("사고 : 실격\n");
            break;
            case FALL_REASON_9:
                printf("감속 없이 시동 끔 : -20\n");
            break;
            case FALL_REASON_10:
                printf("중앙선 침범 : 실격\n");
            break;
        }
    }
}

void printFailResult(Car *car) {
    printf("===================================================================================================================================================================\n");
    printf("이번실기 시험에서는 아쉽게 탈락하셨습니다.\n");
    printf("아래 감점사항을 확인하시고 다음 시험에서는 좋은성적을 받기를 바랍니다.\n");
    printf("===================================================================================================================================================================\n");
    printf("[ 감점로그 (현재점수 : %d) ] \n", car -> score);
    for (int i = 0; i < sizeof(car -> failLog) / sizeof(car -> failLog[0]); i++) {
        if (car -> failLog[i] == 0) break;
        printf("%d. ", i + 1);
        switch (car -> failLog[i]) {
            case FAIL_REASON_1:
                printf("좌회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_2:
                printf("우회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_3:
                printf("좌측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_4:
                printf("우측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_5:
                printf("코스이탈 : 실격\n");
            break;
            case FAIL_REASON_6:
                printf("역주행 : 실격\n");
            break;
            case FAIL_REASON_7:
                printf("신호위반 : 실격\n");
            break;
            case FALL_REASON_8:
                printf("사고 : 실격\n");
            break;
            case FALL_REASON_9:
                printf("감속 없이 시동 끔 : -20\n");
            break;
            case FALL_REASON_10:
                printf("중앙선 침범 : 실격\n");
            break;
        }
    }
    printf("===================================================================================================================================================================\n");
}

void printSuccResult(Car *car) {
    system("clear");
    printf("===================================================================================================================================================================\n");
    printf("축하합니다. 실기시험에 합격하셨습니다.\n");
    printf("아래 감점사항들은 앞으로 운전하는데 조심부탁드립니다.\n");
    printf("===================================================================================================================================================================\n");
    printf("[ 감점로그 (현재점수 : %d) ] \n", car -> score);
    for (int i = 0; i < sizeof(car -> failLog) / sizeof(car -> failLog[0]); i++) {
        if (car -> failLog[i] == 0) break;
        printf("%d. ", i + 1);
        switch (car -> failLog[i]) {
            case FAIL_REASON_1:
                printf("좌회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_2:
                printf("우회전 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_3:
                printf("좌측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_4:
                printf("우측 차선변경 깜빡이 없음 : -10\n");
            break;
            case FAIL_REASON_5:
                printf("코스이탈 : 실격\n");
            break;
            case FAIL_REASON_6:
                printf("역주행 : 실격\n");
            break;
            case FAIL_REASON_7:
                printf("신호위반 : 실격\n");
            break;
            case FALL_REASON_8:
                printf("사고 : 실격\n");
            break;
            case FALL_REASON_9:
                printf("감속 없이 시동 끔 : -20\n");
            break;
            case FALL_REASON_10:
                printf("중앙선 침범 : 실격\n");
            break;
        }
    }
    printf("===================================================================================================================================================================\n");

}

void startTest() {
    sleep(3);
}



void printResultFile(Car *car, User *loginUser){
    char filePath[100] = "";
    strcat(filePath, "/home/lms/CLionProjects/cteam/dataFile/");
    strcat(filePath, loginUser -> id);
    strcat(filePath, "TestResult.txt");

    FILE *fp = fopen(filePath, "at");

    if (fp == NULL) {
        printf("파일을 불러오는데 실패하였습니다.\n");
    }
    //실기,24.11.27 09:34,50,불합격
    char date[30];
    getTime(date, "%d.%d.%d %d:%d");
    fprintf(fp, "실기,%s,%d,%s", date, car -> score, car -> score >= 70 ? "합격" : "불합격");

    fclose(fp);
}

void getTime(char *date, char format[15]){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(date, format,  tm.tm_year - 100, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
}