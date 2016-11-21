#pragma once
#include "def.h"
#include "FrameUI.h"

// CControllerParam 对话框

class CControllerParam : public CDialogEx
{
	DECLARE_DYNAMIC(CControllerParam)

public:
	CControllerParam(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControllerParam();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnRead();					//读取函数
	afx_msg void OnBnSave();					//保存函数
	afx_msg void OnClickedBoardcast();			//广播函数
	afx_msg void OnClickedGroup();				//组播函数
	afx_msg void OnClickedTaget();				//单播函数
	afx_msg void OnEditWrite(UINT ID);			//可写编辑框

private:
	CButton			m_CheckBoardCast;		//广播单选按钮对象
	CButton			m_CheckGroup;			//组播单选按钮对象
	CButton			m_CheckTaget;			//单播单选按钮对象
	CComboBox		m_ComboGroup;			//组播组合框控件对象
	CComboBox		m_ComboTargetAddr;		//单播组合框控件对象
	CONTROLLER_EDIT	m_ControllerEdit[6];	//编辑框数组

private:
	void ShowEdit(unsigned char* cBuf);

private:
	CFrameUI *m_FrameUI;
};
