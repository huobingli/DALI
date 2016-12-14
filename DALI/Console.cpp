// Console.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "Console.h"
#include "afxdialogex.h"

#include <iostream>
#include <cmath>
#include <stdarg.h>
#include <afxwin.h>
using namespace std;

//CRCУ�麯��
/*
uint16_t Tmgr_GetCrcValue(uint8_t *pSrc, uint16_t Length)
{
	uint16_t CrcValue = PRESET_VALUE;//0xFFFF
	for (uint16_t LengthIndex = 0; LengthIndex<Length; LengthIndex++)
	{
		CrcValue ^= pSrc[LengthIndex];
		for (uint8_t CheckIndex = 0; CheckIndex<8; CheckIndex++)
		{
			if ((CrcValue & 0x0001) != 0)
			{
				CrcValue >>= 1;
				CrcValue ^= PLOYNOMIAL;//0x8408
			}
			else
			{
				CrcValue >>= 1;
			}
		}
	}
	return(CrcValue);
}*/
// CConsole �Ի���

IMPLEMENT_DYNAMIC(CConsole, CDialogEx)

CConsole::CConsole(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CConsole::IDD, pParent)
{
	m_FrameUI = pUI;
	m_MessageStopScan = new CMessageStopScan();
}

CConsole::~CConsole()
{
	delete m_MessageStopScan;
}

void CConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//combo �ؼ�
	DDX_Control(pDX, CONSOLE_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, CONSOLE_IDC_COMBO_TARGETADDR, m_ComboTargetAddr);
	DDX_Control(pDX, CONSOLE_IDC_COMBO_SENCE, m_ComboSence);
	//scroll bar �ؼ�
	DDX_Control(pDX, CONSOLE_IDC_SLIDERCTL, m_SliderCtrlConsole);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_BROADCAST, m_CheckBroadCast);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_GROUP, m_CheckGroup);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_AUTOSENCE, m_CheckAutoSence);
	DDX_Control(pDX, CONSOLE_IDC_STATIC_TARGETADRESS, m_ComboTargetAddr);	
	//CEdit�ؼ�
	DDX_Control(pDX, CONSOLE_IDC_EDIT_SENCE, m_EditLightness);
	DDX_Control(pDX, CONSOLE_IDC_EDIT_TIMEINTERVAL, m_EditTimeInterval);


	//Button�ؼ�
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		DDX_Control(pDX, IDC_BUTTON1 + i, m_FrameUI->m_DALIDeviceArray[i].btn);
	}

	
}


BEGIN_MESSAGE_MAP(CConsole, CDialogEx)
	//	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_UP, &CConsole::OnBnClickedIdcButtonUp)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_DOWN, &CConsole::OnBnClickedIdcButtonDown)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_OFF, &CConsole::OnBnClickedIdcButtonOff)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_MAX, &CConsole::OnBnClickedIdcButtonMax)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_MIN, &CConsole::OnBnClickedIdcButtonMin)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_BROADCAST, &CConsole::OnClickedConsoleIdcCheckBoardcast)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_GROUP, &CConsole::OnClickedConsoleIdcCheckGroup)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_AUTOSENCE, OnBnAutoSence)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_GOTOSENCE, OnBnGoToSence)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_ONANDSTEPUP, OnBnOSU)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_OFFANDSTEPDOWN, OnBnSDO)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_QUERY,OnBnQuery)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_SCANDEVICE,OnBnScanDevice)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_DETEDTDEVICE,OnBnDetectDevice)
	ON_COMMAND_RANGE(IDC_BUTTON1,IDC_BUTTON64, OnBnDALIDevice)

//ON_CBN_SETFOCUS(CONSOLE_IDC_COMBO_TARGETADDR, &CConsole::OnSetfocusConsoleIdcComboTargetaddr)
ON_CBN_SELENDOK(CONSOLE_IDC_COMBO_TARGETADDR, &CConsole::OnSelendokConsoleIdcComboTargetaddr)
END_MESSAGE_MAP()


