
// GB28181ServerPlatform.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGB28181ServerPlatformApp:
// �йش����ʵ�֣������ GB28181ServerPlatform.cpp
//

class CGB28181ServerPlatformApp : public CWinAppEx
{
public:
	CGB28181ServerPlatformApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGB28181ServerPlatformApp theApp;