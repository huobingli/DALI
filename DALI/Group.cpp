// Group.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "Group.h"
#include "afxdialogex.h"
#include "FrameUI.h"
#include <cstring>


// CGroup �Ի���

IMPLEMENT_DYNAMIC(CGroup, CDialogEx)

CGroup::CGroup(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroup::IDD, pParent)
{
	m_FrameUI = pUI;
}

CGroup::~CGroup()
{
}

void CGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, GROUP_IDC_COMBO_ADDRESS, m_ComboBoxAdress);
	DDX_Control(pDX, GROUP_IDC_BUTTON_SAVECONFIG, m_ButtonSaveConfig);
	DDX_Control(pDX, GROUP_IDC_BUTTON_READCONFIG, m_ButtonReadConfig);
	DDX_Control(pDX, GROUP_IDC_EDIT_ZEROSEVEN, m_EditLeft);
	DDX_Control(pDX, GROUP_IDC_EDIT_EIGHTFIFTEEN, m_EditRight);
	for (int i = 0; i < GROUP_NUM; i++)
	{
		DDX_Control(pDX, GROUP_IDC_CHECK_0+i, m_GroupCheck[i].CheckButton);
	}
}


BEGIN_MESSAGE_MAP(CGroup, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGroup::OnCbnSelchangeCombo1)
	ON_COMMAND_RANGE(GROUP_IDC_CHECK_0, GROUP_IDC_CHECK_15, OnClickedCheck)
	ON_BN_CLICKED(GROUP_IDC_BUTTON_SAVECONFIG,OnBtnSaveConfig)
	ON_BN_CLICKED(GROUP_IDC_BUTTON_READCONFIG,OnBtnReadConfig)
END_MESSAGE_MAP()


