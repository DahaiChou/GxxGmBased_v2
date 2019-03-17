#ifndef _GxxGmDocker_H_
#define _GxxGmDocker_H_

#include "GxxGmSSH.h"

class GxxGmDocker
{
public:
	GxxGmDocker();
	~GxxGmDocker();

public:
	/**
	 * ��¼��������
	 */
	int LoginToServer(const char *ipaddr, const char *username, const char *password);

	/**
	 * ��ָ���������ǳ�
	 */
	void LogoutFromServer(const char *ipaddr);

public:
	/**
	 * ɨ�����о����б�
	 */
	int ListAllImage();

	/**
	 * �鿴�����������е�����
	 */
	int ListAllContainers();

	/**
	 * �鿴��������ռ�õ���Դ
	 */
	int ListAllContainerResources();

private:
	std::map<std::string, GxxGmSSH *> servers_;
};

#endif//_GxxGmDocker_H_
