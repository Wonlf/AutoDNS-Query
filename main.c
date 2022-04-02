#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

static jmp_buf env;
struct hostent *host;

void sig_handler () {
    longjmp (env, 1); //알람이 울리면 setjmp가 호출된 곳으로 점프하면서 setjmp가 1을 반환 (2)
}

int dnslookup (const char* domain, FILE **fp, long tout) {
    int status = 0;
    signal (SIGALRM, sig_handler); //알람이 울리면 sig_handler를 호출한다. (1)

    if (0 == setjmp(env)) {  //1과 0으로 시간을 지났는지 판단하고 있다가 응답이 오면 해당 도메인에 대한 IP입력 (3) END
        alarm (tout);
        host = gethostbyname (domain);
        if (host != NULL){
            status = 1;  //시간이 지나면 status에 0은 그대로 유지 되어 아래 if구문을 타게되고 (4)
            fprintf(*fp, "%s / %s\n", domain, inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
        }
        signal (SIGALRM, SIG_DFL);
        alarm (0);
    } else { //혹시나 예외처리
        host = NULL;
        fprintf(*fp, "%s / %s\n",domain, "FAIL");
    }

    if(status == 0){
        fprintf(*fp, "%s / %s\n",domain, "FAIL"); //FAIL입력 (5)
    }

    return status;
}

int main(int argc, char *argv[]){
#define IPATH argv[1]
#define OPATH argv[2]

    if (argc < 5) { //argv의 개수가 맞지 필요한 만큼 없으면 오류가 뜨기 때문에 예외 처리
        printf("옵션 개수를 정확하게 맞춰주세요.\n");
        exit(0);
    }

    long argv3ToNum;
    long argv4ToNum;

    argv3ToNum = strtol(argv[3], NULL, 10); //이 부분이 중요한데, argv로 받으면 문자열로 받게 된다.
    argv4ToNum = strtol(argv[4], NULL, 10); //나는 길이를 입력 받기 때문에 정수와 비교해야 하는데,
    //문자를 long형태로 바꿔주는 구문이다.

    struct timeval  tv; //시간 구조체
    double begin, end;
    gettimeofday(&tv, NULL);
    begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; //프로그램 시작시간 입력


    char *ListBuffer;
    int ListSize;

    FILE *InputFile = fopen(IPATH, "r");
    FILE *OutputFile = fopen(OPATH, "w");

    fseek(InputFile, 0, SEEK_END);
    ListSize = ftell(InputFile); //길이를 재고

    ListBuffer = malloc(ListSize); //잰 길이만큼 동적할당
    memset(ListBuffer, 0, ListSize); //할당한 메모리 초기화

    fseek(InputFile, 0, SEEK_SET);
    fread(ListBuffer, ListSize, 1, InputFile); //읽어와서 동적할당한 배열에 넣기

    int count = 0;
    char dest[ListSize];
    int before_i = 0;
    int first_line = 0;

    char *str1;
    char *temp1;

    char *str2;
    char *temp2;

    char *str3;
    char *temp3;

    for (int i = 0; i <= ListSize; i++) { //여기부터는 직접 배열의 index를 넣어보면서 코드를 짠 것이라 설명하기 어렵다.
        if (ListBuffer[i] == '\n') {
            count++;
            if(count == argv3ToNum) { //첫번째 예외 처리
                if(first_line == 0){
                    first_line ++;
                    strncpy(dest, ListBuffer, i);
                    dest[i] = '\0';
                    str1 = NULL;
                    temp1 = strtok_r(dest, "\n", &str1);
                    while (temp1 != NULL){
                        dnslookup(temp1, &OutputFile, argv4ToNum);
                        temp1 = strtok_r(NULL, "\n", &str1);
                    }
                    before_i = i;
                    count = 0;
                }
                else {
                    strncpy(dest, ListBuffer + before_i + 1, (i + 1) - (before_i + 2));
                    dest[(i + 1) - (before_i + 2)] = '\0';
                    str2 = NULL;
                    temp2 = strtok_r(dest, "\n", &str2);
                    while (temp2 != NULL){
                        dnslookup(temp2, &OutputFile, argv4ToNum);
                        temp2 = strtok_r(NULL, "\n", &str2);
                    }
                    before_i = i;
                    count = 0;
                }
            }
        }
        else if(ListBuffer[i] == '\0'){ //마지막 예외 처리
            strncpy(dest, ListBuffer + before_i + 1, i - before_i + 1);
            dest[i - before_i + 1] = '\0';
            str3 = NULL;
            temp3 = strtok_r(dest, "\n", &str3);
            while (temp3 != NULL){
                dnslookup(temp3, &OutputFile, argv4ToNum);
                temp3 = strtok_r(NULL, "\n", &str3);
            }
        }
    }

    fclose(InputFile); //파일 포인터 닫기
    free(ListBuffer); // 동적 메모리 해제
    fclose(OutputFile); //파일 포인터 닫기


    ListBuffer = NULL;


    gettimeofday(&tv, NULL);
    end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; //프로그램 종료시간 대입


    printf("Execution time %f\n", (end - begin) / 1000); //(시작시간 - 종료시간) 출력
}