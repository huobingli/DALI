#pragma once
#include "afxwin.h"
#include "def.h"
#include "FrameUI.h"

/*DALI亮度结构*/
typedef struct _dali_brightness {
	int 		m_nId;					//组编号
	int 		m_nBrightness;			//亮度 0-100 0代表OFF
	CEdit		m_CEdit;				//编辑框对象		
	CButton     m_CButton;				//按钮对象
	CSliderCtrl m_CSliderCtrl;				//滑动条对象
	//CScrollBar  m_CScrollbar;			//滚动条对象
}DALI_BRIGHTNESS, *PDALI_BRIGHTNESS;

/*Sence命令结构*/
/*
typedef struct _sence_command
{
	char cMode;
	int nCommand;
	char cBrightness[DALI_NUM];
	int nGroupID = -1;
	int nDALIID = -1;
}SENCE_COMMAND, *PSENCE_COMMAND;*/

// CSence 对话框
class CSence : public CDialogEx
{
	DECLARE_DYNAMIC(CSence)

public:
	CSence(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSence();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClickedCheck(UINT ID);
	afx_msg void OnScrollEdit(UINT ID);
	afx_msg void OnBnReadConfig();				//读取配置函数
	afx_msg void OnBnSaveConfig();				//保存配置函数
	afx_msg void OnClickedBoardcast();			//广播函数
	afx_msg void OnClickedGroup();				//组播函数
	afx_msg void OnClickedTaget();				//单播函数
	virtual void OnOk();
// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	CFrameUI *m_FrameUI;
	DALI_BRIGHTNESS m_DALI[DALI_NUM];		//DALI亮度组合数组
	CButton			m_CheckBoardCast;		//广播单选按钮对象
	CButton			m_CheckGroup;			//组播单选按钮对象
	CButton			m_CheckTaget;			//单播单选按钮对象
	CComboBox		m_ComboGroup;			//组播组合框控件对象
	CComboBox		m_ComboTargetAddr;		//单播组合框控件对象
	

private:
	//DALI_BRIGHTNESS* FindDaliByScrollBar(CScrollBar* pScrollBar);
	DALI_BRIGHTNESS* FindDaliBySliderCtrl(CSliderCtrl* pSliderCtrl);
	DALI_BRIGHTNESS* GetDALIByCtrlID(UINT ID);
};
