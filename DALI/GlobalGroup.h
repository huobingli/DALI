#pragma once

#include "FrameUI.h"
#include "def.h"
// CGlobalGroup �Ի���

class CGlobalGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CGlobalGroup)

public:
	CGlobalGroup(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGlobalGroup();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	GROUP_CHECK* GetGroupCheckByCtrlID(UINT ID);					//ͨ���ؼ�ID��ȡ��ѡ��ť�ṹ
	void ConfigIntToString(CString &sLeft, CString &sRight, unsigned char* buf);
	void StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf);
	void ShowCheck();
	void ShowEdit();
	void UpdateTag(char* pLBuf, char* pRBuf);						//����������Ϣ�����µ�ѡ��ť�ṹ��Tag��־
	void EightBitCheck(CString &s, char* &c);						//����ַ����Ƿ�����8λ����������ǰ���ȱ������0����
};
