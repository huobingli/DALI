#pragma once
#include "afxwin.h"
#include "def.h"
#include "FrameUI.h"

/*DALI���Ƚṹ*/
typedef struct _dali_brightness {
	int 		m_nId;					//����
	int 		m_nBrightness;			//���� 0-100 0����OFF
	CEdit		m_CEdit;				//�༭�����		
	CButton     m_CButton;				//��ť����
	CSliderCtrl m_CSliderCtrl;				//����������
	//CScrollBar  m_CScrollbar;			//����������
}DALI_BRIGHTNESS, *PDALI_BRIGHTNESS;

/*Sence����ṹ*/
/*
typedef struct _sence_command
{
	char cMode;
	int nCommand;
	char cBrightness[DALI_NUM];
	int nGroupID = -1;
	int nDALIID = -1;
}SENCE_COMMAND, *PSENCE_COMMAND;*/

// CSence �Ի���
class CSence : public CDialogEx
{
	DECLARE_DYNAMIC(CSence)

public:
	CSence(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSence();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClickedCheck(UINT ID);
	afx_msg void OnScrollEdit(UINT ID);
	afx_msg void OnBnReadConfig();				//��ȡ���ú���
	afx_msg void OnBnSaveConfig();				//�������ú���
	afx_msg void OnClickedBoardcast();			//�㲥����
	afx_msg void OnClickedGroup();				//�鲥����
	afx_msg void OnClickedTaget();				//��������
	virtual void OnOk();
// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
private:
	CFrameUI *m_FrameUI;
	DALI_BRIGHTNESS m_DALI[DALI_NUM];		//DALI�����������
	CButton			m_CheckBoardCast;		//�㲥��ѡ��ť����
	CButton			m_CheckGroup;			//�鲥��ѡ��ť����
	CButton			m_CheckTaget;			//������ѡ��ť����
	CComboBox		m_ComboGroup;			//�鲥��Ͽ�ؼ�����
	CComboBox		m_ComboTargetAddr;		//������Ͽ�ؼ�����
	

private:
	//DALI_BRIGHTNESS* FindDaliByScrollBar(CScrollBar* pScrollBar);
	DALI_BRIGHTNESS* FindDaliBySliderCtrl(CSliderCtrl* pSliderCtrl);
	DALI_BRIGHTNESS* GetDALIByCtrlID(UINT ID);
};
