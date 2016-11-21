#pragma once
#include "def.h"
#include "FrameUI.h"

// CParameters 对话框



typedef struct _edit
{
	CEdit Edit;
	char  cVal;
}EDIT,*PEDIT;

class CParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CParameters)

public:
	CParameters(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParameters();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

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
	CFrameUI *m_FrameUI;

	CButton			m_CheckBoardCast;		//广播单选按钮对象
	CButton			m_CheckGroup;			//组播单选按钮对象
	CButton			m_CheckTaget;			//单播单选按钮对象
	CComboBox		m_ComboGroup;			//组播组合框控件对象
	CComboBox		m_ComboTargetAddr;		//单播组合框控件对象								
	EDIT			m_Edit[6];				//编辑框数组
											
	//CEdit			m_EditPowerOnLevel;		[0]									
	//CEdit			m_EditSystemFailure;	[1]
	//CEdit			m_MinLevel;				[2]
	//CEdit			m_MaxLevel;				[3]
	//CEdit			m_FadeRate;				[4]
	//CEdit			m_FadeTime;				[5]
private:
	void ShowEdit(unsigned char* cBuf);
};
