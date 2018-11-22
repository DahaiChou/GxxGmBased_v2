#include <iostream>
#include <tchar.h>
#include <string>
#include <sstream>
#include <vector>

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"
//#include "GSRtspClient.h"

int _tmain(int argc, const TCHAR *argv[])
{
	if (argc < 3)
	{
		return 0;
	}

	char str_govideo_ip[4096] = {0};
	char str_govideo_port[4096] = {0};

	strcpy_s(str_govideo_ip, 4096, argv[1]);
	strcpy_s(str_govideo_port, 4096, argv[2]);

	unsigned short govideo_port = atoi(str_govideo_port);

	// ���ȣ���¼��GoVideo
	GxxGmGoVideo govideo;
	govideo.Initialize();
	//system("pause");
	//govideo.Destroy();
	Sleep(10);

	int errCode = govideo.Login(str_govideo_ip, govideo_port, "cyfid111", "cyfid111", 107, "ad22min", "admin2222");
	if (errCode != 0)
	{
		printf("��¼��GoVideoʧ�ܣ������룺%d\n", errCode);
		return errCode;
	}

	// ��ȡ�����豸��Ϣ
	errCode = govideo.GetAllDevices();
	if (errCode != 0)
	{
		printf("��ȡ�����豸��Ϣʧ�ܣ������룺%d\n", errCode);
		return errCode;
	}

	// ��ȡ�����豸״̬��Ϣ
	errCode = govideo.GetAllDeviceStatus();
	if (errCode != 0)
	{
		printf("��ȡ�����豸״̬��Ϣʧ�ܣ������룺%d\n", errCode);
		return errCode;
	}

	// ������Ϣ
	errCode = govideo.SubscriptionMsg(0, "10.10.2.102", 9900);
	if (errCode != 0)
	{
		printf("����GoVideo��Ϣʧ�ܣ������룺%d\n", errCode);
		return errCode;
	}

	// ��ӡ�����豸����״̬��Ϣ
	std::vector<GOVIDEO_DEVICE_STATUS>::iterator iter;
	for (iter = govideo.devices_status_.begin(); iter != govideo.devices_status_.end(); ++iter)
	{
		GOVIDEO_DEVICE_STATUS *dev_status = &(*iter);
		printf("�豸������룺%s �ڲ��豸ID��%d �豸����״̬��%d\n", dev_status->gb28181_code_.c_str(), dev_status->device_id_, dev_status->status_);
	}
	
	// Ȼ���Է��͵�������
	std::string url;
	errCode = govideo.GetRealStreamByGBCode("44010401901511000001", url);
	if (errCode != 0)
		printf("����ʧ�ܣ������룺%d\n", errCode);
	else
		printf("�����ɹ���URLΪ��%s\n", url.c_str());

	// �����Ƿ��һ��RTSP�ͻ���������

	system("pause");
	return 0;
}