// Sence.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "Sence.h"
#include "afxdialogex.h"


// CSence �Ի���

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

	/*COMBO�ؼ���ʼ��*/
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
	/*DALI�����������*/
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
	/*����ESC�رմ���*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//���λس��رմ��壬���ᵼ�»س��ڴ�����ʧЧ
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
	{
		/*Enter��������*/
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

// CSence ��Ϣ�������
void CSence::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*
	int pos = pScrollBar->GetScrollPos();
	PDALI_BRIGHTNESS temp = FindDaliByScrollBar(pScrollBar);
	if (temp == NULL)
	{
		//AfxMessageageBox(_T("������ID������"));
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
	//�趨������λ��
	pScrollBar->SetScrollPos(temp->m_nBrightness);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	*/
	CSliderCtrl* pS = (CSliderCtrl*)pScrollBar;
	PDALI_BRIGHTNESS temp = FindDaliBySliderCtrl(pS);
	if (temp == NULL)
		m_FrameUI->setStatusBar("������������");
		//AfxMessageageBox(_T("�����������ڣ�"));
	if (temp->m_nBrightness >= 0)
		temp->m_CButton.SetCheck(BST_CHECKED);
	temp->m_nBrightness = temp->m_CSliderCtrl.GetPos();

	CString lightNum = "���� ";
	CString baseStatus = " �ŵ�����ֵΪ ";
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
		m_FrameUI->setStatusBar("������0-100��ֵ");
		//AfxMessageageBox(_T("������0-100��ֵ"));
	}
	else
	{
		pDALI->m_CButton.SetCheck(BST_CHECKED);
		SetDlgItemInt(pDALI->m_CEdit.GetDlgCtrlID(), nBrightness);

		CString lightNum = "���� ";
		CString baseStatus = " �ŵ�����ֵΪ ";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID(), baseStatus, nBrightness);
		m_FrameUI->setStatusBar(status);
	}
	//pDALI->m_CScrollbar.SetScrollPos(nBrightness);
	pDALI->m_CSliderCtrl.SetPos(nBrightness);
}

/*ͨ����ѡ��ť�����ƶ�Ӧ�༭�����ֵ�Լ����������ƶ�
������ѡ��ť��ѡ�У��༭���ֵ��100������������100
�أ���ѡ��ť��ѡ�У��༭���ֵ��0����������0
*/
void CSence::OnClickedCheck(UINT ID)
{
	DALI_BRIGHTNESS *pDALI;
	pDALI = GetDALIByCtrlID(ID);
	int iCheck = pDALI->m_CButton.GetCheck();

	if (iCheck == BST_CHECKED)
	{
		CString lightNum = "�Ѿ�ѡ�� ";
		CString baseStatus = " �ţ�Ĭ��ֵΪ100";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID() - 1300, baseStatus, pDALI->m_nBrightness);
		m_FrameUI->setStatusBar(status);

		pDALI->m_nBrightness = 100;
		SetDlgItemInt(pDALI->m_CEdit.GetDlgCtrlID(), pDALI->m_nBrightness);
	}
	else if (iCheck == BST_UNCHECKED)
	{
		CString lightNum = "ȡ��ѡ�� ";
		CString baseStatus = " �ţ���ΪOFF";
		CString status;
		status.Format("%s%d%s%d", lightNum, pDALI->m_CEdit.GetDlgCtrlID() - 1300, baseStatus, pDALI->m_nBrightness);
		pDALI->m_nBrightness = 0;
		pDALI->m_CEdit.SetWindowText(_T("OFF"));
	}
	//pDALI->m_CScrollbar.SetScrollPos(pDALI->m_nBrightness);
	pDALI->m_CSliderCtrl.SetPos(pDALI->m_nBrightness);
}

