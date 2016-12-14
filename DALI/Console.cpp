// Console.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "Console.h"
#include "afxdialogex.h"

#include <iostream>
#include <cmath>
#include <stdarg.h>
#include <afxwin.h>
using namespace std;

//CRC校验函数
/*
uint16_t Tmgr_GetCrcValue(uint8_t *pSrc, uint16_t Length)
{
	uint16_t CrcValue = PRESET_VALUE;//0xFFFF
	for (uint16_t LengthIndex = 0; LengthIndex<Length; LengthIndex++)
	{
		CrcValue ^= pSrc[LengthIndex];
		for (uint8_t CheckIndex = 0; CheckIndex<8; CheckIndex++)
		{
			if ((CrcValue & 0x0001) != 0)
			{
				CrcValue >>= 1;
				CrcValue ^= PLOYNOMIAL;//0x8408
			}
			else
			{
				CrcValue >>= 1;
			}
		}
	}
	return(CrcValue);
}*/
// CConsole 对话框

IMPLEMENT_DYNAMIC(CConsole, CDialogEx)

CConsole::CConsole(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CConsole::IDD, pParent)
{
	m_FrameUI = pUI;
	m_MessageStopScan = new CMessageStopScan();
}

CConsole::~CConsole()
{
	delete m_MessageStopScan;
}

void CConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//combo 控件
	DDX_Control(pDX, CONSOLE_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, CONSOLE_IDC_COMBO_TARGETADDR, m_ComboTargetAddr);
	DDX_Control(pDX, CONSOLE_IDC_COMBO_SENCE, m_ComboSence);
	//scroll bar 控件
	DDX_Control(pDX, CONSOLE_IDC_SLIDERCTL, m_SliderCtrlConsole);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_BROADCAST, m_CheckBroadCast);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_GROUP, m_CheckGroup);
	DDX_Control(pDX, CONSOLE_IDC_CHECK_AUTOSENCE, m_CheckAutoSence);
	DDX_Control(pDX, CONSOLE_IDC_STATIC_TARGETADRESS, m_ComboTargetAddr);	
	//CEdit控件
	DDX_Control(pDX, CONSOLE_IDC_EDIT_SENCE, m_EditLightness);
	DDX_Control(pDX, CONSOLE_IDC_EDIT_TIMEINTERVAL, m_EditTimeInterval);


	//Button控件
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		DDX_Control(pDX, IDC_BUTTON1 + i, m_FrameUI->m_DALIDeviceArray[i].btn);
	}

	
}


BEGIN_MESSAGE_MAP(CConsole, CDialogEx)
	//	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_UP, &CConsole::OnBnClickedIdcButtonUp)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_DOWN, &CConsole::OnBnClickedIdcButtonDown)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_OFF, &CConsole::OnBnClickedIdcButtonOff)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_MAX, &CConsole::OnBnClickedIdcButtonMax)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_MIN, &CConsole::OnBnClickedIdcButtonMin)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_BROADCAST, &CConsole::OnClickedConsoleIdcCheckBoardcast)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_GROUP, &CConsole::OnClickedConsoleIdcCheckGroup)
	ON_BN_CLICKED(CONSOLE_IDC_CHECK_AUTOSENCE, OnBnAutoSence)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_GOTOSENCE, OnBnGoToSence)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_ONANDSTEPUP, OnBnOSU)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_OFFANDSTEPDOWN, OnBnSDO)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_QUERY,OnBnQuery)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_SCANDEVICE,OnBnScanDevice)
	ON_BN_CLICKED(CONSOLE_IDC_BUTTON_DETEDTDEVICE,OnBnDetectDevice)
	ON_COMMAND_RANGE(IDC_BUTTON1,IDC_BUTTON64, OnBnDALIDevice)

