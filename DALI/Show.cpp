// Show.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "Show.h"
#include "afxdialogex.h"


// CShow 对话框

IMPLEMENT_DYNAMIC(CShow, CDialog)

CShow::CShow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShow::IDD, pParent)
{
}

CShow::~CShow()
{
}

void CShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IP, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CShow, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1,OnButtonGetIP)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IP, &CShow::OnLvnItemchangedListIp)
END_MESSAGE_MAP()

BOOL CShow::PreTranslateMessage(MSG* pMsg)
{
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，但会导致回车在窗体上失效
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
		return TRUE;
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

void CShow::OnOk()
{}

BOOL CShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListCtrl.InsertColumn(0, "IP", LVCFMT_CENTER,80);
	m_ListCtrl.InsertColumn(1, "主机名称", LVCFMT_CENTER,80);
	m_ListCtrl.InsertColumn(2, "主机ID", LVCFMT_CENTER,80);
	i = 0;

	return true;
}

// CShow 消息处理程序
void CShow::OnButtonGetIP()
{
	CString ip;
	ip.Format("127.0.0.%d", i);
	CString id;
	id.Format("DALI%d号",i);
	m_ListCtrl.InsertItem(i, ip);
	//m_ListCtrl.InsertItem(1, "Chris");
	//m_ListCtrl.InsertItem(2, id);
	m_ListCtrl.SetItemText(i, 1, "Chris");
	m_ListCtrl.SetItemText(i, 2, id);
	i++;
}


void CShow::OnLvnItemchangedListIp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//int pos = m_ListCtrl.GetSelectionMark();

	CString s;
	//s.Format(_T("选中第%d行"), pos);
	////AfxMessageageBox(s);
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//AfxMessageageBox(_T("没有选择"));
	}
	else
	{
		while (pos)
		{
			int nIdx = -1;
			nIdx = m_ListCtrl.GetNextSelectedItem(pos);

			if (nIdx >= 0 && nIdx < m_ListCtrl.GetItemCount())
			{
				for (int a = 0; a < 3; a++)
				{
					s =s + m_ListCtrl.GetItemText(nIdx, a);
				}

			}
			//AfxMessageageBox(s);
		}
	}
	pos = NULL;


	*pResult = 0;
}

