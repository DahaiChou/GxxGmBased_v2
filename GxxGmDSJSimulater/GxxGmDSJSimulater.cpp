#include "GxxGmDSJSimulater.h"
#include <Windows.h>
#include <sstream>

#include "Poco/Base64Encoder.h"


GxxGmDSJSimulater::GxxGmDSJSimulater()
: agent_(NULL)
, is_gb28181_heartbeat_thread_need_exit_(false)
, log_file_handle_(NULL)
{
	// 
}

GxxGmDSJSimulater::~GxxGmDSJSimulater()
{
	// 
}

int GxxGmDSJSimulater::Initialize(const char *local_ip, const char *local_port, const char *local_gbcode, const char *server_ip, const char *server_port, const char *server_gbcode, const char *username, const char *password, int is_manual_port, unsigned short begin_port, unsigned short end_port)
{
	int errCode = 0;

	// ��ʼ��Э��ջ
	agent_ = GB28181Agent_Init(2, 16, Enum28181Version::eVERSION_2016, 3000);
	if (agent_ == NULL)
	{
		// 
		printf("[%s]��ʼ��28181Э��ջʧ�ܣ�\n", local_gbcode);
		return -1;
	}

	// ��װ�ص�
	GB28181Agent_SetLogCB(agent_, _AgentLogCallBack, this);
	GB28181Agent_SetDevInfoQueryCB(agent_, _DevInfoQueryCB, this);
	GB28181Agent_SetDevControlCB(agent_, _DevControlCB, this);
	GB28181Agent_SetPlayControlCB(agent_, _PlayControlCB, this);
	GB28181Agent_SetStreamRequestCB(agent_, _StreamRequestCB, this);
	GB28181Agent_SetNotifyInfoCB(agent_, _NotifyInfo_CallBackFunc, this);
	GB28181Agent_SetExtendRequestCB(agent_, _ExtendRqeustCallBack, this);

	GS28181_ERR err = GB28181Agent_Start(agent_, local_ip, atoi(local_port), local_gbcode, EnumTransType::eTYPE_UDP);
	if (err != GS28181_ERR_SUCCESS)
	{
		printf("[%s]����28181Э��ջʧ�ܣ������룺%d\n", local_gbcode, err);
		GB28181Agent_Uninit(agent_);
		return err;
	}

	StruRegistMsg reg_msg;
	
	reg_msg.iExpires = 86400;	// 1��
	strcpy_s(reg_msg.szUserName, STR_USERNAME_LEN, username);
	strcpy_s(reg_msg.szPassword, STR_PASSWORD_LEN, password);
	strcpy_s(reg_msg.stuCnnParam.szIP, STR_IPADDRESS_LEN, server_ip);
	reg_msg.stuCnnParam.iPort = (unsigned int)atoi(server_port);
	strcpy_s(reg_msg.stuCnnParam.szGBCode, STR_GBCODE_LEN, server_gbcode);
	
	char date_time[4096] = {0};
	err = GB28181Agent_Register(agent_, &reg_msg, date_time);
	if (err != GS28181_ERR_SUCCESS)
	{
		printf("[%s]ע�ᵽ��������ʧ�ܣ������룺%d\n", local_gbcode, err);
		GB28181Agent_Stop(agent_);
		GB28181Agent_Uninit(agent_);
		return err;
	}

	printf("[%s]ע�ᵽ�������سɹ��������룺%d\n", local_gbcode, err);

	// ��ʼ����־
	SYSTEMTIME st;
	GetLocalTime(&st);
	char current_time[128] = {0};
	sprintf_s(current_time, 128, "%d-%02d-%02d %02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	char log_file_name[4096] = {0};
	sprintf_s(log_file_name, 4096, "%s.log", current_time, local_gbcode);

	char current_program_path[4096] = {0};
	GetModuleFileNameA(NULL, current_program_path, 4096);
	std::string tmp = current_program_path;
	int pos = tmp.find_last_of('\\');

	std::string log_path = tmp.substr(0, pos + 1);
	log_path.append("log");
	CreateDirectoryA(log_path.c_str(), NULL);
	log_path.append("\\");
	log_path.append(local_gbcode);
	CreateDirectoryA(log_path.c_str(), NULL);
	log_path.append("\\");
	log_path.append(log_file_name);

	log_file_handle_ = CreateFileA(log_path.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (log_file_handle_ == INVALID_HANDLE_VALUE)
	{
		errCode = GetLastError();
		printf("[%s]������־�ļ�ʧ�ܣ�\n", local_gbcode);
	}

	// �������
	local_ip_ = local_ip;
	local_port_ = local_port;
	local_gbcode_ = local_gbcode;
	server_ip_ = server_ip;
	server_port_ = server_port;
	server_gbcode_ = server_gbcode;
	username_ = username;
	password_ = password;

	// ��ʼ��������ģ��
	errCode = stream_mgr_.Initialize(is_manual_port, begin_port, end_port, local_ip);
	if (errCode != 0)
	{
		printf("[%s]��ʼ����������ʧ�ܣ�\n", local_gbcode);
	}

	return errCode;
}

void GxxGmDSJSimulater::Destroy()
{
	// �ȷ������ֹͣ�������ͣ�����ֹͣ������Ϣ����λ��Ϣ������
	is_gb28181_heartbeat_thread_need_exit_ = true;
	gb28181_heartbeat_thread_.join();

	// ��ע��
	StruRegistMsg reg_msg;

	reg_msg.iExpires = 0;	// 1��
	strcpy_s(reg_msg.szUserName, STR_USERNAME_LEN, username_.c_str());
	strcpy_s(reg_msg.szPassword, STR_PASSWORD_LEN, password_.c_str());
	strcpy_s(reg_msg.stuCnnParam.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	reg_msg.stuCnnParam.iPort = (unsigned int)atoi(server_port_.c_str());
	strcpy_s(reg_msg.stuCnnParam.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());

	char date_time[4096] = {0};
	GB28181Agent_Register(agent_, &reg_msg, date_time);

	GB28181Agent_Stop(agent_);
	GB28181Agent_Uninit(agent_);

	CloseHandle(log_file_handle_);
	log_file_handle_ = NULL;
}

void GxxGmDSJSimulater::SetBaseInfo(DEVICE_BASE_INFO base_info)
{
	// 
	base_info_.battery_			= base_info.battery_;
	base_info_.carrieroperator_	= base_info.carrieroperator_;
	base_info_.cpu_				= base_info.cpu_;
	base_info_.local_record_	= base_info.local_record_;
	base_info_.net_type_		= base_info.net_type_;
	base_info_.signal_			= base_info.signal_;
	base_info_.storage_			= base_info.storage_;
	base_info_.version_			= base_info.version_;
}

void GxxGmDSJSimulater::SetLocationInfo(DEVICE_LOCATION_INFO location_info)
{
	// 
	location_info_.direction_			= location_info.direction_;
	location_info_.division_ew_			= location_info.division_ew_;
	location_info_.division_ns_			= location_info.division_ns_;
	location_info_.height_				= location_info.height_;
	location_info_.latitude_			= location_info.latitude_;
	location_info_.location_available_	= location_info.location_available_;
	location_info_.location_time_		= location_info.location_time_;
	location_info_.longitude_			= location_info.longitude_;
	location_info_.radius_				= location_info.radius_;
	location_info_.satellites_			= location_info.satellites_;
	location_info_.speed_				= location_info.speed_;
}

void GxxGmDSJSimulater::SetExceptionInfo(DEVICE_EXCEPTION_INFO excep_info)
{
	// 
	exception_info_.battery_	= excep_info.battery_;
	exception_info_.ccd_		= excep_info.ccd_;
	exception_info_.storage_	= excep_info.storage_;
	exception_info_.mic_		= excep_info.mic_;
}

int GxxGmDSJSimulater::SendBindUserInfo(const char *platform_id, const char *device_imei, const char *user_id, const char *password)
{
	// ����Ҫ�󹹽��ַ���
	const char *msg_format = "<SubCmdType>BindUser</SubCmdType> \
							  <DeviceStates> \
									<PlatformID>GM_SHENZHEN</PlatformID> \
									<DeviceIMEI>358F27F145B</DeviceIMEI> \
									<UserID>GM001</UserID> \
									<Password>111111</Password> \
							  </DeviceStates>";

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format, 
		platform_id, device_imei, user_id, password);

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]�����豸������ʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendBaseInfo()
{
	// ����Ҫ�󹹽��ַ���
	const char *msg_format = "<SubCmdType>DeviceInfo</SubCmdType> \
							  <DeviceStates> \
									<Carrieroperator>%s</Carrieroperator> \
									<Nettype>%s</Nettype> \
									<Signal>%s</Signal> \
									<Battery>%s</Battery> \
									<Storage>%s</Storage> \
									<CPU>%s</CPU> \
									<Version>%s</Version> \
									<LocalRecord>%s</LocalRecord> \
							 </DeviceStates>";

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format,
		base_info_.carrieroperator_.c_str(), base_info_.net_type_.c_str(), base_info_.signal_.c_str(),
		base_info_.battery_.c_str(), base_info_.storage_.c_str(), base_info_.cpu_.c_str(),
		base_info_.version_.c_str(), base_info_.local_record_.c_str());

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]�����豸������Ϣʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendLocationInfo()
{
	// ����Ҫ�󹹽��ַ���
	const char *msg_format = "<SubCmdType>LocationInfo</SubCmdType> \
							  <Location> \
									<DivisionNS>%s</DivisionNS> \
									<DivisionEW>%s</DivisionEW> \
									<Radius>%s</Radius > \
									<Longitude>%s</Longitude> \
									<Latitude>%s</Latitude> \
									<Direction>%s</Direction> \
									<Speed>%s</Speed> \
									<Height>%s</Height> \
									<LocationTime>%s</LocationTime> \
									<Satellites>%s</Satellites> \
									<LocationAvailable>%s</LocationAvailable> \
							 </Location>";

	// ����Ƚ��ر�ÿ�η�֮ǰ��ʱ������������¼���һ��
	SYSTEMTIME st;
	GetLocalTime(&st);
	char current_time[128] = {0};
	sprintf_s(current_time, 128, "%d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format,
		location_info_.division_ns_.c_str(), location_info_.division_ew_.c_str(), location_info_.radius_.c_str(),
		location_info_.longitude_.c_str(), location_info_.latitude_.c_str(), location_info_.direction_.c_str(),
		location_info_.speed_.c_str(), location_info_.height_.c_str(), current_time, location_info_.satellites_.c_str(),
		location_info_.location_available_.c_str());

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]�����豸��λ��Ϣʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendExceptionInfo()
{
	const char *msg_format = "<SubCmdType>DeviceException</SubCmdType> \
							  <Exceptions> \
									<Storage>NotFound</Storage> \
									<Battery>Removed</Battery> \
									<CCD>Error</CCD> \
									<MIC>Error</MIC> \
							  </Exceptions>";

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format,
		exception_info_.storage_.c_str(), exception_info_.battery_.c_str(), exception_info_.ccd_.c_str(), exception_info_.mic_.c_str());

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]�����豸�쳣��Ϣʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendAlarmInfo()
{
	const char *msg_format = "<SubCmdType>DeviceSOS</SubCmdType> \
							  <DeviceSOSInfo> \
									<Datetime>%s</Datetime> \
									<Location>  \
										<DivisionNS>%s</DivisionNS> \
										<DivisionEW>%s</DivisionEW> \
										<Radius>%s</Radius > \
										<Longitude>%s</Longitude> \
										<Latitude>%s</Latitude> \
										<Direction>%s</Direction> \
										<Speed>%s</Speed> \
										<Height>%s</Height> \
										<LocationTime>%s</LocationTime> \
										<Satellites>%s</Satellites> \
										<LocationAvailable>%s</LocationAvailable> \
									</Location> \
							</DeviceSOSInfo>";

	// ����Ƚ��ر�ÿ�η�֮ǰ��ʱ������������¼���һ��
	SYSTEMTIME st;
	GetLocalTime(&st);
	char current_time[128] = {0};
	char current_datetime[128] = {0};
	sprintf_s(current_time, 128, "%d-%02d-%02d %02d-%02d-%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	sprintf_s(current_datetime, 128, "%d-%02d-%02d %02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format, current_datetime,
		location_info_.division_ns_.c_str(), location_info_.division_ew_.c_str(), location_info_.radius_.c_str(),
		location_info_.longitude_.c_str(), location_info_.latitude_.c_str(), location_info_.direction_.c_str(),
		location_info_.speed_.c_str(), location_info_.height_.c_str(), current_time, location_info_.satellites_.c_str(),
		location_info_.location_available_.c_str());

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]�����豸�澯��Ϣʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendFaceInfo(const char *face_img, int face_img_len)
{
	const char *msg_format = "<SubCmdType>FaceRecognition</SubCmdType> \
							  <FaceInfo> \
									<FaceImageData>%s</FaceImageData> \
									<DateTime>%s</DateTime> \
							  </FaceInfo>";

	SYSTEMTIME st;
	GetLocalTime(&st);
	char current_datetime[128] = {0};
	sprintf_s(current_datetime, 128, "%d-%02d-%02d %02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	// ��������base64����
	std::ostringstream str;
	Poco::Base64Encoder encoder(str);
	encoder<<std::string(face_img, face_img_len);
	encoder.close();

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format, str.str(), current_datetime);

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]��������ʶ������ʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

