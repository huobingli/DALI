// ControllerShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "ControllerShow.h"
#include "afxdialogex.h"


// CControllerShow �Ի���

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
	//Button�ؼ�
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
	/*���ر༭��*/
	for (int i = 0; i < 5; ++i)
	{
		GetDlgItem(CS_IDC_EDIT_NAME + i)->EnableWindow(false);
	}
	m_ButtonRead.EnableWindow(false);
	m_ButtonSave.EnableWindow(false);

	//m_FrameUI->setStatusBar("����");
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


// CControllerShow ��Ϣ�������
/*���ܣ������ťɨ���豸*/
void CControllerShow::OnBnScanDevice()
{
	//����ɨ���豸����
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = 0xFF;
	//BLL->�ӿ�(pDeviceCommand,cBuf);

	//��������ID������
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[8] = { 0 };

	m_FrameUI->setStatusBar("����ɨ���������... ���Ժ�...");
	//huobingli	
	m_FrameUI->scanCtrlDevice(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;

	//��ȡ�豸��Ϣ����
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)cBuf[i];
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	ShowDALIDevice();

	m_FrameUI->setStatusBar("ɨ����ɣ�");
}
/*���ܣ������ť����豸*/
void CControllerShow::OnBnDetectDevice()
{
	//����ɨ���豸����
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	//detect
	pDeviceCommand->cCommand = 0x01;
	//BLL->�ӿ�(pDeviceCommand,cBuf);

	//��������ID������
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[8] = { 0 };

	m_FrameUI->setStatusBar("����ɨ���������... ���Ժ�...");
	//huobingli	
	m_FrameUI->scanCtrlDevice(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;

	//��ȡ�豸��Ϣ����
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)cBuf[i];
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	ShowDALIDevice();

	m_FrameUI->setStatusBar("ɨ����ɣ�");
}

/*���ܣ���ȡ����*/
void CControllerShow::OnBnRead()
{
	//���Ͷ�ȡ�豸����
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = DEVICE_READ;
	//BLL->�ӿ�(pDeviceCommand,cBuf);

	//��������ID������
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	char cBuf[INFO_SIZE+1] = { "Chris0000000CA" };

	m_FrameUI->setStatusBar("���ڶ�ȡ������... ���Ժ�...");
	//huobingli
	m_FrameUI->readCtrlSaveConf(pDeviceCommand, (char*)cBuf);
	//huobingli

	delete pDeviceCommand;
	pDeviceCommand = NULL;
	

	//�������ڴ���
	int nDeviceID = GetDlgItemInt(CS_IDC_STATIC_DEVICE_ID);
	memcpy(m_DALIDeviceArray[nDeviceID].cInfo, cBuf, INFO_SIZE);
	//��ʾ��Ϣ
	ShowEdit(nDeviceID);

	m_FrameUI->setStatusBar("��ȡ���óɹ���");

}
/*���ܣ����溯��*/
void CControllerShow::OnBnSave()
{
	DEVICE_COMMAND* pDeviceCommand = new DEVICE_COMMAND();
	pDeviceCommand->cCommand = DEVICE_SAVE;
	pDeviceCommand->nID = GetDlgItemInt(CS_IDC_STATIC_DEVICE_ID);

	//��������ID������
	memcpy(pDeviceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pDeviceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	char cBuf[INFO_SIZE + 1] = { '\0' };
	memcpy(cBuf, m_DALIDeviceArray[pDeviceCommand->nID].cInfo, INFO_SIZE);
	
	//BLL->�ӿ�(pDeviceCommand,cBuf);

	m_FrameUI->setStatusBar("���ڱ�������... ���Ժ�... ");
	//huobingli
	m_FrameUI->sendCtrlSaveConf(pDeviceCommand, cBuf);
	//huobingli

	m_FrameUI->setStatusBar("�������óɹ�!");
	delete pDeviceCommand;
	pDeviceCommand = NULL;
}
/*���ܣ������ѡ��ť��ʾ��Ӧ�༭��*/
void CControllerShow::OnBnDALIDevice(UINT ID)
{
	/*��ձ༭��*/
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
	/*��ʾName*/
	char cName[NAME_SIZE + 1] = { '\0' };
	memcpy(cName, pBuf, NAME_SIZE);
	CString sName;
	sName.Format("%s", cName);
	SetDlgItemText(CS_IDC_EDIT_NAME, sName);

	/*��ʾ��������*/

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
