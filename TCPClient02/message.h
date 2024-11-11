
#include "../Project33/common.h"
#define BUFSIZE    1024
#pragma once
// 서버로부터 메시지를 상시 수신하는 스레드 함수
DWORD WINAPI ReceiveMessages(LPVOID arg) {
    SOCKET sock = *(SOCKET*)arg;
    char buf[BUFSIZE + 1];
    int retval;

    while (1) {
        // 메시지 수신
        retval = recv(sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("서버와의 연결이 종료되었습니다.\n");
            break;
        }

        // 받은 메시지 출력
        buf[retval] = '\0';
        printf("%s\n", buf);
    }

    return 0;
}