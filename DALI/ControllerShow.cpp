// ControllerShow.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "ControllerShow.h"
#include "afxdialogex.h"


// CControllerShow 对话框

IMPLEMENT_DYNAMIC(CControllerShow, CDialogEx)

CControllerShow::CControllerShow(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_DIALOG5, pParent)
{
	m_FrameUI = pUI;
	m_cParam[0] = 0;
	m_cParam[1] = 0;
}

CControllerShow::~CControllerShow()
{
}

void CControllerShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CS_IDC_STATIC_DEVICE_ID, m_DeviceID);
	//DX_Control(pDX, CS_IDC_STATIC_DEVICE_ID_T, m_DeviceID);
	//Button控件
	DDX_Control(pDX, CS_IDC_BUTTON_READ, m_ButtonRead);
	DDX_Control(pDX, CS_IDC_BUTTON_SAVE, m_ButtonSave);
	DDX_Control(pDX, CS_IDC_BUTTON_SCANDEVICE, m_ButtonScanDevice);
	DDX_Control(pDX, CS_IDC_BUTTON_DETEDTDEVICE, m_ButtonDetectDevice);
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		DDX_Control(pDX, CS_IDC_BUTTON1 + i, m_DALIDeviceArray[i].btn);
	}
	for (int i = 0; i < 5; ++i)
	{
		DDX_Control(pDX, CS_IDC_EDIT_NAME + i, m_Edit[i]);
	}
}

BOOL CControllerShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		m_DALIDeviceArray[i].btn.EnableWindow(false);
		memset(m_DALIDeviceArray[i].cInfo, 0, INFO_SIZE);
	}
	/*隐藏编辑框*/
	for (int i = 0; i < 5; ++i)
	{
		GetDlgItem(CS_IDC_EDIT_NAME + i)->EnableWindow(false);
	}
	m_ButtonRead.EnableWindow(false);
	m_ButtonSave.EnableWindow(false);

	//m_FrameUI->setStatusBar("就绪");
	return FALSE;
}

BEGIN_MESSAGE_MAP(CControllerShow, CDialogEx)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(CS_IDC_BUTTON_SCANDEVICE, OnBnScanDevice)
	ON_BN_CLICKED(CS_IDC_BUTTON_DETEDTDEVICE, OnBnDetectDevice)
	ON_BN_CLICKED(CS_IDC_BUTTON_READ, OnBnRead)
	ON_BN_CLICKED(CS_IDC_BUTTON_SAVE, OnBnSave)
	ON_COMMAND_RANGE(CS_IDC_BUTTON1, CS_IDC_BUTTON64, OnBnDALIDevice)
	ON_CONTROL_RANGE(EN_KILLFOCUS, CS_IDC_EDIT_NAME, CS_IDC_EDIT_PANNEL2_GROUP, OnBnEditWrite)
END_MESSAGE_MAP()


// CControllerShow 消息处理程序
/*功能：点击按钮扫描设备*/
void CControllerShow::OnBnScanDevice()
{
	//发送扫描设备命令
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = 0xFF;
	//BLL->接口(pDeviceCommand,cBuf);

	//设置主机ID和密码
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[8] = { 0 };

	m_FrameUI->setStatusBar("正在扫描控制器中... 请稍候...");
	//huobingli	
	m_FrameUI->scanCtrlDevice(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;

	//获取设备信息数组
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)cBuf[i];
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	ShowDALIDevice();

	m_FrameUI->setStatusBar("扫描完成！");
}
/*功能：点击按钮检测设备*/
void CControllerShow::OnBnDetectDevice()
{
	//发送扫描设备命令
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	//detect
	pDeviceCommand->cCommand = 0x01;
	//BLL->接口(pDeviceCommand,cBuf);

	//设置主机ID和密码
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[8] = { 0 };

	m_FrameUI->setStatusBar("正在扫描控制器中... 请稍候...");
	//huobingli	
	m_FrameUI->scanCtrlDevice(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;

	//获取设备信息数组
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)cBuf[i];
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	ShowDALIDevice();

	m_FrameUI->setStatusBar("扫描完成！");
}