int GxxGmDSJSimulater::SendCarIdInfo()
{
	return -1;
	const char *msg_format = "<SubCmdType>CarIDRecognition</SubCmdType> \
							<CarIDInfo> \
							<CarIDImageData>1A2B3C4D5F1A2B3C4D5F��</CarIDImageData> \
							<DateTime>2018-08-15 18:15:07</DateTime> \
							</CarIDInfo>";

	char msg[4096] = {0};
	sprintf_s(msg, 4096, msg_format,
		base_info_.carrieroperator_.c_str(), base_info_.net_type_.c_str(), base_info_.signal_.c_str(),
		base_info_.battery_.c_str(), base_info_.storage_.c_str(), base_info_.cpu_.c_str(),
		base_info_.version_.c_str(), base_info_.local_record_.c_str());

	// ���ýӿڣ�����͸����Ϣ
	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, server_gbcode_.c_str());
	connention_param.iPort = atoi(server_port_.c_str());

	// �����Ƿ�Ҫ����һ�±�������
	GS28181_ERR err = GB28181Agent_NotifyTransData(agent_, &connention_param, local_gbcode_.c_str(), msg, strlen(msg));
	if (err != GS28181_ERR_SUCCESS)
	{
		// 
		printf("[%s]���ͳ���ʶ������ʧ�ܣ������룺%d\n", local_gbcode_.c_str(), err);
	}

	return err;
}

