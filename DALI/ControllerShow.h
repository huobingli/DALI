#pragma once
#include "def.h"
#include "FrameUI.h"
//#include "function.h"

// CControllerShow �Ի���
class CControllerShow : public CDialogEx
{
	DECLARE_DYNAMIC(CControllerShow)

public:
	CControllerShow(CFrameUI *pUI, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControllerShow();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CButton			m_ButtonScanDevice;					//ɨ���豸��ť
	CButton			m_ButtonDetectDevice;				//����豸��ť
	CButton			m_ButtonRead;						//��ȡ��ť
	CButton			m_ButtonSave;						//���水ť
	CTRDALI_DEVICE	m_DALIDeviceArray[DALI_DEVICE_NUM];	//DALI�豸����
	CStatic			m_DeviceID;							//�豸ID
	CEdit			m_Edit[5];
	unsigned char	m_cParam[2];
	CMessageStopScan *m_MessageStopScan;

public:
	afx_msg void OnBnScanDevice();						//ɨ���豸��ť
	afx_msg void OnBnDetectDevice();					//����豸��ť
	afx_msg void OnBnDALIDevice(UINT ID);				//���DALI�豸����
	afx_msg void OnBnEditWrite(UINT ID);				//��д�༭��
	afx_msg void OnBnRead();							//��ȡ����
	afx_msg void OnBnSave();							//���溯��

private:
	void ClearEdit();									//��ձ༭��
	void OptionalEdit();								//�ɱ༭�ı༭��
	void ShowEdit(int nPos);							//��ʾ�༭������
	void ShowDALIDevice();								//��ʾDALI�豸״̬
	void UpdateTag(char* pBuf, int nCycleTime);			//����DALI�豸��־

	void GetParam(char *buffer);
	unsigned char HexToAsc(unsigned char aChar){
		if ((aChar >= 0x30) && (aChar <= 0x39))
			aChar -= 0x30;
		else if ((aChar >= 0x41) && (aChar <= 0x46))//��д��ĸ
			aChar -= 0x37;
		else if ((aChar >= 0x61) && (aChar <= 0x66))//Сд��ĸ
			aChar -= 0x57;
		else aChar = 0xff;
		return aChar;
	}
private:
	CFrameUI *m_FrameUI;
};
