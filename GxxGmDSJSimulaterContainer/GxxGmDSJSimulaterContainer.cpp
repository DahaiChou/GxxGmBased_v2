#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "Poco/DateTimeFormatter.h"
#include "Poco/FileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Timestamp.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

#include "..\GxxGmDSJSimulater\GxxGmDSJSimulater.h"


/**
 * �����Լ���Ӧ����
 */
class GxxGmApplication : public Poco::Util::Application
{
public:
	GxxGmApplication() {}
	~GxxGmApplication() {}

public:
	void initialize(Poco::Util::Application& self)
	{
		Poco::Util::Application::initialize(self);

		// ����ִ����س�ʼ�����������������ָ���������ļ���
		//loadConfiguration();

		// ��ʼ����־
		std::string name = "log_";
		name.append(Poco::DateTimeFormatter::format(Poco::Timestamp(), "%Y%m%d%H%M%S"));
		name.append(".log");

		Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
		fileChannel->setProperty("path", name);
		fileChannel->setProperty("rotation", "1 M");
		fileChannel->setProperty("archive", "timestamp");

		// ÿ����־��ʱ���ʽ
		Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter());
		patternFormatter->setProperty("pattern","%Y %m %d %H:%M:%S %s(%l): %t");

		//��ʼ����Channel
		Poco::AutoPtr<Poco::Channel> channel = new Poco::FormattingChannel(patternFormatter,fileChannel);
		logger().setChannel(channel);
	}

	void uninitialize()
	{
		// ����ִ����ط���ʼ��
		Poco::Util::Application::uninitialize();
	}

	int main(const std::vector<std::string>& args)
	{
		int errCode = 0;
		std::string errStr;

		try
		{
			// ��ȡ�ɼ�վ�����ļ�
			// ���Ȼ�õ�ǰ����Ŀ¼
			std::string current_working_dir = Poco::Path::current();
			Poco::Path config_path(current_working_dir);
			config_path.append("GxxGmDSJSimulater.ini");
			this->loadConfiguration(config_path.toString(Poco::Path::PATH_NATIVE));

			std::string server_ip = config().getString("DeviceGateway.SERVER_IP");
			std::string server_port = config().getString("DeviceGateway.SERVER_PORT");
			std::string server_gbcode = config().getString("DeviceGateway.SERVER_GBCODE");
			std::string server_username = config().getString("DeviceGateway.SERVER_USERNAME");
			std::string server_password = config().getString("DeviceGateway.SERVER_PASSWORD");

			int simulater_count;
			std::string local_ip;
			int local_port_start;
			std::string local_gbcode_pre;
			int local_gbcode_id_start;


			// �ȴ��ն��˳�����
			std::cout<<"Press \"Enter\" key to exit ...";
			getchar();

			// 
			std::vector<GxxGmWSSimulator *>::iterator iter;
			for (iter = simulators.begin(); iter != simulators.end();)
			{
				GxxGmWSSimulator *simulater = *iter;
				simulater->Close();

				delete simulater;
				iter = simulators.erase(iter);
			}
		}
		catch (Poco::Exception e)
		{
			errCode = e.code();
			errStr = e.displayText();
		}

		return Poco::Util::Application::EXIT_OK;
	}
};

