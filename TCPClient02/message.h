
#include "../Project33/common.h"
#define BUFSIZE    1024
#pragma once
// �����κ��� �޽����� ��� �����ϴ� ������ �Լ�
DWORD WINAPI ReceiveMessages(LPVOID arg) {
    SOCKET sock = *(SOCKET*)arg;
    char buf[BUFSIZE + 1];
    int retval;

    while (1) {
        // �޽��� ����
        retval = recv(sock, buf, BUFSIZE, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("�������� ������ ����Ǿ����ϴ�.\n");
            break;
        }

        // ���� �޽��� ���
        buf[retval] = '\0';
        printf("%s\n", buf);
    }

    return 0;
}