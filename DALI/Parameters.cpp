// Parameters.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "Parameters.h"
#include "afxdialogex.h"


// CParameters �Ի���

IMPLEMENT_DYNAMIC(CParameters, CDialogEx)

CParameters::CParameters(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CParameters::IDD, pParent)
{
	m_FrameUI = pUI;
}

CParameters::~CParameters()
{
}

void CParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_BOARDCAST, m_CheckBoardCast);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_GROUP, m_CheckGroup);
	DDX_Control(pDX, PARAMETER_IDC_COMBO_ADDRESS, m_ComboTargetAddr);
	DDX_Control(pDX, PARAMETER_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_ADDRESS, m_CheckTaget);
	for (int i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, PARAMETER_IDC_EDIT_POWER + i, m_Edit[i].Edit);
	}
}

BOOL CParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	char buffer[16] = { 0 };
	for (int i = 0; i < 16; ++i)
	{
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboGroup.InsertString(i, buffer);
	}
	for (int i = 0; i < 64; ++i)
	{
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}

	m_ComboTargetAddr.EnableWindow(false);
	m_ComboGroup.EnableWindow(false);
	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CParameters, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnBnRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnSave)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_ADDRESS, OnClickedTaget)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_BOARDCAST, OnClickedBoardcast)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_GROUP,OnClickedGroup)
	ON_CONTROL_RANGE(EN_KILLFOCUS,PARAMETER_IDC_EDIT_POWER, PARAMETER_IDC_EDIT_FADETIME,OnEditWrite)	
END_MESSAGE_MAP()


// CParameters ��Ϣ�������
/*��д�༭����*/
void CParameters::OnEditWrite(UINT ID)
{
	int nPos = ID - PARAMETER_IDC_EDIT_POWER;
	m_Edit[nPos].cVal = (char)GetDlgItemInt(ID);
}
/*ͨ����ť��ȡ*/
void CParameters::OnBnRead()
{
	int nTargetAddr = m_ComboTargetAddr.GetCurSel();
	if (nTargetAddr == -1)
	{
		//AfxMessageageBox(_T("��ѡ��Address��"));
		return;
	}
	//��װ����ṹ
	PARAM_COMMAND* pParamCommand = new PARAM_COMMAND();
	pParamCommand->cMode = TARGET_MODE;
	pParamCommand->nCommand = PARAM_READ;
	pParamCommand->nDALIID = nTargetAddr;

	//��������ID������
	memcpy(pParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));

	m_FrameUI->setStatusBar("���ڶ�ȡ������... ���Ե�... ");
	//huobingli
	m_FrameUI->readParamSaveConf(pParamCommand, (char*)cBuf);
	//huobingli
	ShowEdit(cBuf);
	m_FrameUI->setStatusBar("��ȡ���óɹ�!");
	delete pParamCommand;
	pParamCommand = NULL;

}
/*ͨ����ť����*/
void CParameters::OnBnSave()
{
	PARAM_COMMAND* pParamCommand = new PARAM_COMMAND();
	pParamCommand->nCommand = PARAM_SAVE;

	//��������ID������
	memcpy(pParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	/*�㲥ģʽ*/
	if (m_CheckBoardCast.GetCheck())
	{
		pParamCommand->cMode = BROADCAST_MODE;
	}
	else if(m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();
		if (nGroupID < 0 || nGroupID>15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
			return;
		}
		pParamCommand->cMode = GROUP_MODE;
		pParamCommand->nGroupID = nGroupID;
	}
	else if (m_CheckTaget.GetCheck())
	{
		int nDALIID = m_ComboTargetAddr.GetCurSel();
		if (nDALIID < 0 || nDALIID>63)
		{
			m_FrameUI->setStatusBar("������0-63��ֵ");
			//AfxMessageageBox(_T("������0-63��ֵ"));
			return;
		}
		pParamCommand->cMode = TARGET_MODE;
		pParamCommand->nDALIID = nDALIID;
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
		return;
	}
	char cBuf[7];
	for (int i = 0; i < 6; ++i)
	{
		memcpy(cBuf + i, &m_Edit[i].cVal, 1);
	}
	//���͸�BLL��
	m_FrameUI->setStatusBar("����������... ���Ժ�...");
	m_FrameUI->sendParamSaveConf(pParamCommand, cBuf);
	m_FrameUI->setStatusBar("�������óɹ�!");
	//AfxMessageageBox(_T("���ͳɹ���"));
}
/*�����ѡ��ť�Ƿ�ѡ��㲥ģʽ*/
void CParameters::OnClickedBoardcast()
{
	if (m_CheckBoardCast.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_FrameUI->setStatusBar("��ѡ��㲥ģʽ");
	}
	else
	{
		m_CheckBoardCast.SetCheck(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
/*�����ѡ��ť�Ƿ�ѡ���鲥ģʽ*/
void CParameters::OnClickedGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_ComboGroup.SetCurSel(0);
		m_FrameUI->setStatusBar("��ѡ�񵥲�ģʽ");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboGroup.EnableWindow(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
/*�����ѡ��ť�Ƿ�ѡ�񵥲�ģʽ*/
void CParameters::OnClickedTaget()
{
	if (m_CheckTaget.GetCheck())
	{
		m_CheckTaget.SetCheck(true);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		m_ComboGroup.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(true);
		m_ComboTargetAddr.SetCurSel(0);
		m_FrameUI->setStatusBar("��ѡ�񵥲�ģʽ");
	}
	else
	{
		m_CheckTaget.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
void CParameters::ShowEdit(unsigned char* cBuf)
{
	int nTag = 0;
	for (int i = 0; i < 32; ++i)
	{
		if (i == 5)
		{
			int nFadeTime = cBuf[i] & 0x0F;
			int nFadeRate = cBuf[i] >> 4 & 0x0F;
			SetDlgItemInt(PARAMETER_IDC_EDIT_FADERATE, nFadeRate);
			SetDlgItemInt(PARAMETER_IDC_EDIT_FADETIME, nFadeTime);
			nTag = 1;
		}
		else
		{
			SetDlgItemInt(PARAMETER_IDC_EDIT_ACTUAL + nTag + i, (int)cBuf[i]);
		}
	}
}