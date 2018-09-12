#ifndef _GxxGmDSJSimulater_H_
#define _GxxGmDSJSimulater_H_

#include "GxxGmDeviceInfo.h"
#include "GB28181AgentSDK.h"
#include <string>

class GxxGmDSJSimulater
{
public:
	GxxGmDSJSimulater();
	~GxxGmDSJSimulater();

public:
	// ����
	int Initialize(const char *local_ip, const char *local_port, const char *local_gbcode, const char *server_ip, const char *server_port, const char *server_gbcode, const char *username, const char *password);

	// �ػ�
	void Destroy();

public:
	// �����豸������Ϣ
	void SetBaseInfo(DEVICE_BASE_INFO base_info);
	// �����豸��λ��Ϣ
	void SetLocationInfo(DEVICE_LOCATION_INFO location_info);
	// �����豸�쳣��Ϣ
	void SetExceptionInfo(DEVICE_EXCEPTION_INFO excep_info);

public:
	// ���Ͱ��û�����
	int SendBindUserInfo(const char *platform_id, const char *device_imei, const char *user_id, const char *password);
	// �����豸������Ϣ
	int SendBaseInfo();
	// �����豸��λ��Ϣ
	int SendLocationInfo();
	// �����豸�쳣��Ϣ
	int SendExceptionInfo();
	// �����豸�澯��Ϣ
	int SendAlarmInfo();
	// ��������ʶ����Ϣ
	int SendFaceInfo(const char *face_img, int face_img_len);
	// ���ͳ���ʶ����Ϣ
	int SendCarIdInfo();

public:
	static void _AgentLogCallBack(EnumLogLevel eLevel, const char * szTemp, int iLen, void * pUserData);
	static SIP_REPSOND_CODE _DevInfoQueryCB(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData);
	static SIP_REPSOND_CODE _DevControlCB(const char * czSrvGBCode, const StruDevCtrlCmd * stuCmd, void * pUserData);
	static SIP_REPSOND_CODE _PlayControlCB(STREAM_HANDLE hStream, const StruPlayCtrlCmd * stuCmd, void * pUserData);
	static SIP_REPSOND_CODE _StreamRequestCB(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest, StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData);
	static SIP_REPSOND_CODE _NotifyInfo_CallBackFunc(EnumNotifyType eType, const char * czSrvGBCode, void * pMsg, void * pUserData);
	static SIP_REPSOND_CODE _ExtendRqeustCallBack(SESSION_HANDLE hSession, EnumExtendType eType, const char * czTargetDevID, void * pMsg, void * pUserData);

public:
	std::string local_ip_;			// ִ����IP
	std::string local_port_;		// ִ���Ƕ˿ں�
	std::string local_gbcode_;		// ִ���ǹ�����
	std::string server_ip_;			// ��������IP
	std::string server_port_;		// ����˿�
	std::string server_gbcode_;		// ���������
	std::string username_;			// ��Ȩ�û���
	std::string password_;			// ��Ȩ����

	AGENT_HANDLE agent_;

public:
	DEVICE_BASE_INFO base_info_;
	DEVICE_LOCATION_INFO location_info_;
	DEVICE_EXCEPTION_INFO exception_info_;

public:
	std::string bind_user_id_;
};

#endif//_GxxGmDSJSimulater_H_
