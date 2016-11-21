// Sence.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "Sence.h"
#include "afxdialogex.h"


// CSence 对话框

IMPLEMENT_DYNAMIC(CSence, CDialogEx)

CSence::CSence(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSence::IDD, pParent)
{
	m_FrameUI = pUI;
	for (int i = 0; i < DALI_NUM; i++)
	{
		m_DALI[i].m_nId = -1;
		m_DALI[i].m_nBrightness = 0;
	}
}

CSence::~CSence()
{

}




void CSence::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, SENCE_IDC_CHECK_BOARDCAST, m_CheckBoardCast);
	DDX_Control(pDX, SENCE_IDC_CHECK_GROUP, m_CheckGroup);
	DDX_Control(pDX, SENCE_IDC_COMBO_ADDRESS, m_ComboTargetAddr);
	DDX_Control(pDX, SENCE_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, SENCE_IDC_CHECK_ADDRESS, m_CheckTaget);
	for (int i = 0; i < DALI_NUM; i++)
	{
		DDX_Control(pDX, SENCE_IDC_SLIDER_0 + i, m_DALI[i].m_CSliderCtrl);
		DDX_Control(pDX, DALI_BRIGHTNESS_FIRST_BUTTON + i, m_DALI[i].m_CButton);
		DDX_Control(pDX, DALI_BRIGHTNESS_FIRST_EDIT + i, m_DALI[i].m_CEdit);
	}
}

BOOL CSence::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/*COMBO控件初始化*/
	char buffer[16] = { 0 };
	for (int i = 0; i < 16; ++i)
	{
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboGroup.InsertString(i, buffer);
	}
	for (int i = 0; i < 64; ++i)
	{
		_itoa_s(i, buffer, sizeof(buffer), 10);
		m_ComboTargetAddr.InsertString(i, buffer);
	}

	for (int i = 0; i < DALI_NUM; i++)
	{
		//m_DALI[i].m_CScrollbar.SetScrollRange(0, 100);
		//m_DALI[i].m_CScrollbar.SetScrollPos(0);
		m_DALI[i].m_CSliderCtrl.SetRange(0, 100);
		m_DALI[i].m_CSliderCtrl.SetPos(0);
		m_DALI[i].m_CButton.SetState(BST_UNCHECKED);
		m_DALI[i].m_CEdit.SetWindowText(_T("OFF"));
	}

	m_ComboTargetAddr.EnableWindow(false);
	m_ComboGroup.EnableWindow(false);
	GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(false);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CSence, CDialogEx)
	/*DALI亮度组合命令*/
	ON_WM_HSCROLL()
	ON_COMMAND_RANGE(DALI_BRIGHTNESS_FIRST_BUTTON, DALI_BRIGHTNESS_LAST_BUTTON, OnClickedCheck)
	ON_CONTROL_RANGE(EN_KILLFOCUS, DALI_BRIGHTNESS_FIRST_EDIT, DALI_BRIGHTNESS_LAST_EDIT, OnScrollEdit)
	ON_BN_CLICKED(SENCE_IDC_BUTTON_READCONFIG,OnBnReadConfig)
	ON_BN_CLICKED(SENCE_IDC_BUTTON_SAVECONFIG,OnBnSaveConfig)
	ON_BN_CLICKED(SENCE_IDC_CHECK_BOARDCAST,OnClickedBoardcast)
	ON_BN_CLICKED(SENCE_IDC_CHECK_GROUP,OnClickedGroup)
	ON_BN_CLICKED(SENCE_IDC_CHECK_ADDRESS,OnClickedTaget)
END_MESSAGE_MAP()

void CSence::OnOk()
{
	//CDialogEx::OnOK();
}

BOOL CSence::PreTranslateMessage(MSG* pMsg)
{
	/*屏蔽ESC关闭窗口*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//屏蔽回车关闭窗体，但会导致回车在窗体上失效
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter结束输入*/
		HWND hWnd = ::GetFocus();
		UINT nID = ::GetDlgCtrlID(hWnd);
		if (nID >= 1300 && nID <= 1365)
		{
			CSence::OnScrollEdit(nID);
		}
		return TRUE;
	}
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}

