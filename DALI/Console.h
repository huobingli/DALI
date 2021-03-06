#pragma once

#include "FrameUI.h"
#include "crtdefs.h"
#include "def.h"
#include "MessageStopScan.h"

using namespace std;

//extern int triggerFlag;

//static HANDLE handleScan;
/*
struct _scan{
	CMessageStopScan *pMSS;
	CStatusBarCtrl *pSBC;
	CCacheTable *pAnalCacheTable;
};

static DWORD WINAPI Scan(LPVOID pM) {
	
	_scan *arg = (_scan*)pM;
	//退出线程方式
	//1、用户点击取消按钮，线程break。
	//2、搜索超时，自动退出。
	CString status;
	CString head = "已经找到 ";
	CString end = " 个主机";
	for (int i = 0; i < 10; i++) {
		//用户没有按下取消按钮，线程继续执行
		if (CPublic::triggerFlag == 0) {
			//搜索分析队列中的操作为0x83的元素，获取里面的
			arg->pAnalCacheTable->scanNode(0x83);
			Sleep(500);
		}
		else {
			return 0;
		}
	}
	//CPublic::readyFlag = 1;
	//执行完毕，关闭对话框
	arg->pMSS->EndDialog(IDCANCEL);
	return 0;
}
*/

// CConsole 对话框
class CConsole : public CDialogEx
{
	DECLARE_DYNAMIC(CConsole)

public:
	CConsole(CFrameUI * pUI, CWnd* pParent = NULL);   // 标准构造函数

	//virtual void OnOk();
	virtual ~CConsole();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CFrameUI *m_FrameUI;
	//控件属性
	CComboBox	m_ComboGroup;						//组播组合框控件对象
	CComboBox	m_ComboTargetAddr;					//单播组合框控件对象
	CComboBox	m_ComboSence;						//Go to Sence 组合框控件
	//CScrollBar	m_ScrollBarSence;			
	CButton		m_CheckBroadCast;					//广播单选按钮对象
	CButton		m_CheckGroup;						//组播单选按钮对象
	CButton		m_CheckAutoSence;					//自动场景单选按钮
	CButton		m_ButtonGoToSence;					//场景按钮对象
	CButton		m_ButtonOSU;						//ON AND STEP UP按钮对象
	CButton		m_ButtonSDO;						//STEP DOWN AND OFF按钮对象
	CButton		m_ButtonQuery;						//查询状态按钮
	CButton		m_ButtonScanDevice;					//扫描设备按钮
	CButton		m_ButtonDetectDevice;				//检测设备按钮
	CEdit		m_EditLightness;					//亮度编辑框对象
	CEdit		m_EditTimeInterval;					//自动场景时间间隔
	//DALI_DEVICE	m_DALIDeviceArray[DALI_DEVICE_NUM];	//DALI设备数组


	CMessageStopScan *m_MessageStopScan;
	CSliderCtrl		m_SliderCtrlConsole;	//滑动条控件对象

public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedIdcButtonUp();				//调高函数
	afx_msg void OnBnClickedIdcButtonDown();			//调低函数
	afx_msg void OnBnClickedIdcButtonOff();				//关闭函数
	afx_msg void OnBnClickedIdcButtonMax();				//调至最亮函数
	afx_msg void OnBnClickedIdcButtonMin();				//调至最小函数
	afx_msg void OnClickedConsoleIdcCheckBoardcast();	//广播函数
	afx_msg void OnClickedConsoleIdcCheckGroup();		//组播函数
	afx_msg void OnBnGoToSence();						//场景函数
	afx_msg void OnBnOSU();								//OSU函数
	afx_msg void OnBnSDO();								//SDO函数
	afx_msg void OnEditBrightness();					//亮度函数
	afx_msg void OnBnAutoSence();						//自动场景函数
	afx_msg void OnBnQuery();							//查询状态按钮
	afx_msg void OnBnScanDevice();						//扫描设备按钮
	afx_msg void OnBnDetectDevice();					//检测设备按钮
	afx_msg void OnBnDALIDevice(UINT ID);				//检测DALI设备函数

private:
	void SendMessage(int nBrightness);					//调节亮度函数
	void NonoptionalPart();								//一开始不可选择的部分函数
	void OptionalPart();								//一定条件后变得可以选择
	void ShowStatus(STATUS* pStatus);					//显示状态
	void ShowDALIDevice();								//显示DALI设备状态
	int UpdateTag(char* pBuf,int nCycleTime);			//更新DALI设备标志
public:
//	afx_msg void OnSetfocusConsoleIdcComboTargetaddr();
//	afx_msg void OnSetfocusConsoleIdcComboTargetaddr();
	afx_msg void OnSelendokConsoleIdcComboTargetaddr();
};