//ON_CBN_SETFOCUS(CONSOLE_IDC_COMBO_TARGETADDR, &CConsole::OnSetfocusConsoleIdcComboTargetaddr)
ON_CBN_SELENDOK(CONSOLE_IDC_COMBO_TARGETADDR, &CConsole::OnSelendokConsoleIdcComboTargetaddr)
END_MESSAGE_MAP()


// CConsole 消息处理程序
BOOL CConsole::PreTranslateMessage(MSG* pMsg)
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
			OnEditBrightness();
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

BOOL CConsole::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//COMBO 控件初始化
	char buffer[16] = { 0 };
	
	for(int i = 0;i < 16;i++)
	{ 
		_itoa_s(i,buffer,sizeof(buffer),10);
		m_ComboGroup.InsertString(i,buffer);
	}
	
	for (int i = 0; i < 64; i++) {
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}

	for (int i = 0; i < 16; i++)
	{
		_itoa_s(i, buffer, sizeof(buffer),10);
		m_ComboSence.InsertString(i, buffer);
	}

	SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 0);
	m_SliderCtrlConsole.SetRange(0, 100);
	m_SliderCtrlConsole.SetPos(100);
	/*开始不可选择的部分*/
	//NonoptionalPart();

	/*DALI设备按钮初始不可用*/
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(false);
	}

	m_ComboTargetAddr.SetCurSel(0);
	
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
	return FALSE;
}



void CConsole::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	int pos = m_SliderCtrlConsole.GetPos();    // 获取水平滚动条当前位置  

	switch (nSBCode)
	{
		// 如果向左滚动一列，则pos减1  
	case SB_LINEUP:
		pos -= 1;
		break;
		// 如果向右滚动一列，则pos加1  
	case SB_LINEDOWN:
		pos += 1;
		break;
		// 如果向左滚动一页，则pos减10  
	case SB_PAGEUP:
		pos -= 10;
		break;
		// 如果向右滚动一页，则pos加10  
	case SB_PAGEDOWN:
		pos += 10;
		break;
		// 如果滚动到最左端，则pos为1  
	case SB_TOP:
		pos = 1;
		break;
		// 如果滚动到最右端，则pos为100  
	case SB_BOTTOM:
		pos = 100;
		break;
		// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值  
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值  
	default:
		SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 100 - pos);
		return;
	}

	// 设置滚动块位置  
	SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, 100 - pos);
	SendMessage(100 - pos);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//组成数据包
/*参数
 * sendCache 数据包
 * ParaNum 参数个数
 * Command 命令
 *
 *//*
int App_WifiSendFormat(uint8_t *SendCache,uint8_t ParaNum, uint8_t Command, ...)
{
	
	SendCache[0] = '<';
	SendCache[3] = ParaNum;
	SendCache[4] = 1;
	SendCache[5] = Command;

	uint8_t ParaLength = 6;
	va_list ap;
	va_start(ap, Command);
	while (--ParaNum)
	{
		uint8_t Length = va_arg(ap, uint8_t);
		uint8_t* ParaContent = va_arg(ap, uint8_t*);
		SendCache[ParaLength++] = Length;

		memcpy(SendCache + ParaLength, ParaContent, Length);

		ParaLength += Length;
		ParaContent += sizeof(ParaContent);
	}

	va_end(ap);
	SendCache[1] = (ParaLength + 1) & 0xFF;
	SendCache[2] = 0;

	uint16_t CrcValue;
	CrcValue = Tmgr_GetCrcValue(SendCache + 1, ParaLength - 1);
	memcpy(SendCache + ParaLength, &CrcValue, sizeof(uint16_t));

	SendCache[ParaLength + 2] = '>';

	ParaLength += 3;
	return ParaLength;
	
}

//DALI主机用户名和密码 通用参数
char masterID[4] = {0x00, 0x00, 0x23, 0xb7};
char password[16] = "self12345678";*/

/*功能：检测DALI设备*/
void CConsole::OnBnDALIDevice(UINT ID)
{
	//CButton* pCButton = (CButton*)GetDlgItem(ID);
	//pCButton->
	//AfxMessageageBox(_T("发送命令！"));
}

