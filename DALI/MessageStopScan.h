#pragma once
#include "Public.h"
#include "resource.h"
// CMessageStopScan �Ի���

//static HANDLE hRequestExitEvent = FALSE;

class CMessageStopScan : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageStopScan)

public:
	CMessageStopScan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageStopScan();

// �Ի�������
	enum { IDD = IDD_DIALOG_STOPSCAN};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
};
