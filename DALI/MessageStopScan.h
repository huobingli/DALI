#pragma once
#include "Public.h"
#include "resource.h"
// CMessageStopScan 对话框

//static HANDLE hRequestExitEvent = FALSE;

class CMessageStopScan : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageStopScan)

public:
	CMessageStopScan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageStopScan();

// 对话框数据
	enum { IDD = IDD_DIALOG_STOPSCAN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
};
