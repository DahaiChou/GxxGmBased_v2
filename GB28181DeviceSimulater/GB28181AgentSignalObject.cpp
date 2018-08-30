#include "stdafx.h"
#include "GB28181AgentSignalObject.h"


#include <Windows.h>

GB28181AgentSignalObject::GB28181AgentSignalObject()
: simulate_device_(new GB28181SimulateDevice())
{

}

GB28181AgentSignalObject::~GB28181AgentSignalObject()
{
	delete simulate_device_;
	simulate_device_ = NULL;
}

int GB28181AgentSignalObject::Start(const char *local_ip, int local_port, const char *local_gbcode)
{
	int errCode = 0;
	char dbg_msg[4096] = {0};

	handle_ = GB28181Agent_Init(2, 16, eVERSION_2016, 3000);
	if (handle_ == NULL)
	{
		errCode = -1;
		return errCode;
	}

	local_ip_ = local_ip;
	local_port_ = local_port;
	local_gbcode_ = local_gbcode;

	GS28181_ERR err = GB28181Agent_Start(handle_, local_ip_.c_str(), local_port_, local_gbcode_.c_str(), eTYPE_UDP);
	if (err != GS28181_ERR_SUCCESS)
	{
		sprintf_s(dbg_msg, 4096, "����28181Э��ջʧ�ܣ������룺%d", err);
#ifdef _DEBUG
		OutputDebugStringA(dbg_msg);
#endif
		GB28181Agent_Uninit(handle_);
		return err;
	}

	sprintf_s(dbg_msg, 4096, "����28181Э��ջ�ɹ��������룺%d", err);
#ifdef _DEBUG
	OutputDebugStringA(dbg_msg);
#endif

	// ��װ��Ϣ�ص�
	GB28181Agent_SetLogCB(handle_, _AgentLogCallBack, this);
	GB28181Agent_SetDevInfoQueryCB(handle_, _DevInfoQueryCB, this);
	GB28181Agent_SetDevControlCB(handle_, _DevControlCB, this);
	GB28181Agent_SetPlayControlCB(handle_, _PlayControlCB, this);
	GB28181Agent_SetStreamRequestCB(handle_, _StreamRequestCB, this);
	GB28181Agent_SetNotifyInfoCB(handle_, _NotifyInfo_CallBackFunc, this);
	GB28181Agent_SetExtendRequestCB(handle_, _ExtendRqeustCallBack, this);

	return err;
}

void GB28181AgentSignalObject::Stop()
{
	// �ȷ�ע��
	UnRegister();

	// ֹͣЭ��ջ
	GB28181Agent_Stop(handle_);
}

int GB28181AgentSignalObject::Register(const char *remote_ip, int remote_port, const char *remote_gbcode, const char *authen_username, const char *authen_password)
{
	int errCode = 0;
	char dbg_msg[4096] = {0};

	remote_ip_ = remote_ip;
	remote_port_ = remote_port;
	remote_gbcode_ = remote_gbcode;

	authen_username_ = authen_username;
	authen_password_ = authen_password;

	StruRegistMsg regmsg;
	regmsg.iExpires = 31536000;		// 10��
	strncpy(regmsg.szUserName, authen_username_.c_str(), STR_USERNAME_LEN);
	strncpy(regmsg.szPassword, authen_password_.c_str(), STR_PASSWORD_LEN);
	strncpy(regmsg.stuCnnParam.szIP, remote_ip_.c_str(), STR_IPADDRESS_LEN);
	strncpy(regmsg.stuCnnParam.szGBCode, remote_gbcode_.c_str(), STR_GBCODE_LEN);
	regmsg.stuCnnParam.iPort = remote_port_;

	char datetime[4096] = {0};
	GS28181_ERR err = GB28181Agent_Register(handle_, &regmsg, datetime);
	if (err != GS28181_ERR_SUCCESS)
	{
		sprintf_s(dbg_msg, 4096, "ע�ᵽ��������ʧ�ܣ������룺%d", err);
#ifdef _DEBUG
		OutputDebugStringA(dbg_msg);
#endif
		return err;
	}

	sprintf_s(dbg_msg, 4096, "ע�ᵽ�������سɹ��������룺%d", err);
#ifdef _DEBUG
	OutputDebugStringA(dbg_msg);
#endif

	return err;
}

