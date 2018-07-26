#include "GB28181DownImp.h"
#include "GB28181Defs.h"
#include "GB28181AgentSDK.h"
#include "GSTime.h"

#include "LogHandler.h"
#include "DevInfoHandler.h"
#include "DevControlHandler.h"
#include "PlayControlHandler.h"
#include "StreamRequestHandler.h"
#include "NotifyInfoHandler.h"
#include "ExtendRqeustHandler.h"

#include <iostream>

GB28181DownImp::GB28181DownImp()
: handler_(NULL)
, device_catlog_mgr_(new DeviceCatlogMgr())
{

}

GB28181DownImp::~GB28181DownImp()
{
	if (device_catlog_mgr_ != NULL)
		delete device_catlog_mgr_;
	device_catlog_mgr_ = NULL;
}

int GB28181DownImp::Init(const char *local_ip, int local_port, const char *local_gbcode, const char *remote_ip, int remote_port, const char *remote_gbcode, const char *username, const char *password)
{
	int errCode = 0;

	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��28181����
	// ����1�̣߳����2�߳�
	// GB/T 28181-2016 Э��
	// 2�볬ʱ
	handler_ = GB28181Agent_Init(1, 2, Enum28181Version::eVERSION_2016, 2000);
	if (handler_ == NULL)
	{
		std::cout<<"��ʼ����������ʧ�ܣ�"<<std::endl;
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��װ�ص�
	GB28181Agent_SetLogCB(handler_, LogHandler::Handler, this);
	GB28181Agent_SetDevInfoQueryCB(handler_, DevInfoHandler::Handler, this);
	GB28181Agent_SetDevControlCB(handler_, DevControlHandler::Handler, this);
	GB28181Agent_SetStreamRequestCB(handler_, StreamRequestHandler::Handler, this);
	GB28181Agent_SetPlayControlCB(handler_, PlayControlHandler::Handler, this);
	GB28181Agent_SetNotifyInfoCB(handler_, NotifyInfoHandler::Handler, this);
	GB28181Agent_SetExtendRequestCB(handler_, ExtendRqeustHandler::Handler, this);

	//////////////////////////////////////////////////////////////////////////
	// �����������أ�Ҳ���Ǵ���ˣ�
	errCode = GB28181Agent_Start(handler_, local_ip, local_port, local_gbcode, EnumTransType::eTYPE_UDP);
	if (errCode != 0)
	{
		std::cout<<"������������ʧ�ܣ������룺"<<errCode<<std::endl;
		return errCode;
	}

	//////////////////////////////////////////////////////////////////////////
	// ע��
	StruRegistMsg reg_msg;
	bzero(&reg_msg, sizeof(reg_msg));
	gs_strncpy(reg_msg.stuCnnParam.szIP, remote_ip, strlen(remote_ip));
	gs_strncpy(reg_msg.stuCnnParam.szGBCode, remote_gbcode, strlen(remote_gbcode));
	reg_msg.stuCnnParam.iPort = remote_port;
	gs_strncpy(reg_msg.szUserName, username, strlen(username));
	gs_strncpy(reg_msg.szPassword, password, strlen(password));
	reg_msg.iExpires = 3600;

	char remote_datetime[4096] = {0};
	GS28181_ERR err = GB28181Agent_Register(handler_, &reg_msg, remote_datetime);
	if (err != GS28181_ERR_SUCCESS)
	{
		std::cout<<"ע�ᵽ�ϼ�ƽ̨ʧ�ܣ������룺"<<err<<std::endl;
		return err;
	}

	//////////////////////////////////////////////////////////////////////////
	// ע��ɹ��ˣ�����������Ҫͬ��ʱ���

	//////////////////////////////////////////////////////////////////////////
	// �������������̣߳����߿�һ����ʱ����ʱ��������

	return 0;
}

void GB28181DownImp::Close()
{

}

void GB28181DownImp::Run()
{
	while (true)
		Sleep(5000);
}