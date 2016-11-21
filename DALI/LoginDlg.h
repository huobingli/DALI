#pragma once

#define PWD_LEN	16+1
// CLoginDlg �Ի���
const char cCheckPwd[PWD_LEN] = { "self12345678" };
class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	char	m_cPwd[PWD_LEN];		//�����ַ�������
	CEdit   m_EditPwd;				//����༭�����
	CButton	m_BtnLogin;				//��¼��ť����
public:
	afx_msg void OnEditWrite();		//�������뺯��
	afx_msg void OnBtnLogin();		//��¼����
private:
	void ClearPwd();			//��ձ༭����
	BOOL checkPwd(char* cPwd);
};
