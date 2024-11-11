#include "../Project33/common.h"

//���� ��Ʈ�� �ִ� �÷��̾� ��
#define SERVERPORT 9000
#define REMOTEIP   "255.255.255.255"
#define BUFSIZE    1014
#define MAX_PLAYERS 10
#pragma once

typedef struct {
	struct sockaddr_in addr; //�÷��̾� �ּ�
	int player_value; //�÷��̾ �Է��� ������
	char data[BUFSIZE];
} Player;
/*
Player players[MAX_PLAYERS];
int player_count = 0;
int game_started = 0; // ���� ���� ���θ� �����ϴ� ����
*/

// Ŭ���̾�Ʈ ���� �迭
SOCKET client_sockets[MAX_PLAYERS];
int client_count = 0;

// Ŭ���̾�Ʈ ���� �߰�
void AddClientSocket(SOCKET client_sock) {
	if (client_count < MAX_PLAYERS) {
		client_sockets[client_count++] = client_sock;
	}
}

// Ŭ���̾�Ʈ ���� ����
// Ŭ���̾�Ʈ���� ������ ���� ���� �� ���� ���� �Ǿ�� ��
void RemoveClientSocket(SOCKET client_sock) {
	for (int i = 0; i < client_count; i++) {
		if (client_sockets[i] == client_sock) {
			// ������ ����Ʈ���� ����
			client_sockets[i] = client_sockets[client_count - 1];
			client_count--;
			break;
		}
	}
}
// Ư�� Ŭ���̾�Ʈ�� �޽����� �����ϴ� �Լ�
int send_message_to_client(SOCKET client_sock, const char* message) {
	int retval;
	int len = strlen(message);

	// �޽��� ����
	retval = send(client_sock, message, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	//printf("Ŭ���̾�Ʈ���� �޽��� ����: %s\n", message);
	return 0;
}

// ��ε�ĳ��Ʈ �޽��� �Լ�
void BroadcastMessage(const char* message, int length) {
	for (int i = 0; i < client_count; i++) {
		int retval = send(client_sockets[i], message, length, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			closesocket(client_sockets[i]);
			RemoveClientSocket(client_sockets[i]);
		}
	}
}
//���� �� ��� �� �߰� �ɶ� ���� Ŭ���̾�Ʈ ���� ��� �����ִ� ������
DWORD WINAPI PrintClientCount(LPVOID arg) {
	return 0;
}
// Ŭ���̾�Ʈ�� ������ ���

// LPVOID = void* : ��� ������ ���� �� ������ ���ϴ� ������ ����ȯ ����� ��
DWORD WINAPI ProcessClient(LPVOID arg)
{
	//return value
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	//��ε� ĳ��Ʈ�� ��� Ŭ���̾�Ʈ�� client_count �ѷ���
	//buf�� client_count �߰��Ͽ� �ٽ� ����
	sprintf(buf, "client count : %d", client_count);
	BroadcastMessage(buf, BUFSIZE);
	//���� ��Ʈ�� ���
	send_message_to_client(client_sock, "\n$$$$$$$\\   $$$$$$\\  $$$$$$$\\         $$$$$$\\   $$$$$$\\  $$\\      $$\\ /*$$$$$$$$\\ \n$$  __$$\\ $$  __$$\\ $$  __$$\\       $$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|\n$$ |  $$ |$$ /  \\__|$$ |  $$ |      $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      \n$$$$$$$  |$$ |      $$$$$$$  |      $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    \n$$  __$$< $$ |      $$  ____/       $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|  \n$$ |  $$ |$$ |  $$\\ $$ |            $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |     \n$$ |  $$ |\\$$$$$$  |$$ |            \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\ \n\\__|  \\__| \\______/ \\__|             \\______/ \\__|  \\__|\\__|     \\__|\\________*/|\n");


	//send_message_to_client(client_sock, "$$$$$$$\\   $$$$$$\\  $$$$$$$\\         $$$$$$\\   $$$$$$\\  $$\\      $$\\ /*$$$$$$$$\\ \n");
	//send_message_to_client(client_sock, "$$  __$$\\ $$  __$$\\ $$  __$$\\       $$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|\n");
	//send_message_to_client(client_sock, "$$ |  $$ |$$ /  \\__|$$ |  $$ |      $$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |      \n");
	//send_message_to_client(client_sock, "$$$$$$$  |$$ |      $$$$$$$  |      $$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\    \n");
	//send_message_to_client(client_sock, "$$  __$$< $$ |      $$  ____/       $$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|  \n");
	//send_message_to_client(client_sock, "$$ |  $$ |$$ |  $$\\ $$ |            $$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |     \n");
	//send_message_to_client(client_sock, "$$ |  $$ |\\$$$$$$  |$$ |            \\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\ \n");
	//send_message_to_client(client_sock, "\\__|  \\__| \\______/ \\__|             \\______/ \\__|  \\__|\\__|     \\__|\\________*/|\n");

	while (1) {
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		//Ŭ���̾�Ʈ�� � ������ ������ ���� �Ǿ��� ��
		if (retval == 0 ||retval == SOCKET_ERROR) {
			printf("Client disconnected.\n");
			RemoveClientSocket(client_sock);
			err_display("recv()");
			break;
		}
		if (client_count == 2) {
			game(client_sockets[0], client_sockets[1]);
		}

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		printf("%s\n",buf);
		BroadcastMessage(buf, BUFSIZE);
		//BroadcastMessage("hello world", BUFSIZE);

	}

	// ���� �ݱ�
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}