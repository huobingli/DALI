// MessageStopScan.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "MessageStopScan.h"
#include "afxdialogex.h"


// CMessageStopScan 对话框

IMPLEMENT_DYNAMIC(CMessageStopScan, CDialogEx)

CMessageStopScan::CMessageStopScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageStopScan::IDD, pParent)
{
	//triggerFlag = 0;
}

CMessageStopScan::~CMessageStopScan()
{
}

void CMessageStopScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageStopScan, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMessageStopScan::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMessageStopScan 消息处理程序


BOOL CMessageStopScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(1, 5000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CMessageStopScan::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	
	//CDialog::OnOK();

	CDialogEx::OnTimer(nIDEvent);
}


void CMessageStopScan::OnBnClickedButton1()
{
	//DWORD dwExitCode = 0;
	//AfxMessageBox("123123");
	CPublic::triggerFlag = 1;
	//SetEvent(hRequestExitEvent);//设置event激发状态通知线程退出  
	//TerminateThread(handleScan, dwExitCode);
	CDialog::OnCancel();
	// TODO:  在此添加控件通知处理程序代码
}
