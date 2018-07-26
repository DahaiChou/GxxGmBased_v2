#include "stdafx.h"
#include "DBBusiness.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/RecordSet.h"

#define USE_SQLITE3
//#define USE_POCOSQLITE

DBBusiness::DBBusiness()
{
	
}

DBBusiness::~DBBusiness()
{
	
}

std::string DBBusiness::GetCurrentIp()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
#ifdef USE_POCOSQLITE
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select current_ip from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
#endif

#ifdef USE_SQLITE3
	int errCode = sqlite3_open("GB28181Client.db3", &db_);
	if (errCode != SQLITE_OK)
		return "";

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "select current_ip from config");

	char *err_msg = NULL;
	char **db_result = NULL;
	int row_count = 0;
	int column_count = 0;
	int index = 0;

	errCode = sqlite3_get_table(db_, sql, &db_result, &row_count, &column_count, &err_msg);
	if (errCode == SQLITE_OK)
	{
		index = column_count;
		for (int i = 0; i < row_count; ++i)
		{
			for (int j = 0; j < column_count; ++j)
			{
				current.push_back(db_result[index]);
				TRACE(db_result[index]);
			}
		}
	}

	sqlite3_free(err_msg);
	sqlite3_free_table(db_result);
#endif

	return current[0];
}

std::string DBBusiness::GetCurrentPort()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select current_port from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetCurrentGbcode()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select current_gbcode from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetRemoteIp()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select remote_ip from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetRemotePort()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select remote_port from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetRemoteGbcode()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select remote_gbcode from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetAuthenUsername()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	select_ << "select authen_username from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

std::string DBBusiness::GetAuthenPassword()
{
	std::vector<std::string> current;

	// ��ʼ�����ݿ�Ự
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);
	
	select_ << "select authen_password from config", Poco::Data::Keywords::into(current), Poco::Data::Keywords::now;
	select_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return current[0];
}

int DBBusiness::SetCurrentIp(const char *current_ip)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET current_ip = '%s'", current_ip);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetCurrentPort(const char *current_port)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET current_port = '%s'", current_port);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetCurrentGbcode(const char *current_gbcode)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET current_gbcode = '%s'", current_gbcode);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetRemoteIp(const char *remote_ip)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET remote_ip = '%s'", remote_ip);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetRemotePort(const char *remote_port)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET remote_port = '%s'", remote_port);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetRemoteGbcode(const char *remote_gbcode)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET remote_gbcode = '%s'", remote_gbcode);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetAuthenUsername(const char *authen_username)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET authen_username = '%s'", authen_username);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::SetAuthenPassword(const char *authen_password)
{
	int errCode = 0;

	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement update_(*session);

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "UPDATE 'config' SET authen_password = '%s'", authen_password);
	update_ << sql, Poco::Data::Keywords::now;
	update_.reset(*session);

	Poco::Data::SQLite::Connector::unregisterConnector();
	return errCode;
}

