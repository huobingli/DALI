#include "stdafx.h"
#include "FrameBLL.h"

#include <conio.h>

//DALI�����û��������� ͨ�ò���
//static unsigned char masterID[4];
//static unsigned char password[16] = "self12345678";

CFrameBLL::CFrameBLL()
{
	//��ʼ�����մ������
	recvCacheTable = new CCacheTable(20);
	analCacheTable = new CCacheTable(20);
	

	//������ʼsocket
	m_SocketMessage = new CSocketMessage(m_AnalyzeMessage, recvCacheTable, analCacheTable);
	//��Ҫ��װmessagePackage���ĺ���
	m_MessagePackage = new CMessagePackage();
	//����cache
	memset(m_SendCache, 0, sizeof(m_SendCache));

	_scan_parameters *m_argScan = (_scan_parameters*)malloc(sizeof(_scan_parameters));
	memset(m_argScan, 0, sizeof(m_argScan));

	m_argScan->pRecvCacheTable = recvCacheTable;
	m_argScan->pAnalCacheTable = analCacheTable;
	
	//test
	//��̨�����߳̽���ɨ��
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
//���SendCache���ͻ���
memset(m_SendCache, 0, sizeof(m_SendCache));

//�����

//�õ�SendCache���ͻ���
int length = m_MessagePackage->App_WifiSendFormat(
m_SendCache,
5,
/*1//,pCP->Command,
4, masterID,
16, password,
1, &(pCP->commandType),
1, &(pCP->command));
//pCP->command = 0x80 + 0x01;
//��������

m_SocketMessage->broadCast((char*)m_SendCache, length);
}*/

void CFrameBLL::changeIP(char *ip) {
	m_SocketMessage->changeSocket(ip);
}

//�㲥��ȡ��������IP��ͨ������
int CFrameBLL::scanDevice(_getdevice_info *pGI, int deviceNum) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	int length = m_MessagePackage->App_WifiSendFormat(m_SendCache,
		1, 0x01
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->broadBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//������һ������
	//����һ������������������
	int ideviceNum = deviceNum;
	cacheNode *tempCacheNode = new cacheNode();
	int DALINUM = analCacheTable->getNodeNum(0x81);
	for (int i = 0; i < DALINUM; i++) {
		m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x81, tempCacheNode);
		
		//�����������ַ�������ɽṹ�巵��
		//ideviceNum ����д�뵽�ṹ��
		ideviceNum = ideviceNum + analyze(tempCacheNode, 55, pGI, ideviceNum);
	}

	delete tempCacheNode;
	tempCacheNode = NULL;
	return ideviceNum;
}


/*
 * ���ܣ�����ڵ㺯����ýڵ�������
 * ����������ڵ㣬��ȡbuffer���ȣ������ϲ�ṹ�壬�����ϲ�ṹ������������
 * ���أ�����������������
 */
int CFrameBLL::analyze(cacheNode *pCacheNode, int length, _getdevice_info *pGI, int deviceNum){
	//��ȡ�����ֶ�
	//������Ϣ��������������
	int num = 0;
	int increaseNum = 0;
	uint8_t buffer[48];
	memset(buffer, 0, sizeof(buffer));
	pCacheNode->getbuffer((char*)buffer, 48);
	
	//��ȡ����
	char ip[20] = { 0 };
	char id[4] = { 0 };
	char pid[9] = { 0 };
	char name[20] = { 0 };
	bool flag = 0;

	//�����Ϣ����������
	memcpy(&num, &buffer[0], 1);
	//�ӵ���λ��ʼȡ����
	//ѭ�����뵽�ṹ��������
	for (int i = 0; i < num; i++) {
		//���buffer��id
		pCacheNode->getIP(ip);
		memcpy(name, (buffer + 2 + i * 22 + 5), 16);
		memcpy(id, (buffer + 2 + i * 22), 4);
		change(id, pid);

		//��ѯ�ṹ���������Ƿ����
		if (deviceNum == 0) {
			pCacheNode->getIP(pGI[deviceNum].cIP);
			memcpy(&pGI[deviceNum].cDeviceID, pid, 8);
			memcpy(&pGI[deviceNum].cDeviceName, (buffer + 2 + i * 22 + 5), 16);
			deviceNum++;
			increaseNum++;
		}
		//��������ݣ����б������ж��Ƿ����ظ�����
		/*
		pCacheNode->getIP(pGI[deviceNum].cIP);
		memcpy(&pGI[deviceNum].cDeviceID, pid, 8);
		memcpy(&pGI[deviceNum].cDeviceName, (buffer + 2 + i * 22 + 5), 16);
		deviceNum++;
		*/
		else {
			//��ʼ����
			for (int j = 0; j < deviceNum; j++) {
				//����ip��ͬ
				if (strcmp(pGI[j].cIP, ip) == 0){
					//����id��ͬ
					if (strcmp(pGI[j].cDeviceID, pid) == 0) {
						//����name��ͬ
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
 * ���ܣ�ʮ������ת�ַ���
 * ������ʮ������buffer,ת�����ַ�������
 * ���أ�ת�����ַ�������
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


//������ͨ�������� 
//���ص�������
void CFrameBLL::sendConsoleParameters(_console_command *pCC) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char mode;
	char command;

	//��ʾ�㲥 ���鲥 ������
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
		//AfxMessageageBox("ģʽ����");
		return;
	}

	//����ģʽ
	//�Զ�����
	if (pCC->nCommand == 16) {
		command = pCC->nCommand + pCC->nSenceID;
	}
	//�������ߡ���
	else if (pCC->nCommand == 7 || pCC->nCommand == 8) {
		command = pCC->nCommand;
	}
	//��ͨ����
	else {
		command = pCC->nCommand;
	}


	//�õ�SendCache���ͻ���
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x05,
		//����ID
		4, pCC->DeviceID,
		//��������
		16, pCC->DevicePWD,
		//
		1, &(char)(mode),
		1, &(command));
	//pCP->command = 0x80 
	//0x01;


	//��������
	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//����ɨ������
void CFrameBLL::sendScan(_console_command *pCC, char *buffer) {

	//memcpy(m_DALIDeviceArray, pCC->oDeviceArray, sizeof(pCC->oDeviceArray));
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x03,
		//����ID
		4, pCC->DeviceID,
		//��������
		16, pCC->DevicePWD,
		//
		1, &(command));
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	
	//end test
	
	//AfxMessageBox(dw);

	//memcpy(m_argScan->pDeviceBtn, pCC->oDeviceArray, sizeof(pCC->oDeviceArray));
	//handleScan�ָ̻߳�
	ResumeThread(handleScan);

	//�߳���������ʱ
	CString head = "ɨ�赹��ʱ ";
	CString end = " ��";
	CString message;
	for (int i = 0; i < 10; i++){
		message.Format("%s%d%s", head, 10 - i, end);
		pCC->pStatusBarCtrl->SetText(message, 0, 0);
		Sleep(1000);
	}

	 
	//10s�̵߳ȴ�
	//CloseHandle(handleScan);

	DWORD dw = WaitForSingleObject(handleScan, 10000);
	
	//handleScan�̹߳���
	SuspendThread(handleScan);

	cacheNode *tempCacheNode = new cacheNode();
	//������һ������
 	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x83, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 20);

	delete tempCacheNode;
	tempCacheNode = NULL;
}

//static DWORD WINAPI wait(LPVOID arg) {
	
//}

void CFrameBLL::detectScan(_console_command *pCC, char *buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char command;
	command = 0x01;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x03,
		//����ID
		4, pCC->DeviceID,
		//��������
		16, pCC->DevicePWD,
		//
		1, &(command));
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(8000);

	cacheNode *tempCacheNode = new cacheNode();

	//����һ������ɨ���߳�
	//HANDLE WaitHandle = CreateThread(NULL, 0, wait, NULL, 0, 0);
	
	//CloseHandle(WaitHandle);

	//������һ������
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
		//����ID
		4, pCC->DeviceID,
		//��������
		16, pCC->DevicePWD,
		1, &mode,
		//
		2, &cBuf
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x09 ����Group������
void CFrameBLL::sendGroupSaveConf(_config_info *pCI) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	char mode;
	mode = 0x00 + pCI->nAddress;

	//�������
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x09,
		//����ID
		4, pCI->DeviceID,
		//��������
		16, pCI->DevicePWD,
		1, &(char)mode,
		//
		2, &(pCI->cBuf)
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x0C ��ȡGroup������
void CFrameBLL::readGroupSaveConf(_config_info *pCI, char *buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	char mode = 0x00 + pCI->nAddress;
	//�������
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0C,
		//����ID
		4, pCI->DeviceID,
		//��������
		16, pCI->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(2000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8c, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 3);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x0a ����Sence������
void CFrameBLL::sendSenceSaveConf(_sence_command *pSC) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char mode;
	char command;

	//��ʾ�㲥 ���鲥 ������
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
		//AfxMessageageBox("ģʽ����");
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
		//����ID
		4, pSC->DeviceID,
		//��������
		16, pSC->DevicePWD,
		1, &(char)mode,
		16, &buffer
		);

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}

//0x0d ��ȡSence������
void CFrameBLL::readSenceSaveConf(_sence_command *pSC, char *buffer) {
	//�������
	char mode;
	char command = 0x0d;

	//��ʾ�㲥 ���鲥 ������
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
		//AfxMessageageBox("ģʽ����");
		return;
	}

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0d,
		//����ID
		4, pSC->DeviceID,
		//��������
		16, pSC->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//��������


	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(3000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8d, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 20);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x0b ����Param������
void CFrameBLL::sendParamSaveConf(_param_command *pPC, char *buffer) {

	//ͨ���ϲ�buffer, ����ɿ���ʹ�õ��ַ�����
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
	
	//����ģʽ
	char mode = 0;
	//��ʾ�㲥 ���鲥 ������
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
		//AfxMessageageBox("ģʽ����");
		return;
	}

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x0b,//d
		//����ID
		4, pPC->DeviceID,
		//��������
		16, pPC->DevicePWD,
		1, &(char)mode,
		5, &pbuffer
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//0x0e ��ȡParam������
void CFrameBLL::readParamSaveConf(_param_command *pPC, char *buffer) {
	//����ģʽ
	char mode = 0;
	//��ʾ�㲥 ���鲥 ������
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
		//AfxMessageageBox("ģʽ����");
		return;
	}
	

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x0e,
		//����ID
		4, pPC->DeviceID,
		//��������
		16, pPC->DevicePWD,
		1, &(char)mode
		//
		);
	//pCP->command = 0x80 + 0x01;
	//��������

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

//0x0F ctrl show ���ͱ�������
void CFrameBLL::sendCtrlSaveConf(_device_command *pCPC, char*buffer) {

	char mode = pCPC->nID;
	//char pbuffer = {}
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		5,
		/*1,*/ 0x0F,//d
		//����ID
		4, pCPC->DeviceID,
		//��������
		16, pCPC->DevicePWD,
		1, &(char)mode,
		14, buffer
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
}


//0x10 ctrl show��ȡ����
void CFrameBLL::readCtrlSaveConf(_device_command *pCPC, char*buffer) {

	pCPC->cCommand;
	char mode = 0x01;
	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x10,//d
		//����ID
		4, pCPC->DeviceID,
		//��������
		16, pCPC->DevicePWD,
		1, &(char)mode
		//5, &buffer
		);
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);
	Sleep(3000);

	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x8e, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 32);

	delete tempCacheNode;
	tempCacheNode = NULL;
}


