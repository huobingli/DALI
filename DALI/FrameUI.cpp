#include "stdafx.h"
#include "FrameUI.h"


CFrameUI::CFrameUI()
{
	m_FrameBLL = new CFrameBLL();
	
	//
	m_DeviceNum = 0;

	m_StatusBarCtrl = new CStatusBarCtrl();

	memset(DevicePWD, 0, 16);
	memcpy(DevicePWD, "self12345678", 12);

}


CFrameUI::~CFrameUI()
{
	delete m_FrameBLL;
	delete m_StatusBarCtrl;
}

