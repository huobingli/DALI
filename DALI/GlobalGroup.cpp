// GlobalGroup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "GlobalGroup.h"
#include "afxdialogex.h"


// CGlobalGroup �Ի���

IMPLEMENT_DYNAMIC(CGlobalGroup, CDialogEx)

CGlobalGroup::CGlobalGroup(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CGlobalGroup::IDD, pParent)
{
	m_FrameUI = pUI;

	m_pGGCommand = new _globalgroup_command();
}

CGlobalGroup::~CGlobalGroup()
{
}

void CGlobalGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, GG_IDC_BUTTON_SAVE, m_ButtonSaveConfig);
	DDX_Control(pDX, GG_IDC_BUTTON_READ, m_ButtonReadConfig);
	DDX_Control(pDX, GG_IDC_EDIT_ZEROSEVEN, m_EditLeft);
	DDX_Control(pDX, GG_IDC_EDIT_EIGHTFIFTEEN, m_EditRight);
	for (int i = 0; i < GROUP_NUM; i++)
	{
		DDX_Control(pDX, GG_IDC_CHECK_0 + i, m_GroupCheck[i].CheckButton);
	}
}


BEGIN_MESSAGE_MAP(CGlobalGroup, CDialogEx)
	ON_BN_CLICKED(GG_IDC_BUTTON_READ, &CGlobalGroup::OnBnClickedIdcButtonRead)
	ON_BN_CLICKED(GG_IDC_BUTTON_SAVE, &CGlobalGroup::OnBnClickedIdcButtonSave)
	ON_COMMAND_RANGE(GG_IDC_CHECK_0, GG_IDC_CHECK_15, OnClickedCheck)
END_MESSAGE_MAP()
BOOL CGlobalGroup::PreTranslateMessage(MSG* pMsg)
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
		GetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
		GetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);
		StringToConfigInt(sLeft, sRight, cBuf);
		ShowCheck();
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

// CGlobalGroup ��Ϣ�������


BOOL CGlobalGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	/*�༭���ʼ��*/
	m_EditLeft.SetWindowText(_T("00000000"));
	m_EditRight.SetWindowText(_T("00000000"));
	//��ʾ�༭�������ݵ�checkbox
	ShowCheck();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

GROUP_CHECK* CGlobalGroup::GetGroupCheckByCtrlID(UINT ID)
{
	for (int i = 0; i < GROUP_NUM; i++)
	{
		if (m_GroupCheck[i].CheckButton.GetDlgCtrlID() == ID)
			return &m_GroupCheck[i];
	}
	return NULL;
}
/*���ݵ�ѡ��ť�ؼ�ID����ʾ�ؼ�״̬�Լ���ѡ��ť�ṹ����ֵ*/
void CGlobalGroup::OnClickedCheck(UINT ID)
{
	GROUP_CHECK* pGroupCheck = GetGroupCheckByCtrlID(ID);

	if (pGroupCheck == NULL)
	{
		m_FrameUI->setStatusBar("�����ڸ��鰴ť");
		return;
	}
	if (pGroupCheck->CheckButton.GetCheck())
	{
		pGroupCheck->CheckButton.SetCheck(false);
		pGroupCheck->bTag = 0;
		m_FrameUI->setStatusBar("δѡ��");
	}
	else
	{
		pGroupCheck->CheckButton.SetCheck(true);
		pGroupCheck->bTag = 1;
		m_FrameUI->setStatusBar("��ѡ��");
	}
	ShowEdit();
}

void CGlobalGroup::OnBnClickedIdcButtonRead()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��������ID������
	memcpy(m_pGGCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(m_pGGCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
	
	//���������ģʽ
	m_pGGCommand->cMode = 0xFF;
	m_pGGCommand->cCommand = 0x12;
	
	CString sLeft;
	CString sRight;
	unsigned char buf[3];
	memset(buf, 0, sizeof(buf));
	m_FrameUI->setStatusBar("��ȡ������... ���Ժ�...");
	m_FrameUI->readGlobalGroupSaveConf(m_pGGCommand, (char*)buf);
	
	//ת��
	ConfigIntToString(sLeft, sRight, buf);
	//��ʾ
	ShowCheck();
	ShowEdit();

	m_FrameUI->setStatusBar("��ȡ���óɹ�!");
}


void CGlobalGroup::OnBnClickedIdcButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString sLeft;
	CString sRight;
	memset(m_pGGCommand, 0, sizeof(_globalgroup_command));
	
	//��������ID������
	memcpy(m_pGGCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(m_pGGCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	m_pGGCommand->cCommand = 0x11;
	m_pGGCommand->cMode = 0xFF;
	GetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
	GetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);

	/*��sLeft��sRightת����int*/
	StringToConfigInt(sLeft, sRight, m_pGGCommand->cBuf);
	ShowCheck();
	/*��������Ϣ����BLL��*/

	m_FrameUI->setStatusBar("�������ò�����... ���Ժ�...");
	//huobingli
	m_FrameUI->sendGlobalGroupSaveConf(m_pGGCommand);
	//huobinlgi

	CString s;
	s.Format("�ѷ������ò���Ϊ%s", m_pGGCommand->cBuf);

	m_FrameUI->setStatusBar(s);
}


/*��������ת�����ַ���*/
void CGlobalGroup::ConfigIntToString(CString &sLeft, CString &sRight, unsigned char* buf)
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
void CGlobalGroup::StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf)
{
	unsigned short nLeft = _tcstol(sLeft, NULL, 2) << 8;
	unsigned short nRight = _tcstol(sRight, NULL, 2);
	buf[0] = nLeft >> 8;
	buf[1] = nRight;
	char* pLBuf = sLeft.GetBuffer();
	char* pRBuf = sRight.GetBuffer();
	UpdateTag(pLBuf, pRBuf);
}

/*��ʾ��ѡ��ť*/
void CGlobalGroup::ShowCheck()
{
	for (int i = 0; i < GROUP_NUM; i++)
	{
		if (m_GroupCheck[i].bTag == 1)
			m_GroupCheck[i].CheckButton.SetCheck(true);
		else
			m_GroupCheck[i].CheckButton.SetCheck(false);
	}
}
/*��ʾ�༭��*/
void CGlobalGroup::ShowEdit()
{
	char cLBuf[9] = { 0 };
	char cRBuf[9] = { 0 };
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
	SetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
	SetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);
}
/*����������Ϣ�е�Tag��־*/
void CGlobalGroup::UpdateTag(char* pLBuf, char* pRBuf)
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
void CGlobalGroup::EightBitCheck(CString &s, char* &c)
{
	int nLen = strlen(c);
	if (nLen < 8)
	{
		char cTemp[9] = { '\0' };
		int nVal = 8 - nLen;
		memset(cTemp, '0', nVal);
		memcpy(cTemp + nVal, c, nLen);
		s.Format("%s", cTemp);
		memcpy(c, cTemp, 8);
	}
}