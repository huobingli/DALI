// Parameters.cpp : 实现文件
//

#include "stdafx.h"
#include "DALI.h"
#include "Parameters.h"
#include "afxdialogex.h"


// CParameters 对话框

IMPLEMENT_DYNAMIC(CParameters, CDialogEx)

CParameters::CParameters(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CParameters::IDD, pParent)
{
	m_FrameUI = pUI;
}

CParameters::~CParameters()
{
}

void CParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_BOARDCAST, m_CheckBoardCast);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_GROUP, m_CheckGroup);
	DDX_Control(pDX, PARAMETER_IDC_COMBO_ADDRESS, m_ComboTargetAddr);
	DDX_Control(pDX, PARAMETER_IDC_COMBO_GROUP, m_ComboGroup);
	DDX_Control(pDX, PARAMETER_IDC_CHECK_SENCE_ADDRESS, m_CheckTaget);
	for (int i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, PARAMETER_IDC_EDIT_POWER + i, m_Edit[i].Edit);
	}
}

BOOL CParameters::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CParameters, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnBnRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnSave)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_ADDRESS, OnClickedTaget)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_BOARDCAST, OnClickedBoardcast)
	ON_BN_CLICKED(PARAMETER_IDC_CHECK_SENCE_GROUP,OnClickedGroup)
	ON_CONTROL_RANGE(EN_KILLFOCUS,PARAMETER_IDC_EDIT_POWER, PARAMETER_IDC_EDIT_FADETIME,OnEditWrite)	
END_MESSAGE_MAP()


// CParameters 消息处理程序
/*可写编辑框函数*/
void CParameters::OnEditWrite(UINT ID)
{
	int nPos = ID - PARAMETER_IDC_EDIT_POWER;
	m_Edit[nPos].cVal = (char)GetDlgItemInt(ID);
}
/*通过按钮读取*/
void CParameters::OnBnRead()
{
	int nTargetAddr = m_ComboTargetAddr.GetCurSel();
	if (nTargetAddr == -1)
	{
		//AfxMessageageBox(_T("请选择Address！"));
		return;
	}
	//封装命令结构
	PARAM_COMMAND* pParamCommand = new PARAM_COMMAND();
	pParamCommand->cMode = TARGET_MODE;
	pParamCommand->nCommand = PARAM_READ;
	pParamCommand->nDALIID = nTargetAddr;

	//设置主机ID和密码
	memcpy(pParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	unsigned char cBuf[32];
	memset(cBuf, 0, sizeof(cBuf));

	m_FrameUI->setStatusBar("正在读取配置中... 请稍等... ");
	//huobingli
	m_FrameUI->readParamSaveConf(pParamCommand, (char*)cBuf);
	//huobingli
	ShowEdit(cBuf);
	m_FrameUI->setStatusBar("读取配置成功!");
	delete pParamCommand;
	pParamCommand = NULL;

}
/*通过按钮保存*/
void CParameters::OnBnSave()
{
	PARAM_COMMAND* pParamCommand = new PARAM_COMMAND();
	pParamCommand->nCommand = PARAM_SAVE;

	//设置主机ID和密码
	memcpy(pParamCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pParamCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	/*广播模式*/
	if (m_CheckBoardCast.GetCheck())
	{
		pParamCommand->cMode = BROADCAST_MODE;
	}
	else if(m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();
		if (nGroupID < 0 || nGroupID>15)
		{
			m_FrameUI->setStatusBar("请输入0-15的值");
			//AfxMessageageBox(_T("请输入0-15的值"));
			return;
		}
		pParamCommand->cMode = GROUP_MODE;
		pParamCommand->nGroupID = nGroupID;
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
		pParamCommand->cMode = TARGET_MODE;
		pParamCommand->nDALIID = nDALIID;
	}
	else
	{
		m_FrameUI->setStatusBar("请选择正确模式");
		//AfxMessageageBox(_T("请选择正确模式"));
		return;
	}
	char cBuf[7];
	for (int i = 0; i < 6; ++i)
	{
		memcpy(cBuf + i, &m_Edit[i].cVal, 1);
	}
	//发送给BLL层
	m_FrameUI->setStatusBar("发送配置中... 请稍候...");
	m_FrameUI->sendParamSaveConf(pParamCommand, cBuf);
	m_FrameUI->setStatusBar("发送配置成功!");
	//AfxMessageageBox(_T("发送成功！"));
}
/*点击单选按钮是否选择广播模式*/
void CParameters::OnClickedBoardcast()
{
	if (m_CheckBoardCast.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(true);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_FrameUI->setStatusBar("已选择广播模式");
	}
	else
	{
		m_CheckBoardCast.SetCheck(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}
/*点击单选按钮是否选择组播模式*/
void CParameters::OnClickedGroup()
{
	if (m_CheckGroup.GetCheck())
	{
		m_CheckTaget.SetCheck(false);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(true);
		m_ComboGroup.EnableWindow(true);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_ComboGroup.SetCurSel(0);
		m_FrameUI->setStatusBar("已选择单播模式");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboGroup.EnableWindow(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}
/*点击单选旋钮是否选择单播模式*/
void CParameters::OnClickedTaget()
{
	if (m_CheckTaget.GetCheck())
	{
		m_CheckTaget.SetCheck(true);
		m_CheckBoardCast.SetCheck(false);
		m_CheckGroup.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(true);
		m_ComboGroup.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(true);
		m_ComboTargetAddr.SetCurSel(0);
		m_FrameUI->setStatusBar("已选择单播模式");
	}
	else
	{
		m_CheckTaget.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(false);
		m_FrameUI->setStatusBar("未选择模式");
	}
}
void CParameters::ShowEdit(unsigned char* cBuf)
{
	int nTag = 0;
	for (int i = 0; i < 32; ++i)
	{
		if (i == 5)
		{
			int nFadeTime = cBuf[i] & 0x0F;
			int nFadeRate = cBuf[i] >> 4 & 0x0F;
			SetDlgItemInt(PARAMETER_IDC_EDIT_FADERATE, nFadeRate);
			SetDlgItemInt(PARAMETER_IDC_EDIT_FADETIME, nFadeTime);
			nTag = 1;
		}
		else
		{
			SetDlgItemInt(PARAMETER_IDC_EDIT_ACTUAL + nTag + i, (int)cBuf[i]);
		}
	}
}