#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define IPATH "input.txt"
#define OPATH "output.txt"


int main(int argc, char *argv[]){

    struct hostent *host;
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

    char *str1 = NULL;
    char *temp1 = strtok_r(ListBuffer, "\n", &str1);


    while (temp1 != NULL){
        host = gethostbyname(temp1);
        if (host == NULL){
            fprintf(OutputFile, "%s / %s\n",temp1, "FAIL");
        }
        else{
            fprintf(OutputFile, "%s / %s\n", temp1, inet_ntoa(*(struct in_addr *)host->h_addr_list[0]));
        }
        temp1 = strtok_r(NULL, "\n", &str1);
    }

    fclose(InputFile); //파일 포인터 닫기
    fclose(OutputFile); //파일 포인터 닫기
    free(ListBuffer); // 동적 메모리 해제


    ListBuffer = NULL;
}
//주의 텍스트파일 만들 때, 윈도우에서 만들면 파일 형식이 "CRLF"인데 리눅스에서 파일을 만들면 "LF"형식이다
// CRLF형식이면 프로그램 오류 뜨니까 리눅스에서 파일 만들기
// 첫번째 방법 배열에 쌓아뒀다가 다 하고 마지막에 파일에 쓰기
// 두번째 방법 처리 할때마다 파일에 쓰기 근데 이게 중간에 취소 됐을때를 생각하면 이게 나음