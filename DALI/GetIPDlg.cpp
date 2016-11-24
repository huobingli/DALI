// GetIPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "GetIPDlg.h"
#include "afxdialogex.h"



// CGetIPDlg 对话框

IMPLEMENT_DYNAMIC(CGetIPDlg, CDialogEx)

char *change(char src[], char *cache) {
	char Num[11] = "0123456789";
	char CodeL[27] = "abcdefghijklmnopqrstuvwxyz";
	char CodeH[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//char cache[9];
	memset(cache, 0, sizeof(cache));
	for (char Index = 0; Index < 4; Index++)
	{
		char diff = src[Index] & 0x0F;
		if (diff>9)
		{
			cache[7 - (Index << 1)] = CodeH[diff - 10];
		}
		else
		{
			cache[7 - (Index << 1)] = Num[diff];
		}

		diff = (src[Index] >> 4) & 0x0f;

		if (diff>9)
		{
			cache[6 - (Index << 1)] = CodeH[diff - 10];
		}
		else
		{
			cache[6 - (Index << 1)] = Num[diff];
		}
	}
	return cache;
}


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


CGetIPDlg::CGetIPDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(IDD_DIALOG7, pParent), i(0)
{
	m_FrameUI = new CFrameUI();
	
	for (int i = 0; i < 6; i++){
		memset(&m_deviceInfo[i], 0, sizeof(_getdevice_info));
	}
	m_deviceNum = 0;
}

CGetIPDlg::~CGetIPDlg()
{
	delete m_FrameUI;
	//delete m_deviceInfo;

	CloseHandle(handleRecv);
	CloseHandle(handleAnal);
}

void CGetIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MAIN_IDC_LIST_IP, m_ListCtrl);
	DDX_Control(pDX, MAIN_IDC_BUTTON_GETIP, m_ButtonGetIP);
}


BEGIN_MESSAGE_MAP(CGetIPDlg, CDialogEx)
	ON_BN_CLICKED(MAIN_IDC_BUTTON_GETIP, OnBtnGetIP)
	ON_NOTIFY(LVN_ITEMCHANGED, MAIN_IDC_LIST_IP, OnLvnItemchangedList)
	ON_BN_CLICKED(MAIN_IDC_BUTTON_CLEARLIST, &CGetIPDlg::OnBnClickedIdcButtonClearlist)
END_MESSAGE_MAP()


// CGetIPDlg 消息处理程序
BOOL CGetIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.InsertColumn(0, "IP", LVCFMT_CENTER, 100);
	m_ListCtrl.InsertColumn(1, "主机名称", LVCFMT_CENTER, 100);
	m_ListCtrl.InsertColumn(2, "主机ID", LVCFMT_CENTER, 100);

	
	/*m_ListCtrl.InsertItem(0, "123.123.0.123");
	m_ListCtrl.SetItemText(0, 1, "123");
	m_ListCtrl.SetItemText(0, 2, "BB122112");
	m_ListCtrl.InsertItem(1, "123.123.0.123");
	m_ListCtrl.SetItemText(1, 1, "123");
	m_ListCtrl.SetItemText(1, 2, "CB123412");
	m_ListCtrl.InsertItem(2, "123.123.0.123");
	m_ListCtrl.SetItemText(2, 1, "123");
	m_ListCtrl.SetItemText(2, 2, "EF123411");*/
	return TRUE;
}
void CGetIPDlg::OnBtnGetIP()
{
	/*
	CString ip;
	ip.Format("127.0.0.%d", i);
	CString id;
	id.Format("DALI%d号", i);
	m_ListCtrl.InsertItem(i, ip);
	//m_ListCtrl.InsertItem(1, "Chris");
	//m_ListCtrl.InsertItem(2, id);
	m_ListCtrl.SetItemText(i, 1, "Chris");
	m_ListCtrl.SetItemText(i, 2, id);
	i++;*/

	//char cBuf[256] = { '\0' };
	//BLL->接口(cBuf,sAddr);

	//huobingli
	m_deviceNum = m_FrameUI->scanDevice(m_deviceInfo, m_deviceNum);
	
	//huobingli
	showDALIInfo(m_deviceInfo, m_deviceNum);
}

void CGetIPDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//int pos = m_ListCtrl.GetSelectionMark();
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nItem = m_ListCtrl.GetNextSelectedItem(pos);
		CString sIP = m_ListCtrl.GetItemText(nItem, 0);
		CString sPass = m_ListCtrl.GetItemText(nItem, 2);

		CDALIDlg *DALIDlg = new CDALIDlg(m_FrameUI);
		m_FrameUI->changeIP(sIP);
		DALIDlg->m_sIP = sIP;
		DALIDlg->m_sPass = sPass;

		
		//获取主机通道
		char* cVal = sPass.GetBuffer();
		for (int i = 0; i < 4;i++) {
			m_FrameUI->DeviceID[3 - i] = (HexToAsc(cVal[2 * i]) << 4) | HexToAsc(cVal[2 * i + 1]);
		}

		//memcpy(masterID, sPass, 4);
		DALIDlg->DoModal();

		delete DALIDlg;
	}
	*pResult = 0;
}

//输出到列表
void CGetIPDlg::showDALIInfo(_getdevice_info* buffer, int deviceNum) {
	m_ListCtrl.DeleteAllItems();
	for (int i = 0; i < deviceNum; i++) {
		m_ListCtrl.InsertItem(i, buffer[i].cIP);
		m_ListCtrl.SetItemText(i, 1, buffer[i].cDeviceName);
		m_ListCtrl.SetItemText(i, 2, buffer[i].cDeviceID);
	}
}
/*
*/


void CGetIPDlg::OnBnClickedIdcButtonClearlist()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ListCtrl.DeleteAllItems();
	
	for (int i = 0; i < m_deviceNum; i++) {
		memset(m_deviceInfo[i].cDeviceID, 0, sizeof(m_deviceInfo[i].cDeviceID));
		memset(m_deviceInfo[i].cDeviceName, 0, sizeof(m_deviceInfo[i].cDeviceName));
		memset(m_deviceInfo[i].cIP, 0, sizeof(m_deviceInfo[i].cIP));
	}
	m_deviceNum = 0;
}
