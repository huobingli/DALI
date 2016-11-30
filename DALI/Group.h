#pragma once
#include "FrameUI.h"

/********************************************************************************/
// CGroup 对话框


/*单选按钮结构
typedef struct _group_check
{
	CButton CheckButton;
	BOOL	bTag = 0;
}GROUP_CHECK,*PGROUP_CHECK;*/
/*配置信息结构
typedef struct _config_info
{
	char			cMode;
	unsigned char	cBuf[3];
	int				nAddress;
}CONFIG_INFO,*PCONFIG_INFO;*/

/**********************************************************************************/
class CGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CGroup)

public:
	CGroup(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroup();

	// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnClickedCheck(UINT ID);
	afx_msg void OnBtnSaveConfig();
	afx_msg void OnBtnReadConfig();
	virtual BOOL OnInitDialog();

private:
	CFrameUI	*m_FrameUI;
	CComboBox	m_ComboBoxAdress;
	GROUP_CHECK m_GroupCheck[GROUP_NUM];
	CButton		m_ButtonSaveConfig;
	CButton		m_ButtonReadConfig;
	CEdit		m_EditLeft;				/*Group0-7*/
	CEdit		m_EditRight;			/*Group8-15*/

private:
	GROUP_CHECK* GetGroupCheckByCtrlID(UINT ID);					//通过控件ID获取单选按钮结构
	void ConfigIntToString(CString &sLeft, CString &sRight,unsigned char* buf);
	void StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf);
	void ShowCheck();
	void ShowEdit();
	void UpdateTag(char* pLBuf,char* pRBuf);						//根据配置信息来更新单选按钮结构的Tag标志
	void EightBitCheck(CString &s,char* &c);						//检测字符串是否满足8位，不满足则将前面空缺部分以0补齐
};