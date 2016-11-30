#pragma once
#include "SocketMessage.h"
#include "MessagePackage.h"
#include "def.h"

static CCacheTable *analCacheTable;
static CCacheTable *recvCacheTable;
static CCacheTable *updtCacheTable;
//设备数组
//extern static DALI_DEVICE DeviceArray[DALI_DEVICE_NUM];	//DALI设备数组
//static DALI_DEVICE DeviceArray2[DALI_DEVICE_NUM];	//DALI设备数组

extern "C"
{
	int WINAPI MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
	int WINAPI MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
};
#ifdef UNICODE
#define MessageBoxTimeout MessageBoxTimeoutW
#else
#define MessageBoxTimeout MessageBoxTimeoutA
#endif


/*配置信息结构*/
/*Group配置信息结构*/
typedef struct _config_info
{
	char			cMode;
	unsigned char	cBuf[3];
	int				nAddress;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}CONFIG_INFO, *PCONFIG_INFO;

/*Sence命令结构*/
typedef struct _sence_command
{
	char cMode;
	int nCommand;
	char cBrightness[DALI_NUM];
	int nGroupID = -1;
	int nDALIID = -1;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}SENCE_COMMAND, *PSENCE_COMMAND;

typedef struct _group_check
{
	CButton CheckButton;
	BOOL	bTag = 0;

}GROUP_CHECK, *PGROUP_CHECK;

//Parameters 结构体
typedef struct _param_command
{
	char cMode;
	int nCommand;
	int nGroupID;
	int nDALIID;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}PARAM_COMMAND, PPARAM_COMMAND;


//扫描设备线程
/*
static DWORD WINAPI scan(LPVOID pM) {
	struct _scan_parameters *arg = (struct _scan_parameters*)pM;
	

	int num = 0;
	CString head = "已扫描到 ";
	CString end = "个设备 ";
	CString message;
	cacheNode *pCacheNode = new cacheNode();
	char buffer[20] = { "0" };
	while (1) {
		arg->pAnalCacheTable->scanTable(0x83, pCacheNode);
		//获取到数据
		pCacheNode->getbuffer(buffer, 20); 
		//完善到按钮数组
		for (int j = 0; j < 8; ++j){
			for (int i = 0; i < 8; ++i)
			{
				if (buffer[i] == '1')
				{
					DeviceArray[j * 8 + i].nTag = 1;
				}
				else
				{
					DeviceArray[j * 8 + i].nTag = 0;
				}
			}
		}
		//刷新界面
		for (int i = 0; i < DALI_DEVICE_NUM; ++i)
		{
			//if (m_DALIDeviceArray[i].nTag == 1)
			//	m_DALIDeviceArray[i].btn.EnableWindow(true);
			//else
				//m_DALIDeviceArray[i].btn.EnableWindow(false);
		}
		Sleep(1000);
	}
}
*/


class CFrameBLL
{
public:
	CFrameBLL();
	~CFrameBLL();

	void changeIP(char *ip);

	void getBroadBuffer(char *pbuffer, int length);

	//getIPDlg
	int scanDevice(_getdevice_info *buffer, int deviceNum);

	int analyze(cacheNode *pCacheNode, int length, _getdevice_info *pGI, int number);

	char *change(char src[], char *cache);

	//void insertORno();

	//GlobalGroup
	void sendGlobalGroupSaveConf(_globalgroup_command *pGG);

	void readGlobalGroupSaveConf(_globalgroup_command *pGG, char *buffer);

	//console
	//发送普通参数命令
	void sendConsoleParameters(_console_command *pConsoleCommand);

	//发送扫描命令
	void sendScan(_console_command *pCC, char *buffer);

	void detectScan(_console_command *pCC, char *buffer);

	void sendChangeLightness(_console_command *pCC);

	//Group
	void sendGroupSaveConf(_config_info *pCI);

	void readGroupSaveConf(_config_info *pCI, char *buffer);

	//Sence
	void sendSenceSaveConf(_sence_command *pSC);

	void readSenceSaveConf(_sence_command *pSC, char *buffer);

	//Parameters
	void sendParamSaveConf(_param_command *pSC, char *buffer);

	void readParamSaveConf(_param_command *pSC, char *buffer);

	//controller show
	void sendCtrlSaveConf(_device_command *pCPC, char*buffer);

	void readCtrlSaveConf(_device_command *pCPC, char*buffer);

	void scanCtrlDevice(_device_command *pCPC, char*buffer);

	void detectCtrlDevice(_device_command *pCPC, char*buffer);

private:
	//发送socket对象
	CSocketMessage *m_SocketMessage;
	//
	CMessagePackage *m_MessagePackage;
	uint8_t m_SendCache[256];
	//socket结构体
	_socket_parameters *m_argSock;

	CAnalyzeMessage *m_AnalyzeMessage;

	_scan_parameters *m_argScan;
};

