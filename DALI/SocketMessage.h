#pragma once
#include <iostream>
#include <stdarg.h>

#include "MessagePackage.h"
#include "AnalyzeMessage.h"
#include "CacheTable.h"

using namespace std;


struct CommunicateParam{
	uint8_t paramNum;
	uint8_t Command;
	char    command;
	char    commandType;
};

//socket参数加上消息分析对象
typedef struct _socket_parameters{
	SOCKET st;
	struct sockaddr_in serverSock;
	struct sockaddr_in clientSock;
	int sockLength;
	uint8_t sendbuffer[256];
	int sendBufferLength;
	uint8_t recvbuffer[256];
	int recvBufferLength;

	//处理类需要结构
	CCacheTable *recvCacheTable;
	CCacheTable *analCacheTable;
};

typedef struct _analyze_parameters{
	CAnalyzeMessage *analyzeMessage;
	int deviceArr[64];
	int groupArr[16];
};

typedef struct _thread_parameters{
	//SOCKET BroadcastSocket
	_socket_parameters *sp;
	_analyze_parameters *ap;
};



//BLL全局接收线程
static HANDLE handleRecv;
static HANDLE handleAnal;
//static HANDLE handleHeart;


//接收参数方法
static DWORD WINAPI Recv(LPVOID pM)
{
	
	struct _socket_parameters *arg = (struct _socket_parameters*)pM;
	unsigned long ul = 1;
	//int ret = ioctlsocket(arg->st, FIONBIO, (unsigned long *)&ul); //非阻塞
	int nb = setsockopt(arg->st, SOL_SOCKET, SO_BROADCAST, (char*)&ul, sizeof(ul));//设置套接字类型
	if (nb == -1) {
		//AfxMessageageBox("socket is error");
	}
	//绑定socket 

	while (1) {
		Sleep(500);
		memset(arg->recvbuffer, 0, sizeof(arg->recvbuffer));
		int i = recvfrom(arg->st,
			(char*)(arg->recvbuffer),
			arg->recvBufferLength,
			0,
			(struct sockaddr*)&arg->serverSock,
			&arg->sockLength
			);
		//arg->recvBufferLength = i;
		if (i > 0) {

			//crc校验确认
			if (arg->recvbuffer[5] == 0xFE) {
				char *clientIP = inet_ntoa(arg->clientSock.sin_addr);
				/*
				
				arg->clientSock.sin_addr.s_addr = inet_addr("192.168.0.255");
				int j = sendto(arg->st, (char*)arg->recvbuffer, i, 0, (sockaddr*)&arg->clientSock, arg->sockLength);
				*/
				char pbuffer[20] = "192.168.0.200";
				for (int j = 0; j < 5; j++){
					arg->clientSock.sin_addr.s_addr = inet_addr(pbuffer);
					sendto(arg->st, (char*)arg->recvbuffer, i, 0, (sockaddr*)&arg->clientSock, arg->sockLength);
					pbuffer[12] = pbuffer[12] + 1;
				}
				/*if (j > 0) {
					//AfxMessageageBox("0xFE接收并广播");
				}*/
				arg->clientSock.sin_addr.s_addr = inet_addr(clientIP);
				clientIP = NULL;
			}
			else {
				////AfxMessageageBox("Recv");
				cacheNode * tempCacheNode = new cacheNode();
				tempCacheNode->setIP(inet_ntoa(arg->serverSock.sin_addr), 16);
				//接收缓冲区设置socket
				tempCacheNode->setst(arg->st);
				tempCacheNode->setLength(i);
				//接收缓冲区设置接收buffer
				tempCacheNode->setbuffer((char*)arg->recvbuffer, i);
				//接收缓冲区操作值置0
				tempCacheNode->setOperate(0);

				//缓冲区加锁
				WaitForSingleObject(recvMutex, 500);
				arg->recvCacheTable->insertNode(tempCacheNode);
				//缓冲区解锁
				ReleaseMutex(recvMutex);
			}
		}
	}
	closesocket(arg->st);
	return 0;
}

//处理线程函数
static DWORD WINAPI Anal(LPVOID pM) {
	
	CAnalyzeMessage *analyzeMessage = new CAnalyzeMessage();
	struct _socket_parameters *arg = (struct _socket_parameters*)pM;
	while (1) {
		Sleep(500);
		//判断是否有待处理节点
		if (arg->recvCacheTable->getNodeNum() > 0) {
			//获取接收缓冲尾节点
			WaitForSingleObject(recvMutex, 500);
			/*CFile file;
			file.Open("log.txt", CFile::modeWrite);
			file.Seek(0L, CFile::end);
			int i = arg->recvCacheTable->getNodeNum();
			char a[20] = "\n anal 加锁";
			char b[20] = "\n anal 解锁";
			strcat(a, (char*)i);
			file.Write(a, 20);*/
			cacheNode *tempLinkNode = arg->recvCacheTable->getEndNode();
			/*file.SeekToEnd();
			file.Write(b, 20);
			file.Close();*/
			ReleaseMutex(recvMutex);
			//创建分析缓冲节点
			cacheNode *insertAnalNode = new cacheNode();

			analyzeMessage->startAnlayzeMessage(arg->analCacheTable, tempLinkNode, insertAnalNode);
			
		}
	}
}


static DWORD WINAPI Heart(LPVOID pM) {
	struct _socket_parameters *arg = (struct _socket_parameters*)pM;
	char a[6] = "heart";
	while (1) {
		int i = sendto(arg->st, a, strlen(a), 0, (sockaddr*)&arg->clientSock, sizeof(sockaddr_in));
		Sleep(10000);
	}
}


//UDPSocket类
class CSocketMessage
{
private:
	//保存服务器和客户端的地址结构
	//ip   sockaddr_in.sin_addr
	//port sockaddr_in.sin_port
	//struct sockaddr_in m_ServerSock;
	//struct sockaddr_in m_ClientSock;
	//地址结构长度
	//int m_Sockaddr_Length;
	//
	//SOCKET m_Socket;
	HANDLE m_handleRecv;

	//SOCKET recvSock;
	_socket_parameters *m_argRecv;

	CAnalyzeMessage *m_AnalyzeMessage;

	//_socket_broadcast *m_argBroad;
public:
	//在创建时传入有关的远程IP地址，直接在本地进行绑定
	CSocketMessage(CAnalyzeMessage *pAM, CCacheTable *recvCacheTable, CCacheTable *analCacheTable);
	CSocketMessage(CAnalyzeMessage *pAM, char *sendAddr);
	~CSocketMessage();

	void initRecvThread();

	void InitSocket(char *sendAddr);

	//初始化socket
	void initBroadSocket(char *sendAddr);

	void initTargetSocket(char *sendAddr);
	//单点发送消息
	void sendBuffer(char *buffer, int length);
	//广播模式发送消息
	void broadBuffer(char *buffer, int length);

	//void broadCastScan(char *buffer, int length);

	void changeSocket(char *ip);

};