// CConsole ��Ϣ�������
BOOL CConsole::PreTranslateMessage(MSG* pMsg)
{
	/*����ESC�رմ���*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//���λس��رմ��壬���ᵼ�»س��ڴ�����ʧЧ
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter��������*/
		if (GetDlgItem(CONSOLE_IDC_EDIT_SENCE) == GetFocus())
		{
			OnEditBrightness();
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

BOOL CConsole::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//COMBO �ؼ���ʼ��
	char buffer[16] = { 0 };
	
	for(int i = 0;i < 16;i++)
	{ 
		_itoa_s(i,buffer,sizeof(buffer),10);
		m_ComboGroup.InsertString(i,buffer);
	}
	
	for (int i = 0; i < 64; i++) {
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}

	for (int i = 0; i < 16; i++)
	{
		_itoa_s(i, buffer, sizeof(buffer),10);
		m_ComboSence.InsertString(i, buffer);
	}

	SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 0);
	m_SliderCtrlConsole.SetRange(0, 100);
	m_SliderCtrlConsole.SetPos(100);
	/*��ʼ����ѡ��Ĳ���*/
	//NonoptionalPart();

	/*DALI�豸��ť��ʼ������*/
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(false);
	}

	m_ComboTargetAddr.SetCurSel(0);
	
	//return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
	return FALSE;
}



void CConsole::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int pos = m_SliderCtrlConsole.GetPos();    // ��ȡˮƽ��������ǰλ��  

	switch (nSBCode)
	{
		// ����������һ�У���pos��1  
	case SB_LINEUP:
		pos -= 1;
		break;
		// ������ҹ���һ�У���pos��1  
	case SB_LINEDOWN:
		pos += 1;
		break;
		// ����������һҳ����pos��10  
	case SB_PAGEUP:
		pos -= 10;
		break;
		// ������ҹ���һҳ����pos��10  
	case SB_PAGEDOWN:
		pos += 10;
		break;
		// �������������ˣ���posΪ1  
	case SB_TOP:
		pos = 1;
		break;
		// ������������Ҷˣ���posΪ100  
	case SB_BOTTOM:
		pos = 100;
		break;
		// ����϶������������ָ��λ�ã���pos��ֵΪnPos��ֵ  
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// �����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧�������ڴ˴����ñ༭������ʾ��ֵ  
	default:
		SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 100 - pos);
		return;
	}

	// ���ù�����λ��  
	SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 100 - pos);
	SendMessage(100 - pos);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//������ݰ�
/*����
 * sendCache ���ݰ�
 * ParaNum ��������
 * Command ����
 *
 *//*
int App_WifiSendFormat(uint8_t *SendCache,uint8_t ParaNum, uint8_t Command, ...)
{
	
	SendCache[0] = '<';
	SendCache[3] = ParaNum;
	SendCache[4] = 1;
	SendCache[5] = Command;

	uint8_t ParaLength = 6;
	va_list ap;
	va_start(ap, Command);
	while (--ParaNum)
	{
		uint8_t Length = va_arg(ap, uint8_t);
		uint8_t* ParaContent = va_arg(ap, uint8_t*);
		SendCache[ParaLength++] = Length;

		memcpy(SendCache + ParaLength, ParaContent, Length);

		ParaLength += Length;
		ParaContent += sizeof(ParaContent);
	}

	va_end(ap);
	SendCache[1] = (ParaLength + 1) & 0xFF;
	SendCache[2] = 0;

	uint16_t CrcValue;
	CrcValue = Tmgr_GetCrcValue(SendCache + 1, ParaLength - 1);
	memcpy(SendCache + ParaLength, &CrcValue, sizeof(uint16_t));

	SendCache[ParaLength + 2] = '>';

	ParaLength += 3;
	return ParaLength;
	
}

//DALI�����û��������� ͨ�ò���
char masterID[4] = {0x00, 0x00, 0x23, 0xb7};
char password[16] = "self12345678";*/

/*���ܣ����DALI�豸*/
void CConsole::OnBnDALIDevice(UINT ID)
{
	//CButton* pCButton = (CButton*)GetDlgItem(ID);
	//pCButton->
	//AfxMessageageBox(_T("�������"));
}

/*���ܣ������ť�����ȵ���*/
void CConsole::OnBnClickedIdcButtonUp()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = UP;

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("�㲥ģʽ������");
		//AfxMessageageBox(_T("�㲥ģʽ������"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())	
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = UP;
		pConsoleCommand->nGroupID = nGroupID;
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ������");
		//AfxMessageageBox(_T("�鲥ģʽ������"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >=0 )
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = UP;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ������");
		//AfxMessageageBox(_T("����ģʽ,����"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
		return;
	}
}
/*���ܣ������ť�����ȵ���*/
void CConsole::OnBnClickedIdcButtonDown()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = DOWN;

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�㲥ģʽ������");
		//AfxMessageageBox(_T("�㲥ģʽ������"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = DOWN;
		pConsoleCommand->nGroupID = nGroupID;
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ������");
		//AfxMessageageBox(_T("�鲥ģʽ������"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = DOWN;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ������");
		//AfxMessageageBox(_T("����ģʽ,����"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ť�رյ�*/
