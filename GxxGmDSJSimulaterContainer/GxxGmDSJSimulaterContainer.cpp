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
	bRet = GetPrivateProfileStringA("[DeviceGateway]", "SERVER_IP", "127.0.0.1", server_ip, 4096, config_path.c_str());
	int errCode = GetLastError();
	bRet = GetPrivateProfileStringA("[DeviceGateway]", "SERVER_PORT", "5060", server_port, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("[DeviceGateway]", "SERVER_GBCODE", "00000000000000000000", server_gbcode, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("[DeviceGateway]", "SERVER_USERNAME", "1", server_username, 4096, config_path.c_str());
	bRet = GetPrivateProfileStringA("[DeviceGateway]", "SERVER_PASSWORD", "1", server_password, 4096, config_path.c_str());

	int simulater_count = 1;
	char local_ip[4096] = {0};
	int local_port_start = 0;
	char local_gbcode_pre[4096] = {0};
	int local_gbcode_id_start = 0;

	simulater_count = GetPrivateProfileIntA("[GxxGmDSJSimulater]", "[INSTANCE_COUNT]", 1, config_path.c_str());
	bRet = GetPrivateProfileStringA("[GxxGmDSJSimulater]", "[DEVICE_IP]", "127.0.0.1", local_ip, 4096, config_path.c_str());
	local_port_start = GetPrivateProfileIntA("[GxxGmDSJSimulater]", "[DEVICE_PORT_START]", 9000, config_path.c_str());
	bRet = GetPrivateProfileStringA("[GxxGmDSJSimulater]", "[GBCODE_PRE]", "00000000000000", local_gbcode_pre, 4096, config_path.c_str());
	local_gbcode_id_start = GetPrivateProfileIntA("[GxxGmDSJSimulater]", "[GBCODE_ID_START]", 0, config_path.c_str());

	// �����豸�������������豸
	for (int index = 0; index < simulater_count; ++index)
	{
		// ���������ģ�����˿ںš����������Ϣ
		//GxxGmDSJSimulater *simulater = new GxxGmDSJSimulater();
		//simulater->Initialize(local_ip)
	}


	return 0;
}