int main(int argc, const char *argv[])
{
	printf("�����˹��� ִ����ģ����(GB28181-2016) V2.5\n");
	printf("\n");
	printf("����˵����\n");
	printf("1. ֧��GB28181-2011 �� GB28181-2016�����汾��Э��ͨ�ţ�\n");
	printf("2. ֧�������ƽ̨����GB28181����������Ĭ��30�뷢һ�Σ������ļ�����Ƶ�ʣ�\n");
	printf("3. ֧�������ƽ̨������չ���豸������Ϣ��Ĭ��5�뷢һ�Σ������ļ�����Ƶ�ʣ�\n");
	printf("4. ֧�������ƽ̨������չ���豸��λ��Ϣ��Ĭ��5�뷢һ�Σ������ļ�����Ƶ�ʣ�\n");
	printf("5. ֧��ʵʱ��Ƶ��������������ԴĬ��Ϊvideo.gmf�������ļ�����������ʽ��Ƶ�ļ�����Ƶ�ļ��������ΪH.264��\n");
	printf("6. ������֡����Ƶ�Ĳ��ţ�Ĭ�ϰ�30pfs���в��ţ�\n");
	printf("7. ����ƵԴΪG711ʱ����������Ƶ���ݣ�\n");
	printf("8. ģ���������ö�λ���ꣻ\n");
	printf("9. ģ����28181�����������豸������Ϣ���豸��λ��Ϣ���ͼ��֧�ֵ����뼶��\n");
	printf("\n");
	system("pause");

	GxxGmApplication app;
	app.run();

	//// ���ȸ��������ļ���������
	//char current_program_path[4096] = {0};
	//GetModuleFileNameA(NULL, current_program_path, 4096);
	//std::string tmp = current_program_path;
	//int pos = tmp.find_last_of('\\');

	//std::string config_path = tmp.substr(0, pos + 1);
	//config_path.append("GxxGmDSJSimulater.ini");

	//// ��ȡ���ò���
	//char server_ip[4096] = {0};
	//char server_port[4096] = {0};
	//char server_gbcode[4096] = {0};
	//char server_username[4096] = {0};
	//char server_password[4096] = {0};

	//DWORD bRet = 0;
	//int errCode = GetLastError();
	//bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_IP", "127.0.0.1", server_ip, 4096, config_path.c_str());
	//bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_PORT", "5060", server_port, 4096, config_path.c_str());
	//bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_GBCODE", "00000000000000000000", server_gbcode, 4096, config_path.c_str());
	//bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_USERNAME", "1", server_username, 4096, config_path.c_str());
	//bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_PASSWORD", "1", server_password, 4096, config_path.c_str());

	int simulater_count = 1;
	char local_ip[4096] = {0};
	int local_port_start = 0;
	char local_gbcode_pre[4096] = {0};
	int local_gbcode_id_start = 0;

	simulater_count = GetPrivateProfileIntA("GxxGmDSJSimulater", "INSTANCE_COUNT", 1, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "DEVICE_IP", "127.0.0.1", local_ip, 4096, config_path.c_str());
	local_port_start = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEVICE_PORT_START", 9000, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "GBCODE_PRE", "00000000000000", local_gbcode_pre, 4096, config_path.c_str());
	local_gbcode_id_start = GetPrivateProfileIntA("GxxGmDSJSimulater", "GBCODE_ID_START", 0, config_path.c_str());

	int manual_port = GetPrivateProfileIntA("GxxGmDSJSimulater", "MANUAL_PORT", 0, config_path.c_str());
	int rtp_port_begin = GetPrivateProfileIntA("GxxGmDSJSimulater", "RTP_PORT_START", 10000, config_path.c_str());
	int rtp_port_count = GetPrivateProfileIntA("GxxGmDSJSimulater", "RTP_PORT_COUNT", 10, config_path.c_str());

	char sip_net[4096] = {0};
	char rtp_net[4096] = {0};
	char stream_file[4096] = {0};
	char platform_id[4096] = {0};

	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "SIP_NET", "UDP", sip_net, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "RTP_NET", "UDP", rtp_net, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "SIM_STREAM_FILE", "video.gmf", stream_file, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "PLATFORM_ID", "GM_SHENZHEN", platform_id, 4096, config_path.c_str());
	int gb28181_hb_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "GB28181_HB_TIME", 30, config_path.c_str());
	int dev_baseinfo_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEV_BASE_INFO_TIME", 5, config_path.c_str());
	int dev_location_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEV_LOCATION_TIME", 5, config_path.c_str());
	int dev_imei_index_start = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEVICE_IMEI_START ", 0, config_path.c_str());

	char location_latitude[4096] = {0};
	char location_longtitude[4096] = {0};
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "DEV_LOCATION_LATITUDE", "23.174992", location_latitude, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "DEV_LOCATION_LONGTITUDE", "113.456393", location_longtitude, 4096, config_path.c_str());

	std::vector<GxxGmDSJSimulater *> simulaters;

	// �����豸�������������豸
	for (int index = 0; index < simulater_count; ++index)
	{
		// ����ģ�����˿ں�
		int current_client_port = local_port_start + index;
		char current_client_port_string[64] = {0};
		sprintf_s(current_client_port_string, 64, "%d", current_client_port);

		// ����ģ�����������
		char current_client_device_index[8] = {0};
		sprintf_s(current_client_device_index, "%07d", local_gbcode_id_start + index);

		char current_client_gbcode[21] = {0};
		sprintf_s(current_client_gbcode, 21, "%s%s", local_gbcode_pre, current_client_device_index);

		// ����ģ����IMEI
		int current_client_imei_index = dev_imei_index_start + index;
		char current_client_imei[64] = {0};
		sprintf_s(current_client_imei, 64, "GxxGmSim%04d", current_client_imei_index);

		// ����RTP��صĶ˿���Ϣ
		int current_client_rtp_port_begin = rtp_port_begin * (1 + index) + rtp_port_count * index + 1;
		int current_client_rtp_port_end = current_client_rtp_port_begin + rtp_port_count - 1;

		GxxGmDSJSimulater *simulater = new GxxGmDSJSimulater();

		// �����豸������Ϣ
		DEVICE_BASE_INFO base_info;
		base_info.carrieroperator_ = BASE_INFO_Carrieroperator_CMCC;
		base_info.net_type_ = BASE_INFO_Nettype_4G;
		base_info.signal_ = "100";
		base_info.battery_ = "100";
		base_info.storage_ = "64";
		base_info.cpu_ = "30";
		base_info.version_ = "GxxGm_simulater_1.0.0.1";
		base_info.local_record_ = BASE_INFO_LocalRecord_NO;
		simulater->SetBaseInfo(base_info);

		// �����豸��λ��Ϣ
		DEVICE_LOCATION_INFO location_info;
		location_info.division_ns_ = LOCATION_INFO_DivisionN;
		location_info.division_ew_ = LOCATION_INFO_DivisionE;
		location_info.radius_ = "3";
		location_info.longitude_ = location_longtitude;
		location_info.latitude_ = location_latitude;
		location_info.direction_ = "0";
		location_info.speed_ = "0";
		location_info.height_ = "0";
		location_info.satellites_ = "20";
		location_info.location_available_ = "1";
		simulater->SetLocationInfo(location_info);

		struct SimulaterInitInfo init_info;
		init_info.local_ip_ = local_ip;
		init_info.local_port_ = current_client_port_string;
		init_info.local_gbcode_ = current_client_gbcode;
		init_info.server_ip_ = server_ip;
		init_info.server_port_ = server_port;
		init_info.server_gbcode_ = server_gbcode;
		init_info.username_ = server_username;
		init_info.password_ = server_password;
		init_info.manual_port_ = manual_port;
		init_info.begin_port_ = current_client_rtp_port_begin;
		init_info.end_port_ = current_client_rtp_port_end;
		init_info.sip_net_ = sip_net;
		init_info.rtp_net_ = rtp_net;
		init_info.stream_file_ = stream_file;
		init_info.gb28181_hb_time_ = gb28181_hb_time;
		init_info.dev_baseinfo_time_ = dev_baseinfo_time;
		init_info.dev_location_time_ = dev_location_time;
		init_info.imei_ = current_client_imei;
		init_info.platform_id_ = platform_id;

		int errCode = simulater->Initialize(init_info);
		if (errCode != 0)
		{
			printf("��ʼ��%dģ����%sʧ�ܣ������룺%d\n", index, current_client_gbcode, errCode);
			delete simulater;
			continue;
		}

		simulaters.push_back(simulater);
	}

	printf("ģ�����Ѿ����У���������ر�ģ����....");
	getchar();

	// ��������ע��
	std::vector<GxxGmDSJSimulater *>::iterator iter;
	for (iter = simulaters.begin(); iter != simulaters.end(); ++iter)
	{
		(*iter)->Destroy();
		delete *iter;
	}

	simulaters.clear();
	return 0;
}