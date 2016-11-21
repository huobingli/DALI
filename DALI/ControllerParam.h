#pragma once
#include "def.h"
#include "FrameUI.h"

// CControllerParam �Ի���

class CControllerParam : public CDialogEx
{
	DECLARE_DYNAMIC(CControllerParam)

public:
	CControllerParam(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControllerParam();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

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
	CButton			m_CheckBoardCast;		//�㲥��ѡ��ť����
	CButton			m_CheckGroup;			//�鲥��ѡ��ť����
	CButton			m_CheckTaget;			//������ѡ��ť����
	CComboBox		m_ComboGroup;			//�鲥��Ͽ�ؼ�����
	CComboBox		m_ComboTargetAddr;		//������Ͽ�ؼ�����
	CONTROLLER_EDIT	m_ControllerEdit[6];	//�༭������

private:
	void ShowEdit(unsigned char* cBuf);

private:
	CFrameUI *m_FrameUI;
};
