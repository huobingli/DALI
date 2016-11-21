#include "stdafx.h"
#include "AnalyzeMessage.h"




CAnalyzeMessage::CAnalyzeMessage()
{
	recvMutex = CreateMutex(NULL, FALSE, NULL);
	analMutex = CreateMutex(NULL, FALSE, NULL);
}


CAnalyzeMessage::~CAnalyzeMessage()
{
}

void CAnalyzeMessage::scanAnalzyeCache(CCacheTable* analCacheTable, int operate, cacheNode *pCacheNode) {
	analCacheTable->scanTable(operate, pCacheNode);
}


void CAnalyzeMessage::startAnlayzeMessage(CCacheTable* analCacheTable, cacheNode *recvNode, cacheNode* analNode) {
	char recvBuffer[256];
	char analbuffer[256];
	memset(recvBuffer, 0, sizeof(recvBuffer));
	memset(analbuffer, 0, sizeof(analbuffer));

	//获得接收缓存的buffer
	recvNode->getbuffer(recvBuffer);
	//创建处理缓存的buffer
	//cacheNode *analNode = new cacheNode();
	switch (recvBuffer[5] & 0xFF){
		//recv scan device message
		case 0x83: {
			//获得设备数目	;
			analNode->setOperate(0x83);
			analNode->setst(recvNode->getst());
			analNode->setLength(recvNode->getLength());
			//通过change,得到有用的信息
			changeBuffer(recvBuffer, 12, (int)recvBuffer[12], analbuffer);
			//传入需要返回的参数
			analNode->setbuffer(analbuffer, 20);
			//放入分析缓存
			analCacheTable->insertNode(analNode);
			break;
		}

		//Group recv config message
		case 0x8c:{
			//获得设备数目	;
			analNode->setOperate(0x8c);
			analNode->setst(recvNode->getst());

			//通过change,得到有用的信息
			changeBuffer(recvBuffer, 14, (int)recvBuffer[13], analbuffer);
			//传入需要返回的参数
			analNode->setbuffer(analbuffer, (int)recvBuffer[13]);
			analNode->setLength(2);
			//放入分析缓存
			analCacheTable->insertNode(analNode);
			break;
		}
		//Sence recv config message
		case 0x8d: {
			analNode->setOperate(0x8d);
			changeBuffer(recvBuffer, 14, (int)recvBuffer[13], analbuffer);
			analNode->setbuffer(analbuffer, 16);
			analNode->setLength(16);
			//放入分析缓存
			analCacheTable->insertNode(analNode);
			break;
		}
		//
		case 0x8e: {
			analNode->setOperate(0x8e);
			changeBuffer(recvBuffer, 14, (int)recvBuffer[13], analbuffer);
			analNode->setbuffer(analbuffer, 32);
			analNode->setLength(32);
			//放入分析缓存
			analCacheTable->insertNode(analNode);
			break;
		}
		//getIPDlg
		case 0x81:{
			//判断在缓冲区中是否存在该元素
			//如果存在
			/*if (findElement(analCacheTable, recvNode)){
				break;
			} else {*/
				char ipbuffer[16];
				analNode->setOperate(0x81);
				recvNode->getIP(ipbuffer);
				analNode->setIP(ipbuffer, 16);

				changeBuffer(recvBuffer, 7, 22 * recvBuffer[7] + 1, analbuffer);
				analNode->setbuffer(analbuffer, 22 * recvBuffer[7]);
				analNode->setLength(22 * recvBuffer[7] + 1);
				//放入分析缓存
				analCacheTable->insertNode(analNode);
			//}
			break;
		}

		//scan ctrl show 
		case 0x84: {
			//获得设备数目	;
			analNode->setOperate(0x84);
			analNode->setst(recvNode->getst());
			analNode->setLength(recvNode->getLength());
			//通过change,得到有用的信息
			changeBuffer(recvBuffer, 12, (int)recvBuffer[11], analbuffer);
			//传入需要返回的参数
			analNode->setbuffer(analbuffer, 10);
			//放入分析缓存
			analCacheTable->insertNode(analNode);
			break;
		}
	}

}

//获取buffer
void CAnalyzeMessage::changeBuffer(char *buffer, int start, int length, char *recvBuffer) {
	//获取需要转换的buffer字段
	//将buffer指定部分设置到对应接收buffer
	memcpy(recvBuffer, &(buffer[start]), length);
}

bool CAnalyzeMessage::findElement(CCacheTable* analCacheTable, cacheNode *pCacheNode) {
	char recvbuffer[50];
	char findbuffer[256];
	pCacheNode->getbuffer(findbuffer);
	changeBuffer(findbuffer, 7, 22 * findbuffer[7] + 1, recvbuffer);
	if (analCacheTable->findElement(recvbuffer))
		return true;
	else
		return false;
}