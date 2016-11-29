// TParameters.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "TParameters.h"
#include "afxdialogex.h"



// CTParameters �Ի���

IMPLEMENT_DYNAMIC(CTParameters, CDialogEx)

CTParameters::CTParameters(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTParameters::IDD, pParent)
{
	m_FrameUI = pUI;
}

CTParameters::~CTParameters()
{
}

void CTParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTParameters, CDialogEx)
	ON_BN_CLICKED(TT_IDC_BUTTON_READ, &CTParameters::OnBnClickedIdcButtonRead)
	ON_BN_CLICKED(TT_IDC_BUTTON_SAVE, &CTParameters::OnBnClickedIdcButtonSave)
END_MESSAGE_MAP()


// CTParameters ��Ϣ�������
BOOL CTParameters::PreTranslateMessage(MSG* pMsg)
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
			//OnEditBrightness();
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}


void CTParameters::OnBnClickedIdcButtonRead()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//��װ����ṹ
	_tparameters_command* pTParamCommand = new _tparameters_command();
	pTParamCommand->cMode = 0x00;
	pTParamCommand->nCommand = 0x00;

	//��������ID������
	memcpy(pTParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pTParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));

	m_FrameUI->setStatusBar("���ڶ�ȡ������... ���Ե�... ");
	//huobingli
	m_FrameUI->readTParamSaveConf(pTParamCommand, (char*)cBuf);
	//huobingli
	//ShowEdit(cBuf);
	m_FrameUI->setStatusBar("��ȡ���óɹ�!");
	delete pTParamCommand;
	pTParamCommand = NULL;

	
}


void CTParameters::OnBnClickedIdcButtonSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
