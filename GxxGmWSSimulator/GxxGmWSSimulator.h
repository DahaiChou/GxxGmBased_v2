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
		int heartbeat_rate, int fileupload_rate, const char *video_path,
		const char *audio_path, const char *image_path, const char *gateway_ip,
		int gateway_port, const char *authkey, const char *domain);

	void Close();

public:
	void SetLogger(Poco::Logger *logger);

public:
	// ��������
	int SendHeartBeat();

	// �ϱ��ļ���Ϣ
	int SendFileInfo();

	// �ϱ�ִ���Ƕ�λ��Ϣ
	int SendLocationInfo();

private:
	int GetDiskTotalSpace();
	int GetDiskUsedSpace();
	int GetCpuUsage();
	int GetMemoryUsage();

public:
	// �������
	std::string workstaion_id_;		// �ɼ�վID
	std::string workstaion_ip_;		// �ɼ�վIP
	
	int hearbeat_rate_;				// ��������Ƶ��
	int fileupload_rate_;			// �ļ��ϱ���ϢƵ��
	int locationupload_rate_;		// ��λ��Ϣ�ϱ�Ƶ��

	std::string video_path_;		// ��Ƶ�ļ�·��
	std::string audio_path_;		// ��Ƶ�ļ�·��
	std::string image_path_;		// ͼƬ·��

	std::string gateway_ip_;		// �ɼ�վ��������IP
	int gateway_port_;				// �ɼ�վ�������ض˿ں�
	std::string authkey_;			// ��ȨKEY��������
	std::string domain_;			// ��������

	// �ϱ��ļ���Ϣ���
	std::string file_domain_;		// �ļ�������
	std::string dsj_id_;			// ִ���ǹ���ID

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
