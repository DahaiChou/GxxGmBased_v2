#ifndef _GxxGmGoVideo_H_
#define _GxxGmGoVideo_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "Poco/Thread.h"

class GxxGmHttpServer;

enum enStreamAgentType
{
	StreamAgentNone = 0,
	StreamAgentRtsp,
	StreamAgentRtmp,
	StreamAgentRtp,
	StreamAgentWebRTC,
	StreamAgentHttpFlv
};

typedef struct _GOVIDEO_DEVICE_INFO_
{
	unsigned int device_id_;
	unsigned int device_index_;
	std::string device_name_;
	unsigned int model_id_;
	unsigned int category_id_;
	std::string device_code_;
	std::string device_connection_info_;
	std::string device_version_;
	std::string device_username_;
	std::string device_password_;
	std::string device_extra_info_;
	std::string gb28181_code_;
	std::string device_name_abbr_;
	unsigned int version_;
	std::string remark_;
	unsigned int dgw_server_id_;

} GOVIDEO_DEVICE_INFO, *PGOVIDEO_DEVICE_INFO;

typedef struct _GOVIDEO_DEVICE_STATUS_
{
	std::string gb28181_code_;
	unsigned int device_id_;
	unsigned int status_;
	std::map<int, unsigned int> chennal_status_;
} GOVIDEO_DEVICE_STATUS, *PGOVIDEO_DEVICE_STATUS;

class GxxGmGoVideo
{
public:
	GxxGmGoVideo();
	~GxxGmGoVideo();

public:
	int Initialize(int http_port = 9900);
	void Destroy();

public:
	int Login(const char *govideo_ip, unsigned short govideo_port, const char *sequence_id = "admin", const char *login_id = "admin", int login_type = 107, const char *username = "admin", const char *password = "admin");
	int Logout();

public:
	//////////////////////////////////////////////////////////////////////////
	//
	// ������ؽӿ�
	//
	//////////////////////////////////////////////////////////////////////////

	// ��ȡ�豸���������б�
	int GetDeviceGatewayList();

public:
	//////////////////////////////////////////////////////////////////////////
	//
	// �豸���
	//
	//////////////////////////////////////////////////////////////////////////

	// ע���豸
	int RegisterDevice();

	// ע���豸
	int UnregisterDevice();

public:
	
	// ��ѯ�����豸(ͨ��)
	int GetAllDevices();

	// ��ѯ�豸״̬����õ��豸����״̬���豸ͨ���б��Լ�ͨ������״̬
	int GetDeviceStatus(const char *device_gb28181_code, GOVIDEO_DEVICE_STATUS &device_status);

	// ��ѯ�����豸״̬
	int GetAllDeviceStatus();

	// ��������GoVideo��Ϣ
	int SubscriptionMsg(int subscript_type, const char *http_server_ip, int http_server_port);

	// ����
	int GetRealStream(unsigned int device_id, std::string &stream_url, enum enStreamAgentType stream_type = StreamAgentNone);
	int GetRealStreamByGBCode(const char *device_gb28181_code, std::string &stream_url);

	// �����Խ�
	int StartTalk(const char *device_id, std::string &stream_url);

public:
	Poco::Thread http_server_thread_;
	static void HttpServerThread(void* param);

public:
	Poco::Thread hb_thread_;
	static void HeartBeatThread(void* param);

public:
	void* http_session_;
	std::string token_;

public:
	std::vector<GOVIDEO_DEVICE_INFO *> devices_;
	std::vector<GOVIDEO_DEVICE_STATUS> devices_status_;

private:
	std::string host_;
	unsigned short port_;

public:
	GxxGmHttpServer *http_server_;

};

#endif//_GxxGmGoVideo_H_
