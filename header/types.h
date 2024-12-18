typedef struct{
    int row;
    int col;
} Position;

typedef struct{
    char questionNumber[5];
    char correct[50];
} Question;

typedef struct{
    char fileName[30];
    char useYn;
    Question question;
} Result;

typedef struct{
    char name[20];
    char tel[20];
    char id[20];
    char pwd[20];
    char birth[9];
    Result userResult;
} User;

typedef struct{
    char leftLight;
    char rightLight;
    double kph;
    int score;
    char failYn;
    char startupYn;
    char direction;
    Position now;
    Position before;
    char beforeBlock;
    int failCnt;
    char failLog[5];
    int turn;
    char inputKey;
} Car;


typedef struct {
    Position blinker;
    Position crossWolkBlock[20];
} CrossWolk;

typedef struct {
    Position pos;
    char direction;
    char gubun;
    char moveYn;
    char beforeBlock;
    char crossYn;
} PersonAndCar;

typedef struct {
    char gubun[10];
    char date[20];
    char totalScore[10];
    char result[10];
} TestResult;