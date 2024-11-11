#include <stdio.h>
#include "client.h"
#pragma once

void game(SOCKET client1, SOCKET client2) {
	BroadcastMessage("가위바위보 게임을 시작하겠습니다...\n",BUFSIZE);
	
}