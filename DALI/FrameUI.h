#pragma once

#include "FrameBLL.h"

static DWORD WINAPI Update(LPVOID pM) {
	struct _update_parameters *arg = (struct _update_parameters*)pM;

	cacheNode *tempCacheNode = new cacheNode();
	char message[30];
	int waittime = 0;
	while (1){
		if (arg->pUpdateCacheTable->getNodeNum() != 0){
			memset(message, 0, 30);
			//获得末尾节点
			tempCacheNode = arg->pUpdateCacheTable->getEndNode();
			//获得显示参数
			tempCacheNode->getbuffer(message, 30);
			waittime = tempCacheNode->getLength();
			if (strlen(message) != 0) {
				arg->pStatusBarCtrl->SetText(message, 0, 0);
				Sleep(waittime);
			}
		}
	}
}

class CFrameUI
{
public:
	CFrameUI();
	~CFrameUI();

	void changeIP(CString ip) {
		//char ipBuffer[20];
		//memset(ipBuffer, 0, sizeof(ipBuffer));
		int strlen = ip.GetLength();
		char* ipBuffer = new char[strlen + 1];
		memset(ipBuffer, 0, strlen + 1);
		for (int i = 0; i<strlen; i++)  ipBuffer[i] = ip.GetAt(i);
		m_FrameBLL->changeIP(ipBuffer);

	}

	void broadCastTest();

	void broadCastBuffer(char *buffer, int length);

	//getIPDlg
	int scanDevice(_getdevice_info *pGI, int deviceNum) {
		return m_FrameBLL->scanDevice(pGI, deviceNum);
	}

	//GlobalGroup
	void sendGlobalGroupSaveConf(_globalgroup_command *pGG) {
		m_FrameBLL->sendGlobalGroupSaveConf(pGG);
	}

	void readGlobalGroupSaveConf(_globalgroup_command *pGG, char *buffer){
		m_FrameBLL->readGlobalGroupSaveConf(pGG, buffer);
	}

	//consoleDlg
	void sendConsoleParameters(_console_command *pCP) {
		m_FrameBLL->sendConsoleParameters(pCP);
	}

	void sendScan(_console_command *pCC, char *buffer) {
		m_FrameBLL->sendScan(pCC, buffer);
	}

	void detectScan(_console_command *pCC, char *buffer) {
		m_FrameBLL->detectScan(pCC, buffer);
	}

	void sendChangeLightness(_console_command *pCC) {
		m_FrameBLL->sendChangeLightness(pCC);
	}

	//groupDlg
	void sendGroupSaveConf(CONFIG_INFO* pCI) {
		m_FrameBLL->sendGroupSaveConf(pCI);
	}
	void readGroupSaveConf(CONFIG_INFO* pCI, char *buffer) {
		m_FrameBLL->readGroupSaveConf(pCI, buffer);
	}

	//senceDlg
	void sendSenceSaveConf(_sence_command * pSC) {
		m_FrameBLL->sendSenceSaveConf(pSC);
	}

	void readSenceSaveConf(_sence_command *pSC, char *buffer) {
		m_FrameBLL->readSenceSaveConf(pSC, buffer);
	}

	//parametersDlg
	void sendParamSaveConf(_param_command *pPC, char *buffer) {
		m_FrameBLL->sendParamSaveConf(pPC, buffer);
	}

	void readParamSaveConf(_param_command *pPC, char *buffer) {
		m_FrameBLL->readParamSaveConf(pPC, buffer);
	}

	//controller showDlg
	void sendCtrlSaveConf(_device_command *pCPC, char*buffer) {
		m_FrameBLL->sendCtrlSaveConf(pCPC, buffer);
	}

	void readCtrlSaveConf(_device_command *pCPC, char*buffer) {
		m_FrameBLL->readCtrlSaveConf(pCPC, buffer);
	}

	void scanCtrlDevice(_device_command *pCPC, char*buffer) {
		m_FrameBLL->scanCtrlDevice(pCPC, buffer);
	}

	void detectCtrlDevice(_device_command *pCPC, char*buffer) {
		m_FrameBLL->detectCtrlDevice(pCPC, buffer);
	}

	int getDeviceNum() {
		return m_DeviceNum;
	}

	void setDeviceNum(int iNum) {
		m_DeviceNum = iNum;
	}

	CStatusBarCtrl* getObjStatusBarCtrl() {
		return m_StatusBarCtrl;
	}

	void setStatusBar(CString status) {
		m_StatusBarCtrl->SetText(status, 0, 0);
	}

	void insertNode(cacheNode *pcacheNode) {
		m_argUpdate->pUpdateCacheTable->insertNode(pcacheNode);
	}

	CStatusBarCtrl *getStatusBarCtrl() {
		return m_StatusBarCtrl;
	}
private:
	CFrameBLL *m_FrameBLL;
	int m_DeviceNum;
	//存在于所有页面的状态栏
	CStatusBarCtrl *m_StatusBarCtrl;

public:
	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
	DALI_DEVICE m_DALIDeviceArray[DALI_DEVICE_NUM];
	_update_parameters *m_argUpdate;
};


