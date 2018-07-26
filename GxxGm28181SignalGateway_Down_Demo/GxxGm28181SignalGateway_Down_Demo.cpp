// GxxGm28181SignalGateway_Down_Demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>

#include "GB28181DownImp.h"

//////////////////////////////////////////////////////////////////////////
// �¼�ƽ̨����
#define LOCAL_IP		"192.168.199.102"
#define LOCAL_PORT		5090
#define LOCAL_GBCODE	"34020000001320000001"

//////////////////////////////////////////////////////////////////////////
// �ϼ�ƽ̨����
#define REMOTE_IP		"192.168.199.102"
#define REMOTE_PORT		5060
#define REMOTE_GBCODE	"34020000002000000001"
#define USERNAME		"admin"
#define PASSWORD		"12345678"


int _tmain(int argc, _TCHAR* argv[])
{
	GB28181DownImp gb28181;
	int errCode = gb28181.Init(LOCAL_IP, LOCAL_PORT, LOCAL_GBCODE, REMOTE_IP, REMOTE_PORT, REMOTE_GBCODE, USERNAME, PASSWORD);
	if (errCode != 0)
	{
		return -1;
	}

	gb28181.Run();

	return 0;
}

