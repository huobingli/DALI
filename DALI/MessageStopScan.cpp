// MessageStopScan.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "MessageStopScan.h"
#include "afxdialogex.h"


// CMessageStopScan �Ի���

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


// CMessageStopScan ��Ϣ�������


BOOL CMessageStopScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1, 5000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CMessageStopScan::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	//CDialog::OnOK();

	CDialogEx::OnTimer(nIDEvent);
}


void CMessageStopScan::OnBnClickedButton1()
{
	//DWORD dwExitCode = 0;
	//AfxMessageBox("123123");
	CPublic::triggerFlag = 1;
	//SetEvent(hRequestExitEvent);//����event����״̬֪ͨ�߳��˳�  
	//TerminateThread(handleScan, dwExitCode);
	CDialog::OnCancel();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
