#pragma once
#include "FrameUI.h"

/********************************************************************************/
// CGroup �Ի���


/*��ѡ��ť�ṹ
typedef struct _group_check
{
	CButton CheckButton;
	BOOL	bTag = 0;
}GROUP_CHECK,*PGROUP_CHECK;*/
/*������Ϣ�ṹ
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
	CGroup(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroup();

	// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	GROUP_CHECK* GetGroupCheckByCtrlID(UINT ID);					//ͨ���ؼ�ID��ȡ��ѡ��ť�ṹ
	void ConfigIntToString(CString &sLeft, CString &sRight,unsigned char* buf);
	void StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf);
	void ShowCheck();
	void ShowEdit();
	void UpdateTag(char* pLBuf,char* pRBuf);						//����������Ϣ�����µ�ѡ��ť�ṹ��Tag��־
	void EightBitCheck(CString &s,char* &c);						//����ַ����Ƿ�����8λ����������ǰ���ȱ������0����
};