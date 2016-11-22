#include "DALIDlg.h"
#pragma once
#include "FrameUI.h"


// CGetIPDlg �Ի���

class CGetIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGetIPDlg)

public:
	CGetIPDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetIPDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBtnGetIP();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CListCtrl	 m_ListCtrl;
	CButton		 m_ButtonGetIP;
	int			 i;
private:
	//void showDALIInfo(char* cBuf);

//huobingli
public:
	//char *change(char src[], char *cache);
	void showDALIInfo(_getdevice_info* buffer, int deviceNum);
private:
	CFrameUI *m_FrameUI;
	_getdevice_info m_deviceInfo[6];
	int m_deviceNum;
public:
	afx_msg void OnBnClickedIdcButtonClearlist();
};
