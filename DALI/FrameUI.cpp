#include "stdafx.h"
#include "FrameUI.h"


CFrameUI::CFrameUI()
{
	
	//memset(m_DALIDeviceArray, 0, sizeof(m_DALIDeviceArray));
	
	m_StatusBarCtrl = new CStatusBarCtrl();
	
	m_FrameBLL = new CFrameBLL();
	
	m_DeviceNum = 0;

	memset(DevicePWD, 0, 16);
	memcpy(DevicePWD, "self12345678", 12);

	updtCacheTable = new CCacheTable(20);


	//������������²�������
	m_argUpdate = (_update_parameters*)malloc(sizeof(_update_parameters));
	m_argUpdate->pUpdateCacheTable = updtCacheTable;
	//����״̬��ָ��
	m_argUpdate->pStatusBarCtrl = m_StatusBarCtrl;

	//handleUpdate = CreateThread(NULL, 0, Update, m_argUpdate, NULL, 0);
}


CFrameUI::~CFrameUI()
{
	delete m_FrameBLL;
	delete m_StatusBarCtrl;
}

