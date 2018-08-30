#include "stdafx.h"
#include "GB28181ServerSignalObject.h"

GB28181ServerSignalObject::GB28181ServerSignalObject()
: handle_(NULL)
{

}

GB28181ServerSignalObject::~GB28181ServerSignalObject()
{

}

int GB28181ServerSignalObject::Start(const char *srv_ip, int srv_port, const char *srv_gbcode, const char *srv_authen_pin)
{
	int errCode = 0;
	char dbg_msg[4096] = {0};

	handle_ = GB28181Server_Init(2, 8, Enum28181Version::eVERSION_2016, 3000);
	if (handle_ == NULL)
	{
		errCode = -1;
		return errCode;
	}

	srv_ip_ = srv_ip;
	srv_port_ = srv_port_;
	srv_gbcode_ = srv_gbcode;

	GS28181_ERR err = GB28181Server_Start(handle_, srv_ip, srv_port_, srv_gbcode, srv_authen_pin, EnumTransType::eTYPE_UDP);
	if (err != GS28181_ERR_SUCCESS)
	{
		sprintf_s(dbg_msg, 4096, "����28181Э��ջʧ�ܣ������룺%d", err);
#ifdef _DEBUG
		OutputDebugStringA(dbg_msg);
#endif
		GB28181Server_Uninit(handle_);
		return err;
	}

	sprintf_s(dbg_msg, 4096, "����28181Э��ջ�ɹ��������룺%d", err);
#ifdef _DEBUG
	OutputDebugStringA(dbg_msg);
#endif

	// ע��ص�
	GB28181Server_SetLogCallBack(handle_, ServerLogCallBack, this);
	GB28181Server_SetRegistCallBack(handle_ _DevRegist_CallBackFunc, this);
	GB28181Server_SetNotifyCallBack(handle_, NotifyInfo_CallBackFunc, this);
	GB28181Server_SetStreamReqCallBack(handle_, StreamRequest_CallBackFunc, this);

	return errCode;
}

void GB28181ServerSignalObject::Stop()
{
	GB28181Server_Stop(handle_);
	GB28181Server_Uninit(handle_);
}

SIP_REPSOND_CODE GB28181ServerSignalObject::_DevRegist_CallBackFunc(const StruRegistMsg * stuMsg, void * pUserData)
{
	GB28181ServerSignalObject *object = (GB28181ServerSignalObject*)pUserData;

	// ���豸/ƽ̨ע��
	// �����Ȩͨ���ˣ���ô�ͽ���Ϣ��������
	

	return SIP_RESPONSE_CODE_SUCCESS;
}

void GB28181ServerSignalObject::_QueryResult_CallBackFunc(EnumQueryType eType, const char * czAgentGBCode, void * pMsg, void * pUserData)
{
	GB28181ServerSignalObject *object = (GB28181ServerSignalObject*)pUserData;

	// ��ѯ����ظ�

	return ;
}

SIP_REPSOND_CODE GB28181ServerSignalObject::_NotifyInfo_CallBackFunc(EnumNotifyType eType, const char * czAgentGBCode, void * pMsg, void * pUserData)
{
	GB28181ServerSignalObject *object = (GB28181ServerSignalObject*)pUserData;

	// �¼��豸/ƽ̨��������֪ͨ����

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181ServerSignalObject::_StreamRequest_CallBackFunc(STREAM_HANDLE hStream, const char * czAgentGode, EnumStreamRequest eRequest, 
		const StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
{
	GB28181ServerSignalObject *object = (GB28181ServerSignalObject*)pUserData;

	// ������յ�����˻ظ���������������

	return SIP_RESPONSE_CODE_SUCCESS;
}

void GB28181ServerSignalObject::_ServerLogCallBack(EnumLogLevel eLevel, const char * szTemp, int iLen, void * pUserData)
{
	GB28181ServerSignalObject *object = (GB28181ServerSignalObject*)pUserData;

	// 28181Э��ջͨ����־

	return ;
}
