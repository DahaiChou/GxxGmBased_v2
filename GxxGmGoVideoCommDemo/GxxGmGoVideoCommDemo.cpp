#include <iostream>
#include <tchar.h>
#include <string>
#include <sstream>

#include "..\GxxGmGoVideo\GxxGmGoVideo.h"

int _tmain(int argc, const TCHAR *argv[])
{
	const char *ip = "10.10.9.176";
	unsigned short port = 99;

	// ���ȣ���¼��GoVideo
	GxxGmGoVideo govideo;
	int errCode = govideo.Login(ip, port, "cyfid111", "cyfid111", 107, "ad22min", "admin2222");
	
	// Ȼ���Է��͵�������
	system("pause");
	return 0;
}