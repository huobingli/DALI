#include "stdafx.h"
#include "SocketMessage.h"


CSocketMessage::CSocketMessage(CAnalyzeMessage *pAM, CCacheTable *recvCacheTable, CCacheTable *analCacheTable)
{
	WSADATA wsaData;
	/*加载库*/
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		cout << "加载库失败！" << endl;
		return;
	}

	m_argRecv = (_socket_parameters *)malloc(sizeof(_socket_parameters));
	memset(m_argRecv, 0, sizeof(_socket_parameters));

	m_AnalyzeMessage = new CAnalyzeMessage();

	//初始化st为广播socket
	//后面需要调整通过changeSocket()函数传入目标IP值
	InitSocket("192.168.0.255");

	//设置两个缓冲区
	m_argRecv->recvCacheTable = recvCacheTable;
	m_argRecv->analCacheTable = analCacheTable;

	//启动线程
	handleRecv = CreateThread(NULL, 0, Recv, m_argRecv, 0, NULL);
	handleAnal = CreateThread(NULL, 0, Anal, m_argRecv, 0, NULL);
}

CSocketMessage::CSocketMessage(CAnalyzeMessage *pAM, char *sendAddr)
{
	WSADATA wsaData;
	/*加载库*/
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		cout << "加载库失败！" << endl;
		return;
	}
	//初始化广播st
	/*m_argSockBroadCast = (_socket_parameters *)malloc(sizeof(_socket_parameters));
	m_argSockTarget = (_socket_parameters *)malloc(sizeof(_socket_parameters));

	memset(m_argSockBroadCast, 0, sizeof(_socket_parameters));
	memset(m_argSockTarget, 0, sizeof(_socket_parameters));
	m_argSockBroadCast->sockLength = sizeof(m_argSockBroadCast->serverSock);
	m_argSockTarget->sockLength = sizeof(m_argSockTarget->serverSock);

	//initBroadSocket("192.168.0.255");
	//initTargetSocket("192.168.0.104");
	*/

	m_argRecv = (_socket_parameters *)malloc(sizeof(_socket_parameters));
	memset(m_argRecv, 0, sizeof(_socket_parameters));

	m_AnalyzeMessage = new CAnalyzeMessage();

	InitSocket(sendAddr);
}


CSocketMessage::~CSocketMessage()
{
	delete m_AnalyzeMessage;
	free(m_argRecv);
}


void CSocketMessage::sendBuffer(char *buffer, int length) {
	//InitSocket("192.168.0.104");
	if (strlen(buffer) == 0) {
		//AfxMessageageBox("广播内容为空, 请重试！！！");
		return;
	}
	
	int i = sendto(m_argRecv->st, buffer, length, 0, (sockaddr*)&m_argRecv->clientSock, m_argRecv->sockLength);
}

void CSocketMessage::broadBuffer(char *buffer, int length) {
	char pbuffer[20] = "192.168.0.200";
	for (int j = 0; j < 5; j++){
		changeSocket(pbuffer);
		int i = sendto(m_argRecv->st, buffer, length, 0, (sockaddr*)&m_argRecv->clientSock, m_argRecv->sockLength);
		pbuffer[12] = pbuffer[12] + 1;
		//changeSocket(pbuffer);
	}
}

void CSocketMessage::InitSocket(char *sendAddr) {
	const int opt = true; 
	m_argRecv->st = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int nb = setsockopt(m_argRecv->st, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
	if (nb == -1) {
		//AfxMessageageBox("SOCKET错误");
	}
	if (m_argRecv->st == INVALID_SOCKET) {
		//AfxMessageageBox("SOCKET初始化失败");
		closesocket(m_argRecv->st);
		return;
	}

	m_argRecv->clientSock.sin_family = AF_INET;
	m_argRecv->clientSock.sin_port = htons(49157);
	m_argRecv->clientSock.sin_addr.s_addr = inet_addr(sendAddr);

	m_argRecv->serverSock.sin_family = AF_INET;
	m_argRecv->serverSock.sin_port = htons(10000);
	m_argRecv->serverSock.sin_addr.s_addr = htonl(INADDR_ANY);

	//m_argRecv->st = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	m_argRecv->sockLength = sizeof(m_argRecv->serverSock);
	m_argRecv->recvBufferLength = sizeof(m_argRecv->recvbuffer);

	//绑定服务端sockaddr和socket
	if (bind(m_argRecv->st, (struct sockaddr *)&(m_argRecv->serverSock), sizeof(m_argRecv->serverSock)) < 0) {
		perror("广播st绑定失败，程序退出");
		exit(1);
	}
	else {
		////AfxMessageageBox("广播st绑定成功");
	}
}


void CSocketMessage::changeSocket(char *ip) {
	m_argRecv->clientSock.sin_addr.s_addr = inet_addr(ip);
	//m_argRecv->clientSock.sin_port = htons(port);
}