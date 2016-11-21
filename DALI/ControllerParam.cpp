// ControllerParam.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "ControllerParam.h"
#include "afxdialogex.h"


// CControllerParam 对话框

IMPLEMENT_DYNAMIC(CControllerParam, CDialogEx)

CControllerParam::CControllerParam(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{
	m_FrameUI = pUI;
}

CControllerParam::~CControllerParam()
{
}

void CControllerParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, CP_IDC_CHECK_SENCE_BOARDCAST, m_CheckBoardCast);
	DDX_Control(pDX, CP_IDC_CHECK_SENCE_GROUP, m_CheckGroup);
	DDX_Control(pDX, CP_IDC_COMBO_ADDRESS, m_ComboTargetAddr);
	DDX_Control(pDX, CP_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, CP_IDC_CHECK_SENCE_ADDRESS, m_CheckTaget);
	for (int i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, CP_IDC_EDIT_POWER + i, m_ControllerEdit[i].Edit);
	}
}

BOOL CControllerParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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

	m_ComboTargetAddr.EnableWindow(false);
	m_ComboGroup.EnableWindow(false);
	GetDlgItem(CP_IDC_BUTTON_READ)->EnableWindow(false);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CControllerParam, CDialogEx)
	ON_BN_CLICKED(CP_IDC_BUTTON_READ, OnBnRead)
	ON_BN_CLICKED(CP_IDC_BUTTON_SAVE, OnBnSave)
	ON_BN_CLICKED(CP_IDC_CHECK_SENCE_ADDRESS, OnClickedTaget)
	ON_BN_CLICKED(CP_IDC_CHECK_SENCE_BOARDCAST, OnClickedBoardcast)
	ON_BN_CLICKED(CP_IDC_CHECK_SENCE_GROUP, OnClickedGroup)
	ON_CONTROL_RANGE(EN_KILLFOCUS, CP_IDC_EDIT_POWER, CP_IDC_EDIT_FADETIME, OnEditWrite)
END_MESSAGE_MAP()


// CControllerParam 消息处理程序
/*可写编辑框函数*/
void CControllerParam::OnEditWrite(UINT ID)
{
	int nPos = ID - CP_IDC_EDIT_POWER;
	m_ControllerEdit[nPos].cVal = (char)GetDlgItemInt(ID);
}
/*通过按钮读取*/
void CControllerParam::OnBnRead()
{
	int nTargetAddr = m_ComboTargetAddr.GetCurSel();
	if (nTargetAddr == -1)
	{
		//AfxMessageageBox(_T("请选择Address！"));
		return;
	}
	//封装命令结构
	CONTROLLER_PARAM_COMMAND* pCParamCommand = new CONTROLLER_PARAM_COMMAND();
	pCParamCommand->cMode = TARGET_MODE;
	pCParamCommand->nCommand = CPARAM_READ;
	pCParamCommand->nDALIID = nTargetAddr;

	unsigned char cBuf[32] = { 0x01,0x02,0x03,0x04,0x05,0x9F,0x0F };
	//BLL->接口(pParamCommand,cBuf);
	ShowEdit(cBuf);

	delete pCParamCommand;
	pCParamCommand = NULL;

}
/*通过按钮保存*/
void CControllerParam::OnBnSave()
{
	CONTROLLER_PARAM_COMMAND* pCParamCommand = new CONTROLLER_PARAM_COMMAND();
	pCParamCommand->nCommand = PARAM_SAVE;
	/*广播模式*/
	if (m_CheckBoardCast.GetCheck())
	{
		pCParamCommand->cMode = BROADCAST_MODE;
	}
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();
		if (nGroupID < 0 || nGroupID>15)
		{
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		pCParamCommand->cMode = GROUP_MODE;
		pCParamCommand->nGroupID = nGroupID;
	}
	else if (m_CheckTaget.GetCheck())
	{
		int nDALIID = m_ComboTargetAddr.GetCurSel();
		if (nDALIID < 0 || nDALIID>63)
		{
			//AfxMessageageBox(_T("请输入0-63的值"));
			return;
		}
		pCParamCommand->cMode = TARGET_MODE;
		pCParamCommand->nDALIID = nDALIID;
	}
	else
	{
		//AfxMessageageBox(_T("请选择正确模式"));
		return;
	}
	char cBuf[7];
	for (int i = 0; i < 6; ++i)
	{
		memcpy(cBuf + i, &m_ControllerEdit[i].cVal, 1);
	}
	//发送给BLL层
	//BLL->接口(pParamCommand);
}
/*点击单选按钮是否选择广播模式*/
void CControllerParam::OnClickedBoardcast()
{
	if (m_CheckBoardCast.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(CP_IDC_BUTTON_READ)->EnableWindow(false);
	}
	else
	{
		m_CheckBoardCast.SetCheck(false);
	}
}
/*点击单选按钮是否选择组播模式*/
void CControllerParam::OnClickedGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(CP_IDC_BUTTON_READ)->EnableWindow(false);
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboGroup.EnableWindow(false);
	}
}
/*点击单选旋钮是否选择单播模式*/
void CControllerParam::OnClickedTaget()
{
	if (m_CheckTaget.GetCheck())
	{
		m_CheckTaget.SetCheck(true);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		m_ComboGroup.EnableWindow(false);
		GetDlgItem(CP_IDC_BUTTON_READ)->EnableWindow(true);
	}
	else
	{
		m_CheckTaget.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(CP_IDC_BUTTON_READ)->EnableWindow(false);
	}
}
void CControllerParam::ShowEdit(unsigned char* cBuf)
{
	int nTag = 0;
	for (int i = 0; i < 32; ++i)
	{
		if (i == 5)
		{
			int nFadeTime = cBuf[i] & 0x0F;
			int nFadeRate = cBuf[i] >> 4 & 0x0F;
			SetDlgItemInt(CP_IDC_EDIT_FADERATE, nFadeRate);
			SetDlgItemInt(CP_IDC_EDIT_FADETIME, nFadeTime);
			nTag = 1;
		}
		else
		{
			SetDlgItemInt(CP_IDC_EDIT_ACTUAL + nTag + i, (int)cBuf[i]);
		}
	}
}