void CConsole::OnBnClickedIdcButtonOff()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = OFF;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("�㲥ģʽ���ر�");
		//AfxMessageageBox(_T("�㲥ģʽ���ر�"));

	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);

		
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("�鲥ģʽ���ر�");
		//AfxMessageageBox(_T("�鲥ģʽ���ر�"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);

		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("����ģʽ,�ر�");
		//AfxMessageageBox(_T("����ģʽ,�ر�"));
	}
	else
	{
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ť�����ȵ������ֵ*/
void CConsole::OnBnClickedIdcButtonMax()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = MAX;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("�㲥ģʽ�����"));
		m_FrameUI->setStatusBar("�㲥ģʽ�����");
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = MAX;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("�鲥ģʽ�����"));
		m_FrameUI->setStatusBar("�鲥ģʽ�����");
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = MAX;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ�����");
		//AfxMessageageBox(_T("����ģʽ,���"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ�����");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ť�����ȵ�����Сֵ*/
void CConsole::OnBnClickedIdcButtonMin()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = MIN;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�㲥ģʽ����С");
		//AfxMessageageBox(_T("�㲥ģʽ����С"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = MIN;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ����С");
		//AfxMessageageBox(_T("�鲥ģʽ����С"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ����С");
		//AfxMessageageBox(_T("����ģʽ,��С"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ť��������*/
void CConsole::OnBnGoToSence()
{
	int nSenceID = m_ComboSence.GetCurSel();
	if (nSenceID < 0 || nSenceID > 15)
	{
		m_FrameUI->setStatusBar("��ѡ�񳡾�");
		//AfxMessageageBox(_T("��ѡ�񳡾�"));
		return;
	}
	/*�㲥ģʽ*/
	bool bCheck = m_CheckAutoSence.GetCheck();
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nSenceID = nSenceID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�㲥ģʽ����������");
		//AfxMessageageBox(_T("�㲥ģʽ����������"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nGroupID = nGroupID;
		pConsoleCommand->nSenceID = nSenceID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ����������");
		//AfxMessageageBox(_T("�鲥ģʽ����������"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		pConsoleCommand->nSenceID = nSenceID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ����������");
		//AfxMessageageBox(_T("����ģʽ,��������"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ѡ��ťѡ���Զ���������*/
void CConsole::OnBnAutoSence()
{}
/*���ܣ������ťOSU*/
void CConsole::OnBnOSU()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = OSU;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�㲥ģʽ��OSU");
		//AfxMessageageBox(_T("�㲥ģʽ��OSU"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = OSU;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ��OSU");
		//AfxMessageageBox(_T("�鲥ģʽ��OSU"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OSU;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ,OSU");
		//AfxMessageageBox(_T("����ģʽ,OSU"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
/*���ܣ������ťSDO*/
void CConsole::OnBnSDO()
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = SDO;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�㲥ģʽ��SDO");
		//AfxMessageageBox(_T("�㲥ģʽ��SDO"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = SDO;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("�鲥ģʽ��SDO");
		//AfxMessageageBox(_T("�鲥ģʽ��SDO"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = SDO;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("����ģʽ��SDO");
		//AfxMessageageBox(_T("����ģʽ,SDO"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");

		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}