void GB28181AgentSignalObject::UnRegister()
{
	char dbg_msg[4096] = {0};

	StruRegistMsg regmsg;
	regmsg.iExpires = 0;
	strncpy(regmsg.szUserName, authen_username_.c_str(), STR_USERNAME_LEN);
	strncpy(regmsg.szPassword, authen_password_.c_str(), STR_PASSWORD_LEN);
	strncpy(regmsg.stuCnnParam.szIP, remote_ip_.c_str(), STR_IPADDRESS_LEN);
	strncpy(regmsg.stuCnnParam.szGBCode, remote_gbcode_.c_str(), STR_GBCODE_LEN);
	regmsg.stuCnnParam.iPort = remote_port_;

	char datetime[4096] = {0};
	GS28181_ERR err = GB28181Agent_Register(handle_, &regmsg, datetime);

	sprintf_s(dbg_msg, 4096, "ע���������룺%d", err);
#ifdef _DEBUG
	OutputDebugStringA(dbg_msg);
#endif
}

void GB28181AgentSignalObject::_AgentLogCallBack(EnumLogLevel eLevel, const char * szTemp, int iLen, void * pUserData)
{
	// ��־��Ϣ����Ҫ����Ӧ����SIP��������
	std::string dbg_msg;
	dbg_msg = szTemp;
	
	OutputDebugStringA(dbg_msg.c_str());
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_DevInfoQueryCB(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData)
{
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	int errCode = 0;
	char dbg_msg[4096] = {0};

	// �����յ���ѯ�豸��Ϣ����
	if (stuQuery == NULL)
		return SIP_RESPONSE_CODE_FAIL;

	if (stuQuery->eType == eQUE_DEV_CATALOG)
	{
		// �豸Ŀ¼��ѯ
		StruDeviceCatalog stuCata;
		strncpy(stuCata.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

		int catlog_count = 0;
		StruCatalogInfo *catlog_infos = NULL;
		errCode = object->simulate_device_->GetDeviceCatlog(catlog_count, &catlog_infos);

		stuCata.iSumNum = catlog_count;
		stuCata.ptrCatalog = catlog_infos;

		GS28181_ERR err = GB28181Agent_RespondDevCatalog(hSession, &stuCata, 0, false);
		if( GS28181_ERR_SUCCESS == err )
		{
			sprintf_s(dbg_msg, 4096, "[�豸Ŀ¼��ѯ] ָ����Ӧ�ɹ��������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
		else
		{
			sprintf_s(dbg_msg, 4096, "[�豸Ŀ¼��ѯ] ָ����Ӧʧ�ܣ������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}

		delete [] stuCata.ptrCatalog;
		stuCata.ptrCatalog = NULL;
	}
	else if (stuQuery->eType == eQUE_DEV_INFO)
	{
		// �豸��Ϣ��ѯ
		StruDeviceInfo stuInfo;
		ZeroMemory(&stuInfo, sizeof(StruDeviceInfo));
		strncpy(stuInfo.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

		errCode = object->simulate_device_->GetDeviceInfo(&stuInfo);

		GS28181_ERR err = GB28181Agent_RespondDevInfo(hSession, &stuInfo);
		if( GS28181_ERR_SUCCESS == err )
		{
			sprintf_s(dbg_msg, 4096, "[�豸��Ϣ��ѯ] ָ����Ӧ�ɹ��������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
		else
		{
			sprintf_s(dbg_msg, 4096, "[�豸��Ϣ��ѯ] ָ����Ӧʧ�ܣ������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
	}
	else if (stuQuery->eType == eQUE_DEV_STATUS)
	{
		// �豸״̬��ѯ
		StruDeviceStatus stuStatus;
		ZeroMemory(&stuStatus, sizeof(StruDeviceStatus));
		strncpy(stuStatus.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

		errCode = object->simulate_device_->GetDeviceStatus(&stuStatus);

		GS28181_ERR err = GB28181Agent_RespondDevStatus(hSession, &stuStatus);
		if( GS28181_ERR_SUCCESS == err )
		{
			sprintf_s(dbg_msg, 4096, "[�豸״̬��ѯ] ָ����Ӧ�ɹ��������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
		else
		{
			sprintf_s(dbg_msg, 4096, "[�豸״̬��ѯ] ָ����Ӧʧ�ܣ������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}

		delete stuStatus.ptrAlarm;
		stuStatus.ptrAlarm = NULL;
	}
	else if (stuQuery->eType == eQUE_DEV_RECORDINDEX)
	{
		// ¼��������ѯ
		StruRecordIndex stuIndex;
		strncpy(stuIndex.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

		errCode = object->simulate_device_->GetDeviceRecordInfo(stuQuery->unionQueDescri.stuCataPeriod.czStartTime, stuQuery->unionQueDescri.stuCataPeriod.czEndTime, &stuIndex);

		GS28181_ERR err = GB28181Agent_RespondRecordIndex(hSession, &stuIndex, 0, false);
		if( GS28181_ERR_SUCCESS == err )
		{
			sprintf_s(dbg_msg, 4096, "[¼��������ѯ] ָ����Ӧ�ɹ��������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
		else
		{
			sprintf_s(dbg_msg, 4096, "[¼��������ѯ] ָ����Ӧʧ�ܣ������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}

		delete [] stuIndex.ptrPeroid;
		stuIndex.ptrPeroid = NULL;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_DevControlCB(const char * czSrvGBCode, const StruDevCtrlCmd * stuCmd, void * pUserData)
{
	//
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	char dbg_msg[4096] = {0};
	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_PlayControlCB(STREAM_HANDLE hStream, const StruPlayCtrlCmd * stuCmd, void * pUserData)
{
	//
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	char dbg_msg[4096] = {0};
	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_StreamRequestCB(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest,
								  StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
{
	//
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	int errCode = 0;
	char dbg_msg[4096] = {0};

	if (eRequest == eSTREAM_REALPLAY)
	{
		StruMediaInfo out_media_info;
		ZeroMemory(&out_media_info, sizeof(StruMediaInfo));

		errCode = object->MakeLocalMediaInfo(hStream, pInMedia, &out_media_info);
		if (errCode != 0)
		{
			sprintf_s(dbg_msg, 4096, "[ʵʱ��Ƶ�㲥] ����������������ʧ�ܣ������룺%d", errCode);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
			return SIP_RESPONSE_CODE_DECLINE;
		}

		// 
		// ���ϼ�ƽ̨�������
		GS28181_ERR err = GB28181Agent_RespondPlayRequest(hStream, eSTREAM_REALPLAY, &out_media_info, NULL);
		if( GS28181_ERR_SUCCESS == err )
		{
			sprintf_s(dbg_msg, 4096, "[ʵʱ��Ƶ�㲥] ָ����Ӧ�ɹ��������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif

			// ���ﻹ�к���������Ҫ����֪ͨ������ģ�飬������Ƶ����
			errCode = object->simulate_device_->StartRealStream(hStream, pInMedia);
		}
		else
		{
			sprintf_s(dbg_msg, 4096, "[ʵʱ��Ƶ�㲥] ָ����Ӧʧ�ܣ������룺%d", err);
#ifdef _DEBUG
			OutputDebugStringA(dbg_msg);
#endif
		}
	}
	else if (eRequest == eSTREAM_BYE)
	{
		// ���ҹرյĶ�����������Զ��󣬾͹ر�ʵʱ�����������ʷ���󣬾͹ر���ʷ��
		object->simulate_device_->StopStream(hStream);
	}
	else if (eRequest == eSTREAM_AUDIO)
	{
		// ������Ƶ
	}
	else if (eRequest == eSTREAM_DOWNLOAD)
	{
		// ��ʷ��Ƶ����
	}
	else if (eRequest == eSTREAM_PLAYBACK)
	{
		// ��ʷ��Ƶ�ط�
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_NotifyInfo_CallBackFunc(EnumNotifyType eType, const char * czSrvGBCode, void * pMsg, void * pUserData)
{
	//
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	char dbg_msg[4096] = {0};
	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GB28181AgentSignalObject::_ExtendRqeustCallBack(SESSION_HANDLE hSession, EnumExtendType eType, const char * czTargetDevID, void * pMsg, void * pUserData)
{
	//
	GB28181AgentSignalObject *object = (GB28181AgentSignalObject *)pUserData;
	char dbg_msg[4096] = {0};
	return SIP_RESPONSE_CODE_SUCCESS;
}


int GB28181AgentSignalObject::MakeLocalMediaInfo(STREAM_HANDLE hStream, StruMediaInfo *remote_media_info, StruMediaInfo *local_media_info)
{
	int errCode = 0;

	// �ȷ�����������Ϣ
	if (remote_media_info)
	{
		// ����Ĵ���Ӧ���ر�ע�⣬Ӧ������Ϊ���ز������ظ��ϼ�ƽ̨��
		// ������һЩ����Ӧ�ý���ת��
		ZeroMemory(local_media_info, sizeof(local_media_info));

		// ����Ŀ���豸ID
		strncpy(local_media_info->czDeviceID, remote_media_info->czDeviceID, STR_GBCODE_LEN);
		// ��������RTP�������IP��ַ
		strncpy(local_media_info->czIP, remote_media_info->czIP, STR_IPADDRESS_LEN);

		// �����ֵʱ��Э������������������Play/Playback/download/Talk��
		strncpy(local_media_info->czMediaName, remote_media_info->czMediaName, STR_NAME_LEN);

		// �Ƿ�֧��RTCP
		local_media_info->bEnableRTCP = remote_media_info->bEnableRTCP;

		// ���䷽ʽ������Ҫת���ɱ��صģ�����������Ҫ��Rtp���ͷ�ת
		if (remote_media_info->eRtpType == eRTP_OVER_UDP)
		{
			// RTP over UDP����
			local_media_info->eRtpType = eRTP_OVER_UDP;
		}
		else if (remote_media_info->eRtpType == eRTP_OVER_TCP_ACTIVE)
		{
			// Զ����RTP over TCP��������
			// ���������Ӧ���Ǳ�������
			local_media_info->eRtpType = eRTP_OVER_TCP_PASSIVE;
		}
		else if (remote_media_info->eRtpType = eRTP_OVER_TCP_PASSIVE)
		{
			// RTP over TCP��������
			local_media_info->eRtpType = eRTP_OVER_TCP_ACTIVE;
		}

		//��������  1:������  2:������
		local_media_info->iStreamType = remote_media_info->iStreamType;

		// ��ʼUNIXʱ���
		local_media_info->iStart = remote_media_info->iStart;
		// ����UNIXʱ���
		local_media_info->iEnd = remote_media_info->iEnd;

		//�����������ʱ������ɲ���
		//Ϊ��֤SSRC�������ԣ�����ͳһ�о���Ҫôÿ�β��Ҫôÿ�ζ���
		local_media_info->iSSRC = remote_media_info->iSSRC;

		local_media_info->stuDescri.iDescriNum = remote_media_info->stuDescri.iDescriNum;
		for (int index = 0; index < local_media_info->stuDescri.iDescriNum; ++index)
		{
			local_media_info->stuDescri.mapDescri[index].eMediaType = remote_media_info->stuDescri.mapDescri[index].eMediaType;
			local_media_info->stuDescri.mapDescri[index].iRtpmapNum = remote_media_info->stuDescri.mapDescri[index].iRtpmapNum;
			for (int index2 = 0; index2 < local_media_info->stuDescri.mapDescri[index].iRtpmapNum; ++index2)
			{
				strncpy(local_media_info->stuDescri.mapDescri[index].mapRtp[index2].czMimeType, remote_media_info->stuDescri.mapDescri[index].mapRtp[index2].czMimeType, STR_RTPTYPENAME_LEN);
				local_media_info->stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat = remote_media_info->stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat;
				local_media_info->stuDescri.mapDescri[index].mapRtp[index2].iSampleRate = remote_media_info->stuDescri.mapDescri[index].mapRtp[index2].iSampleRate;
			}
		}
	}

	unsigned short local_port = 0;
	errCode = simulate_device_->AddRealStream(hStream, remote_media_info->iSSRC, local_port);
	local_media_info->iPort = local_port;

	return errCode;
}