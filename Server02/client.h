#include "../Project33/common.h"

//서버 포트와 최대 플레이어 수
#define SERVERPORT 9000
#define REMOTEIP   "255.255.255.255"
#define BUFSIZE    1014
#define MAX_PLAYERS 10
#pragma once

typedef struct {
	struct sockaddr_in addr; //플레이어 주소
	int player_value; //플레이어가 입력한 정수값
	char data[BUFSIZE];
} Player;
/*
Player players[MAX_PLAYERS];
int player_count = 0;
int game_started = 0; // 게임 시작 여부를 추적하는 변수
*/

// 클라이언트 소켓 배열
SOCKET client_sockets[MAX_PLAYERS];
int client_count = 0;

// 클라이언트 소켓 추가
void AddClientSocket(SOCKET client_sock) {
	if (client_count < MAX_PLAYERS) {
		client_sockets[client_count++] = client_sock;
	}
}

// 클라이언트 소켓 삭제
// 클라이언트에서 접속을 종료 했을 때 소켓 삭제 되어야 함
void RemoveClientSocket(SOCKET client_sock) {
	for (int i = 0; i < client_count; i++) {
		if (client_sockets[i] == client_sock) {
			// 소켓을 리스트에서 제거
			client_sockets[i] = client_sockets[client_count - 1];
			client_count--;
			break;
		}
	}
}
// 특정 클라이언트에 메시지를 전송하는 함수
int send_message_to_client(SOCKET client_sock, const char* message) {
	int retval;
	int len = strlen(message);

	// 메시지 전송
	retval = send(client_sock, message, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	//printf("클라이언트에게 메시지 전송: %s\n", message);
	return 0;
}

// 브로드캐스트 메시지 함수
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
//접속 한 사람 이 추가 될때 마다 클라이언트 수를 출력 시켜주는 스레드
DWORD WINAPI PrintClientCount(LPVOID arg) {
	return 0;
}
// 클라이언트와 데이터 통신

// LPVOID = void* : 모든 정보를 받을 수 있으나 원하는 정보로 형변환 해줘야 함
DWORD WINAPI ProcessClient(LPVOID arg)
{
	//return value
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
	//브로드 캐스트로 모든 클라이언트에 client_count 뿌려줌
	//buf에 client_count 추가하여 다시 저장
	sprintf(buf, "client count : %d", client_count);
	BroadcastMessage(buf, BUFSIZE);
	//게임 인트로 출력
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
		// 데이터 받기
		retval = recv(client_sock, buf, BUFSIZE, 0);
		//클라이언트가 어떤 이유로 연결이 종료 되었을 때
		if (retval == 0 ||retval == SOCKET_ERROR) {
			printf("Client disconnected.\n");
			RemoveClientSocket(client_sock);
			err_display("recv()");
			break;
		}
		if (client_count == 2) {
			game(client_sockets[0], client_sockets[1]);
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		printf("%s\n",buf);
		BroadcastMessage(buf, BUFSIZE);
		//BroadcastMessage("hello world", BUFSIZE);

	}

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
	return 0;
}