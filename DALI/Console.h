#pragma once

#include "FrameUI.h"
#include "crtdefs.h"
#include "def.h"
using namespace std;


// CConsole �Ի���
class CConsole : public CDialogEx
{
	DECLARE_DYNAMIC(CConsole)

public:
	CConsole(CFrameUI * pUI, CWnd* pParent = NULL);   // ��׼���캯��

	virtual void OnOk();
	virtual ~CConsole();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CFrameUI *m_FrameUI;
	//�ؼ�����
	CComboBox	m_ComboGroup;						//�鲥��Ͽ�ؼ�����
	CComboBox	m_ComboTargetAddr;					//������Ͽ�ؼ�����
	CComboBox	m_ComboSence;						//Go to Sence ��Ͽ�ؼ�
	//CScrollBar	m_ScrollBarSence;			
	CButton		m_CheckBroadCast;					//�㲥��ѡ��ť����
	CButton		m_CheckGroup;						//�鲥��ѡ��ť����
	CButton		m_CheckAutoSence;					//�Զ�������ѡ��ť
	CButton		m_ButtonGoToSence;					//������ť����
	CButton		m_ButtonOSU;						//ON AND STEP UP��ť����
	CButton		m_ButtonSDO;						//STEP DOWN AND OFF��ť����
	CButton		m_ButtonQuery;						//��ѯ״̬��ť
	CButton		m_ButtonScanDevice;					//ɨ���豸��ť
	CButton		m_ButtonDetectDevice;				//����豸��ť
	CEdit		m_EditLightness;					//���ȱ༭�����
	CEdit		m_EditTimeInterval;					//�Զ�����ʱ����
	//DALI_DEVICE	m_DALIDeviceArray[DALI_DEVICE_NUM];	//DALI�豸����

	CSliderCtrl		m_SliderCtrlConsole;	//�������ؼ�����

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedIdcButtonUp();				//���ߺ���
	afx_msg void OnBnClickedIdcButtonDown();			//���ͺ���
	afx_msg void OnBnClickedIdcButtonOff();				//�رպ���
	afx_msg void OnBnClickedIdcButtonMax();				//������������
	afx_msg void OnBnClickedIdcButtonMin();				//������С����
	afx_msg void OnClickedConsoleIdcCheckBoardcast();	//�㲥����
	afx_msg void OnClickedConsoleIdcCheckGroup();		//�鲥����
	afx_msg void OnBnGoToSence();						//��������
	afx_msg void OnBnOSU();								//OSU����
	afx_msg void OnBnSDO();								//SDO����
	afx_msg void OnEditBrightness();					//���Ⱥ���
	afx_msg void OnBnAutoSence();						//�Զ���������
	afx_msg void OnBnQuery();							//��ѯ״̬��ť
	afx_msg void OnBnScanDevice();						//ɨ���豸��ť
	afx_msg void OnBnDetectDevice();					//����豸��ť
	afx_msg void OnBnDALIDevice(UINT ID);				//���DALI�豸����

private:
	void SendMessage(int nBrightness);					//�������Ⱥ���
	void NonoptionalPart();								//һ��ʼ����ѡ��Ĳ��ֺ���
	void OptionalPart();								//һ���������ÿ���ѡ��
	void ShowStatus(STATUS* pStatus);					//��ʾ״̬
	void ShowDALIDevice();								//��ʾDALI�豸״̬
	void UpdateTag(char* pBuf,int nCycleTime);			//����DALI�豸��־
};
