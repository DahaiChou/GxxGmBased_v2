#ifndef _GxxGmDSJSimulater_H_
#define _GxxGmDSJSimulater_H_

#include "GxxGmDeviceInfo.h"
#include "GxxGmDSJSimulaterStreamMgr.h"
#include "GB28181AgentSDK.h"
#include <string>

#include "Poco/Thread.h"
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"

class GxxGmWordSpeaker;

struct SimulaterInitInfo
{
	std::string local_ip_;
	std::string local_port_;
	std::string local_gbcode_;

	std::string server_ip_;
	std::string server_port_;
	std::string server_gbcode_;
	std::string username_;
	std::string password_;

	int manual_port_;
	unsigned short begin_port_;
	unsigned short end_port_;

	std::string sip_net_;
	std::string rtp_net_;
	std::string stream_file_;
	int gb28181_hb_time_;
	int dev_baseinfo_time_;
	int dev_location_time_;

	std::string imei_;
	std::string platform_id_;
};

class GxxGmDSJSimulaterNotifer
{
public:
	// �յ��豸�����󶨻ظ�
	virtual void RecvBindUser(const char *result) = 0;

	// �յ�Զ�̰��û�����
	virtual void RecvRemoteBindUser() = 0;

	// �յ�����
	virtual void RecvEmergency(const char *emergency_id, const char *start_time, const char *end_time) = 0;
};

class GxxGmDSJSimulater : public GxxGmDSJSimulaterNotifer
{
public:
	GxxGmDSJSimulater();
	~GxxGmDSJSimulater();

public:
	// ����
	int Initialize(struct SimulaterInitInfo &init_info);

	// �ػ�
	void Destroy();

	void SetNotifer(GxxGmDSJSimulaterNotifer *notifer);

public:
	// �����豸������Ϣ
	void SetBaseInfo(DEVICE_BASE_INFO base_info);
	// �����豸��λ��Ϣ
	void SetLocationInfo(DEVICE_LOCATION_INFO location_info);
	// �����豸�쳣��Ϣ
	void SetExceptionInfo(DEVICE_EXCEPTION_INFO excep_info);

public:
	// ���Ͱ��û�����
	int SendBindUserInfo(const char *user_id, const char *password);
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
	// �յ��豸�����󶨻ظ�
	virtual void RecvBindUser(const char *result);
	// �յ�Զ�̰��û�����
	virtual void RecvRemoteBindUser();
	// �յ�����
	virtual void RecvEmergency(const char *emergency_id, const char *start_time, const char *end_time);

public:
	static void GB28181HeartbeatThreadFun(void *param);
	Poco::Thread gb28181_heartbeat_thread_;
	bool is_gb28181_heartbeat_thread_need_exit_;
	bool is_standard_gb28181_mobile_position_;

public:
	StruRegistMsg reg_msg;

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
	// �򵥴ֱ�һ�㣬�ĵ���־��ֱ������ͨͨ��д�ĵ�
	HANDLE log_file_handle_;

public:
	int gb28181_hb_time_;		// �����������ʱ��
	int dev_baseinfo_time_;		// ������Ϣ���ʱ��
	int dev_location_time_;		// ��λ��Ϣ���ʱ��

	std::string imei_;			// ģ����IMEI
	std::string platform_id_;

	DEVICE_BASE_INFO base_info_;
	DEVICE_LOCATION_INFO location_info_;
	DEVICE_EXCEPTION_INFO exception_info_;

public:
	// ˮӡ��������
	std::string dev_name_;
	std::string frame_rate_;
	std::string bit_rate_;
	std::string trans_resolution_;

	std::string machine_id_;
	std::string bind_user_id_;		// ���Ҳ�ͱ����ļ�¼���Լ��ļ��ϴ�����
	std::string bind_user_name_;	// ���Ҳ�ͱ����ļ�¼���Լ��ļ��ϴ�����
	std::string dep_name_;
	std::string dep_id_;
	std::string show_location_;
	std::string show_datetime_;

public:
	GxxGmDSJSimulaterStreamMgr stream_mgr_;
	GxxGmDSJSimulaterNotifer *notifer_;

public:
	GxxGmWordSpeaker *speaker_;
};

#endif//_GxxGmDSJSimulater_H_
