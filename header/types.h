typedef struct{
    int row;
    int col;
} Position;

typedef struct{
    char questionNumber[5];
    char correct[50];
} Question;

typedef struct{
    char gubun;
    int userInput;
    char useYn;
    char date[8];
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
    char failLog[10];
    int turn;
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