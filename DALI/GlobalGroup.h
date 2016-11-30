#pragma once

#include "FrameUI.h"
#include "def.h"
// CGlobalGroup 对话框

class CGlobalGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CGlobalGroup)

public:
	CGlobalGroup(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGlobalGroup();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

private:
	CFrameUI	*m_FrameUI;
	GROUP_CHECK m_GroupCheck[GROUP_NUM];
	CButton		m_ButtonSaveConfig;
	CButton		m_ButtonReadConfig;
	CEdit		m_EditLeft;				/*Group0-7*/
	CEdit		m_EditRight;			/*Group8-15*/

	_globalgroup_command *m_pGGCommand;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedIdcButtonRead();
	afx_msg void OnBnClickedIdcButtonSave();
	afx_msg void OnClickedCheck(UINT ID);

private:
	GROUP_CHECK* GetGroupCheckByCtrlID(UINT ID);					//通过控件ID获取单选按钮结构
	void ConfigIntToString(CString &sLeft, CString &sRight, unsigned char* buf);
	void StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf);
	void ShowCheck();
	void ShowEdit();
	void UpdateTag(char* pLBuf, char* pRBuf);						//根据配置信息来更新单选按钮结构的Tag标志
	void EightBitCheck(CString &s, char* &c);						//检测字符串是否满足8位，不满足则将前面空缺部分以0补齐
};
