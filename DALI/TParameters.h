#pragma once

#include "FrameUI.h"
#include "def.h"
// CTParameters �Ի���

class CTParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CTParameters)

public:
	CTParameters(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTParameters();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIdcButtonRead();
	afx_msg void OnBnClickedIdcButtonSave();

private:
	CFrameUI *m_FrameUI;
};