/*功能：点击按钮将亮度调亮*/
void CConsole::OnBnClickedIdcButtonUp()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = UP;

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("广播模式，调亮");
		//AfxMessageageBox(_T("广播模式，调亮"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())	
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = UP;
		pConsoleCommand->nGroupID = nGroupID;
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，调亮");
		//AfxMessageageBox(_T("组播模式，调亮"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >=0 )
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = UP;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，调亮");
		//AfxMessageageBox(_T("单播模式,调亮"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
		return;
	}
}
/*功能：点击按钮将亮度调暗*/
void CConsole::OnBnClickedIdcButtonDown()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = DOWN;

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("广播模式，调暗");
		//AfxMessageageBox(_T("广播模式，调暗"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = DOWN;
		pConsoleCommand->nGroupID = nGroupID;
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，调暗");
		//AfxMessageageBox(_T("组播模式，调暗"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = DOWN;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，调暗");
		//AfxMessageageBox(_T("单播模式,调暗"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击按钮关闭灯*/
void CConsole::OnBnClickedIdcButtonOff()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = OFF;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("广播模式，关闭");
		//AfxMessageageBox(_T("广播模式，关闭"));

	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);

		
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("组播模式，关闭");
		//AfxMessageageBox(_T("组播模式，关闭"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);

		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;

		m_FrameUI->setStatusBar("单播模式,关闭");
		//AfxMessageageBox(_T("单播模式,关闭"));
	}
	else
	{
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击按钮将亮度调至最大值*/
void CConsole::OnBnClickedIdcButtonMax()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = MAX;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("广播模式，最大"));
		m_FrameUI->setStatusBar("广播模式，最大");
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = MAX;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("组播模式，最大"));
		m_FrameUI->setStatusBar("组播模式，最大");
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = MAX;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，最大");
		//AfxMessageageBox(_T("单播模式,最大"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式，最大");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击按钮将亮度调至最小值*/
void CConsole::OnBnClickedIdcButtonMin()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = MIN;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("广播模式，最小");
		//AfxMessageageBox(_T("广播模式，最小"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = MIN;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，最小");
		//AfxMessageageBox(_T("组播模式，最小"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OFF;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，最小");
		//AfxMessageageBox(_T("单播模式,最小"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击按钮调整场景*/
void CConsole::OnBnGoToSence()
{
	int nSenceID = m_ComboSence.GetCurSel();
	if (nSenceID < 0 || nSenceID > 15)
	{
		m_FrameUI->setStatusBar("请选择场景");
		//AfxMessageageBox(_T("请选择场景"));
		return;
	}
	/*广播模式*/
	bool bCheck = m_CheckAutoSence.GetCheck();
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nSenceID = nSenceID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("广播模式，调整场景");
		//AfxMessageageBox(_T("广播模式，调整场景"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nGroupID = nGroupID;
		pConsoleCommand->nSenceID = nSenceID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，调整场景");
		//AfxMessageageBox(_T("组播模式，调整场景"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = GOTOSENCE;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();
		pConsoleCommand->nSenceID = nSenceID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		if (bCheck)
		{
			pConsoleCommand->nTimeInterval = GetDlgItemInt(CONSOLE_IDC_EDIT_TIMEINTERVAL, NULL, TRUE);
		}
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，调整场景");
		//AfxMessageageBox(_T("单播模式,调整场景"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击单选按钮选择自动调整场景*/
void CConsole::OnBnAutoSence()
{}
/*功能：点击按钮OSU*/
void CConsole::OnBnOSU()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = OSU;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("广播模式，OSU");
		//AfxMessageageBox(_T("广播模式，OSU"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = OSU;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，OSU");
		//AfxMessageageBox(_T("组播模式，OSU"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = OSU;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式,OSU");
		//AfxMessageageBox(_T("单播模式,OSU"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
/*功能：点击按钮SDO*/
void CConsole::OnBnSDO()
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = BROADCAST_MODE;
		pConsoleCommand->nCommand = SDO;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("广播模式，SDO");
		//AfxMessageageBox(_T("广播模式，SDO"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = GROUP_MODE;
		pConsoleCommand->nCommand = SDO;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("组播模式，SDO");
		//AfxMessageageBox(_T("组播模式，SDO"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = TARGET_MODE;
		pConsoleCommand->nCommand = SDO;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//huobingli
		m_FrameUI->sendConsoleParameters(pConsoleCommand);
		//huobingli
		delete pConsoleCommand;
		pConsoleCommand = NULL;
		m_FrameUI->setStatusBar("单播模式，SDO");
		//AfxMessageageBox(_T("单播模式,SDO"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");

		//AfxMessageageBox(_T("请选择正确模式"));
	}
}