/*���ܣ������ťɨ���豸*/
void CConsole::OnBnScanDevice()
{
	//reset ComboTargetAddr������
	::SendMessage(m_ComboTargetAddr, CB_RESETCONTENT, NULL, NULL);
	//m_FrameUI->setStatusBar("ɨ���豸��");
	
	cacheNode *pcacheNode = new cacheNode();
	pcacheNode->setbuffer("ɨ���豸��", 10);
	m_FrameUI->insertNode(pcacheNode);
	
	CString head = "����ʱ ";
	CString end = " ��";
	CString message;
	for (int i = 0; i < 10; i++){
		message.Format("%s%d%s", head, 10 - i, end);
		cacheNode *pcacheNode = new cacheNode();
		pcacheNode->setbuffer(message.GetBuffer(), 15);
		m_FrameUI->insertNode(pcacheNode);
		pcacheNode->setLength(1000);	
	}
	
	//huobingli
	CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
	pConsoleCommand->nCommand = 0x03;
	pConsoleCommand->pStatusBarCtrl = m_FrameUI->getStatusBarCtrl();

	pConsoleCommand->pMessageStopScan = m_MessageStopScan;

	unsigned char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	//����ȡ��������ID������д�뵽�ṹ����
	memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	//huobingli
	
	//���Ͳ�����BLL����д�������buffer���ڷ��ص����������
	m_FrameUI->sendScan(pConsoleCommand, (char*)buffer);
	//huobingli

	/*
	_scan *pScan = (_scan*)malloc(sizeof(pScan));
	pScan->pMSS = m_MessageStopScan;
	pScan->pSBC = m_FrameUI->getStatusBarCtrl();
	pScan->pAnalCacheTable = analCacheTable;
	handleScan = CreateThread(NULL, 0, Scan, pScan, NULL, 0);
	//hRequestExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//���ֵ���ȴ�����
	//��ʾ�û��������أ��߳��Զ��˳�
	if (m_MessageStopScan->DoModal() == IDOK) {
		m_FrameUI->setStatusBar("������ɣ���"); 
		//ȥȡ����
	}
	else {
		m_FrameUI->setStatusBar("��ȡ����������");
	}
	//�ͷ��̲߳���
	//free(pScan);
	//ɾ�����ζ���
	*/
	delete pConsoleCommand;
	pConsoleCommand = NULL;

	//��ʾ����
	//buffer[0] = 0xff;
	int totalNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)buffer[i];
		//totalNum = totalNum + nVal;
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		totalNum = totalNum + UpdateTag(cVal, i);
	}
	if (totalNum != 0) {
		//��ȡ�������ֵ
		m_FrameUI->setDeviceNum(totalNum);
		//�����������ֵ
		for (int i = 0; i < totalNum; i++) {
			_itoa_s(i, (char*)buffer, sizeof(buffer), 10);
			m_ComboTargetAddr.InsertString(i, (char*)buffer);
		}
		//m_ComboGroup.set
		//��ʾ�豸
		ShowDALIDevice();
		//���������������ֵ������һ��ֵ��ΪĬ��ֵ
		if (m_ComboTargetAddr.GetCount() != 0)
			m_ComboTargetAddr.SetCurSel(0);
		m_FrameUI->setStatusBar("ɨ���豸���");
	}
	else {
		m_FrameUI->setStatusBar("û��ɨ�赽�豸");
	}
	
}
/*���ܣ������ť����豸*/
void CConsole::OnBnDetectDevice()
{
	m_FrameUI->setStatusBar("ɨ���豸��");
	
	CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();

	//����ȡ��������ID������д�뵽�ṹ����
	memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	pConsoleCommand->nCommand = 0x03;

	unsigned char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	//huobingli
	m_FrameUI->detectScan(pConsoleCommand, (char*)buffer);
	//huobingli

	delete pConsoleCommand;
	pConsoleCommand = NULL;

	int totalNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)buffer[i];
		totalNum = totalNum + nVal;
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	m_FrameUI->setDeviceNum(totalNum);

	/*for (int i = 0; i < totalNum; i++) {
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}*/
	//m_ComboGroup.set
	ShowDALIDevice();
	m_FrameUI->setStatusBar("ɨ���豸���");

}
/*���ܣ������ѡ��ť�Ƿ�ѡ��㲥ģʽ*/
void CConsole::OnClickedConsoleIdcCheckBoardcast()
{
	if (m_CheckBroadCast.GetCheck())
	{
		m_CheckBroadCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		OptionalPart();
		m_FrameUI->setStatusBar("��ѡ��㲥ģʽ");
	}
	else
	{
		m_CheckBroadCast.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		//NonoptionalPart();
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
/*���ܣ������ѡ��ť�Ƿ�ѡ���鲥ģʽ*/
void CConsole::OnClickedConsoleIdcCheckGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckBroadCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboGroup.SetCurSel(0);
		m_ComboTargetAddr.EnableWindow(false);
		OptionalPart();
		m_FrameUI->setStatusBar("��ѡ���鲥ģʽ");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		//NonoptionalPart();
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}

void CConsole::OnEditBrightness()
{
	int nBrightness = GetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, NULL, TRUE);
	if (nBrightness < 0 || nBrightness > 100)
	{
		m_FrameUI->setStatusBar("������0-100��ֵ");
		//AfxMessageageBox(_T("������0-100��ֵ"));
	}
	else
	{
		SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, nBrightness);
	}
	m_SliderCtrlConsole.SetPos(100 - nBrightness);
}

