typedef struct{
    int col;
    int row;
} Position;

typedef struct{
    int questionNumber;
    char question[500];
    char answer1[200];
    char answer2[200];
    char answer3[200];
    char answer4[200];
    int correct;
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
    int leftLight;
    int rightLight;
    double kph;
    int score;
    int failYn;
    int startupYn;
    char direction;
    Position position;
    int failCnt;
    int failLog[10];
} Car;

typedef struct{
    Position position;
    int sign;
} Blinker;