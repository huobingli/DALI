#include "stdafx.h"
#include "FrameBLL.h"

#include <conio.h>

//DALI主机用户名和密码 通用参数
//static unsigned char masterID[4];
//static unsigned char password[16] = "self12345678";

CFrameBLL::CFrameBLL()
{
	//初始化接收处理队列
	recvCacheTable = new CCacheTable(20);
	analCacheTable = new CCacheTable(20);
	

	//创建初始socket
	m_SocketMessage = new CSocketMessage(m_AnalyzeMessage, recvCacheTable, analCacheTable);
	//需要封装messagePackage包的函数
	m_MessagePackage = new CMessagePackage();
	//发送cache
	memset(m_SendCache, 0, sizeof(m_SendCache));

	_scan_parameters *m_argScan = (_scan_parameters*)malloc(sizeof(_scan_parameters));
	memset(m_argScan, 0, sizeof(m_argScan));

	m_argScan->pRecvCacheTable = recvCacheTable;
	m_argScan->pAnalCacheTable = analCacheTable;
	
	//test
	//后台启动线程进行扫描
	//handleScan = CreateThread(NULL, 0, scan, m_argScan, NULL, 0);
	SuspendThread(handleScan);
}


CFrameBLL::~CFrameBLL()
{
	delete m_SocketMessage;
	delete m_MessagePackage;
	delete recvCacheTable;
	delete analCacheTable;
	//delete m_SendCache;
}

/*
void CFrameBLL::getBroadBuffer(char *pBuffer, int length) {
m_SocketMessage->InitSocket("192.168.0.255");
m_SocketMessage->broadCast(pBuffer, length);
}

void CFrameBLL::sendMessagePackage(CommunicateParam *pCP) {
//清空SendCache发送缓存
memset(m_SendCache, 0, sizeof(m_SendCache));

//命令处理

//得到SendCache发送缓存
int length = m_MessagePackage->App_WifiSendFormat(
m_SendCache,
5,
/*1//,pCP->Command,
4, masterID,
16, password,
1, &(pCP->commandType),
1, &(pCP->command));
//pCP->command = 0x80 + 0x01;
//发送数据

m_SocketMessage->broadCast((char*)m_SendCache, length);
}*/

void CFrameBLL::changeIP(char *ip) {
	m_SocketMessage->changeSocket(ip);
}

//广播获取所有主机IP和通道个数
int CFrameBLL::scanDevice(_getdevice_info *pGI, int deviceNum) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	int length = m_MessagePackage->App_WifiSendFormat(m_SendCache,
		1, 0x01
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->broadBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//返回上一级界面
	//定义一个变量接收主机个数
	int ideviceNum = deviceNum;
	cacheNode *tempCacheNode = new cacheNode();
	int DALINUM = analCacheTable->getNodeNum(0x81);
	for (int i = 0; i < DALINUM; i++) {
		m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x81, tempCacheNode);
		
		//处理函数，将字符串处理成结构体返回
		//ideviceNum 用来写入到结构体
		ideviceNum = ideviceNum + analyze(tempCacheNode, 55, pGI, ideviceNum);
	}

	delete tempCacheNode;
	tempCacheNode = NULL;
	return ideviceNum;
}


/*
 * 功能：处理节点函数获得节点中数据
 * 参数：链表节点，获取buffer长度，返回上层结构体，存在上层结构体中主机个数
 * 返回：返回增加主机个数
 */
