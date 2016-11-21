
// DALIDlg.h : ͷ�ļ�
//

#pragma once
//#include "Show.h"
#include "Group.h"
#include "Console.h"
#include "Sence.h"
#include "Parameters.h"
#include "ControllerShow.h"

#include <conio.h>

//#include "ControllerParam.h"

//#include "FrameUI.h"

// CDALIDlg �Ի���
class CDALIDlg : public CDialogEx
{
// ����
public:
	CDALIDlg(CFrameUI *pUI, CWnd* pParent = NULL);	// ��׼���캯��
	~CDALIDlg();
// �Ի�������
	enum { IDD = IDD_DALI_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	//CShow		 		m_ShowDlg;
	/*CConsole			m_ConsoleDlg;
	CGroup				m_GroupDlg;
	CSence				m_SenceDlg;
	CParameters			m_ParameterDlg;*/
	//CControllerParam	m_ControllerParamDlg;
	CControllerShow		*m_ControllerShowDlg;
	//CFrameUI			*m_FrameUI;
	CFrameUI *m_FrameUI;
	CConsole *m_ConsoleDlg;
	CGroup *m_GroupDlg;
	CSence *m_SenceDlg;
	CParameters *m_ParameterDlg;
	CStatic				m_IP;
	
	//CStatusBarCtrl		m_StatusBarCtrl;
public:
	CString				m_sIP;
	CString				m_sPass;
};