void GxxGmDSJSimulater::_AgentLogCallBack(EnumLogLevel eLevel, const char * szTemp, int iLen, void * pUserData)
{
	// �������е�SIPͨ����־�����Կ���д����־�ļ�
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	// ��Ļ���
	printf("%s\n", szTemp);

	// dbgview���
	OutputDebugStringA(szTemp);

	// ��־�ļ����
	DWORD written = 0;
	WriteFile(simulater->log_file_handle_, szTemp, iLen, &written, NULL);
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_DevInfoQueryCB(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData)
{
	// �����ﱻ��ѯĿ¼֮�����������̣߳���ʼ����28181��������
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	int errCode = 0;
	char dbg_msg[4096] = {0};

	if (stuQuery == NULL)
		return SIP_RESPONSE_CODE_FAIL;

	if (stuQuery->eType == EnumQueryType::eQUE_DEV_INFO)
	{
		// �豸��Ϣ��ѯ
		StruDeviceInfo stuInfo;
		ZeroMemory(&stuInfo, sizeof(StruDeviceInfo));
		strcpy_s(stuInfo.czGBCode, STR_GBCODE_LEN, stuQuery->czGBCode);

		stuInfo.bResultOK = true;
		strcpy_s(stuInfo.czDeviceName, STR_NAME_LEN, "�����˹���ģ������ͷ");
		strcpy_s(stuInfo.czManufacturer, STR_MANUFACTURER_LEN, "GOSUNCN");
		strcpy_s(stuInfo.czModel, STR_MODEL_LEN, "GXX-GM-SIMULATE-001");
		strcpy_s(stuInfo.czFirmware, STR_FIREWARE_LEN, "V1.0, build 0001");
		stuInfo.iChanal = 1;

		GS28181_ERR err = GB28181Agent_RespondDevInfo(hSession, &stuInfo);
		if (err != GS28181_ERR_SUCCESS)
		{
			printf("[%s]��Ӧ�豸��Ϣ��ѯʧ�ܣ������룺%d\n", simulater->local_gbcode_.c_str(), err);
			return SIP_RESPONSE_CODE_SUCCESS;
		}
	}
	else if (stuQuery->eType == EnumQueryType::eQUE_DEV_STATUS)
	{
		// �豸״̬��ѯ
		StruDeviceStatus stuStatus;
		ZeroMemory(&stuStatus, sizeof(StruDeviceStatus));
		strcpy_s(stuStatus.czGBCode, STR_GBCODE_LEN, stuQuery->czGBCode);

		SYSTEMTIME st;
		GetLocalTime(&st);
		char device_datetime[STR_DATETIME_LEN] = {0};
		sprintf_s(device_datetime, STR_DATETIME_LEN, "%d-%02d-%02dT%02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		strcpy_s(stuStatus.czDevDateTime, STR_DATETIME_LEN, device_datetime);
		strcpy_s(stuStatus.czErrReason, STR_ERRREASON_LEN, "NO ERROR");
		stuStatus.bResult = true;
		stuStatus.bOnLine = true;
		stuStatus.bRecord = false;
		stuStatus.bEncode = false;
		stuStatus.bStatusOK = true;
		stuStatus.iAlarmNum = 1;

		StruAlarmStatus * stuAlarm = new StruAlarmStatus;
		ZeroMemory(stuAlarm, sizeof(StruAlarmStatus));
		strcpy_s(stuAlarm->czAlarmID, STR_ALARMID_LEN, simulater->local_gbcode_.c_str());	// �����豸����
		stuAlarm->eStatus = eONDUTY;

		stuStatus.ptrAlarm = stuAlarm;

		GS28181_ERR err = GB28181Agent_RespondDevStatus(hSession, &stuStatus);

		delete stuAlarm;
		stuAlarm = NULL;

		if (err != GS28181_ERR_SUCCESS)
		{
			printf("[%s]��Ӧ�豸״̬��ѯʧ�ܣ������룺%d\n", simulater->local_gbcode_.c_str(), err);
			return SIP_RESPONSE_CODE_SUCCESS;
		}
	}
	else if (stuQuery->eType == EnumQueryType::eQUE_DEV_CATALOG)
	{
		// �豸Ŀ¼��ѯ
		StruDeviceCatalog stuCata;
		strcpy_s(stuCata.czGBCode, STR_GBCODE_LEN, stuQuery->czGBCode);

		stuCata.iSumNum = 1;
		
		StruCatalogInfo *catlog = new StruCatalogInfo[stuCata.iSumNum];
		memset(catlog, 0, sizeof(StruCatalogInfo) * stuCata.iSumNum);

		strcpy_s(catlog[0].czGBCode, STR_GBCODE_LEN, simulater->local_gbcode_.c_str());
		strcpy_s(catlog[0].czDeviceName, STR_NAME_LEN, "�����˹���G4ִ����¼��");
		strcpy_s(catlog[0].czModel, STR_MODEL_LEN, "DSJ-G4");
		strcpy_s(catlog[0].czManufacturer, STR_MANUFACTURER_LEN, "�����˹����Ƽ����޹�˾");
		strcpy_s(catlog[0].czOwner, STR_OWNER_LEN, "440000");
		strcpy_s(catlog[0].czCivilCode, STR_CIVILCODE_LEN, "440100");
		strcpy_s(catlog[0].czBlock, STR_BLOCK_LEN, "���Ͻֵ��߸ھ���");
		//strcpy_s(catlog[0].czAddress, STR_ADDRESS_LEN, "�����л������������2819��");
		strcpy_s(catlog[0].czAddress, STR_ADDRESS_LEN, "GuangZhou HuangpPu KaiChuang Road No.2819");
		catlog[0].iParental = 0;
		strcpy_s(catlog[0].czParentID, STR_PARENTID_LEN, simulater->local_gbcode_.c_str());
		catlog[0].iSafetyWay = 0;
		catlog[0].iRegisterWay = 1;
		strcpy_s(catlog[0].czCertNum, STR_CERTNUM_LEN, "CertNum 0");
		catlog[0].iCertifiable = 0;
		catlog[0].iErrCode = 400;
		strcpy_s(catlog[0].czEndTime, STR_DATETIME_LEN, "2010-11-11T19:46:17");
		catlog[0].iSecrecy = 0;
		strcpy_s(catlog[0].czIPAddress, STR_IPADDRESS_LEN, simulater->local_ip_.c_str());
		catlog[0].iPort = atoi(simulater->local_port_.c_str());
		strcpy_s(catlog[0].czPassword, STR_PASSWORD_LEN, "Password 0");
		catlog[0].iStatus = 1;
		catlog[0].dLongitude = 0;
		catlog[0].dLatitude = 0;
		catlog[0].iPtzType = 2;
		catlog[0].iRoomType = 3;

		stuCata.ptrCatalog = catlog;		

		GS28181_ERR err = GB28181Agent_RespondDevCatalog(hSession, &stuCata, 0, false);

		delete [] catlog;
		catlog = NULL;

		if(GS28181_ERR_SUCCESS != err)
		{
			// ��ѯĿ¼��Ӧʧ��
			printf("[%s]��Ӧ�豸Ŀ¼��ѯʧ�ܣ������룺%d\n", simulater->local_gbcode_.c_str(), err);
			return SIP_RESPONSE_CODE_SUCCESS;
		}

		// ��ѯĿ¼��Ӧ�ɹ�
		// ��ʼ���������豸״̬��Ϣ�Ͷ�λ��Ϣ
		// ʹ��Poco�Ķ��߳̿������
		if (!simulater->gb28181_heartbeat_thread_.isRunning())
		{
			simulater->gb28181_heartbeat_thread_.start(simulater->GB28181HeartbeatThreadFun, simulater);
			Sleep(10);
		}
	}
	else if (stuQuery->eType == EnumQueryType::eQUE_DEV_RECORDINDEX)
	{
		// �豸¼���ѯ
	}
	else if (stuQuery->eType == EnumQueryType::eQUE_DEV_CONFIGDOWNLOAD)
	{
		// �豸���ò�ѯ
	}
	else if (stuQuery->eType == EnumQueryType::eQUE_DEV_PRESET)
	{
		// �豸Ԥ��λ��ѯ
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_DevControlCB(const char * czSrvGBCode, const StruDevCtrlCmd * stuCmd, void * pUserData)
{
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_PlayControlCB(STREAM_HANDLE hStream, const StruPlayCtrlCmd * stuCmd, void * pUserData)
{
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_StreamRequestCB(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest, StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
{
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	// �ȷ�����������Ϣ
	if (eRequest == eSTREAM_REALPLAY)
	{
		StruMediaInfo out_media_info;
		ZeroMemory(&out_media_info, sizeof(StruMediaInfo));

		// ����Ŀ���豸ID
		strncpy(out_media_info.czDeviceID, pInMedia->czDeviceID, STR_GBCODE_LEN);
		// ��������RTP�������IP��ַ
		strncpy(out_media_info.czIP, pInMedia->czIP, STR_IPADDRESS_LEN);

		// �����ֵʱ��Э������������������Play/Playback/download/Talk��
		strncpy(out_media_info.czMediaName, pInMedia->czMediaName, STR_NAME_LEN);

		// �Ƿ�֧��RTCP
		out_media_info.bEnableRTCP = pInMedia->bEnableRTCP;

		// ���䷽ʽ������Ҫת���ɱ��صģ�����������Ҫ��Rtp���ͷ�ת
		if (pInMedia->eRtpType == eRTP_OVER_UDP)
		{
			// RTP over UDP����
			out_media_info.eRtpType = eRTP_OVER_UDP;
		}
		else if (pInMedia->eRtpType == eRTP_OVER_TCP_ACTIVE)
		{
			// Զ����RTP over TCP��������
			// ���������Ӧ���Ǳ�������
			out_media_info.eRtpType = eRTP_OVER_TCP_PASSIVE;
		}
		else if (pInMedia->eRtpType = eRTP_OVER_TCP_PASSIVE)
		{
			// RTP over TCP��������
			out_media_info.eRtpType = eRTP_OVER_TCP_ACTIVE;
		}

		//��������  1:������  2:������
		out_media_info.iStreamType = pInMedia->iStreamType;

		// ��ʼUNIXʱ���
		out_media_info.iStart = pInMedia->iStart;
		// ����UNIXʱ���
		out_media_info.iEnd = pInMedia->iEnd;

		//�����������ʱ������ɲ���
		//Ϊ��֤SSRC�������ԣ�����ͳһ�о���Ҫôÿ�β��Ҫôÿ�ζ���
		out_media_info.iSSRC = pInMedia->iSSRC;

		out_media_info.stuDescri.iDescriNum = pInMedia->stuDescri.iDescriNum;
		for (int index = 0; index < out_media_info.stuDescri.iDescriNum; ++index)
		{
			out_media_info.stuDescri.mapDescri[index].eMediaType = pInMedia->stuDescri.mapDescri[index].eMediaType;
			out_media_info.stuDescri.mapDescri[index].iRtpmapNum = pInMedia->stuDescri.mapDescri[index].iRtpmapNum;
			for (int index2 = 0; index2 < out_media_info.stuDescri.mapDescri[index].iRtpmapNum; ++index2)
			{
				strncpy(out_media_info.stuDescri.mapDescri[index].mapRtp[index2].czMimeType, pInMedia->stuDescri.mapDescri[index].mapRtp[index2].czMimeType, STR_RTPTYPENAME_LEN);
				out_media_info.stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat = pInMedia->stuDescri.mapDescri[index].mapRtp[index2].iMediaFormat;
				out_media_info.stuDescri.mapDescri[index].mapRtp[index2].iSampleRate = pInMedia->stuDescri.mapDescri[index].mapRtp[index2].iSampleRate;
			}
		}

		// ע��RTPʵʱ��
		unsigned short local_port = 0;
		int errCode = simulater->stream_mgr_.AddRealStream(hStream, pInMedia->iSSRC, local_port);
		out_media_info.iPort = local_port;

		// ���ϼ�ƽ̨�������
		GS28181_ERR err = GB28181Agent_RespondPlayRequest(hStream, eSTREAM_REALPLAY, &out_media_info, NULL);
		if (err != GS28181_ERR_SUCCESS)
		{
			// ���ص�����Ϣʧ�ܣ�
			return SIP_RESPONSE_CODE_FAIL;
		}

		// ������
		simulater->stream_mgr_.StartRealStream();
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_NotifyInfo_CallBackFunc(EnumNotifyType eType, const char * czSrvGBCode, void * pMsg, void * pUserData)
{
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	return SIP_RESPONSE_CODE_SUCCESS;
}

SIP_REPSOND_CODE GxxGmDSJSimulater::_ExtendRqeustCallBack(SESSION_HANDLE hSession, EnumExtendType eType, const char * czTargetDevID, void * pMsg, void * pUserData)
{
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)pUserData;

	// ����������͸�������İ�

	return SIP_RESPONSE_CODE_SUCCESS;
}

void GxxGmDSJSimulater::GB28181HeartbeatThreadFun(void *param)
{
	// 
	GxxGmDSJSimulater *simulater = (GxxGmDSJSimulater *)param;

	StruConnectParam connention_param;
	strcpy_s(connention_param.szIP, STR_IPADDRESS_LEN, simulater->server_ip_.c_str());
	strcpy_s(connention_param.szGBCode, STR_GBCODE_LEN, simulater->server_gbcode_.c_str());
	connention_param.iPort = atoi(simulater->server_port_.c_str());

	int heartbeat_count = 0;
	int baseinfo_count = 0;
	while (!simulater->is_gb28181_heartbeat_thread_need_exit_)
	{
		Sleep(1000);
		++heartbeat_count;
		++baseinfo_count;

		if (heartbeat_count == 30)
		{
			// ���ͱ�������
			StruErrorList *error_list = NULL;
			GS28181_ERR err = GB28181Agent_HeartBeat(simulater->agent_, &connention_param, 1, NULL);
			if (err != GS28181_ERR_SUCCESS)
			{
				printf("[%s]����28181��������ʧ�ܣ�\n", simulater->local_gbcode_.c_str());
			}

			heartbeat_count = 0;
		}

		if (baseinfo_count == 5)
		{
			// �����豸������Ϣ
			simulater->SendBaseInfo();

			// ���Ͷ�λ��Ϣ
			simulater->SendLocationInfo();

			baseinfo_count = 0;
		}
	}
}