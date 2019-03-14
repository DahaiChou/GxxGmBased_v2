#ifndef _GxxGmWSSimulator_H_
#define _GxxGmWSSimulator_H_

#ifdef GXXGMWSSIMULATOR_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <string>

namespace Poco {
	class Thread;
	class Logger;

	namespace Net {
		class HTTPClientSession;
	};
};

class DLL_API GxxGmWSSimulator
{
public:
	GxxGmWSSimulator();
	~GxxGmWSSimulator();

public:
	// ��ʼ���ɼ�����վ
	// ������
	//	*ws_id		�ɼ�վID
	//	*authkey	��ȨKEY
	//	*domain		����
	int Initialize(const char *ws_id, const char *ws_ip,
		int heartbeat_rate, int fileupload_rate, int query_orgs_rate, int query_users_rate, const char *video_path,
		const char *audio_path, const char *image_path, const char *gateway_ip,
		int gateway_port, const char *authkey, const char *domain);

	void Close();

public:
	void SetLogger(Poco::Logger *logger);

public:
	void SetFileUploadInfo(std::string file_domain, std::string dsj_id, int file_size,
		std::string file_type, int file_duration, std::string tag_info, std::string org_code, std::string org_name,
		std::string police_code, std::string police_name, std::string storage_path, std::string local_path, std::string play_path,
		std::string storage_server, std::string thumb);

public:
	// ��������
	int SendHeartBeat();

	// �ϱ��ļ���Ϣ
	int SendFileInfo();

	// �ϱ�ִ���Ƕ�λ��Ϣ
	int SendLocationInfo();

public:
	int GetOrgInfo();
	int GetUserInfo();

private:
	int GetDiskTotalSpace();
	int GetDiskUsedSpace();
	int GetCpuUsage();
	int GetMemoryUsage();

public:
	// �������
	std::string workstaion_id_;			// �ɼ�վID�������ļ���ȡ��
	std::string workstaion_ip_;			// �ɼ�վIP�������ļ���ȡ��
	std::string workstaion_org_code_;	// �ɼ�վ�������ű�ţ��������غ��Զ���䣩
	std::string workstaion_org_name_;	// �ɼ�վ�����������ƣ��������غ��Զ���䣩
	
	int hearbeat_rate_;					// ��������Ƶ�ʣ������ļ���ȡ��
	int fileupload_rate_;				// �ļ��ϱ���ϢƵ�ʣ������ļ���ȡ��
	int locationupload_rate_;			// ��λ��Ϣ�ϱ�Ƶ��
	int query_orgs_rate_;				// ��֯�ܹ���ѯƵ��
	int query_users_rate_;				// �û���Ϣ��ѯƵ��

	std::string video_path_;			// ��Ƶ�ļ�·���������ļ���ȡ��
	std::string audio_path_;			// ��Ƶ�ļ�·���������ļ���ȡ��
	std::string image_path_;			// ͼƬ·���������ļ���ȡ��

	std::string gateway_ip_;		// �ɼ�վ��������IP
	int gateway_port_;				// �ɼ�վ�������ض˿ں�
	std::string authkey_;			// ��ȨKEY��������
	std::string domain_;			// ��������

	// �ϱ��ļ���Ϣ���
	std::string file_domain_;		// �ļ�������
	std::string dsj_id_;			// ִ���ǹ���ID
	int file_size_;					// �ļ���С
	std::string file_type_;			// �ļ�����
	int file_duration_;				// �ļ�ʱ��
	std::string tag_info_;			// ��ע��Ϣ
	std::string org_code_;			// ��λ��Ż��ű��
	std::string org_name_;			// ��λ����
	std::string police_code_;		// ��Ա���
	std::string police_name_;		// ��Ա����
	std::string storage_path_;		// �洢λ��
	std::string local_path_;		// ����λ�ã��ɼ�վ��ԭ�ļ������洢·��
	std::string play_path_;			// ����λ�ã�HTTP����·��
	std::string storage_server_;	// �洢������
	std::string thumb_;				// �ɼ�վ����ͼ

	// ִ���Ƕ�λ��Ϣ
	std::string latitude_;
	std::string longtitude_;

public:
	Poco::Logger *logger_;
	Poco::Net::HTTPClientSession *session_;

	Poco::Thread *working_thread_;
	bool is_need_stop_;
	static void WorkingThreadFunc(void* param);
};

#endif//_GxxGmWSSimulator_H_