int CFrameBLL::analyze(cacheNode *pCacheNode, int length, _getdevice_info *pGI, int deviceNum){
	//获取数据字段
	//保存消息中数据条数变量
	int num = 0;
	int increaseNum = 0;
	uint8_t buffer[48];
	memset(buffer, 0, sizeof(buffer));
	pCacheNode->getbuffer((char*)buffer, 48);
	
	//获取个数
	char ip[20] = { 0 };
	char id[4] = { 0 };
	char pid[9] = { 0 };
	char name[20] = { 0 };
	bool flag = 0;

	//获得消息中数据条数
	memcpy(&num, &buffer[0], 1);
	//从第六位开始取数据
	//循环加入到结构体数组中
	for (int i = 0; i < num; i++) {
		//获得buffer和id
		pCacheNode->getIP(ip);
		memcpy(name, (buffer + 2 + i * 22 + 5), 16);
		memcpy(id, (buffer + 2 + i * 22), 4);
		change(id, pid);

		//查询结构体中数据是否存在
		if (deviceNum == 0) {
			pCacheNode->getIP(pGI[deviceNum].cIP);
			memcpy(&pGI[deviceNum].cDeviceID, pid, 8);
			memcpy(&pGI[deviceNum].cDeviceName, (buffer + 2 + i * 22 + 5), 16);
			deviceNum++;
			increaseNum++;
		}
		//如果有数据，进行遍历，判断是否有重复数据
		/*
		pCacheNode->getIP(pGI[deviceNum].cIP);
		memcpy(&pGI[deviceNum].cDeviceID, pid, 8);
		memcpy(&pGI[deviceNum].cDeviceName, (buffer + 2 + i * 22 + 5), 16);
		deviceNum++;
		*/
		else {
			//开始遍历
			for (int j = 0; j < deviceNum; j++) {
				//存在ip相同
				if (strcmp(pGI[j].cIP, ip) == 0){
					//存在id相同
					if (strcmp(pGI[j].cDeviceID, pid) == 0) {
						//存在name相同
						flag = 0;
						break;
					}
					else {
						flag = 1;
					}
				}
				else {
					flag = 1;
				}
			}
			if (flag == 1) {
				pCacheNode->getIP(pGI[deviceNum].cIP);
				memcpy(&pGI[deviceNum].cDeviceID, pid, 8);
				memcpy(&pGI[deviceNum].cDeviceName, (buffer + 2 + i * 22 + 5), 16);
				deviceNum++;
				increaseNum++;
				flag = 0;
			}
		}
	}
	return increaseNum;
}

/*
 * 功能：十六进制转字符串
 * 参数：十六进制buffer,转换后字符串数组
 * 返回：转换后字符串数组
 */
char *CFrameBLL::change(char src[], char *cache) {

	char Num[11] = "0123456789";
	char CodeL[27] = "abcdefghijklmnopqrstuvwxyz";
	char CodeH[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//char cache[9];
	memset(cache, 0, sizeof(cache));
	for (char Index = 0; Index < 4; Index++)
	{
		char diff = src[Index] & 0x0F;
		if (diff>9)
		{
			cache[7 - (Index << 1)] = CodeH[diff - 10];
		}
		else
		{
			cache[7 - (Index << 1)] = Num[diff];
		}

		diff = (src[Index] >> 4) & 0x0f;

		if (diff>9)
		{
			cache[6 - (Index << 1)] = CodeH[diff - 10];
		}
		else
		{
			cache[6 - (Index << 1)] = Num[diff];
		}
	}
	return cache;
}


//发送普通参数命令 
//开关调亮调暗
void CFrameBLL::sendConsoleParameters(_console_command *pCC) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char mode;
	char command;

	//表示广播 、组播 、单播
	if (pCC->cMode == -1) {
		mode = pCC->cMode;
	}
	else if (pCC->cMode == -128) {
		mode = pCC->cMode + pCC->nGroupID;
	}
	else if (pCC->cMode == 0) {
		mode = pCC->cMode + pCC->nDALIID;
	}
	else {
		//AfxMessageageBox("模式错误");
		return;
	}

	//场景模式
	//自动场景
	if (pCC->nCommand == 16) {
		command = pCC->nCommand + pCC->nSenceID;
	}
	//单步调高、低
	else if (pCC->nCommand == 7 || pCC->nCommand == 8) {
		command = pCC->nCommand;
	}
	//普通命令
	else {
		command = pCC->nCommand;
	}


	//得到SendCache发送缓存
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x05,
		//主机ID
		4, pCC->DeviceID,
		//主机密码
		16, pCC->DevicePWD,
		//
		1, &(char)(mode),
		1, &(command));
	//pCP->command = 0x80 
	//0x01;


	//发送数据
	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//发送扫描命令
