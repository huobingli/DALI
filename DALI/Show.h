#pragma once
#include "Console.h"

// CShow 对话框

/*DALI信息结构*/
typedef struct _dali_info
{
	char cIP[20];
	char cName[20];
	int  nDALIID;
}DALI_INFO,*PDALI_INFO;

class CShow : public CDialogEx
{
	DECLARE_DYNAMIC(CShow)

public:
	CShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual void OnOk();	
	virtual ~CShow();

//对话框数据
	enum { IDD = IDD_DIALOG0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnButtonGetIP();
private:
	CListCtrl	m_ListCtrl;
	int i;
public:
	afx_msg void OnLvnItemchangedListIp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
};