// CSence 消息处理程序
void CSence::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	int pos = pScrollBar->GetScrollPos();
	PDALI_BRIGHTNESS temp = FindDaliByScrollBar(pScrollBar);
	if (temp == NULL)
	{
		//AfxMessageageBox(_T("滚动条ID不存在"));
	}
	if (temp->m_nBrightness >= 0)
		temp->m_CButton.SetCheck(BST_CHECKED);
	switch (nSBCode)
	{
	case SB_LINEUP:
		temp->m_nBrightness -= 1;
		break;
	case SB_LINEDOWN:
		temp->m_nBrightness += 1;
		break;
	case SB_PAGEUP:
		temp->m_nBrightness -= 10;
		break;
	case SB_PAGEDOWN:
		temp->m_nBrightness += 10;
		break;
	case SB_TOP:
		temp->m_nBrightness = 1;
	case SB_BOTTOM:
		pos = 100;
	case SB_THUMBPOSITION:
		temp->m_nBrightness = nPos;
		break;
	default:
	{
		SetDlgItemInt(temp->m_CEdit.GetDlgCtrlID(), temp->m_nBrightness);
	}

	return;
	}
	//设定滚动块位置
	pScrollBar->SetScrollPos(temp->m_nBrightness);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	*/
	CSliderCtrl* pS = (CSliderCtrl*)pScrollBar;
	PDALI_BRIGHTNESS temp = FindDaliBySliderCtrl(pS);
	if (temp == NULL)
		m_FrameUI->setStatusBar("滑动条不存在");
		//AfxMessageageBox(_T("滑动条不存在！"));
	if (temp->m_nBrightness >= 0)
		temp->m_CButton.SetCheck(BST_CHECKED);
	temp->m_nBrightness = temp->m_CSliderCtrl.GetPos();

	CString lightNum = "调节 ";
	CString baseStatus = " 号的亮度值为 ";
	CString status;
	status.Format("%s%d%s%d", lightNum, temp->m_CEdit.GetDlgCtrlID(), baseStatus, temp->m_nBrightness);
	m_FrameUI->setStatusBar(status);

	SetDlgItemInt(temp->m_CEdit.GetDlgCtrlID(), temp->m_nBrightness);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSence::OnScrollEdit(UINT ID)
{
	DALI_BRIGHTNESS *pDALI;
	int nBrightness;
	pDALI = GetDALIByCtrlID(ID);
	pDALI->m_nBrightness = GetDlgItemInt(ID, NULL);
	nBrightness = pDALI->m_nBrightness;
	if (nBrightness < 0 || nBrightness > 100)
	{
		m_FrameUI->setStatusBar("请输入0-100的值");
		//AfxMessageageBox(_T("请输入0-100的值"));
	}
	else
	{
		pDALI->m_CButton.SetCheck(BST_CHECKED);
		SetDlgItemInt(pDALI->m_CEdit.GetDlgCtrlID(), nBrightness);

		CString lightNum = "调节 ";
		CString baseStatus = " 号的亮度值为 ";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID(), baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);
	}
	//pDALI->m_CScrollbar.SetScrollPos(nBrightness);
	pDALI->m_CSliderCtrl.SetPos(nBrightness);
}

/*通过单选按钮来控制对应编辑框的数值以及滑动条的移动
开：单选按钮被选中，编辑框的值到100，滑动条滑到100
关：单选按钮不选中，编辑框的值到0，滑动条到0
*/
void CSence::OnClickedCheck(UINT ID)
{
	DALI_BRIGHTNESS *pDALI;
	pDALI = GetDALIByCtrlID(ID);
	int iCheck = pDALI->m_CButton.GetCheck();

	if (iCheck == BST_CHECKED)
	{
		CString lightNum = "已经选中 ";
		CString baseStatus = " 号，默认值为100";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID() - 1300, baseStatus, pDALI->m_nBrightness);
		m_FrameUI->setStatusBar(status);

		pDALI->m_nBrightness = 100;
		SetDlgItemInt(pDALI->m_CEdit.GetDlgCtrlID(), pDALI->m_nBrightness);
	}
	else if (iCheck == BST_UNCHECKED)
	{
		CString lightNum = "取消选择 ";
		CString baseStatus = " 号，置为OFF";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID() - 1300, baseStatus, pDALI->m_nBrightness);
		pDALI->m_nBrightness = 0;
		pDALI->m_CEdit.SetWindowText(_T("OFF"));
	}
	//pDALI->m_CScrollbar.SetScrollPos(pDALI->m_nBrightness);
	pDALI->m_CSliderCtrl.SetPos(pDALI->m_nBrightness);
}

