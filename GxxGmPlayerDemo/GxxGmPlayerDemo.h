
// GxxGmPlayerDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGxxGmPlayerDemoApp:
// �йش����ʵ�֣������ GxxGmPlayerDemo.cpp
//

class CGxxGmPlayerDemoApp : public CWinAppEx
{
public:
	CGxxGmPlayerDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGxxGmPlayerDemoApp theApp;