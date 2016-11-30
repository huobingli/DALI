// GlobalGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "GlobalGroup.h"
#include "afxdialogex.h"


// CGlobalGroup 对话框

IMPLEMENT_DYNAMIC(CGlobalGroup, CDialogEx)

CGlobalGroup::CGlobalGroup(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CGlobalGroup::IDD, pParent)
{
	m_FrameUI = pUI;

	m_pGGCommand = new _globalgroup_command();
}

CGlobalGroup::~CGlobalGroup()
{
}

void CGlobalGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, GG_IDC_BUTTON_SAVE, m_ButtonSaveConfig);
	DDX_Control(pDX, GG_IDC_BUTTON_READ, m_ButtonReadConfig);
	DDX_Control(pDX, GG_IDC_EDIT_ZEROSEVEN, m_EditLeft);
	DDX_Control(pDX, GG_IDC_EDIT_EIGHTFIFTEEN, m_EditRight);
	for (int i = 0; i < GROUP_NUM; i++)
	{
		DDX_Control(pDX, GG_IDC_CHECK_0 + i, m_GroupCheck[i].CheckButton);
	}
}


BEGIN_MESSAGE_MAP(CGlobalGroup, CDialogEx)
	ON_BN_CLICKED(GG_IDC_BUTTON_READ, &CGlobalGroup::OnBnClickedIdcButtonRead)
	ON_BN_CLICKED(GG_IDC_BUTTON_SAVE, &CGlobalGroup::OnBnClickedIdcButtonSave)
	ON_COMMAND_RANGE(GG_IDC_CHECK_0, GG_IDC_CHECK_15, OnClickedCheck)
END_MESSAGE_MAP()
BOOL CGlobalGroup::PreTranslateMessage(MSG* pMsg)
{
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		CString sLeft;
		CString sRight;
		unsigned char cBuf[3] = { '\0' };
		GetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
		GetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);
		StringToConfigInt(sLeft, sRight, cBuf);
		ShowCheck();
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

// CGlobalGroup 消息处理程序


BOOL CGlobalGroup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	/*编辑框初始化*/
	m_EditLeft.SetWindowText(_T("00000000"));
	m_EditRight.SetWindowText(_T("00000000"));
	//显示编辑框内数据到checkbox
	ShowCheck();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

GROUP_CHECK* CGlobalGroup::GetGroupCheckByCtrlID(UINT ID)
{
	for (int i = 0; i < GROUP_NUM; i++)
	{
		if (m_GroupCheck[i].CheckButton.GetDlgCtrlID() == ID)
			return &m_GroupCheck[i];
	}
	return NULL;
}
/*根据单选按钮控件ID来显示控件状态以及单选按钮结构的数值*/
void CGlobalGroup::OnClickedCheck(UINT ID)
{
	GROUP_CHECK* pGroupCheck = GetGroupCheckByCtrlID(ID);

	if (pGroupCheck == NULL)
	{
		m_FrameUI->setStatusBar("不存在该组按钮");
		return;
	}
	if (pGroupCheck->CheckButton.GetCheck())
	{
		pGroupCheck->CheckButton.SetCheck(false);
		pGroupCheck->bTag = 0;
		m_FrameUI->setStatusBar("未选择");
	}
	else
	{
		pGroupCheck->CheckButton.SetCheck(true);
		pGroupCheck->bTag = 1;
		m_FrameUI->setStatusBar("已选择");
	}
	ShowEdit();
}