void CConsole::OnBnQuery()
{
	STATUS* pStatus = new STATUS();

	//memcpy();
	ShowStatus(pStatus);
	delete pStatus;
	pStatus = NULL;
}


/*private*/
void CConsole::SendMessage(int nBrightness)
{
	/*�㲥ģʽ*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = 0xff;
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//pConsoleCommand->nDeviceAddr = 0xFE;

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);
		m_FrameUI->sendChangeLightness(pConsoleCommand);
		CString baseStatus = "�㲥ģʽ������  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;

		////AfxMessageageBox(_T("�㲥ģʽ��ֱ�ӵ���"));
	}
	/*�鲥ģʽ*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//�鲥ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = 0x80 + m_ComboGroup.GetCurSel();
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;
		pConsoleCommand->nGroupID = nGroupID;

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);

		m_FrameUI->sendChangeLightness(pConsoleCommand);

		CString baseStatus = "�鲥ģʽ������  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("�鲥ģʽ��ֱ�ӵ���"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = m_ComboTargetAddr.GetCurSel();
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//����ȡ��������ID������д�뵽�ṹ����
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);

		m_FrameUI->sendChangeLightness(pConsoleCommand);
		
		CString baseStatus = "����ģʽ������  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("����ģʽ,ֱ�ӵ���"));
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
	}
}
void CConsole::NonoptionalPart()
{
	m_ComboGroup.EnableWindow(false);
	//m_ComboTargetAddr.EnableWindow(false);
	m_ComboSence.EnableWindow(false);
	m_SliderCtrlConsole.EnableWindow(false);
	m_EditLightness.EnableWindow(false);
	m_EditTimeInterval.EnableWindow(false);
	GetDlgItem(CONSOLE_IDC_CHECK_AUTOSENCE)->EnableWindow(false);
}
void CConsole::OptionalPart()
{
	m_ComboSence.EnableWindow(true);
	m_SliderCtrlConsole.EnableWindow(true);
	m_EditLightness.EnableWindow(true);
	m_EditTimeInterval.EnableWindow(true);
	GetDlgItem(CONSOLE_IDC_CHECK_AUTOSENCE)->EnableWindow(true);
}
void CConsole::ShowStatus(STATUS* pStatus)
{
	SetDlgItemInt(IDC_EDIT_DEVICETYPE, pStatus->nDeviceType);
	SetDlgItemInt(IDC_EDIT_STATUSOFBALLAST,pStatus->nStatusOfBallast);
	SetDlgItemInt(IDC_EDIT_VERSION,pStatus->nVersion);
	SetDlgItemInt(IDC_EDIT_LAMPFAILURE,pStatus->nLampFailure);
	SetDlgItemInt(IDC_EDIT_EDIT_LAMPARCPOWERON,pStatus->nLampArcPowerOn);
	SetDlgItemInt(IDC_EDIT_LIMITERROR,pStatus->nLimitError);
	SetDlgItemInt(IDC_EDIT_FADEREADY,pStatus->nFadeReady);
	SetDlgItemInt(IDC_EDIT_RESETSTATE,pStatus->nResetState);
	SetDlgItemInt(IDC_EDIT_MISSINGSHORTADDRESS,pStatus->nMissingShortAddress);
	SetDlgItemInt(IDC_EDIT_POWERFAILURE,pStatus->nPowerFailure);
}
void CConsole::ShowDALIDevice()
{
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		if (m_FrameUI->m_DALIDeviceArray[i].nTag == 1)
			m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(true);
		else
			m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(false);
	}
}
int CConsole::UpdateTag(char* pBuf,int nCycleTime)
{
	int deviceNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (pBuf[i] == '1')
		{
			m_FrameUI->m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 1;
			++deviceNum;
		}
		else
		{
			m_FrameUI->m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 0;
		}
	}
	
	return deviceNum;
}


void CConsole::OnSelendokConsoleIdcComboTargetaddr()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_ComboTargetAddr.GetCurSel() > 0)  {
		m_FrameUI->setStatusBar("��ѡ�񵥲�ģʽ");
		OptionalPart();
	}
}
