#pragma once

#define PWD_LEN	16+1
// CLoginDlg 对话框
const char cCheckPwd[PWD_LEN] = { "self12345678" };
class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	char	m_cPwd[PWD_LEN];		//密码字符串对象
	CEdit   m_EditPwd;				//密码编辑框对象
	CButton	m_BtnLogin;				//登录按钮对象
public:
	afx_msg void OnEditWrite();		//输入密码函数
	afx_msg void OnBtnLogin();		//登录函数
private:
	void ClearPwd();			//清空编辑框函数
	BOOL checkPwd(char* cPwd);
};