/*通过按钮读取配置*/
void CSence::OnBnReadConfig()
{
	int nTargetAddr = m_ComboTargetAddr.GetCurSel();
	if (nTargetAddr == -1)
	{
		//AfxMessageageBox(_T("请选择Address！"));
		return;
	}
	//封装命令，传递给BLL
	SENCE_COMMAND* pSenceCommand = new SENCE_COMMAND();
	pSenceCommand->cMode = TARGET_MODE;
	pSenceCommand->nCommand = SENCE_READCONFIG;
	pSenceCommand->nDALIID = nTargetAddr;

	//设置主机ID和密码
	memcpy(pSenceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pSenceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
	
	//huobingli
	char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	unsigned char nBrightness[20] = { 0 };
	m_FrameUI->setStatusBar("正在读取配置中... 请稍后...");
	m_FrameUI->readSenceSaveConf(pSenceCommand, buffer);
	for (int i = 0; i < 20; i++) {
		nBrightness[i] = (int)buffer[i];
	}
	//显示
	int light = 0;
	for (int i = 0; i < DALI_NUM; ++i)
	{
		if (nBrightness[i] !=  0xFF)
		{
			light = (nBrightness[i] + 1) * 100 / 254;
			m_DALI[i].m_nBrightness = light;
			m_DALI[i].m_CButton.SetCheck(true);
			SetDlgItemInt(m_DALI[i].m_CEdit.GetDlgCtrlID(), m_DALI[i].m_nBrightness);
			//m_DALI[i].m_CScrollbar.SetScrollPos(m_DALI[i].m_nBrightness);
			m_DALI[i].m_CSliderCtrl.SetPos(light);
		}
		else {
			m_DALI[i].m_nBrightness = 0;
			m_DALI[i].m_CButton.SetCheck(false);
			m_DALI[i].m_CEdit.SetWindowText(_T("OFF"));
			m_DALI[i].m_CSliderCtrl.SetPos(0);
			//SetDlgItemInt(m_DALI[i].m_CEdit.GetDlgCtrlID(), m_DALI[i].m_nBrightness);
			//m_DALI[i].m_CScrollbar.SetScrollPos(m_DALI[i].m_nBrightness);
		}
		/*
		light = nBrightness[i] * 100 / 254;
		m_DALI[i].m_nBrightness = light;
		m_DALI[i].m_CButton.SetCheck(true);
		SetDlgItemInt(m_DALI[i].m_CEdit.GetDlgCtrlID(), m_DALI[i].m_nBrightness);
		m_DALI[i].m_CScrollbar.SetScrollPos(m_DALI[i].m_nBrightness);*/
	}//huobingli

	m_FrameUI->setStatusBar("读取配置成功!");
	/*
	//显示
	for (int i = 0; i < DALI_NUM; ++i)
	{
		m_DALI[i].m_nBrightness = (int)cBrightness[i];
		m_DALI[i].m_CButton.SetCheck(true);
		SetDlgItemInt(m_DALI[i].m_CEdit.GetDlgCtrlID(), m_DALI[i].m_nBrightness);
		m_DALI[i].m_CScrollbar.SetScrollPos(m_DALI[i].m_nBrightness);
	}*/
}

void CSence::OnBnSaveConfig()
{
	SENCE_COMMAND* pSenceCommand = new SENCE_COMMAND();
	pSenceCommand->nCommand = SENCE_SAVECONFIG;

	//设置主机ID和密码
	memcpy(pSenceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pSenceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	/*广播模式*/
	if (m_CheckBoardCast.GetCheck())
	{
		pSenceCommand->cMode = BROADCAST_MODE;
	}
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		pSenceCommand->cMode = GROUP_MODE;
		pSenceCommand->nGroupID = nGroupID;
	}
	else if (m_CheckTaget.GetCheck())
	{
		int nDALIID = m_ComboTargetAddr.GetCurSel();
		if (nDALIID < 0 || nDALIID>63)
		{
			m_FrameUI->setStatusBar("请输入0-63的值");
			//AfxMessageageBox(_T("请输入0-63的值"));
			return;
		}
		pSenceCommand->cMode = TARGET_MODE;
		pSenceCommand->nDALIID = nDALIID;
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
		return;
	}
	for (int i = 0; i < DALI_NUM; ++i)
	{
		if (m_DALI[i].m_nBrightness  != 0)
			pSenceCommand->cBrightness[i] = (unsigned char)m_DALI[i].m_nBrightness * 254 / 100;
		else
		{
			pSenceCommand->cBrightness[i] = 255;
		}
	}
	//发送给BLL层

	m_FrameUI->setStatusBar("正在保存配置中... 请稍后... ");
	//huobingli
	m_FrameUI->sendSenceSaveConf(pSenceCommand);
	//huobingli

	CString s;
	CString t("保存成功!\n");
	CString temp;
	for (int i = 0; i < DALI_NUM; ++i)
	{
		temp.Format("第%d个%d \n", i, (int)pSenceCommand->cBrightness[i]);
		s = s + temp;
	}
	//AfxMessageageBox(t + s);

	m_FrameUI->setStatusBar("保存配置成功！");
	delete pSenceCommand;
	pSenceCommand = NULL;
}
/*点击单选按钮是否选择广播模式*/
void CSence::OnClickedBoardcast()
{
	if (m_CheckBoardCast.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(false);
		m_FrameUI->setStatusBar("已选择广播模式");
	}
	else
	{
		m_CheckBoardCast.SetCheck(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}
/*点击单选按钮是否选择组播模式*/
void CSence::OnClickedGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(false);
		m_ComboGroup.SetCurSel(0);

		m_FrameUI->setStatusBar("已选择组播模式");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboGroup.EnableWindow(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}
/*点击单选旋钮是否选择单播模式*/
void CSence::OnClickedTaget()
{
	if (m_CheckTaget.GetCheck())
	{
		m_CheckTaget.SetCheck(true);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		m_ComboGroup.EnableWindow(false);
		GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(true);
		m_ComboGroup.SetCurSel(0);
		m_FrameUI->setStatusBar("已选择单播模式");
	}
	else
	{
		m_CheckTaget.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}

/*自定义函数*/
/*通过滑动条寻找对应DALI亮度结构*/
//DALI_BRIGHTNESS* CSence::FindDaliByScrollBar(CScrollBar* pScrollBar)
//{
//	for (int i = 0; i < DALI_NUM; i++)
//	{
		//if (m_DALI[i].m_CScrollbar.GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
		//	return &m_DALI[i];
//	}
	//return NULL;
//}

/*通过Slider寻找对应DALI亮度结构*/
DALI_BRIGHTNESS* CSence::FindDaliBySliderCtrl(CSliderCtrl* pSliderCtrl)
{
	for (int i = 0; i < DALI_NUM; i++)
	{
		if (m_DALI[i].m_CSliderCtrl.GetDlgCtrlID() == pSliderCtrl->GetDlgCtrlID())
			return &m_DALI[i];
	}
}
/*通过控件ID获得对应DALI数组*/
DALI_BRIGHTNESS* CSence::GetDALIByCtrlID(UINT ID)
{
	int id;
	for (int i = 0; i < DALI_NUM; i++)
	{
		/*1300-1315*/
		if (ID >= DALI_BRIGHTNESS_FIRST_EDIT && ID <= DALI_BRIGHTNESS_LAST_EDIT)
		{
			id = m_DALI[i].m_CEdit.GetDlgCtrlID();		
		}
		/*1316-1331*/
		if (ID >= DALI_BRIGHTNESS_FIRST_BUTTON && ID <= DALI_BRIGHTNESS_LAST_BUTTON)
		{
			id = m_DALI[i].m_CButton.GetDlgCtrlID();
		}
		/*1332-1347*/
		//if (ID >= DALI_BRIGHTNESS_FIRST_SCROLLBAR && ID <= DALI_BRIGHTNESS_LAST_SCROLLBAR)		
		//{
		//	id = m_DALI[i].m_CScrollbar.GetDlgCtrlID();
		//}	
		/*1350-1365*/
		if (ID >= SENCE_IDC_SLIDER_0 && ID <= SENCE_IDC_SLIDER_15)
		{
			id = m_DALI[i].m_CSliderCtrl.GetDlgCtrlID();
		}
		if (id == ID)
		{
			return &m_DALI[i];
		}
	}
	return NULL;
}
/*根据亮度数组在对应编辑框显示以及对应滑动条位置*/