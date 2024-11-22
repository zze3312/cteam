#include <stdio.h>
#include </home/lms/CLionProjects/cteam/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

enum mapIcon{
    MAP_ICON1_NUM = '0',
    MAP_ICON2_NUM = '1',
    MAP_ICON3_NUM = '2',
    MAP_ICON4_NUM = '3',
    MAP_ICON5_NUM = '4',
    CAR_ICON_NUM = '5',
    CAR_LIGHT_ON_NUM = '6',
    CAR_LIGHT_OFF_NUM = '7',
    CROSSWALK_NUM = '8',
    BLINKER_NUM = '9'
};

enum color {
    BLINKER_NUM_RED = 0,
    BLINKER_NUM_ORG = 1,
    BLINKER_NUM_GRN = 2,
};

enum onOff {
    OFF = 0,
    ON = 1
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
};

#define COL 101
#define ROW 100

#define MAP_ICON1 "⬛"
#define MAP_ICON2 "⬜"
#define MAP_ICON3 "🟨"
#define MAP_ICON4 "🟦"
#define MAP_ICON5 "🟩"
#define CAR_ICON "🚖"
#define CAR_LIGHT_ON_ICON "🟧"
#define CAR_LIGHT_OFF_ICON "⬛"
#define CROSSWALK_ICON "◽"
#define BLINKER_ICON_RED "🟥"
#define BLINKER_ICON_ORG "🟧"
#define BLINKER_ICON_GRN "🟩"

#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_LEFT_LIGHT 'q'
#define KEY_RIGHT_LIGHT 'e'
#define KEY_CHANGE_LINE_LEFT 'z'
#define KEY_CHANGE_LINE_RIGHT 'c'
#define KEY_ACCEL 'x'

void setMap(char [ROW][COL], char [ROW][COL]);
void printMap(char [ROW][COL], int, int);
void searchCarPosition(char [ROW][COL], Car *);
void printStatus(Car, char);
char carMove(char [ROW][COL], char [ROW][COL], Car *);

void main() {
    srand(time(NULL));
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;    // non-canonical input 설정
    term.c_lflag &= ~ECHO;      // 입력 시 터미널에 보이지 않게
    term.c_cc[VMIN] = 1;        // 최소 입력 버퍼 크기
    term.c_cc[VTIME] = 0;       //버퍼 비우는 시간 (timeout)
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    char origin_map[ROW][COL] = {};
    char map[ROW][COL] = {};
    char course = rand() % 4 + 'A';
    int map_num = 4; //시작할 맵 번호
    int count = 1;


    Car userCar = {OFF, OFF, 0.0, 100, OFF, OFF, 'W'};

    setMap(origin_map, map);
    searchCarPosition(map, &userCar);

    while (1) {
        count++;
        system("clear");
        printMap(map, map_num, count);
        printStatus(userCar, course);
        while (1) {
            carMove(origin_map, map, &userCar);
        }

    }


    printf("\n\n");
}

void setMap(char ori_map[ROW][COL], char map[ROW][COL]) {
    FILE *fp = fopen("map.txt", "rt");
    if (fp == NULL) {
        printf("파일에 문제가 있습니다.\n");
    }

    fread(ori_map, 1, 11000, fp);
    fclose(fp);

    fp = fopen("map.txt", "rt");
    if (fp == NULL) {
        printf("파일에 문제가 있습니다.\n");
    }

    fread(map, 1, 11000, fp);
    fclose(fp);

}

void printMap(char map[ROW][COL], int map_num, int count) {
    int map_col = 0, map_row = 0;

    if (map_num == 1) {
        map_col = (COL - 1) / 2;
        map_row = ROW / 2;
    } else if (map_num == 2) {
        map_col = COL - 1;
        map_row = ROW / 2;
    } else if (map_num == 3) {
        map_col = (COL - 1) / 2;
        map_row = ROW;
    } else if (map_num == 4) {
        map_col = COL - 1;
        map_row = ROW;
    }

    for (int i = map_row == ROW ? ROW / 2 : 0; i < map_row; i++) {
        for (int j = map_col == COL - 1 ? (COL - 1) / 2 : 0; j < map_col; j++) {
            //printf("%c", '0');
            if (map[i][j] == MAP_ICON1_NUM) {
                printf("%5s", MAP_ICON1);
            } else if (map[i][j] == MAP_ICON2_NUM) {
                printf("%5s", MAP_ICON2);
            } else if (map[i][j] == MAP_ICON3_NUM) {
                printf("%6s", MAP_ICON3);
            } else if (map[i][j] == MAP_ICON4_NUM) {
                printf("%6s", MAP_ICON4);
            } else if (map[i][j] == MAP_ICON5_NUM) {
                printf("%6s", MAP_ICON5);
            } else if (map[i][j] == CAR_ICON_NUM) {
                printf("%6s", CAR_ICON);
            } else if (map[i][j] == CROSSWALK_NUM) {
                printf("%5s", CROSSWALK_ICON);
            } else if (map[i][j] == BLINKER_NUM) {
                if (count % 6 == 1 || count % 6 == 2) {
                    printf("%6s", BLINKER_ICON_GRN);
                }else if (count % 3 == 0) {
                    printf("%6s", BLINKER_ICON_ORG);
                }else {
                    printf("%6s", BLINKER_ICON_GRN);
                }
            } else {
                printf("?");
            }
        }
        printf("\n");
    }
}

