
// DALI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#define _CRTDBG_MAP_ALLOC = 1

#include "resource.h"		// ������


// CDALIApp: 
// �йش����ʵ�֣������ DALI.cpp
//

class CDALIApp : public CWinApp
{
public:
	CDALIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
	CString GetCmdValue(const CString& key);
};

extern CDALIApp theApp;