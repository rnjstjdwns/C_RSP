#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../Project33/common.h"
#include "message.h"
#define SERVERIP   "127.0.0.1"  // 서버 IP 주소 (필요에 따라 변경)
#define SERVERPORT 9000         // 서버 포트 번호


//사용자 1명당 스레드 1개씩 할당 받고 각각 프로그램이 동작되어야 함
//메시지 받는 것도 계속 대기하다가 메시지가 왔을 때 실행되게끔 함
//스레드 동시 실행

int main() {
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // 서버 주소 설정
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // 서버에 연결
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    printf("서버에 연결되었습니다.\n");

    // 서버로부터 메시지를 상시 수신하는 스레드 생성
    HANDLE hThread = CreateThread(NULL, 0, ReceiveMessages, (LPVOID)&sock, 0, NULL);
    if (hThread == NULL) {
        printf("스레드를 생성하지 못했습니다.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buf[BUFSIZE + 1];
    int len;
    // 서버로부터 메시지 수신
    //retval = recv(sock, buf, BUFSIZE, 0);
    //if (retval > 0) {
    //    buf[retval] = '\0';  // 널 문자로 종료
    //    printf("%s", buf);   // 메시지를 한 번에 출력
    //}

    while (1) {
        
        // 사용자로부터 메시지 입력
        printf("보낼 메시지: ");
        if (fgets(buf, BUFSIZE, stdin) == NULL)
            break;

        // 메시지 길이 조정 (개행 문자 제거)
        //len = strlen(buf);
        //if (buf[len - 1] == '\n')
        //    buf[len - 1] = '\0';
        len = strlen(buf);

        if (len == 0)
            continue;

        // 서버로 메시지 보내기
        retval = send(sock, buf, len, 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        //if (retval == SOCKET_ERROR) {
        //    err_display("recv()");
        //    break;
        //}
        else if (retval == 0) {
            printf("서버와의 연결이 종료되었습니다.\n");
            break;
        }

    }

    // 소켓 닫기
    closesocket(sock);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    // 윈속 종료
    WSACleanup();
    return 0;
}
