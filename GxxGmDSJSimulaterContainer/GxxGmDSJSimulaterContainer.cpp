#include <iostream>
#include <string>
#include <Windows.h>
#include "..\GxxGmDSJSimulater\GxxGmDSJSimulater.h"

int main(int argc, const char *argv[])
{
	// ���ȸ��������ļ���������
	char current_program_path[4096] = {0};
	GetModuleFileNameA(NULL, current_program_path, 4096);
	std::string tmp = current_program_path;
	int pos = tmp.find_last_of('\\');

	std::string config_path = tmp.substr(0, pos + 1);
	config_path.append("GxxGmDSJSimulater.ini");

	// ��ȡ���ò���
	char server_ip[4096] = {0};
	char server_port[4096] = {0};
	char server_gbcode[4096] = {0};
	char server_username[4096] = {0};
	char server_password[4096] = {0};

	DWORD bRet = 0;
	int errCode = GetLastError();
	bRet = GetPrivateProfileStringA("DeviceGateway", "SERVER_IP", "127.0.0.1", server_ip, 4096, config_path.c_str());
	errCode = GetLastError();
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

	// �����豸�������������豸
	for (int index = 0; index < simulater_count; ++index)
	{
		// ���������ģ�����˿ںš����������Ϣ
		int current_client_port = local_port_start + index;
		char current_client_port_string[64] = {0};
		sprintf_s(current_client_port_string, 64, "%d", current_client_port);

		char current_client_device_index[8] = {0};
		sprintf_s(current_client_device_index, "%07d", local_gbcode_id_start + index);

		char current_client_gbcode[21] = {0};
		sprintf_s(current_client_gbcode, 21, "%s%s", local_gbcode_pre, current_client_device_index);

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
		location_info.longitude_ = "113.463612";
		location_info.latitude_ = "23.18003";
		location_info.direction_ = "0";
		location_info.speed_ = "0";
		location_info.height_ = "0";
		location_info.satellites_ = "20";
		location_info.location_available_ = "1";
		simulater->SetLocationInfo(location_info);

		int errCode = simulater->Initialize(local_ip, current_client_port_string, current_client_gbcode, server_ip, server_port, server_gbcode, server_username, server_password);
		if (errCode != 0)
		{
			printf("��ʼ��%dģ����%sʧ�ܣ������룺%d\n", index, current_client_gbcode, errCode);
		}
	}

	system("pause");
	return 0;
}