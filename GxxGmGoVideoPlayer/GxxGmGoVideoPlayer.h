
// GxxGmGoVideoPlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGxxGmGoVideoPlayerApp:
// �йش����ʵ�֣������ GxxGmGoVideoPlayer.cpp
//

class CGxxGmGoVideoPlayerApp : public CWinAppEx
{
public:
	CGxxGmGoVideoPlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGxxGmGoVideoPlayerApp theApp;