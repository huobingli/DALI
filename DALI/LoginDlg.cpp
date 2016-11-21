// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "GetIPDlg.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

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


// CLoginDlg 消息处理程序
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，但会导致回车在窗体上失效
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter登录*/
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
		//AfxMessageageBox(_T("登录失败！"));
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