//0x03 ctrl show ɨ�������
void CFrameBLL::scanCtrlDevice(_device_command *pCPC, char*buffer) {

	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char command;
	command = 0xFF;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x04,
		//����ID
		4, pCPC->DeviceID,
		//��������
		16, pCPC->DevicePWD,
		//
		1, &(pCPC->cCommand));
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//������һ������
	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x84, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 8);

	delete tempCacheNode;
	tempCacheNode = NULL;
}

void CFrameBLL::detectCtrlDevice(_device_command *pCPC, char*buffer) {
	memset(m_SendCache, 0, sizeof(m_SendCache));

	//�������
	char command;
	command = 0x01;

	int length = m_MessagePackage->App_WifiSendFormat(
		m_SendCache,
		4,
		/*1,*/ 0x04,
		//����ID
		4, pCPC->DeviceID,
		//��������
		16, pCPC->DevicePWD,
		//
		1, &(pCPC->cCommand));
	//pCP->command = 0x80 + 0x01;
	//��������

	m_SocketMessage->sendBuffer((char*)m_SendCache, length);

	Sleep(5000);

	//������һ������
	cacheNode *tempCacheNode = new cacheNode();
	m_AnalyzeMessage->scanAnalzyeCache(analCacheTable, 0x84, tempCacheNode);
	tempCacheNode->getbuffer(buffer, 8);

	delete tempCacheNode;
	tempCacheNode = NULL;
}