void CGlobalGroup::OnBnClickedIdcButtonRead()
{
	// TODO:  在此添加控件通知处理程序代码
	//设置主机ID和密码
	memcpy(m_pGGCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(m_pGGCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
	
	//设置命令和模式
	m_pGGCommand->cMode = 0xFF;
	m_pGGCommand->cCommand = 0x12;
	
	CString sLeft;
	CString sRight;
	unsigned char buf[3];
	memset(buf, 0, sizeof(buf));
	m_FrameUI->setStatusBar("读取配置中... 请稍后...");
	m_FrameUI->readGlobalGroupSaveConf(m_pGGCommand, (char*)buf);
	
	//转换
	ConfigIntToString(sLeft, sRight, buf);
	//显示
	ShowCheck();
	ShowEdit();

	m_FrameUI->setStatusBar("读取配置成功!");
}


void CGlobalGroup::OnBnClickedIdcButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码

	CString sLeft;
	CString sRight;
	memset(m_pGGCommand, 0, sizeof(_globalgroup_command));
	
	//设置主机ID和密码
	memcpy(m_pGGCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(m_pGGCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	m_pGGCommand->cCommand = 0x11;
	m_pGGCommand->cMode = 0xFF;
	GetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
	GetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);

	/*将sLeft和sRight转换成int*/
	StringToConfigInt(sLeft, sRight, m_pGGCommand->cBuf);
	ShowCheck();
	/*将配置信息传给BLL层*/

	m_FrameUI->setStatusBar("发送配置参数中... 请稍后...");
	//huobingli
	m_FrameUI->sendGlobalGroupSaveConf(m_pGGCommand);
	//huobinlgi

	CString s;
	s.Format("已发送配置参数为%s", m_pGGCommand->cBuf);

	m_FrameUI->setStatusBar(s);
}


/*配置整型转换成字符串*/
void CGlobalGroup::ConfigIntToString(CString &sLeft, CString &sRight, unsigned char* buf)
{
	int nLeft = (int)buf[0];
	int nRight = (int)buf[1];
	/*int 转字符串*/
	itoa(nLeft, sLeft.GetBuffer(8), 2);
	itoa(nRight, sRight.GetBuffer(8), 2);
	char* pLBuf = sLeft.GetBuffer();
	EightBitCheck(sLeft, pLBuf);
	char* pRBuf = sRight.GetBuffer();
	EightBitCheck(sRight, pRBuf);
	/*填写单选按钮结构数组tag标志*/
	UpdateTag(pLBuf, pRBuf);
}
/*字符串转换成配置整型*/
void CGlobalGroup::StringToConfigInt(CString sLeft, CString sRight, unsigned char* buf)
{
	unsigned short nLeft = _tcstol(sLeft, NULL, 2) << 8;
	unsigned short nRight = _tcstol(sRight, NULL, 2);
	buf[0] = nLeft >> 8;
	buf[1] = nRight;
	char* pLBuf = sLeft.GetBuffer();
	char* pRBuf = sRight.GetBuffer();
	UpdateTag(pLBuf, pRBuf);
}

/*显示单选按钮*/
void CGlobalGroup::ShowCheck()
{
	for (int i = 0; i < GROUP_NUM; i++)
	{
		if (m_GroupCheck[i].bTag == 1)
			m_GroupCheck[i].CheckButton.SetCheck(true);
		else
			m_GroupCheck[i].CheckButton.SetCheck(false);
	}
}
/*显示编辑框*/
void CGlobalGroup::ShowEdit()
{
	char cLBuf[9] = { 0 };
	char cRBuf[9] = { 0 };
	/*left*/
	for (int i = 0; i < 8; i++)
	{
		cLBuf[7 - i] = m_GroupCheck[i].bTag + 48;
	}
	CString sLeft(cLBuf);
	/*right*/
	for (int i = 8; i < 16; i++)
	{
		cRBuf[15 - i] = m_GroupCheck[i].bTag + 48;
	}
	CString sRight(cRBuf);
	SetDlgItemText(GG_IDC_EDIT_ZEROSEVEN, sLeft);
	SetDlgItemText(GG_IDC_EDIT_EIGHTFIFTEEN, sRight);
}
/*更新配置信息中的Tag标志*/
void CGlobalGroup::UpdateTag(char* pLBuf, char* pRBuf)
{
	for (int i = 0; i < 8; i++)
	{
		if (pLBuf[i] == '1')
		{
			m_GroupCheck[7 - i].bTag = 1;
		}
		else
		{
			m_GroupCheck[7 - i].bTag = 0;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (pRBuf[i] == '1')
		{
			m_GroupCheck[15 - i].bTag = 1;
		}
		else
		{
			m_GroupCheck[15 - i].bTag = 0;
		}
	}
}
/*8位检测函数*/
void CGlobalGroup::EightBitCheck(CString &s, char* &c)
{
	int nLen = strlen(c);
	if (nLen < 8)
	{
		char cTemp[9] = { '\0' };
		int nVal = 8 - nLen;
		memset(cTemp, '0', nVal);
		memcpy(cTemp + nVal, c, nLen);
		s.Format("%s", cTemp);
		memcpy(c, cTemp, 8);
	}
}