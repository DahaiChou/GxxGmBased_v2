// GxxGmSSHDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include "..\GxxGmSSH\GxxGmSSH.h"

#pragma comment(lib, "GxxGmSSH.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	GxxGmSSH ssh;

	int errCode = ssh.Initialize();
	if (errCode != 0)
	{
		std::cout<<"��ʼ��ʧ�ܣ������룺"<<errCode<<std::endl;

		system("pause");
		return errCode;
	}

	errCode = ssh.Login("192.168.231.139", "root", "871511");
	if (errCode != 0)
	{
		std::cout<<"��¼ʧ�ܣ������룺"<<errCode<<std::endl;

		system("pause");
		return errCode;
	}

	while (true)
	{
		std::string command;
		std::cin.clear();
		std::cin>>command;

		std::string result;
		errCode = ssh.Exec(command.c_str(), result);
		if (errCode != 0)
		{
			std::cout<<"����ִ��ʧ�ܣ������룺"<<errCode<<std::endl;
			break;
		}

		std::cout<<result.c_str()<<std::endl;
	}

	system("pause");
	return 0;
}