void CFrameBLL::sendScan(_console_command *pCC, char *buffer) {

	//memcpy(m_DALIDeviceArray, pCC->oDeviceArray, sizeof(pCC->oDeviceArray));
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x03,
		//主机ID
		4, pCC->DeviceID,
		//主机密码
		16, pCC->DevicePWD,
		//
		1, &(command));
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	
	//end test
	
	//AfxMessageBox(dw);

	//memcpy(m_argScan->pDeviceBtn, pCC->oDeviceArray, sizeof(pCC->oDeviceArray));
	//handleScan线程恢复
	ResumeThread(handleScan);

	//线程搜索倒计时
	CString head = "扫描倒计时 ";
	CString end = " 秒";
	CString message;
	for (int i = 0; i < 10; i++){
		message.Format("%s%d%s", head, 10 - i, end);
		pCC->pStatusBarCtrl->SetText(message, 0, 0);
		Sleep(1000);
	}

	 
	//10s线程等待
	//CloseHandle(handleScan);

	DWORD dw = WaitForSingleObject(handleScan, 10000);
	
	//handleScan线程挂起
	SuspendThread(handleScan);

	cacheNode *tempCacheNode = new cacheNode();
	//返回上一级界面
 	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x83, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 20);

	delete tempCacheNode;
	tempCacheNode = NULL;
}

//static DWORD WINAPI wait(LPVOID arg) {
	
//}

void CFrameBLL::detectScan(_console_command *pCC, char *buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char command;
	command = 0x01;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x03,
		//主机ID
		4, pCC->DeviceID,
		//主机密码
		16, pCC->DevicePWD,
		//
		1, &(command));
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(8000);

	cacheNode *tempCacheNode = new cacheNode();

	//启动一个处理扫描线程
	//HANDLE WaitHandle = CreateThread(NULL, 0, wait, NULL, 0, 0);
	
	//CloseHandle(WaitHandle);

	//返回上一级界面
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x83, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 20);

	delete tempCacheNode;
	tempCacheNode = NULL;
}



