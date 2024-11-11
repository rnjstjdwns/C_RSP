#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../Project33/common.h"
#include "message.h"
#define SERVERIP   "127.0.0.1"  // ���� IP �ּ� (�ʿ信 ���� ����)
#define SERVERPORT 9000         // ���� ��Ʈ ��ȣ


//����� 1��� ������ 1���� �Ҵ� �ް� ���� ���α׷��� ���۵Ǿ�� ��
//�޽��� �޴� �͵� ��� ����ϴٰ� �޽����� ���� �� ����ǰԲ� ��
//������ ���� ����

int main() {
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // ���� �ּ� ����
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // ������ ����
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    printf("������ ����Ǿ����ϴ�.\n");

    // �����κ��� �޽����� ��� �����ϴ� ������ ����
    HANDLE hThread = CreateThread(NULL, 0, ReceiveMessages, (LPVOID)&sock, 0, NULL);
    if (hThread == NULL) {
        printf("�����带 �������� ���߽��ϴ�.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buf[BUFSIZE + 1];
    int len;
    // �����κ��� �޽��� ����
    //retval = recv(sock, buf, BUFSIZE, 0);
    //if (retval > 0) {
    //    buf[retval] = '\0';  // �� ���ڷ� ����
    //    printf("%s", buf);   // �޽����� �� ���� ���
    //}

    while (1) {
        
        // ����ڷκ��� �޽��� �Է�
        printf("���� �޽���: ");
        if (fgets(buf, BUFSIZE, stdin) == NULL)
            break;

        // �޽��� ���� ���� (���� ���� ����)
        //len = strlen(buf);
        //if (buf[len - 1] == '\n')
        //    buf[len - 1] = '\0';
        len = strlen(buf);

        if (len == 0)
            continue;

        // ������ �޽��� ������
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
            printf("�������� ������ ����Ǿ����ϴ�.\n");
            break;
        }

    }

    // ���� �ݱ�
    closesocket(sock);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    // ���� ����
    WSACleanup();
    return 0;
}
