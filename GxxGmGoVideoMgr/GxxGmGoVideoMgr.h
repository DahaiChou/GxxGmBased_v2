
// GxxGmGoVideoMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGxxGmGoVideoMgrApp:
// �йش����ʵ�֣������ GxxGmGoVideoMgr.cpp
//

class CGxxGmGoVideoMgrApp : public CWinAppEx
{
public:
	CGxxGmGoVideoMgrApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGxxGmGoVideoMgrApp theApp;