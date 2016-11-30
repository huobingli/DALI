#pragma once
#include <iostream>
#include <stdarg.h>

#include "MessagePackage.h"
#include "AnalyzeMessage.h"
#include "CacheTable.h"
#include "def.h"


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

//需要将接收队列和处理队列传入进行查询，不改变内容
typedef struct _scan_parameters{
	//处理类需要结构
	CCacheTable *pRecvCacheTable;
	CCacheTable *pAnalCacheTable;
	_dali_device pDeviceBtn[64];
	char buffer[16];
};

typedef struct _update_parameters {
	//更新状态栏数据缓冲区
	CCacheTable *pUpdateCacheTable;

	CStatusBarCtrl *pStatusBarCtrl;
};

struct _scan{
	CMessageStopScan *pMSS;
	CStatusBarCtrl *pSBC;
	CCacheTable *pAnalCacheTable;
};

//BLL全局接收线程
static HANDLE handleRecv;
static HANDLE handleAnal;
static HANDLE handleScan;

static HANDLE handleUpdate;
//static HANDLE handleHeart;


static DWORD WINAPI Scan(LPVOID pM) {

	_scan *arg = (_scan*)pM;
	//退出线程方式
	//1、用户点击取消按钮，线程break。
	//2、搜索超时，自动退出。
	CString status;
	CString head = "已经找到 ";
	CString end = " 个主机";
	cacheNode *pCacheNode = new cacheNode();
	for (int i = 0; i < 400; i++) {
		//用户没有按下取消按钮，线程继续执行
		if (CPublic::triggerFlag == 0) {
			//搜索分析队列中的操作为0x00的元素，获取里面的
			arg->pAnalCacheTable->scanTable(0x00, pCacheNode);
			//取出数据，并显示在状态栏
			//pCacheNode->
			Sleep(500);
		}
		else {
			CPublic::triggerFlag = 0;
			return 0;
		}
	}
	//CPublic::readyFlag = 1;
	//执行完毕，关闭对话框
	arg->pMSS->EndDialog(IDOK);
	return 0;
}

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
		//Sleep(10000);
	}
}


//UDPSocket类
class CSocketMessage
{
private:
	//HANDLE m_handleRecv;

	_socket_parameters *m_argRecv;

	CAnalyzeMessage *m_AnalyzeMessage;

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
