void echoOff(){
  	struct termios new_termios;

    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~ICANON;    // non-canonical input 설정
    new_termios.c_lflag &= ~ECHO;      // 입력 시 터미널에 보이지 않게
    new_termios.c_cc[VMIN] = 1;        // 최소 입력 버퍼 크기
    new_termios.c_cc[VTIME] = 0;       //버퍼 비우는 시간 (timeout)
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    return;
}

void echoOn(){
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    return;
}

void clearBuffer()
{
    while (getchar() != '\n');
}

void getTime(char *date, char format[15]){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(date, format,  tm.tm_year - 100, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
}

void mainMenu(){
    int mainMenuNum;
    printf("+-----------------------------------------------------------+\n");
    printf("|                  브고트 운전면허 프로그램                 |\n");
    printf("+-----------------------------------------------------------+\n");
    printf(" 1. 로그인\n");
    printf(" 2. 회원가입\n");
    printf(" 3. 아이디/비밀번호 찾기\n");
    printf(" 4. 종료\n");
    printf("+-----------------------------------------------------------+\n");
    printf(" 진행하실 번호를 입력해주세요: ");
    scanf("%d", &mainMenuNum);

    switch(mainMenuNum)
    {
        case 1: // 로그인
            User loginUser = {"","","","",""};
        system("clear");
        userLogin(&loginUser);
        if (strlen(loginUser.id) != 0)
        {
            printf(" %s님의 접속을 환영합니다.\n", loginUser.id);
            sleep(1);
            system("clear");
            subMenu(&loginUser);
        }
        else
        {
            printf(" 가입되지 않은 정보입니다.\n");
        }
        break;
        case 2: // 회원가입
            userInfoAdd();
        break;
        case 3: // 가입내역조회
            userInfoFind();
        break;
        case 4: // 종료
            printf(" 브고트 운전면허 프로그램을 종료합니다.\n");
        exit(0); // 프로그램 종료
        default:
            printf(" 잘못된 입력입니다. 다시 선택해주세요.\n");
        break;
    }

}

int subMenu(User *loginUser)
{
    if (strlen(loginUser -> id) == 0)
    {
        printf(" 로그인된 정보가 없습니다.\n");
        sleep(1);
        return 0;
    }

    while(1)
    {
        system("clear");
        printf(" 현재 로그인된 아이디 : %s\n", loginUser -> id);
        printf("+--------------------- 사 용 자 메 뉴 ----------------------+\n");
        printf(" 1. 모의테스트\n");
        printf(" 2. 실전테스트(필기)\n");
        printf(" 3. 실기시험 - 도로주행시험\n");
        printf(" 4. 시험합격여부\n");
        printf(" 5. 오답노트\n");
        printf(" 6. 로그아웃\n");
        printf("+-----------------------------------------------------------+\n");
        printf(" 진행하실 번호를 입력해주세요: ");

        int subMenuNum;
        scanf("%d", &subMenuNum);
        clearBuffer();

        if (subMenuNum == 1) // 모의테스트
        {
            mockTest(loginUser);
        }
        else if (subMenuNum == 2) // 실전테스트(필기)
        {
            writtenTest(loginUser);
        }
        else if (subMenuNum == 3) // 실기시험 - 도로주행시험
        {
            practicalTest(loginUser);
            echoOn();
        }
        else if (subMenuNum == 4) // 시험합격여부
        {
            printf(" 시험합격여부 실행\n");
        }
        else if (subMenuNum == 5) // 오답노트
        {
            wrongAnswerNote(loginUser);
        }
        else if (subMenuNum == 6) // 로그아웃
        {
            strcpy(loginUser -> name, "\0");
            strcpy(loginUser -> birth, "\0");
            strcpy(loginUser -> tel, "\0");
            strcpy(loginUser -> id, "\0");
            strcpy(loginUser -> pwd, "\0");
            printf(" 로그아웃 합니다.\n");

            break;
        }
        else
        {
            printf(" 잘못된 입력입니다. 다시 선택해주세요.\n");
        }
    }
}

void userInfoFind()
{
	char userName[USER_SIZE];
	char userTel[USER_SIZE];
	char line[STRING_SIZE];
	User userInfo;
	int found = 0;

	clearBuffer();
	printf("+------------------- 아이디/비밀번호 찾기 --------------------+\n");
	printf(" 가입하신 이름과 휴대폰번호를 입력해주세요.\n");
	fputs(" 이름 : ", stdout); fgets(userName, sizeof(userName), stdin);
	strcpy(strstr(userName, "\n"),"\0");

	fputs(" 휴대폰번호 : ", stdout); fgets(userTel, sizeof(userTel), stdin);
	strcpy(strstr(userTel, "\n"),"\0");
	FILE * fp = fopen(FILE_LOGIN, "rt");
	if (fp == NULL)
	{
		printf(" 가입된 내역이 없습니다. 회원가입을 진행해주세요.\n");
		return;
	}

	while (fgets(line, sizeof(line), fp))
	{
		strcpy(userInfo.name,strtok(line, ","));
		strcpy(userInfo.birth, strtok(NULL, ","));
		strcpy(userInfo.tel, strtok(NULL, ","));
		strcpy(userInfo.id, strtok(NULL, ","));
		strcpy(userInfo.pwd, strtok(NULL, "\n"));

		if (strcmp(userInfo.name, userName) == 0 && strcmp(userInfo.tel, userTel) == 0)
		{
			printf("+------------------- 회원 가입 정보 --------------------+\n");
			printf(" 이름 : %s\n", userInfo.name);
			printf(" 생년월일 : %s\n", userInfo.birth);
			printf(" 휴대폰번호 : %s\n", userInfo.tel);
			printf(" ID : %s\n", userInfo.id);
			printf(" PWD : %s\n", userInfo.pwd);
			found = 1;
		}
	}
	if (!found)
	{
		printf(" 해당 정보로 가입된 사용자를 찾을 수 없습니다.\n");
	}
	fclose(fp);
}

void userInfoAdd()
{
	User userInput; // 구조체 호출
	User tempUser;
	char flag = 'Y';
	char line[STRING_SIZE];

	while(1)
	{
		char quit;
		clearBuffer();
		if (quit == 'q') // 잘못 선택했을 경우 메인 출력
		{
			break;
		}
		printf("+----------------------- 회 원 가 입 -----------------------+\n");
		printf(" 회원가입을 위해 아래의 정보를 입력해주세요.\n");
	    fputs(" 1.이름 : ", stdout); fgets(userInput.name, sizeof(userInput.name), stdin);
	    strcpy(strstr(userInput.name, "\n"),"\0");
	    fputs(" 2.생년월일(yymmdd) : ", stdout); fgets(userInput.birth, sizeof(userInput.birth), stdin);
		strcpy(strstr(userInput.birth, "\n"),"\0");
	    fputs(" 3.휴대폰번호(숫자만 입력해주세요) : ", stdout); fgets(userInput.tel, sizeof(userInput.tel), stdin);
		strcpy(strstr(userInput.tel, "\n"),"\0");
	    fputs(" 4.ID : ", stdout); fgets(userInput.id, sizeof(userInput.id), stdin);
		strcpy(strstr(userInput.id, "\n"),"\0");
	    fputs(" 5.Password : ", stdout); fgets(userInput.pwd, sizeof(userInput.pwd), stdin);
		strcpy(strstr(userInput.pwd, "\n"),"\0");

		FILE * fp = fopen(FILE_LOGIN, "rt");
		while (fgets(line, sizeof(line), fp))
		{
			strcpy(tempUser.name,strtok(line, ","));
			strcpy(tempUser.birth, strtok(NULL, ","));
			strcpy(tempUser.tel, strtok(NULL, ","));
			strcpy(tempUser.id, strtok(NULL, ","));
			strcpy(tempUser.pwd, strtok(NULL, "\n"));

			if (strcmp(tempUser.tel, userInput.tel) == 0 || strcmp(tempUser.id, userInput.id) == 0)
			{
				flag = 'N';
				break;
			}
		}
		fclose(fp);

		if (flag == 'N')
		{
			printf(" 이미 가입된 정보입니다.\n 다시 입력해 주세요.\n 뒤로 가시려면 q를 입력하세요 : ");
			scanf("%c",&quit);
		}
		else
		{
			fp = fopen(FILE_LOGIN, "at");
			if (fp == NULL)
			{
				printf(" 파일을 불러올 수 없습니다.\n");
				return;
			}
			fprintf(fp, "%s,%s,%s,%s,%s\n", userInput.name, userInput.birth, userInput.tel, userInput.id, userInput.pwd);
			printf(" %s님 회원가입이 완료되었습니다. 가입을 축하드립니다.\n", userInput.name);

            char filename[100] = "";
            strcat(filename, "/home/lms/CLionProjects/cteam/dataFile/");
            strcat(filename, userInput.id);
            mkdir(filename, 0777);

			fclose(fp);


			break;
		}
	}
}

void userLogin(User *loginUser) // 사용자 입력 요청 - 로그인 : 아이디 id, 비번 pw
{
    char userId[USER_SIZE];
    char userPwd[USER_SIZE];
    char line[STRING_SIZE];
    User tempUser;

    printf("+------------------------ 로 그 인 -------------------------+\n"); clearBuffer();
    fputs(" ID : ", stdout); fgets(userId, sizeof(userId), stdin);
    strcpy(strstr(userId, "\n"),"\0");
    fputs(" Password : ", stdout); fgets(userPwd, sizeof(userPwd), stdin);
    strcpy(strstr(userPwd, "\n"),"\0");

    FILE * fp = fopen(FILE_LOGIN, "rt");
    if (fp == NULL)
    {
        printf(" 회원정보 파일이 없습니다. 관리자에게 문의해주세요.\n");
    }
    while(fgets(line, sizeof(line), fp))
    {
        strcpy(tempUser.name, strtok(line, ",")); // 데이터 구분
        strcpy(tempUser.birth, strtok(NULL, ","));
        strcpy(tempUser.tel, strtok(NULL, ","));
        strcpy(tempUser.id, strtok(NULL, ","));
        strcpy(tempUser.pwd, strtok(NULL, "\n"));

        if (strcmp(tempUser.id, userId) == 0 && strcmp(tempUser.pwd, userPwd) == 0) // tempUser(작성한 id/pwd)와 userId(회원가입 id/pwd)가 동일할 경우
        {
            strcpy(loginUser -> name, tempUser.name);
            strcpy(loginUser -> birth, tempUser.birth);
            strcpy(loginUser -> tel, tempUser.tel);
            strcpy(loginUser -> id, tempUser.id);
            strcpy(loginUser -> pwd, tempUser.pwd);

            printf(" %s : 로그인에 성공하셨습니다\n", loginUser -> id);
            break;
        }
    }
    fclose(fp);
}

void mockTest(User *loginUser) {
    for (int i = 0; i < 40; i++){
        char selNum[5];
        int randomNum;
        while (1){
            randomNum = rand() % 1000 + 1;
            if(randomNum != 292 && randomNum != 569 && !(randomNum >=681 && randomNum <= 865)){
                break;
            }
        }

        sprintf(selNum, "%d", randomNum);

        Question qst;
        selQuestion(selNum, &qst);

        char result[2] = "";
        printf("답 입력(q 입력 시 종료) : ");
        fscanf(stdin, "%s", result);

        if(result[0] == 'q'){
            break;
        }
        char passYn = 'N';
        for (int i = 0; i < strlen(qst.correct); i++) {
            if (qst.correct[i] == result[0]) {
                passYn = 'Y';
                break;
            }
        }

        if (passYn == 'Y') printf("정답!\n");
        else {
            char date[30] = "";
            char fileName[100] = "";
            printf("오답!\n");
            getTime(date, "_%d%d%d_%d%d");

            strcat(fileName, "/home/lms/CLionProjects/cteam/dataFile/");
            strcat(fileName, loginUser -> id);
            strcat(fileName, "/");

            strcat(fileName, "WrongNote");
            strcat(fileName, date);
            strcat(fileName, ".txt");

            FILE *fp = fopen(fileName, "at");
            //문제번호,사용여부
            fprintf(fp, "%s,\n", qst.questionNumber);
            fclose(fp);
        }
    }
}

void writtenTest(User *loginUser) {

    Question questionList[40];
    Question userInputList[40];
    int randomNum = 0;

    for (int i = 0; i < 40; i++) {
        strcpy(questionList[i].questionNumber, "");
        strcpy(questionList[i].correct, "");
        strcpy(userInputList[i].correct, "");
    }

    for(int i = 0; i < 40; i++) {
        while (1) {
            randomNum = rand() % 1000 + 1;
            for (int j = 0; j < i; j++){
                if (questionList[i].questionNumber == questionList[j].questionNumber) {
                    i--;
                    continue;
                }
            }
            if(randomNum != 292 && randomNum != 569 && !(randomNum >=681 && randomNum <= 865)){
                break;
            }
        }
        sprintf(questionList[i].questionNumber, "%d", randomNum);
    }

    for (int i = 0; i < 40; i++) {
        system("clear");
        selQuestion(questionList[i].questionNumber, &questionList[i]);

        printf("답 입력 : ");
        fscanf(stdin, "%s", userInputList[i].correct);
    }

    double score = 0.0;

    for (int i = 0; i < 40; i++) {
        char passYn = 'N';
        for (int j = 0; j < strlen(questionList[i].correct); j++) {
            if (questionList[i].correct[j] == userInputList[i].correct[0]) {
                passYn = 'Y';
                break;
            }
        }
        if (passYn == 'Y') score += 2.5;
    }

    printWrittenResultFile(score, loginUser);
}

void practicalTest(User *loginUser) {

    echoOff();

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

    printPracticalResultFile(&userCar, loginUser);

    sleep(3);
}

void wrongAnswerNote(User *loginUser){
	Result list[10];
	char filePath[100] = "";
	char fileName[30] = "";
	strcat(filePath, "/home/lms/CLionProjects/cteam/dataFile/");
	strcat(filePath, loginUser -> id);
	strcat(filePath, "/");

	printf("+----------------------- 오 답 노 트 ------------------------+\n");
	printf("  회차  |       일시       |   제출자   \n");

	int i = 0;

	DIR *dp = opendir(filePath);
	struct dirent *dir;

	if(dp == NULL){
		fprintf(stderr,"directory open error\n");
		exit(-1);
	}

	while((dir = readdir(dp)) != NULL || i >= 10){
		if(dir->d_ino == 0 || dir->d_name[0] == '.' || dir->d_name[0] != 'W') continue;
		strcpy(list[i].fileName, dir->d_name);
		printf("%5d      ", i + 1);
		strcpy(fileName, strstr(dir->d_name, "2"));
		strcpy(strstr(fileName, "."), "\0");
		printf("%s       ", fileName);
		printf("%s\n",loginUser -> id); //제출자
		i++;
	}
	closedir(dp);

	printf("+----------------------------------------------------------+\n");
	printf("풀이하실 회차를 선택해주세요 : ");
	int userInput = fgetc(stdin) - '0';
	strcat(filePath, list[userInput - 1].fileName);
	FILE *fp = fopen(filePath, "rt");
	if (fp == NULL) {
		printf("오답노트 파일이 없습니다.\n");
	}
	Result wrong[40];

    for(i = 0; i < 40; i++){
        strcpy(wrong[i].fileName, "");
        strcpy(wrong[i].question.questionNumber, "");
        wrong[i].useYn = NULL;
        strcpy(wrong[i].question.correct, "");
    }
	i = 0;
	char line[STRING_SIZE];
	while (fgets(line, STRING_SIZE, fp) != NULL) {
		strcpy(wrong[i].question.questionNumber, strtok(line, ","));
		wrong[i].useYn = 'Y';
		i++;
	}
	fclose(fp);

	i = 0;
	while (strcmp(wrong[i].question.questionNumber, "") != 0) {
		//문제불러오기
		selQuestion(wrong[i].question.questionNumber, &(wrong[i].question));

		char result[2] = "";
		printf("답 입력 : ");
		fscanf(stdin, "%s", result);
		char passYn = 'N';
        printf("strlen(wrong[i].question.correct) : %d\n", strlen(wrong[i].question.correct));
		for (int j = 0; j < strlen(wrong[i].question.correct); j++) {
			if (wrong[i].question.correct[j] == result[0]) {
				passYn = 'Y';
				break;
			}
		}

		if (passYn == 'Y') {
			printf("정답!\n");
			printf("삭제하시겠습니까?\n");
			fscanf(stdin, "%s", result);

			if (toupper(result[0]) == 'Y') {
				wrong[i].useYn = 'N';
			}
		} else {
			printf("오답!\n");
		}
		i++;
	}

	FILE *fp2 = fopen(filePath, "wt");
	if (fp2 == NULL) {
		printf("잘못된 파일입니다.\n");
	}
	i = 0;
	while (strcmp(wrong[i].question.questionNumber, "")) {
		if (wrong[i].useYn == 'Y') {
			fprintf(fp2, "%s,\n", wrong[i].question.questionNumber);
		}
        i++;
	}
	fclose(fp2);
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
                strcpy(question.correct, strtok(NULL, "\n"));
            }
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
                    if ((temp[j] == '.' && gubun == 0) || (temp[j] == ',' && (cnt == 0 || cnt == 2)) || temp[j] == '\n') {
                        splitYn = 'Y';
                        break;
                    }
                }
                if (splitYn == 'Y') {
                    if (gubun == 0) {
                        strcpy(strstr(buffer, "."),"");
                        strcpy(question.questionNumber, buffer);
                        if(strcmp(question.questionNumber, num) == 0){
                            printf("%s. ", question.questionNumber);
                        }
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

void printWrittenResultFile(double score, User *loginUser) {
    char filePath[100] = "";
    strcat(filePath, "/home/lms/CLionProjects/cteam/dataFile/");
    strcat(filePath, loginUser -> id);
    strcat(filePath, "/TestResult.txt");

    FILE *fp = fopen(filePath, "at");

    if (fp == NULL) {
        printf("파일을 불러오는데 실패하였습니다.\n");
    }
    //실기,24.11.27 09:34,50,불합격
    char date[30];
    getTime(date, "%d.%d.%d %d:%d");
    fprintf(fp, "필기,%s,%.1f,%s\n", date, score, (int) score >= 70 ? "합격" : "불합격");

    fclose(fp);
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

void printPracticalResultFile(Car *car, User *loginUser){
    printf("===================================================================================================================================================================\n");
    if (car -> score < 70){
        printf("이번실기 시험에서는 아쉽게 탈락하셨습니다.\n");
        printf("아래 감점사항을 확인하시고 다음 시험에서는 좋은성적을 받기를 바랍니다.\n");
    }else{
        printf("축하합니다. 실기시험에 합격하셨습니다.\n");
        printf("아래 감점사항들은 앞으로 운전하는데 조심부탁드립니다.\n");
    }
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


    char filePath[100] = "";
    strcat(filePath, "/home/lms/CLionProjects/cteam/dataFile/");
    strcat(filePath, loginUser -> id);
    strcat(filePath, "/TestResult.txt");

    FILE *fp = fopen(filePath, "at");

    if (fp == NULL) {
        printf("파일을 불러오는데 실패하였습니다.\n");
    }
    //실기,24.11.27 09:34,50,불합격
    char date[30];
    getTime(date, "%d.%d.%d %d:%d");
    fprintf(fp, "실기,%s,%d,%s\n", date, car -> score, car -> score >= 70 ? "합격" : "불합격");

    fclose(fp);
}