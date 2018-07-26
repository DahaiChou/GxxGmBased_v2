#ifndef _DevInfoHandler_H_
#define _DevInfoHandler_H_

#include "GB28181Defs.h"

class DeviceCatlogMgr
{
public:
	DeviceCatlogMgr();
	~DeviceCatlogMgr();

public:
	// ��ʼ���豸Ŀ¼����ģ�飬�����豸Ŀ¼��Ϣ
	int Init();
	int Update();
	StruCatalogInfo* ToGB28181Format(int &count);
	void ReleaseInfoBuffer(StruCatalogInfo** info);
};

class DevInfoHandler
{
public:
	static SIP_REPSOND_CODE Handler(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData);
};

#endif//_DevInfoHandler_H_
