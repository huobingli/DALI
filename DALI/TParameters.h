#pragma once

#include "FrameUI.h"
#include "def.h"
// CTParameters 对话框

class CTParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CTParameters)

public:
	CTParameters(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTParameters();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIdcButtonRead();
	afx_msg void OnBnClickedIdcButtonSave();

private:
	CFrameUI *m_FrameUI;
};
