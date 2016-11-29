// TParameters.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "TParameters.h"
#include "afxdialogex.h"



// CTParameters 对话框

IMPLEMENT_DYNAMIC(CTParameters, CDialogEx)

CTParameters::CTParameters(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTParameters::IDD, pParent)
{
	m_FrameUI = pUI;
}

CTParameters::~CTParameters()
{
}

void CTParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTParameters, CDialogEx)
	ON_BN_CLICKED(TT_IDC_BUTTON_READ, &CTParameters::OnBnClickedIdcButtonRead)
	ON_BN_CLICKED(TT_IDC_BUTTON_SAVE, &CTParameters::OnBnClickedIdcButtonSave)
END_MESSAGE_MAP()


// CTParameters 消息处理程序
BOOL CTParameters::PreTranslateMessage(MSG* pMsg)
{
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，但会导致回车在窗体上失效
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter结束输入*/
		if (GetDlgItem(CONSOLE_IDC_EDIT_SENCE) == GetFocus())
		{
			//OnEditBrightness();
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}


void CTParameters::OnBnClickedIdcButtonRead()
{
	// TODO:  在此添加控件通知处理程序代码

	//封装命令结构
	_tparameters_command* pTParamCommand = new _tparameters_command();
	pTParamCommand->cMode = 0x00;
	pTParamCommand->nCommand = 0x00;

	//设置主机ID和密码
	memcpy(pTParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pTParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));

	m_FrameUI->setStatusBar("正在读取配置中... 请稍等... ");
	//huobingli
	m_FrameUI->readTParamSaveConf(pTParamCommand, (char*)cBuf);
	//huobingli
	//ShowEdit(cBuf);
	m_FrameUI->setStatusBar("读取配置成功!");
	delete pTParamCommand;
	pTParamCommand = NULL;

	
}


void CTParameters::OnBnClickedIdcButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
}