/*功能：读取函数*/
void CControllerShow::OnBnRead()
{
	//发送读取设备命令
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = DEVICE_READ;
	//BLL->接口(pDeviceCommand,cBuf);

	//设置主机ID和密码
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	char cBuf[INFO_SIZE+1] = { "Chris0000000CA" };

	m_FrameUI->setStatusBar("正在读取配置中... 请稍候...");
	//huobingli
	m_FrameUI->readCtrlSaveConf(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;
	

	//拷贝到内存中
	int nDeviceID = GetDlgItemInt(CS_IDC_STATIC_DEVICE_ID);
	memcpy(m_DALIDeviceArray[nDeviceID].cInfo, cBuf, INFO_SIZE);
	//显示信息
	ShowEdit(nDeviceID);

	m_FrameUI->setStatusBar("读取配置成功！");

}
/*功能：保存函数*/
void CControllerShow::OnBnSave()
{
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = DEVICE_SAVE;
	pDeviceCommand->nID = GetDlgItemInt(CS_IDC_STATIC_DEVICE_ID);

	//设置主机ID和密码
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	char cBuf[INFO_SIZE + 1] = { '\0' };
	memcpy(cBuf, m_DALIDeviceArray[pDeviceCommand->nID].cInfo, INFO_SIZE);
	
	//BLL->接口(pDeviceCommand,cBuf);

	m_FrameUI->setStatusBar("正在保存配置... 请稍候... ");
	//huobingli
	m_FrameUI->sendCtrlSaveConf(pDeviceCommand, cBuf);
	//huobingli

	m_FrameUI->setStatusBar("保存配置成功!");
	delete pDeviceCommand;
	pDeviceCommand = NULL;
}
/*功能：点击单选按钮显示对应编辑框*/
void CControllerShow::OnBnDALIDevice(UINT ID)
{
	/*清空编辑框*/
	ClearEdit();

	int nID = ID - CS_IDC_BUTTON1;
	SetDlgItemInt(CS_IDC_STATIC_DEVICE_ID, nID, -1);
	OptionalEdit();
}

void CControllerShow::OnBnEditWrite(UINT ID)
{
	int nID = GetDlgItemInt(CS_IDC_STATIC_DEVICE_ID);
	int nWriteID = ID - CS_IDC_EDIT_NAME;
	char* pInfo = m_DALIDeviceArray[nID].cInfo;
	if (nWriteID == 0)
	{
		CString sInfo;
		GetDlgItemText(ID, sInfo);
		char cBuf[NAME_SIZE + 1] = { '\0' };
		strcpy(cBuf, sInfo);
		memcpy(pInfo, cBuf, NAME_SIZE);
	}
	else if (nWriteID == 1 || nWriteID == 2)
	{
		if (nWriteID == 1)
		{
			m_cParam[0] = (char)GetDlgItemInt(ID, NULL, 1) << 4;
		}
		else
		{
			char a = (char)GetDlgItemInt(ID, NULL, 1);
			m_cParam[0] = m_cParam[0] | a;
		}
		memcpy(pInfo + NAME_SIZE, &m_cParam[0], 1);
	}
	else
	{
		if (nWriteID == 3)
		{
			m_cParam[1] = (char)GetDlgItemInt(ID, NULL, 1) << 4;
		}
		else
		{
			char a = (char)GetDlgItemInt(ID, NULL, 1);
			m_cParam[1] = m_cParam[1] | a;
		}
		memcpy(pInfo + NAME_SIZE + 1, &m_cParam[1], 1);
	}

}


/*private*/
void CControllerShow::ClearEdit()
{
	for (int i = 0; i < 7; ++i)
	{
		SetDlgItemText(CS_IDC_EDIT_NAME + i, "");
	}
}
void CControllerShow::OptionalEdit()
{
	for (int i = 0; i < 5; ++i)
	{
		GetDlgItem(CS_IDC_EDIT_NAME + i)->EnableWindow(true);
	}
	m_ButtonRead.EnableWindow(true);
	m_ButtonSave.EnableWindow(true);
}
void CControllerShow::ShowEdit(int nPos)
{
	char* pBuf = m_DALIDeviceArray[nPos].cInfo;
	/*显示Name*/
	char cName[NAME_SIZE + 1] = { '\0' };
	memcpy(cName, pBuf, NAME_SIZE);
	CString sName;
	sName.Format("%s", cName);
	SetDlgItemText(CS_IDC_EDIT_NAME, sName);

	/*显示其他参数*/

	int nP1S = *(pBuf + NAME_SIZE) >> 4;
	SetDlgItemInt(CS_IDC_EDIT_PANNEL1_SENCE, nP1S);
	int nP1G = *(pBuf + NAME_SIZE) & 0x0F;
	SetDlgItemInt(CS_IDC_EDIT_PANNEL1_GROUP, nP1G);
	int nP2S = *(pBuf + NAME_SIZE + 1) >> 4;
	SetDlgItemInt(CS_IDC_EDIT_PANNEL2_SENCE, nP2S);
	int nP2G = *(pBuf + NAME_SIZE + 1) & 0x0F;
	SetDlgItemInt(CS_IDC_EDIT_PANNEL2_GROUP, nP2G);

}
void CControllerShow::ShowDALIDevice()
{
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		if (m_DALIDeviceArray[i].nTag == 1)
			m_DALIDeviceArray[i].btn.EnableWindow(true);
		else
			m_DALIDeviceArray[i].btn.EnableWindow(false);
	}
}
void CControllerShow::UpdateTag(char* pBuf, int nCycleTime)
{
	for (int i = 0; i < 8; ++i)
	{
		if (pBuf[i] == '1')
		{
			m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 1;
		}
		else
		{
			m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 0;
		}
	}
}
