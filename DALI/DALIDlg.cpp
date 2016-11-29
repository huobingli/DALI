
// DALIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DALI.h"
#include "DALIDlg.h"
#include "afxdialogex.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_VSCROLL()
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDALIDlg �Ի���



CDALIDlg::CDALIDlg(CFrameUI *pUI, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDALIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_FrameUI = pUI;
	//pUI->changeIP(m_sIP);
	m_TParameterDlg = new CTParameters(m_FrameUI);
	m_ConsoleDlg = new CConsole(m_FrameUI);
	m_GroupDlg = new CGroup(m_FrameUI);
	m_SenceDlg = new CSence(m_FrameUI);
	m_ParameterDlg = new CParameters(m_FrameUI);
	m_ControllerShowDlg = new CControllerShow(m_FrameUI);
}

CDALIDlg::~CDALIDlg() {
	delete m_FrameUI;
	delete m_ConsoleDlg;
	delete m_GroupDlg;
	delete m_SenceDlg;
	delete m_ParameterDlg;
	delete m_ControllerShowDlg;
	delete m_TParameterDlg;
}

void CDALIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, MAIN_IDC_STATIC_IP, m_IP);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CDALIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDALIDlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// CDALIDlg ��Ϣ�������
BOOL CDALIDlg::PreTranslateMessage(MSG *pMsg)
{
	/*����ESC�رմ���*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	//���λس��رմ��壬���ᵼ�»س��ڴ�����ʧЧ
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&pMsg->wParam)
		return TRUE;
	else
	{
		return CDialog::PreTranslateMessage(pMsg);
	}
}


BOOL CDALIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	//�����޷��Դ��ڽ����������
	pSysMenu->RemoveMenu(2, MF_BYPOSITION);


	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_IP.SetWindowText(m_sIP);

	//m_TabCtrl.InsertItem(0, "Show");
	m_TabCtrl.InsertItem(0, "TParameters");
	m_TabCtrl.InsertItem(1, "Console");
	m_TabCtrl.InsertItem(2, "Group");
	m_TabCtrl.InsertItem(3, "Sence");
	m_TabCtrl.InsertItem(4, "Parameters");
	m_TabCtrl.InsertItem(5, "ControllerShow");
	//m_TabCtrl.InsertItem(5, "ControllerParam");
	
	
	//m_ShowDlg.Create(IDD_DIALOG0_SHOW, &m_TabCtrl);
	m_TParameterDlg->Create(IDD_DIALOG_TPARAMETER, &m_TabCtrl);
	m_ConsoleDlg->Create(IDD_DIALOG_CONSOLE, &m_TabCtrl);
	m_GroupDlg->Create(IDD_DIALOG_GROUP, &m_TabCtrl);
	m_SenceDlg->Create(IDD_DIALOG_SENCE, &m_TabCtrl);
	m_ParameterDlg->Create(IDD_DIALOG_PARAMETER, &m_TabCtrl);
	m_ControllerShowDlg->Create(IDD_DIALOG5, &m_TabCtrl);
	//m_ControllerParamDlg.Create(IDD_DIALOG6, &m_TabCtrl);


	CRect rc;
	m_TabCtrl.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	//m_ShowDlg.MoveWindow(&rc);
	m_TParameterDlg->MoveWindow(&rc);
	m_ConsoleDlg->MoveWindow(&rc);
	m_GroupDlg->MoveWindow(&rc);
	m_SenceDlg->MoveWindow(&rc);
	m_ParameterDlg->MoveWindow(&rc);
	m_ControllerShowDlg->MoveWindow(&rc);
	//m_ControllerParamDlg.MoveWindow(&rc);

	m_TabCtrl.SetCurSel(0);
	m_TParameterDlg->ShowWindow(true);
	//m_ShowDlg.ShowWindow(true);


	//״̬���ؼ�
	RECT m_Rect;
	GetClientRect(&m_Rect); //��ȡ�Ի���ľ�������
	m_Rect.top = m_Rect.bottom; //����״̬���ľ�������
	m_FrameUI->getObjStatusBarCtrl()->Create(WS_BORDER | WS_VISIBLE | CBRS_BOTTOM, m_Rect, this, 3);
	m_FrameUI->getObjStatusBarCtrl()->SetText("����", 0, 0); //��һ����������"���ǵ�һ��ָʾ��"
//	m_FrameUI->getObjStatusBarCtrl()->SetPaneInfo(SBPS_NOBORDERS);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDALIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDALIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDALIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDALIDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel = m_TabCtrl.GetCurSel();
	switch (CurSel)
	{
	case 0:
		//m_ShowDlg.ShowWindow(true);
		m_TParameterDlg->ShowWindow(true);
		m_ConsoleDlg->ShowWindow(false);
		m_GroupDlg->ShowWindow(false);
		m_SenceDlg->ShowWindow(false);
		m_ParameterDlg->ShowWindow(false);
		m_ControllerShowDlg->ShowWindow(false);
		m_FrameUI->setStatusBar("����");
		//m_ControllerParamDlg.ShowWindow(false);
		break;
	case 1:
		//m_ShowDlg.ShowWindow(false);
		m_TParameterDlg->ShowWindow(false);
		m_ConsoleDlg->ShowWindow(true);
		m_GroupDlg->ShowWindow(false);
		m_SenceDlg->ShowWindow(false);
		m_ParameterDlg->ShowWindow(false);
		m_ControllerShowDlg->ShowWindow(false);
		m_FrameUI->setStatusBar("����");
		//m_ControllerParamDlg.ShowWindow(false);
		break;
	case 2:
		//m_ShowDlg.ShowWindow(false);
		m_TParameterDlg->ShowWindow(false);
		m_ConsoleDlg->ShowWindow(false);
		m_GroupDlg->ShowWindow(true);
		m_SenceDlg->ShowWindow(false);
		m_ParameterDlg->ShowWindow(false);
		m_ControllerShowDlg->ShowWindow(false);
		m_FrameUI->setStatusBar("����");
		//m_ControllerParamDlg.ShowWindow(false);
		break;
	case 3:
		//m_ShowDlg.ShowWindow(false);
		m_TParameterDlg->ShowWindow(false);
		m_ConsoleDlg->ShowWindow(false);
		m_GroupDlg->ShowWindow(false);
		m_SenceDlg->ShowWindow(true);
		m_ParameterDlg->ShowWindow(false);
		m_ControllerShowDlg->ShowWindow(false);
		m_FrameUI->setStatusBar("����");
		//m_ControllerParamDlg.ShowWindow(false);
		break;
	case 4:
		//m_ShowDlg.ShowWindow(false);
		m_TParameterDlg->ShowWindow(false);
		m_ConsoleDlg->ShowWindow(false);
		m_GroupDlg->ShowWindow(false);
		m_SenceDlg->ShowWindow(false);
		m_ParameterDlg->ShowWindow(true);
		m_ControllerShowDlg->ShowWindow(false);
		m_FrameUI->setStatusBar("����");
		//m_ControllerParamDlg.ShowWindow(false);
		break;
	case 5:
		m_TParameterDlg->ShowWindow(false);
		m_ConsoleDlg->ShowWindow(false);
		m_GroupDlg->ShowWindow(false);
		m_SenceDlg->ShowWindow(false);
		m_ParameterDlg->ShowWindow(false);
		m_ControllerShowDlg->ShowWindow(true);
		m_FrameUI->setStatusBar("����");
		break;
	//case 6:
		//m_ShowDlg.ShowWindow(false);
		//m_ConsoleDlg.ShowWindow(false);
		//m_GroupDlg.ShowWindow(false);
		//m_SenceDlg.ShowWindow(false);
		//m_ParameterDlg.ShowWindow(false);
		//m_ControllerShowDlg.ShowWindow(false);
		//m_ControllerParamDlg.ShowWindow(true);
		//break;
	default:
		break;
	}
	*pResult = 0;
}