void CFrameBLL::sendChangeLightness(_console_command *pCC) {
	memset(m_SendCache, 0, sizeof(m_SendCache));
	
	char cBuf[3] = { '\0' };
	char mode = pCC->cMode;
	char command = pCC->nCommand;
	cBuf[0] = 0xFE;
	cBuf[1] = pCC->nBrightness * 254 / 100;
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ command,
		//主机ID
		4, pCC->DeviceID,
		//主机密码
		16, pCC->DevicePWD,
		1, &mode,
		//
		2, &cBuf
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x09 保存Group的配置
void CFrameBLL::sendGroupSaveConf(_config_info *pCI) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	char mode;
	mode = 0x00 + pCI->nAddress;

	//打包参数
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x09,
		//主机ID
		4, pCI->DeviceID,
		//主机密码
		16, pCI->DevicePWD,
		1, &(char)mode,
		//
		2, &(pCI->cBuf)
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x0C 读取Group的配置
void CFrameBLL::readGroupSaveConf(_config_info *pCI, char *buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	char mode = 0x00 + pCI->nAddress;
	//打包参数
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0C,
		//主机ID
		4, pCI->DeviceID,
		//主机密码
		16, pCI->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(2000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8c, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 3);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x0a 保存Sence的配置
void CFrameBLL::sendSenceSaveConf(_sence_command *pSC) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char mode;
	char command;

	//表示广播 、组播 、单播
	if (pSC->cMode == -1) {
		mode = pSC->cMode;
	}
	else if (pSC->cMode == -128) {
		mode = pSC->cMode + pSC->nGroupID;
	}
	else if (pSC->cMode == 0) {
		mode = pSC->cMode + (pSC->nDALIID & 0xFF);
	}
	else {
		//AfxMessageageBox("模式错误");
		return;
	}

	command = pSC->nCommand;

	unsigned char buffer[17];
	memset(buffer, 0, sizeof(buffer));
	for (int i = 0; i < 16; i++)
	{
		memcpy(buffer + i, pSC->cBrightness + i, 1);
		buffer[i] = buffer[i] & 0xFF;
	}

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x0a,
		//主机ID
		4, pSC->DeviceID,
		//主机密码
		16, pSC->DevicePWD,
		1, &(char)mode,
		16, &buffer
		);

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x0d 读取Sence的配置
void CFrameBLL::readSenceSaveConf(_sence_command *pSC, char *buffer) {
	//打包参数
	char mode;
	char command = 0x0d;

	//表示广播 、组播 、单播
	if (pSC->cMode == -1) {
		mode = pSC->cMode;
	}
	else if (pSC->cMode == -128) {
		mode = pSC->cMode + pSC->nGroupID;
	}
	else if (pSC->cMode == 0) {
		mode = pSC->cMode + pSC->nDALIID;
	}
	else {
		//AfxMessageageBox("模式错误");
		return;
	}

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0d,
		//主机ID
		4, pSC->DeviceID,
		//主机密码
		16, pSC->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据


	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(3000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8d, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 20);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x0b 保存Param的配置
void CFrameBLL::sendParamSaveConf(_param_command *pPC, char *buffer) {

	//通过上层buffer, 处理成可以使用的字符数组
	char pbuffer[6];
	memset(pbuffer, 0, sizeof(pbuffer));
	for (int i = 0; i < 5; i++) {
		pbuffer[i] = buffer[i];
	}
	if (buffer[4] == 0) {
		if (buffer[5] == 0) {
			pbuffer[4] = 0;
		}
		else {
			pbuffer[4] = 0x00 + buffer[5];
		}
	}
	else{
		if (buffer[5] == 0){
			pbuffer[4] = buffer[4] << 4;
		}
		else {
			pbuffer[4] = (buffer[4] << 4) + buffer[5];
		}
	}
	
	//参数模式
	char mode = 0;
	//表示广播 、组播 、单播
	if (pPC->cMode == -1) {
		mode = pPC->cMode;
	}
	else if (pPC->cMode == -128) {
		mode = pPC->cMode + pPC->nGroupID;
	}
	else if (pPC->cMode == 0) {
		mode = pPC->cMode + pPC->nDALIID;
	}
	else {
		//AfxMessageageBox("模式错误");
		return;
	}

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x0b,//d
		//主机ID
		4, pPC->DeviceID,
		//主机密码
		16, pPC->DevicePWD,
		1, &(char)mode,
		5, &pbuffer
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//0x0e 读取Param的配置
void CFrameBLL::readParamSaveConf(_param_command *pPC, char *buffer) {
	//参数模式
	char mode = 0;
	//表示广播 、组播 、单播
	if (pPC->cMode == -1) {
		mode = pPC->cMode;
	}
	else if (pPC->cMode == -128) {
		mode = pPC->cMode + pPC->nGroupID;
	}
	else if (pPC->cMode == 0) {
		mode = pPC->cMode + pPC->nDALIID;
	}
	else {
		//AfxMessageageBox("模式错误");
		return;
	}
	

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0e,
		//主机ID
		4, pPC->DeviceID,
		//主机密码
		16, pPC->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(3000);
	//char pbuffer[32];
	//memset(pbuffer, 0, sizeof(pbuffer));
	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8e, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 32);

	delete tempCacheNode;
	tempCacheNode = NULL;
}

//0x0F ctrl show 发送保存配置
void CFrameBLL::sendCtrlSaveConf(_device_command *pCPC, char*buffer) {

	char mode = pCPC->nID;
	//char pbuffer = {}
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x0F,//d
		//主机ID
		4, pCPC->DeviceID,
		//主机密码
		16, pCPC->DevicePWD,
		1, &(char)mode,
		14, buffer
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//0x10 ctrl show读取配置
void CFrameBLL::readCtrlSaveConf(_device_command *pCPC, char*buffer) {

	pCPC->cCommand;
	char mode = 0x01;
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x10,//d
		//主机ID
		4, pCPC->DeviceID,
		//主机密码
		16, pCPC->DevicePWD,
		1, &(char)mode
		//5, &buffer
		);
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
	Sleep(3000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8e, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 32);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x03 ctrl show 扫描控制器
void CFrameBLL::scanCtrlDevice(_device_command *pCPC, char*buffer) {

	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x04,
		//主机ID
		4, pCPC->DeviceID,
		//主机密码
		16, pCPC->DevicePWD,
		//
		1, &(pCPC->cCommand));
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//返回上一级界面
	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x84, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 8);

	delete tempCacheNode;
	tempCacheNode = NULL;
}

void CFrameBLL::detectCtrlDevice(_device_command *pCPC, char*buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//打包参数
	char command;
	command = 0x01;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x04,
		//主机ID
		4, pCPC->DeviceID,
		//主机密码
		16, pCPC->DevicePWD,
		//
		1, &(pCPC->cCommand));
	//pCP->command = 0x80 + 0x01;
	//发送数据

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//返回上一级界面
	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x84, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 8);

	delete tempCacheNode;
	tempCacheNode = NULL;
}