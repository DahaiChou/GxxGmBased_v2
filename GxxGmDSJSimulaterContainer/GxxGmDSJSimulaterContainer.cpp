#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "..\GxxGmDSJSimulater\GxxGmDSJSimulater.h"

int main(int argc, const char *argv[])
{
	printf("高新兴国迈 执法仪模拟器(GB28181-2016) V2.0\n");
	printf("功能说明：\n");
	printf("1. 支持GB28181-2011 和 GB28181-2016两个版本的协议通信；\n");
	printf("2. 支持向接入平台发送GB28181保活心跳，默认30秒发一次，配置文件可配频率；\n");
	printf("3. 支持向接入平台发送扩展的设备基础信息，默认5秒发一次，配置文件可配频率；\n");
	printf("4. 支持向接入平台发送扩展的设备定位信息，默认5秒发一次，配置文件可配频率；\n");
	printf("5. 支持实时视频点流，流数据来源默认为video.gmf，配置文件可配其他格式视频文件，视频文件编码必须为H.264；\n");
	system("pause");

	// 首先根据配置文件加载数据
	char current_program_path[4096] = {0};
	GetModuleFileNameA(NULL, current_program_path, 4096);
	std::string tmp = current_program_path;
	int pos = tmp.find_last_of('\\');

	std::string config_path = tmp.substr(0, pos + 1);
	config_path.append("GxxGmDSJSimulater.ini");

	// 读取配置参数
	char server_ip[4096] = {0};
	char server_port[4096] = {0};
	char server_gbcode[4096] = {0};
	char server_username[4096] = {0};
	char server_password[4096] = {0};

	DWORD bRet = 0;
	int errCode = GetLastError();
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_IP", "127.0.0.1", server_ip, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_PORT", "5060", server_port, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_GBCODE", "00000000000000000000", server_gbcode, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_USERNAME", "1", server_username, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_PASSWORD", "1", server_password, 4096, config_path.c_str());

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

	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "SIP_NET", "UDP", sip_net, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "RTP_NET", "UDP", rtp_net, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("GxxGmDSJSimulater", "SIM_STREAM_FILE", "video.gmf", stream_file, 4096, config_path.c_str());
	int gb28181_hb_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "GB28181_HB_TIME", 30, config_path.c_str());
	int dev_baseinfo_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEV_BASE_INFO_TIME", 5, config_path.c_str());
	int dev_location_time = GetPrivateProfileIntA("GxxGmDSJSimulater", "DEV_LOCATION_TIME", 5, config_path.c_str());

	std::vector<GxxGmDSJSimulater *> simulaters;

	// 根据设备数量启动所有设备
	for (int index = 0; index < simulater_count; ++index)
	{
		// 在这里计算模拟器端口号、国标码等信息
		int current_client_port = local_port_start + index;
		char current_client_port_string[64] = {0};
		sprintf_s(current_client_port_string, 64, "%d", current_client_port);

		char current_client_device_index[8] = {0};
		sprintf_s(current_client_device_index, "%07d", local_gbcode_id_start + index);

		char current_client_gbcode[21] = {0};
		sprintf_s(current_client_gbcode, 21, "%s%s", local_gbcode_pre, current_client_device_index);

		// 计算RTP相关的端口信息
		int current_client_rtp_port_begin = rtp_port_begin * (1 + index) + rtp_port_count * index + 1;
		int current_client_rtp_port_end = current_client_rtp_port_begin + rtp_port_count - 1;

		GxxGmDSJSimulater *simulater = new GxxGmDSJSimulater();

		// 设置设备基本信息
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

		// 设置设备定位信息
		DEVICE_LOCATION_INFO location_info;
		location_info.division_ns_ = LOCATION_INFO_DivisionN;
		location_info.division_ew_ = LOCATION_INFO_DivisionE;
		location_info.radius_ = "3";
		location_info.longitude_ = "113.463612";
		location_info.latitude_ = "23.18003";
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

		//int errCode = simulater->Initialize(local_ip, current_client_port_string, current_client_gbcode, server_ip, server_port, server_gbcode, server_username, server_password, manual_port, current_client_rtp_port_begin, current_client_rtp_port_end);
		int errCode = simulater->Initialize(init_info);
		if (errCode != 0)
		{
			printf("初始化%d模拟器%s失败，错误码：%d\n", index, current_client_gbcode, errCode);
			delete simulater;
			continue;
		}

		simulaters.push_back(simulater);
	}

	printf("模拟器已经运行，按任意键关闭模拟器....");
	getchar();

	// 这里批量注销
	std::vector<GxxGmDSJSimulater *>::iterator iter;
	for (iter = simulaters.begin(); iter != simulaters.end(); ++iter)
	{
		(*iter)->Destroy();
		delete *iter;
	}

	simulaters.clear();
	return 0;
}