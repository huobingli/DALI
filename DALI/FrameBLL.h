#pragma once
#include "SocketMessage.h"
#include "MessagePackage.h"
#include "def.h"

static CCacheTable *analCacheTable;
static CCacheTable *recvCacheTable;


/*������Ϣ�ṹ*/
/*Group������Ϣ�ṹ*/
typedef struct _config_info
{
	char			cMode;
	unsigned char	cBuf[3];
	int				nAddress;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}CONFIG_INFO, *PCONFIG_INFO;

/*Sence����ṹ*/
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

//Parameters �ṹ��
typedef struct _param_command
{
	char cMode;
	int nCommand;
	int nGroupID;
	int nDALIID;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}PARAM_COMMAND, PPARAM_COMMAND;

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

	//console
	//������ͨ��������
	void sendConsoleParameters(_console_command *pConsoleCommand);

	//����ɨ������
	void sendScan(_console_command *pCC, char *buffer);

	void CFrameBLL::detectScan(_console_command *pCC, char *buffer);

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
	//����socket����
	CSocketMessage *m_SocketMessage;
	//
	CMessagePackage *m_MessagePackage;
	uint8_t m_SendCache[256];
	//socket�ṹ��
	_socket_parameters *m_argSock;

	CAnalyzeMessage *m_AnalyzeMessage;
};