void searchCarPosition(char map[ROW][COL], Car *cptr) {
    cptr->position.row = 70;
    cptr->position.col = 82;
    map[cptr->position.row][cptr->position.col] = CAR_ICON_NUM;
}

void printStatus(Car car, char course) {
    printf("===================================================================================================================================================================\n");
    printf("시험을 진행할 코스는 %c 코스입니다\n", course);
    printf("현재위치 : [ %d, %d ]\n", car.position.row, car.position.col);
    printf("방향 : [ %c ]\n", car.direction);
    printf("시동 : [ %s ]\n", car.startupYn == ON ? "켜짐" : "꺼짐");
    printf("시속 : [ %.1f ] k/h \n", car.kph);
    printf("방향지시등 : [%5s %5s  ]\n", car.leftLight == ON ? CAR_LIGHT_ON_ICON : CAR_LIGHT_OFF_ICON,
           car.rightLight == ON ? CAR_LIGHT_ON_ICON : CAR_LIGHT_OFF_ICON);
}

char carMove(char ori_map[ROW][COL], char map[ROW][COL], Car *car) {
    char move;
    read(0, &move, sizeof(move));
    if ((car -> startupYn) == ON){
        map[car->position.row][car->position.col] = ori_map[car->position.row][car->position.col];
        if (move == KEY_UP || move == KEY_ACCEL || move == KEY_DOWN) {
            if (move == KEY_ACCEL) {
                car -> kph += 20.0;
            }

            if (move == KEY_DOWN) {
                car -> kph -= 20.0;
            }

            if (car -> direction == WEST) {
                car -> position.col = car -> position.col + (car -> kph / 20.0);
            }else if (car -> direction == EAST) {
                (car -> position.col) -=  car -> kph / 20.0;
            }else if (car -> direction == SOUTH) {
                (car -> position.row) += car -> kph / 20.0;
            }else if (car -> direction == NORTH) {
                (car -> position.row) -=  car -> kph / 20.0;
            }
        }else if (move == KEY_LEFT) {

            if (car -> leftLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_1;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                car -> direction = EAST;
            }else if (car -> direction == EAST) {
                car -> direction = SOUTH;
            }else if (car -> direction == SOUTH) {
                car -> direction = WEST;
            }else if (car -> direction == WEST) {
                car -> direction = NORTH;
            }

        }else if (move == KEY_RIGHT) {
            if (car -> rightLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_2;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                car -> direction = WEST;
            }else if (car -> direction == EAST) {
                car -> direction = NORTH;
            }else if (car -> direction == SOUTH) {
                car -> direction = EAST;
            }else if (car -> direction == WEST) {
                car -> direction = SOUTH;
            }
        } else if (move == KEY_LEFT_LIGHT) {
            if (car -> rightLight == ON) {
                car -> rightLight = OFF;
            }
            if (car -> leftLight == ON) {
                car -> leftLight = OFF;
            }else {
                car -> leftLight = ON;
            }
        } else if (move == KEY_RIGHT_LIGHT) {
            if (car -> leftLight == ON) {
                car -> leftLight = OFF;
            }

            if (car -> rightLight == ON) {
                car -> rightLight = OFF;
            }else {
                car -> rightLight = ON;
            }
        } else if (move == KEY_CHANGE_LINE_LEFT) {
            if (car -> leftLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_3;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                (car -> position.col)--;
            }else if (car -> direction == EAST) {
                (car -> position.row)++;
            }else if (car -> direction == SOUTH) {
                (car -> position.col)++;
            }else if (car -> direction == WEST) {
                (car -> position.row)--;
            }
        } else if (move == KEY_CHANGE_LINE_RIGHT) {
            if (car -> rightLight == OFF) {
                car -> failLog[car -> failCnt] = FAIL_REASON_4;
                (car -> failCnt)++;
                car -> score -= 10;
            }

            car -> leftLight = OFF;
            car -> rightLight = OFF;

            if (car -> direction == NORTH) {
                (car -> position.col)++;
            }else if (car -> direction == EAST) {
                (car -> position.row)--;
            }else if (car -> direction == SOUTH) {
                (car -> position.col)--;
            }else if (car -> direction == WEST) {
                (car -> position.row)++;
            }
        }
        map[car->position.row][car->position.col] = CAR_ICON_NUM;
    }

    if (move == KEY_UP && (car -> startupYn) == OFF) {
        car -> startupYn = ON;
        car -> kph = 20.0;
    }

    if (move == KEY_DOWN && (car -> kph) == 0 && (car -> startupYn) == ON) {
        car -> startupYn = OFF;
    }

    return move;
}