/*ͨ����ť��ȡ����*/
void CSence::OnBnReadConfig()
{
	int nTargetAddr = m_ComboTargetAddr.GetCurSel();
	if (nTargetAddr == -1)
	{
		//AfxMessageageBox(_T("��ѡ��Address��"));
		return;
	}
	//��װ������ݸ�BLL
	SENCE_COMMAND* pSenceCommand = new SENCE_COMMAND();
	pSenceCommand->cMode = TARGET_MODE;
	pSenceCommand->nCommand = SENCE_READCONFIG;
	pSenceCommand->nDALIID = nTargetAddr;

	//��������ID������
	memcpy(pSenceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pSenceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);
	
	//huobingli
	char buffer[20];
	memset(buffer, 0, sizeof(buffer));

	unsigned char nBrightness[20] = { 0 };
	m_FrameUI->setStatusBar("���ڶ�ȡ������... ���Ժ�...");
	m_FrameUI->readSenceSaveConf(pSenceCommand, buffer);
	for (int i = 0; i < 20; i++) {
		nBrightness[i] = (int)buffer[i];
	}
	//��ʾ
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

	m_FrameUI->setStatusBar("��ȡ���óɹ�!");
	/*
	//��ʾ
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

	//��������ID������
	memcpy(pSenceCommand->DeviceID, m_FrameUI->DeviceID, 4);
	memcpy(pSenceCommand->DevicePWD, m_FrameUI->DevicePWD, 16);

	/*�㲥ģʽ*/
	if (m_CheckBoardCast.GetCheck())
	{
		pSenceCommand->cMode = BROADCAST_MODE;
	}
	else if (m_CheckGroup.GetCheck())
	{
		int nGroupID = m_ComboGroup.GetCurSel();
		if (nGroupID < 0 || nGroupID > 15)
		{
			m_FrameUI->setStatusBar("������0-15��ֵ");
			//AfxMessageageBox(_T("������0-15��ֵ"));
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
			m_FrameUI->setStatusBar("������0-63��ֵ");
			//AfxMessageageBox(_T("������0-63��ֵ"));
			return;
		}
		pSenceCommand->cMode = TARGET_MODE;
		pSenceCommand->nDALIID = nDALIID;
	}
	else
	{
		m_FrameUI->setStatusBar("��ѡ����ȷģʽ");
		//AfxMessageageBox(_T("��ѡ����ȷģʽ"));
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
	//���͸�BLL��

	m_FrameUI->setStatusBar("���ڱ���������... ���Ժ�... ");
	//huobingli
	m_FrameUI->sendSenceSaveConf(pSenceCommand);
	//huobingli

	CString s;
	CString t("����ɹ�!\n");
	CString temp;
	for (int i = 0; i < DALI_NUM; ++i)
	{
		temp.Format("��%d��%d \n", i, (int)pSenceCommand->cBrightness[i]);
		s = s + temp;
	}
	//AfxMessageageBox(t + s);

	m_FrameUI->setStatusBar("�������óɹ���");
	delete pSenceCommand;
	pSenceCommand = NULL;
}
/*�����ѡ��ť�Ƿ�ѡ��㲥ģʽ*/
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
		m_FrameUI->setStatusBar("��ѡ��㲥ģʽ");
	}
	else
	{
		m_CheckBoardCast.SetCheck(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
/*�����ѡ��ť�Ƿ�ѡ���鲥ģʽ*/
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

		m_FrameUI->setStatusBar("��ѡ���鲥ģʽ");
	}
	else
	{
		m_CheckGroup.SetCheck(false);
		m_ComboGroup.EnableWindow(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}
/*�����ѡ��ť�Ƿ�ѡ�񵥲�ģʽ*/
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
		m_FrameUI->setStatusBar("��ѡ�񵥲�ģʽ");
	}
	else
	{
		m_CheckTaget.SetCheck(false);
		m_ComboTargetAddr.EnableWindow(false);
		GetDlgItem(SENCE_IDC_BUTTON_READCONFIG)->EnableWindow(false);
		m_FrameUI->setStatusBar("δѡ��ģʽ");
	}
}

/*�Զ��庯��*/
/*ͨ��������Ѱ�Ҷ�ӦDALI���Ƚṹ*/
//DALI_BRIGHTNESS* CSence::FindDaliByScrollBar(CScrollBar* pScrollBar)
//{
//	for (int i = 0; i < DALI_NUM; i++)
//	{
		//if (m_DALI[i].m_CScrollbar.GetDlgCtrlID() == pScrollBar->GetDlgCtrlID())
		//	return &m_DALI[i];
//	}
	//return NULL;
//}

/*ͨ��SliderѰ�Ҷ�ӦDALI���Ƚṹ*/
DALI_BRIGHTNESS* CSence::FindDaliBySliderCtrl(CSliderCtrl* pSliderCtrl)
{
	for (int i = 0; i < DALI_NUM; i++)
	{
		if (m_DALI[i].m_CSliderCtrl.GetDlgCtrlID() == pSliderCtrl->GetDlgCtrlID())
			return &m_DALI[i];
	}
}
/*ͨ���ؼ�ID��ö�ӦDALI����*/
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
/*�������������ڶ�Ӧ�༭����ʾ�Լ���Ӧ������λ��*/