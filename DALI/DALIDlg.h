
// DALIDlg.h : 头文件
//

#pragma once
//#include "Show.h"
#include "Group.h"
#include "Console.h"
#include "Sence.h"
#include "Parameters.h"
#include "ControllerShow.h"
#include "GlobalGroup.h"

#include <conio.h>

//#include "ControllerParam.h"

//#include "FrameUI.h"

// CDALIDlg 对话框
class CDALIDlg : public CDialogEx
{
// 构造
public:
	CDALIDlg(CFrameUI *pUI, CWnd* pParent = NULL);	// 标准构造函数
	~CDALIDlg();
// 对话框数据
	enum { IDD = IDD_DALI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	
private:
	CTabCtrl			m_TabCtrl;
	
	CFrameUI			*m_FrameUI;
	CGlobalGroup		*m_GlobalGroup;
	CConsole			*m_ConsoleDlg;
	CGroup				*m_GroupDlg;
	CSence				*m_SenceDlg;
	CParameters			*m_ParameterDlg;
	CControllerShow		*m_ControllerShowDlg;
	CStatic				m_IP;
	
public:
	CString				m_sIP;
	CString				m_sPass;
};
