
// GB28181ClientPlatform.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGB28181ClientPlatformApp:
// �йش����ʵ�֣������ GB28181ClientPlatform.cpp
//

class CGB28181ClientPlatformApp : public CWinAppEx
{
public:
	CGB28181ClientPlatformApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGB28181ClientPlatformApp theApp;