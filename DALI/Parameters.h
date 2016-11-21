#pragma once
#include "def.h"
#include "FrameUI.h"

// CParameters �Ի���



typedef struct _edit
{
	CEdit Edit;
	char  cVal;
}EDIT,*PEDIT;

class CParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CParameters)

public:
	CParameters(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParameters();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnRead();					//��ȡ����
	afx_msg void OnBnSave();					//���溯��
	afx_msg void OnClickedBoardcast();			//�㲥����
	afx_msg void OnClickedGroup();				//�鲥����
	afx_msg void OnClickedTaget();				//��������
	afx_msg void OnEditWrite(UINT ID);			//��д�༭��
private:
	CFrameUI *m_FrameUI;

	CButton			m_CheckBoardCast;		//�㲥��ѡ��ť����
	CButton			m_CheckGroup;			//�鲥��ѡ��ť����
	CButton			m_CheckTaget;			//������ѡ��ť����
	CComboBox		m_ComboGroup;			//�鲥��Ͽ�ؼ�����
	CComboBox		m_ComboTargetAddr;		//������Ͽ�ؼ�����								
	EDIT			m_Edit[6];				//�༭������
											
	//CEdit			m_EditPowerOnLevel;		[0]									
	//CEdit			m_EditSystemFailure;	[1]
	//CEdit			m_MinLevel;				[2]
	//CEdit			m_MaxLevel;				[3]
	//CEdit			m_FadeRate;				[4]
	//CEdit			m_FadeTime;				[5]
private:
	void ShowEdit(unsigned char* cBuf);
};
