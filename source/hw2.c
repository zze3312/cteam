#include <stdio.h>
#include <time.h>
#include <string.h>
#define TIME_LIMIT 2400.0


void timer(time_t start);
void timer2(time_t start);

int main()
{
    time_t start;
    time(&start);
    printf("제한시간 40분");

    timer2(start);
    return 0;
}

// void timer(time_t start)    // Timer
// {

//     char input[100];
//     time_t end; // 현재 시간 저장
//     double elapsed, remaining_time;

//     while (1)
//     {
//         printf("입력 : ");
//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = 0;

//         if (strcmp(input, "exit") == 0)
//         {
//             printf("종료\n");
//             break;
//         }

//         time(&end);

//         elapsed = difftime(end, start);

//         remaining_time = 0 + elapsed;

//         if (remaining_time <= 0)
//         {
//             printf("제한 시간 종료");
//             // 남은 문제 오답처리

//             break;
//         }

//         printf("남은 시간: %.f분 %.f초\n", remaining_time / 60, remaining_time);

//     }

// }

void timer2(time_t start)    // countdown
{

    char input[100];
    time_t end; // 현재 시간 저장
    double elapsed, remaining_time;

    while (1)
    {
        printf("입력 : ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
        {
            printf("종료\n");
            break;
        }

        time(&end);

        elapsed = difftime(end, start);

        remaining_time = TIME_LIMIT - elapsed;

        if (remaining_time <= 0)
        {
            printf("제한 시간 종료");
            // 남은 문제 오답처리

            break;
        }

        printf("남은 시간: %.f분 총 %.f초\n", remaining_time / 60, remaining_time);

    }

}