BOOL CGroup::PreTranslateMessage(MSG* pMsg)
{
	/*����ESC�رմ���*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//���λس��رմ��壬
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		CString sLeft;
		CString sRight;
		unsigned char cBuf[3] = { '\0' };
		GetDlgItemText(GROUP_IDC_EDIT_ZEROSEVEN, sLeft);
		GetDlgItemText(GROUP_IDC_EDIT_EIGHTFIFTEEN, sRight);
		StringToConfigInt(sLeft, sRight, cBuf);
		ShowCheck();
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

// CGroup ��Ϣ�������


void CGroup::OnCbnSelchangeCombo1()
{
}


BOOL CGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char buffer[5] = { 0 };
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for (int i = 0; i < 64; i++)
	{
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboBoxAdress.InsertString(i, buffer);
	}

	m_ComboBoxAdress.SetCurSel(0);

	/*�༭���ʼ��*/
	m_EditLeft.SetWindowText(_T("00000000"));
	m_EditRight.SetWindowText(_T("00000000"));
	ShowCheck();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

/*���ݵ�ѡ��ť�ؼ�ID����ʾ�ؼ�״̬�Լ���ѡ��ť�ṹ����ֵ*/
void CGroup::OnClickedCheck(UINT ID)
{
	GROUP_CHECK* pGroupCheck = GetGroupCheckByCtrlID(ID);

	if (pGroupCheck == NULL)
	{
		m_FrameUI->setStatusBar("�����ڸó�����ť");
		return;
	}
	if (pGroupCheck->CheckButton.GetCheck())
	{
		pGroupCheck->CheckButton.SetCheck(false);
		pGroupCheck->bTag = 0;
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
	else
	{
		pGroupCheck->CheckButton.SetCheck(true);
		pGroupCheck->bTag = 1;
		m_FrameUI->setStatusBar("��ѡ���鲥ģʽ");
	}
	ShowEdit();
}
/*�����ť����������Ϣ��ʾ�ڶ�Ӧ�༭�����Լ���Ӧ��ѡ��״̬*/
void CGroup::OnBtnSaveConfig()
{
	int nAddress = m_ComboBoxAdress.GetCurSel();
	if (nAddress == -1)
	{
		//AfxMessageageBox(_T("����ѡ��Address!"));
		return;
	}
	CString sLeft;
	CString sRight;
	CONFIG_INFO* pConfigInfo = new CONFIG_INFO();

	//��������ID������
	memcpy(pConfigInfo->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConfigInfo->DevicePWD, m_FrameUI->DevicePWD, 16);

	pConfigInfo->cMode = GROUP_SAVECONFIG;
	GetDlgItemText(GROUP_IDC_EDIT_ZEROSEVEN, sLeft);
	GetDlgItemText(GROUP_IDC_EDIT_EIGHTFIFTEEN, sRight);

	/*��sLeft��sRightת����int*/
	StringToConfigInt(sLeft, sRight, pConfigInfo->cBuf);
	pConfigInfo->nAddress = m_ComboBoxAdress.GetCurSel();
	ShowCheck();
	/*��������Ϣ����BLL��*/

	m_FrameUI->setStatusBar("�������ò�����... ���Ժ�...");
	//huobingli
	m_FrameUI->sendGroupSaveConf(pConfigInfo);
	//huobinlgi

	CString s;
	s.Format("�ѷ������ò���Ϊ%s���ݵ�ַΪ%d", pConfigInfo->cBuf,pConfigInfo->nAddress);

	m_FrameUI->setStatusBar(s);
	delete pConfigInfo;
	pConfigInfo = NULL;
	//AfxMessageageBox(s);
}

/*�����ť��ȡ������Ϣ��ʾ�ڶ�Ӧ�༭�����Լ���Ӧ��ѡ��״̬*/
void CGroup::OnBtnReadConfig()
{
	int nAddress = m_ComboBoxAdress.GetCurSel();
	if (nAddress == -1)
	{
		//AfxMessageageBox(_T("����ѡ��Address!"));
		return;
	}
	//��װ����ݸ�BLL
	CONFIG_INFO* pConfigInfo = new CONFIG_INFO();
	pConfigInfo->cMode = GROUP_READCONFIG;
	pConfigInfo->nAddress = nAddress;
	
	//��������ID������
	memcpy(pConfigInfo->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConfigInfo->DevicePWD, m_FrameUI->DevicePWD, 16);

	CString sLeft;
	CString sRight;

	//BLL->�ӿ�(pConfigInfo);
	unsigned char buf[3];
	memset(buf, 0, sizeof(buf));
	m_FrameUI->setStatusBar("��ȡ������... ���Ժ�...");
	m_FrameUI->readGroupSaveConf(pConfigInfo, (char*)buf);

	ConfigIntToString(sLeft, sRight, buf);
	
	ShowCheck();
	ShowEdit();

	m_FrameUI->setStatusBar("��ȡ���óɹ�!");
	delete pConfigInfo;
	pConfigInfo = NULL;
}

/*private*/
/*���ݿؼ�ID�����Ҷ�Ӧ�ĵ�ѡ��ť�ṹ*/
GROUP_CHECK* CGroup::GetGroupCheckByCtrlID(UINT ID)
{
	for (int i = 0; i < GROUP_NUM; i++)
	{
		if (m_GroupCheck[i].CheckButton.GetDlgCtrlID() == ID)
			return &m_GroupCheck[i];
	}
	return NULL;
}
/*��������ת�����ַ���*/
void CGroup::ConfigIntToString(CString &sLeft, CString &sRight, unsigned char* buf)
{
	int nLeft = (int)buf[0];
	int nRight = (int)buf[1];
	/*int ת�ַ���*/
	itoa(nLeft, sLeft.GetBuffer(8), 2);
	itoa(nRight, sRight.GetBuffer(8), 2);
	char* pLBuf = sLeft.GetBuffer();
	EightBitCheck(sLeft, pLBuf);
	char* pRBuf = sRight.GetBuffer();
	EightBitCheck(sRight, pRBuf);
	/*��д��ѡ��ť�ṹ����tag��־*/
	UpdateTag(pLBuf, pRBuf);
}
/*�ַ���ת������������*/
void CGroup::StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf)
{
	unsigned short nLeft = _tcstol(sLeft,NULL,2) << 8;
	unsigned short nRight = _tcstol(sRight, NULL, 2);
	buf[0] = nLeft>>8;
	buf[1] = nRight;
	char* pLBuf = sLeft.GetBuffer();
	char* pRBuf = sRight.GetBuffer();
	UpdateTag(pLBuf, pRBuf);
}

/*��ʾ��ѡ��ť*/
void CGroup::ShowCheck()
{
	for(int i=0 ;i<GROUP_NUM;i++)
	{
		if (m_GroupCheck[i].bTag == 1)
			m_GroupCheck[i].CheckButton.SetCheck(true);
		else
			m_GroupCheck[i].CheckButton.SetCheck(false);
	}
}
/*��ʾ�༭��*/
void CGroup::ShowEdit()
{
	char cLBuf[9] = {0};
	char cRBuf[9] = {0};
	/*left*/
	for (int i = 0; i < 8; i++)
	{
		cLBuf[7 - i] = m_GroupCheck[i].bTag + 48;
	}
	CString sLeft(cLBuf);
	/*right*/
	for (int i = 8; i < 16; i++)
	{
		cRBuf[15 - i] = m_GroupCheck[i].bTag + 48;
	}
	CString sRight(cRBuf);
	SetDlgItemText(GROUP_IDC_EDIT_ZEROSEVEN, sLeft);
	SetDlgItemText(GROUP_IDC_EDIT_EIGHTFIFTEEN, sRight);
}
/*����������Ϣ�е�Tag��־*/
void CGroup::UpdateTag(char* pLBuf,char* pRBuf)
{
	for (int i = 0; i < 8; i++)
	{
		if (pLBuf[i] == '1')
		{
			m_GroupCheck[7 - i].bTag = 1;
		}
		else
		{
			m_GroupCheck[7 - i].bTag = 0;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (pRBuf[i] == '1')
		{
			m_GroupCheck[15 - i].bTag = 1;
		}
		else
		{
			m_GroupCheck[15 - i].bTag = 0;
		}
	}
}
/*8λ��⺯��*/
void CGroup::EightBitCheck(CString &s,char* &c)
{
	int nLen = strlen(c);
	if (nLen < 8)
	{
		char cTemp[9] = {'\0'};
		int nVal = 8 - nLen;
		memset(cTemp, '0',nVal);
		memcpy(cTemp + nVal, c, nLen);
		s.Format("%s", cTemp);
		memcpy(c, cTemp, 8);
	}
}