int DBBusiness::GetOrgInfos(struct OrgInfo &infos)
{
	/*
	�����㷨��
	1. �жϸ��ڵ㣬�����ѯ���ļ�¼�Ǹ��ڵ㣬��
	*/
	std::string ids;
	std::string names;
	std::string gbcodes;
	std::string father_gbcodes;

	// ��ѯ��������֯�ܹ���Ϣ
	// ��ʼ�����ݿ�Ự
#ifdef USE_POCOSQLITE
	int errCode = 0;
	Poco::Data::SQLite::Connector::registerConnector();
	Poco::Data::Session *session = new Poco::Data::Session("SQLite", "GB28181Client.db3");
	Poco::Data::Statement select_(*session);

	try 
	{
		//select_ << "SELECT name, gbcode, father_gbcode FROM 'orgnization'", 
		select_ << "SELECT * FROM orgnization", Poco::Data::Keywords::now;
		
		Poco::Data::RecordSet rs(select_);
		size_t cols = rs.columnCount();
		
		// �ȵõ�ÿһ���ֶε�ֵ

		// Ȼ������һ��
	}
	catch (Poco::Exception& exception)
	{
		const char *msg = exception.name();
		errCode = exception.code();
	}
	
	
	select_.reset(*session);
	Poco::Data::SQLite::Connector::unregisterConnector();
#endif

#ifdef USE_SQLITE3
	int errCode = sqlite3_open("GB28181Client.db3", &db_);
	if (errCode != SQLITE_OK)
		return errCode;

	char sql[4096] = {0};
	sprintf_s(sql, 4096, "SELECT name, gbcode, father_gbcode FROM orgnization");

	char *err_msg = NULL;
	char **db_result = NULL;
	int row_count = 0;
	int column_count = 0;
	int index = 0;
	sqlite3_stmt *stmp = NULL;

	errCode = sqlite3_prepare_v2(db_, sql, -1, &stmp, NULL);
	if (errCode != SQLITE_OK)
	{
		sqlite3_close(db_);
		return errCode;
	}

	while (sqlite3_step(stmp) == SQLITE_ROW)
	{
		const unsigned char *name = sqlite3_column_text(stmp, 0);
		const unsigned char *gbcode = sqlite3_column_text(stmp, 1);
		const unsigned char *father_gbcode = sqlite3_column_text(stmp, 2);

		// ����ת��
		wchar_t name_w[4096] = {0};
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)name, -1, name_w, 4096);
		char name_a[4096] = {0};
		WideCharToMultiByte(CP_ACP, 0, name_w, -1, name_a, 4096, NULL, NULL);

		// ����ݹ麯�����д���
		OrgInfo info;
		info.name = name_a;
		info.gbcode = (const char *)gbcode;
		info.father_gbcode = (const char *)father_gbcode;

		errCode = BuildOrgTree(&infos, info, 2);
	}
#endif

	// ������֯һ�²㼶��ϵ

	
	return errCode;
}

int DBBusiness::BuildOrgTree(struct OrgInfo *root_info, OrgInfo current_info, int level)
{
	int errCode = 0;

	if (level > 4)
		return -2;

	// ����õ��Ľڵ㵱ǰ����ID�븸����IDһ�£�����Ϊ���ڵ㣬ֱ������
	// �����һ�£����鵱ǰ���ڵ��ǲ������ĸ��ڵ㣬����ǣ�������ӽڵ㣬������ǣ�����������ӽڵ���Ϊ���ڵ���м��
	if (current_info.gbcode.compare(current_info.father_gbcode) == 0)
	{
		root_info->name = current_info.name;
		root_info->gbcode = current_info.gbcode;
		root_info->father_gbcode = current_info.father_gbcode;
		return 0;
	}
	else
	{
		if (root_info->gbcode.compare(current_info.father_gbcode) == 0)
		{
			root_info->sub_orgs.push_back(current_info);
		}
		else
		{
			std::vector<OrgInfo>::iterator iter;
			for (iter = root_info->sub_orgs.begin(); iter != root_info->sub_orgs.end(); ++iter)
			{
				// �����ǵ�һ���ӽڵ㣬�ܹ�ֻ�������ӽڵ�
				// ��һ���ӽڵ㣬�Ƚ�ǰ��λ�������ͬ�������Ƿ��Ǳ����ӽڵ�
				//if (memcpy((void*)iter->gbcode.c_str(), (void*)current_info.father_gbcode.c_str(), level * 2) == 0)
				{
					// ���ȫ��
					if (iter->gbcode.compare(current_info.father_gbcode) == 0)
					{
						// �ҵ����ڵ��ˣ���ӽ�ȥ
						iter->sub_orgs.push_back(current_info);
						break;
					}
					else
					{
						// ���Ǹ��ڵ㣬���������е���һ���ӽڵ㣬����Ƿ��Ǹ��ڵ�
						std::vector<OrgInfo> *sub_org = &iter->sub_orgs;
						std::vector<OrgInfo>::iterator sub_iter;
						for (sub_iter = sub_org->begin(); sub_iter != sub_org->end(); ++sub_iter)
						{
							struct OrgInfo *sub_org_info = &(*sub_iter);
							errCode = BuildOrgTree(sub_org_info, current_info, ++level);
						}

					}
				}
			}
		}
	}

	

	return errCode;
}