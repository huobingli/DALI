// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "GetIPDlg.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_DIALOG8, pParent)
{
	m_cPwd[PWD_LEN] = { '\0' };
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LOGIN_IDC_EDIT_PWD, m_EditPwd);
	DDX_Control(pDX, LOGIN_IDC_BUTTON_LOGIN, m_BtnLogin);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_CONTROL(EN_KILLFOCUS, LOGIN_IDC_EDIT_PWD, OnEditWrite)
	ON_BN_CLICKED(LOGIN_IDC_BUTTON_LOGIN, OnBtnLogin)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	/*����ESC�رմ���*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//���λس��رմ��壬���ᵼ�»س��ڴ�����ʧЧ
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter��¼*/
		CLoginDlg::OnEditWrite();
		CLoginDlg::OnBtnLogin();
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}
void CLoginDlg::OnEditWrite()
{
	CString sPwd;
	GetDlgItemText(LOGIN_IDC_EDIT_PWD, sPwd);
	memcpy(m_cPwd, sPwd.GetBuffer(), PWD_LEN);
}
void CLoginDlg::OnBtnLogin()
{
	if (checkPwd(m_cPwd))
	{
		this->ShowWindow(SW_HIDE);
		CGetIPDlg dlg;
		dlg.DoModal();
		SendMessage(WM_CLOSE);
	}
	else
	{
		ClearPwd();
		//AfxMessageageBox(_T("��¼ʧ�ܣ�"));
		m_EditPwd.SetFocus();
	}
}
void CLoginDlg::ClearPwd()
{
	SetDlgItemText(LOGIN_IDC_EDIT_PWD, "");
	m_cPwd[PWD_LEN] = { '\0' };
}
BOOL CLoginDlg::checkPwd(char* cPwd)
{
	int nPos = 0;
	if (*(cPwd + nPos) == '\0')
	{
		return FALSE;
	}
	while (*(cPwd + nPos) != '\0')
	{
		if (*(cPwd + nPos) != cCheckPwd[nPos])
			return FALSE;
		++nPos;
	}
	return TRUE;
}
