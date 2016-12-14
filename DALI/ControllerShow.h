#pragma once
#include "def.h"
#include "FrameUI.h"
//#include "function.h"

// CControllerShow 对话框
class CControllerShow : public CDialogEx
{
	DECLARE_DYNAMIC(CControllerShow)

public:
	CControllerShow(CFrameUI *pUI, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControllerShow();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CButton			m_ButtonScanDevice;					//扫描设备按钮
	CButton			m_ButtonDetectDevice;				//检测设备按钮
	CButton			m_ButtonRead;						//读取按钮
	CButton			m_ButtonSave;						//保存按钮
	CTRDALI_DEVICE	m_DALIDeviceArray[DALI_DEVICE_NUM];	//DALI设备数组
	CStatic			m_DeviceID;							//设备ID
	CEdit			m_Edit[5];
	unsigned char	m_cParam[2];
	CMessageStopScan *m_MessageStopScan;

public:
	afx_msg void OnBnScanDevice();						//扫描设备按钮
	afx_msg void OnBnDetectDevice();					//检测设备按钮
	afx_msg void OnBnDALIDevice(UINT ID);				//检测DALI设备函数
	afx_msg void OnBnEditWrite(UINT ID);				//可写编辑框
	afx_msg void OnBnRead();							//读取函数
	afx_msg void OnBnSave();							//保存函数

private:
	void ClearEdit();									//清空编辑框
	void OptionalEdit();								//可编辑的编辑框
	void ShowEdit(int nPos);							//显示编辑框内容
	void ShowDALIDevice();								//显示DALI设备状态
	void UpdateTag(char* pBuf, int nCycleTime);			//更新DALI设备标志

	void GetParam(char *buffer);
	unsigned char HexToAsc(unsigned char aChar){
		if ((aChar >= 0x30) && (aChar <= 0x39))
			aChar -= 0x30;
		else if ((aChar >= 0x41) && (aChar <= 0x46))//大写字母
			aChar -= 0x37;
		else if ((aChar >= 0x61) && (aChar <= 0x66))//小写字母
			aChar -= 0x57;
		else aChar = 0xff;
		return aChar;
	}
private:
	CFrameUI *m_FrameUI;
};
