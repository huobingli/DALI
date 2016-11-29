#pragma once
//�Զ���ṹ�壬������
/*�궨��*/
#define BROADCAST_MODE					0xFF		//�㲥ģʽ
#define GROUP_MODE						0x80		//�鲥ģʽ
#define TARGET_MODE						0x00		//����ģʽ
/*Console �궨��*/
#define UP								0x03		//����
#define	DOWN							0x04		//����
#define	OFF								0x00		//�ر�
#define MAX								0x05		//���
#define MIN								0x06		//��С

#define GOTOSENCE						0x10		//ת������
#define	AUTOSENCE						0x10		//�Զ�����
#define OSU								0x08		//OSU
#define SDO								0x07		//SDO

//#define DIRECT						0xDE		//ֱ�ӿ�������
#define	DALI_DEVICE_NUM					64			//DALI�豸����
/*Group�궨��*/
#define GROUP_NUM						16
#define GROUP_SAVECONFIG				0x10		//Group��������
#define GROUP_READCONFIG				0x11		//Group��ȡ����
/*Sence�궨��*/
#define SENCE_SAVECONFIG				0x10		//��������
#define	SENCE_READCONFIG				0x11		//��ȡ����
#define DALI_NUM						16
#define DALI_BRIGHTNESS_FIRST_EDIT			SENCE_IDC_EDIT_0
#define DALI_BRIGHTNESS_LAST_EDIT			SENCE_IDC_EDIT_15
#define DALI_BRIGHTNESS_FIRST_BUTTON		SENCE_IDC_CHECK_0
#define DALI_BRIGHTNESS_LAST_BUTTON			SENCE_IDC_CHECK_15
#define DALI_BRIGHTNESS_FIRST_SCROLLBAR		SENCE_IDC_SCROLLBAR_0
#define DALI_BRIGHTNESS_LAST_SCROLLBAR		SENCE_IDC_SCROLLBAR_15

/*Parameters�궨��*/
#define PARAM_READ						0x10		//��ȡ
#define PARAM_SAVE						0x11		//����	
/*ControllerParam�궨��*/
#define CPARAM_READ						0x10		//��ȡ
#define CPARAM_SAVE						0x11		//����	

/*ControllerShow�궨��*/
#define NAME_SIZE							12
#define PARAM_SIZE							2
#define INFO_SIZE							NAME_SIZE + PARAM_SIZE
#define DEVICE_READ							0x10
#define DEVICE_SAVE							0x11	
#define	DEVICE_SCAN							0xFF
#define DEVICE_DETECT						0x13

/*ControllerParam�궨��*/
#define CPARAM_READ	0x10					//��ȡ
#define CPARAM_SAVE	0x11					//����	
/****************************************************************************************/

/*DALI�豸�ṹ*/
typedef struct _dali_device
{
	CButton btn;
	int		nTag = 0;
}DALI_DEVICE, *PDALI_DEVICE;




/*getIPDlg �ṹ��*/
struct _getdevice_info{
	char cIP[15];
	char cDeviceID[9];
	char cDeviceName[20];
};

typedef struct _tparameters_command {
	char cMode;
	int nCommand;
	
	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
};


/*console �ṹ��*/
/*Console����ṹ*/
typedef struct _console_command
{
	char	cMode;
	int		nCommand;
	int		nGroupID = -1;
	int		nDALIID = -1;
	int		nBrightness = -1;
	int		nSenceID = -1;
	int		nTimeInterval = -1;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
	
	CStatusBarCtrl *pStatusBarCtrl;
	_dali_device  oDeviceArray[64];
}CONSOLE_COMMAND, *PCONSOLE_COMMAND;

typedef struct _status
{
	int nVersion = 1;
	int nDeviceType = 2;
	int nStatusOfBallast = 3;
	int nLampFailure = 4;
	int nLampArcPowerOn = 5;
	int nLimitError = 6;
	int nFadeReady = 7;
	int nResetState = 8;
	int nMissingShortAddress = 9;
	int nPowerFailure = 10;
}STATUS, *PSTATUS;

/*Group�ṹ��*/

/*Sence�ṹ��*/

/*ControllerParam�ṹ��*/
typedef struct _controller_param_command
{
	char cMode;
	int nCommand;
	int nGroupID;
	int nDALIID;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}CONTROLLER_PARAM_COMMAND,*PCONTROLLER_PARAM_COMMAND;

typedef struct _controller_edit
{
	CEdit Edit;
	char  cVal;
}CONTROLLER_EDIT,*PCONTROLLER_EDIT;

/**********************************************************************************/

/*ControllerShow�ṹ��*/
typedef struct _controller_device_info
{
	CButton btn;
	int		nTag = 0;
	char	cInfo[INFO_SIZE + 1];
}CTRDALI_DEVICE, *PCTRDALI_DEVICE;
/*�豸����ṹ*/
typedef struct _device_command
{
	char cCommand;
	int nID = 0;

	unsigned char DeviceID[4];
	unsigned char DevicePWD[16];
}DEVICE_COMMAND, *PDEVICE_COMMAND;


