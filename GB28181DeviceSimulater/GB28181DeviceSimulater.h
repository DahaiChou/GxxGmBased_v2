
// GB28181DeviceSimulater.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGB28181DeviceSimulaterApp:
// �йش����ʵ�֣������ GB28181DeviceSimulater.cpp
//

class CGB28181DeviceSimulaterApp : public CWinAppEx
{
public:
	CGB28181DeviceSimulaterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGB28181DeviceSimulaterApp theApp;