/*功能：点击按钮扫描设备*/
void CConsole::OnBnScanDevice()
{
	//reset ComboTargetAddr中数据
	::SendMessage(m_ComboTargetAddr, CB_RESETCONTENT, NULL, NULL);
	//m_FrameUI->setStatusBar("扫描设备中");
	
	cacheNode *pcacheNode = new cacheNode();
	pcacheNode->setbuffer("扫描设备中", 10);
	m_FrameUI->insertNode(pcacheNode);
	
	CString head = "倒计时 ";
	CString end = " 秒";
	CString message;
	for (int i = 0; i < 10; i++){
		message.Format("%s%d%s", head, 10 - i, end);
		cacheNode *pcacheNode = new cacheNode();
		pcacheNode->setbuffer(message.GetBuffer(), 15);
		m_FrameUI->insertNode(pcacheNode);
		pcacheNode->setLength(1000);	
	}
	
	//huobingli
	CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
	pConsoleCommand->nCommand = 0x03;
	pConsoleCommand->pStatusBarCtrl = m_FrameUI->getStatusBarCtrl();

	pConsoleCommand->pMessageStopScan = m_MessageStopScan;

	unsigned char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	//将获取到的主机ID和密码写入到结构体中
	memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	//huobingli
	
	//发送参数到BLL层进行处理，返回buffer用于返回到界面的数据
	m_FrameUI->sendScan(pConsoleCommand, (char*)buffer);
	//huobingli

	/*
	_scan *pScan = (_scan*)malloc(sizeof(pScan));
	pScan->pMSS = m_MessageStopScan;
	pScan->pSBC = m_FrameUI->getStatusBarCtrl();
	pScan->pAnalCacheTable = analCacheTable;
	handleScan = CreateThread(NULL, 0, Scan, pScan, NULL, 0);
	//hRequestExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//出现弹框等待弹框
	//表示用户按键返回，线程自动退出
	if (m_MessageStopScan->DoModal() == IDOK) {
		m_FrameUI->setStatusBar("搜索完成！！"); 
		//去取数据
	}
	else {
		m_FrameUI->setStatusBar("已取消搜索！！");
	}
	//释放线程参数
	//free(pScan);
	//删除传参对象
	*/
	delete pConsoleCommand;
	pConsoleCommand = NULL;

	//显示数据
	//buffer[0] = 0xff;
	int totalNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)buffer[i];
		//totalNum = totalNum + nVal;
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		totalNum = totalNum + UpdateTag(cVal, i);
	}
	if (totalNum != 0) {
		//获取下拉框的值
		m_FrameUI->setDeviceNum(totalNum);
		//设置下拉框的值
		for (int i = 0; i < totalNum; i++) {
			_itoa_s(i, (char*)buffer, sizeof(buffer), 10);
			m_ComboTargetAddr.InsertString(i, (char*)buffer);
		}
		//m_ComboGroup.set
		//显示设备
		ShowDALIDevice();
		//如果在下拉框中有值，将第一个值置为默认值
		if (m_ComboTargetAddr.GetCount() != 0)
			m_ComboTargetAddr.SetCurSel(0);
		m_FrameUI->setStatusBar("扫描设备完成");
	}
	else {
		m_FrameUI->setStatusBar("没有扫描到设备");
	}
	
}
/*功能：点击按钮检测设备*/
void CConsole::OnBnDetectDevice()
{
	m_FrameUI->setStatusBar("扫描设备中");
	
	CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();

	//将获取到的主机ID和密码写入到结构体中
	memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	pConsoleCommand->nCommand = 0x03;

	unsigned char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	//huobingli
	m_FrameUI->detectScan(pConsoleCommand, (char*)buffer);
	//huobingli

	delete pConsoleCommand;
	pConsoleCommand = NULL;

	int totalNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		int nVal = (int)buffer[i];
		totalNum = totalNum + nVal;
		CString sVal;
		itoa(nVal, sVal.GetBuffer(8), 2);
		char* cVal = sVal.GetBuffer();
		UpdateTag(cVal, i);
	}
	m_FrameUI->setDeviceNum(totalNum);

	/*for (int i = 0; i < totalNum; i++) {
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}*/
	//m_ComboGroup.set
	ShowDALIDevice();
	m_FrameUI->setStatusBar("扫描设备完成");

}
/*功能：点击单选按钮是否选择广播模式*/
void CConsole::OnClickedConsoleIdcCheckBoardcast()
{
	if (m_CheckBroadCast.GetCheck())
	{
		m_CheckBroadCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		OptionalPart();
		m_FrameUI->setStatusBar("已选择广播模式");
	}
	else
	{
		m_CheckBroadCast.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		//NonoptionalPart();
		m_FrameUI->setStatusBar("未选择模式");
	}
}
/*功能：点击单选按钮是否选择组播模式*/
void CConsole::OnClickedConsoleIdcCheckGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckBroadCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboGroup.SetCurSel(0);
		m_ComboTargetAddr.EnableWindow(false);
		OptionalPart();
		m_FrameUI->setStatusBar("已选择组播模式");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		//NonoptionalPart();
		m_FrameUI->setStatusBar("未选择模式");
	}
}

void CConsole::OnEditBrightness()
{
	int nBrightness = GetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, NULL, TRUE);
	if (nBrightness < 0 || nBrightness > 100)
	{
		m_FrameUI->setStatusBar("请输入0-100的值");
		//AfxMessageageBox(_T("请输入0-100的值"));
	}
	else
	{
		SetDlgItemInt(CONSOLE_IDC_EDIT_SENCE, nBrightness);
	}
	m_SliderCtrlConsole.SetPos(100 - nBrightness);
}

void CConsole::OnBnQuery()
{
	STATUS* pStatus = new STATUS();

	//memcpy();
	ShowStatus(pStatus);
	delete pStatus;
	pStatus = NULL;
}


/*private*/
void CConsole::SendMessage(int nBrightness)
{
	/*广播模式*/
	if (m_CheckBroadCast.GetCheck())
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = 0xff;
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
		
		//pConsoleCommand->nDeviceAddr = 0xFE;

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);
		m_FrameUI->sendChangeLightness(pConsoleCommand);
		CString baseStatus = "广播模式，调亮  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;

		////AfxMessageageBox(_T("广播模式，直接调亮"));
	}
	/*组播模式*/
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();		//组播ID
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = 0x80 + m_ComboGroup.GetCurSel();
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;
		pConsoleCommand->nGroupID = nGroupID;

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);

		m_FrameUI->sendChangeLightness(pConsoleCommand);

		CString baseStatus = "组播模式，调亮  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("组播模式，直接调亮"));
	}
	else if (m_ComboTargetAddr.GetCurSel() >= 0)
	{
		CONSOLE_COMMAND *pConsoleCommand = new CONSOLE_COMMAND();
		pConsoleCommand->cMode = m_ComboTargetAddr.GetCurSel();
		pConsoleCommand->nCommand = 0x05;
		pConsoleCommand->nBrightness = nBrightness;
		pConsoleCommand->nDALIID = m_ComboTargetAddr.GetCurSel();

		//将获取到的主机ID和密码写入到结构体中
		memcpy(pConsoleCommand->DeviceID, m_FrameUI->DeviceID, 4);
		memcpy(pConsoleCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

		//pFrameBLL->sendConsoleParameters(pConsoleCommand);

		m_FrameUI->sendChangeLightness(pConsoleCommand);
		
		CString baseStatus = "单播模式，调亮  ";
		CString status;
		status.Format("%s %d", baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);

		delete pConsoleCommand;
		pConsoleCommand = NULL;
		//AfxMessageageBox(_T("单播模式,直接调亮"));
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
	}
}
void CConsole::NonoptionalPart()
{
	m_ComboGroup.EnableWindow(false);
	//m_ComboTargetAddr.EnableWindow(false);
	m_ComboSence.EnableWindow(false);
	m_SliderCtrlConsole.EnableWindow(false);
	m_EditLightness.EnableWindow(false);
	m_EditTimeInterval.EnableWindow(false);
	GetDlgItem(CONSOLE_IDC_CHECK_AUTOSENCE)->EnableWindow(false);
}
void CConsole::OptionalPart()
{
	m_ComboSence.EnableWindow(true);
	m_SliderCtrlConsole.EnableWindow(true);
	m_EditLightness.EnableWindow(true);
	m_EditTimeInterval.EnableWindow(true);
	GetDlgItem(CONSOLE_IDC_CHECK_AUTOSENCE)->EnableWindow(true);
}
void CConsole::ShowStatus(STATUS* pStatus)
{
	SetDlgItemInt(IDC_EDIT_DEVICETYPE, pStatus->nDeviceType);
	SetDlgItemInt(IDC_EDIT_STATUSOFBALLAST,pStatus->nStatusOfBallast);
	SetDlgItemInt(IDC_EDIT_VERSION,pStatus->nVersion);
	SetDlgItemInt(IDC_EDIT_LAMPFAILURE,pStatus->nLampFailure);
	SetDlgItemInt(IDC_EDIT_EDIT_LAMPARCPOWERON,pStatus->nLampArcPowerOn);
	SetDlgItemInt(IDC_EDIT_LIMITERROR,pStatus->nLimitError);
	SetDlgItemInt(IDC_EDIT_FADEREADY,pStatus->nFadeReady);
	SetDlgItemInt(IDC_EDIT_RESETSTATE,pStatus->nResetState);
	SetDlgItemInt(IDC_EDIT_MISSINGSHORTADDRESS,pStatus->nMissingShortAddress);
	SetDlgItemInt(IDC_EDIT_POWERFAILURE,pStatus->nPowerFailure);
}
void CConsole::ShowDALIDevice()
{
	for (int i = 0; i < DALI_DEVICE_NUM; ++i)
	{
		if (m_FrameUI->m_DALIDeviceArray[i].nTag == 1)
			m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(true);
		else
			m_FrameUI->m_DALIDeviceArray[i].btn.EnableWindow(false);
	}
}
int CConsole::UpdateTag(char* pBuf,int nCycleTime)
{
	int deviceNum = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (pBuf[i] == '1')
		{
			m_FrameUI->m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 1;
			++deviceNum;
		}
		else
		{
			m_FrameUI->m_DALIDeviceArray[nCycleTime * 8 + i].nTag = 0;
		}
	}
	
	return deviceNum;
}


void CConsole::OnSelendokConsoleIdcComboTargetaddr()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_ComboTargetAddr.GetCurSel() > 0)  {
		m_FrameUI->setStatusBar("已选择单播模式");
		OptionalPart();
	}
}
