// GxxGmWSSimulatorContainer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include "Poco/DateTimeFormatter.h"
#include "Poco/FileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Timestamp.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

#include "../GxxGmWSSimulator/GxxGmWSSimulator.h"

#pragma comment(lib, "GxxGmWSSimulator.lib")

/**
 * �����Լ���Ӧ����
 */
class GxxGmApplication : public Poco::Util::Application
{
public:
	GxxGmApplication() {}
	~GxxGmApplication() {}

public:
	void initialize(Poco::Util::Application& self)
	{
		Poco::Util::Application::initialize(self);

		// ����ִ����س�ʼ�����������������ָ���������ļ���
		//loadConfiguration();

		// ��ʼ����־
		std::string name = "log_";
		name.append(Poco::DateTimeFormatter::format(Poco::Timestamp(), "%Y%m%d%H%M%S"));
		name.append(".log");

		Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
		fileChannel->setProperty("path", name);
		fileChannel->setProperty("rotation", "1 M");
		fileChannel->setProperty("archive", "timestamp");

		// ÿ����־��ʱ���ʽ
		Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter());
		patternFormatter->setProperty("pattern","%Y %m %d %H:%M:%S %s(%l): %t");

		//��ʼ����Channel
		Poco::AutoPtr<Poco::Channel> channel = new Poco::FormattingChannel(patternFormatter,fileChannel);
		logger().setChannel(channel);
	}

	void uninitialize()
	{
		// ����ִ����ط���ʼ��
		Poco::Util::Application::uninitialize();
	}

	int main(const std::vector<std::string>& args)
	{
		int errCode = 0;
		std::string errStr;

		try
		{
			// ��ȡ�ɼ�վ�����ļ�
			// ���Ȼ�õ�ǰ����Ŀ¼
			std::string current_working_dir = Poco::Path::current();
			Poco::Path config_path(current_working_dir);
			config_path.append("GxxGmWSSimulaterConf.ini");
			this->loadConfiguration(config_path.toString(Poco::Path::PATH_NATIVE));

			// ȡ����Ҫģ����豸����
			int simulater_count = config().getInt("WorkStationSimulater.INSTANCE_COUNT");

			// ����IDǰ׺
			std::string gbcode_pre = config().getString("WorkStationSimulater.GBCODE_PRE");
			int gbcode_start = config().getInt("WorkStationSimulater.GBCODE_ID_START");

			// �豸IP
			std::string simulater_ip = config().getString("WorkStationSimulater.DEVICE_IP");

			// �����Զ�������Ƶ��
			int heartbeat_rate = config().getInt("WorkStationSimulater.HEARTBEAT_RATE");
			int fileupload_rate = config().getInt("WorkStationSimulater.FILE_UPLOAD_RATE");

			// ����ý���ļ�·��
			std::string video_name = config().getString("WorkStationSimulater.VIDEO_FILE");
			std::string audio_name = config().getString("WorkStationSimulater.AUDIO_FILE");
			std::string image_name = config().getString("WorkStationSimulater.IMAGE_FILE");

			// ��ȡ������������
			std::string gateway_ip = config().getString("DeviceGateway.SERVER_IP");
			int gateway_port = config().getInt("DeviceGateway.SERVER_PORT");
			std::string gateway_domain = config().getString("DeviceGateway.SERVER_DOMAIN");
			std::string gateway_authkey = config().getString("DeviceGateway.SERVER_AUTHKEY");

			// �ļ��ϴ���ز���
			std::string file_domain = config().getString("FILE_UPOLAD.DOMAIN");		// �ļ�������
			std::string dsj_id = config().getString("DSJ_INFO.DSJ_GBCODE");			// ִ���ǹ���ID
			//std::string police_name = config().getString("FILE_UPOLAD.POLICE_NAME");		// ������
			int file_size = config().getInt("FILE_UPOLAD.FILE_SIZE");					// �ļ���С
			std::string file_type = config().getString("FILE_UPOLAD.FILE_TYPE");			// �ļ�����
			int file_duration = config().getInt("FILE_UPOLAD.FILE_DURATION");				// �ļ�ʱ��
			std::string tag_info = config().getString("FILE_UPOLAD.TAG_INFO");			// ��ע��Ϣ
			std::string org_code = config().getString("FILE_UPOLAD.ORG_CODE");			// ��λ��Ż��ű��
			std::string org_name = config().getString("FILE_UPOLAD.ORG_NAME");			// ��λ����
			std::string police_code = config().getString("FILE_UPOLAD.POLICE_CODE");		// ��Ա���
			std::string police_name = config().getString("FILE_UPOLAD.POLICE_NAME");		// ��Ա����
			std::string storage_path = config().getString("FILE_UPOLAD.STORAGE_PATH");		// �洢λ��
			std::string local_path = config().getString("FILE_UPOLAD.LOCAL_PATH");		// ����λ�ã��ɼ�վ��ԭ�ļ������洢·��
			std::string play_path = config().getString("FILE_UPOLAD.PLAY_PATH");			// ����λ�ã�HTTP����·��
			std::string storage_server = config().getString("FILE_UPOLAD.STORAGE_SERVER");	// �洢������
			std::string thumb = config().getString("FILE_UPOLAD.THUMB");

			// Ȼ��ѭ����ʼ��ָ�������Ĺ���վģ����
			Poco::Logger &logger = this->logger();
			std::vector<GxxGmWSSimulator *> simulators;
			for (int index = 0; index < simulater_count; ++index)
			{
				// ��ǰģ������ID
				char current_device_id[4096] = {0};
				sprintf_s(current_device_id, 4096, "%s%07d", gbcode_pre.c_str(), gbcode_start + index);

				// ��ǰģ�����ļ�ȫ·��
				Poco::Path video_path(current_working_dir);
				video_path.append(video_name);
				std::string video_path_str = video_path.toString(Poco::Path::PATH_NATIVE);

				Poco::Path audio_path(current_working_dir);
				audio_path.append(video_name);
				std::string audio_path_str = audio_path.toString(Poco::Path::PATH_NATIVE);

				Poco::Path image_path(current_working_dir);
				image_path.append(image_name);
				std::string image_path_str = image_path.toString(Poco::Path::PATH_NATIVE);

				GxxGmWSSimulator *simulater = new GxxGmWSSimulator;
				simulater->SetLogger(&logger);

				errCode = simulater->Initialize(current_device_id, simulater_ip.c_str(), heartbeat_rate,
					fileupload_rate, video_path_str.c_str(), audio_path_str.c_str(), image_path_str.c_str(),
					gateway_ip.c_str(),	gateway_port, gateway_authkey.c_str(), gateway_domain.c_str());

				if (errCode != 0)
				{
					// ��ģ������ʼ��ʧ����
				}

				simulater->SetFileUploadInfo(file_domain, dsj_id, file_size, file_type, file_duration, 
					tag_info, org_code, org_name, police_code, police_name, storage_path, local_path,
					play_path, storage_server, thumb);
			}


			// �ȴ��ն��˳�����
			std::cout<<"Press \"Enter\" key to exit ...";
			getchar();

			// 
			std::vector<GxxGmWSSimulator *>::iterator iter;
			for (iter = simulators.begin(); iter != simulators.end();)
			{
				GxxGmWSSimulator *simulater = *iter;
				simulater->Close();

				delete simulater;
				iter = simulators.erase(iter);
			}
		}
		catch (Poco::Exception e)
		{
			errCode = e.code();
			errStr = e.displayText();
		}

		return Poco::Util::Application::EXIT_OK;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	printf("�����˹��� �ɼ�����վģ���� V1.1\n");
	printf("\n");
	printf("����˵����\n");
	printf("1. ֧�ֽ���3.0ƽ̨��\n");
	printf("2. ֧�ֲɼ�վ���ߣ�����������CPU״̬���ڴ�״̬����Ϣ��\n");
	printf("3. ֧�ֲɼ�վ�ϱ��ļ���Ϣ��Ĭ��1�뷢һ�Σ������ļ�����Ƶ�ʣ�\n");
	//printf("4. ֧�������ƽ̨������չ���豸��λ��Ϣ��Ĭ��5�뷢һ�Σ������ļ�����Ƶ�ʣ�\n");
	//printf("5. ֧��ʵʱ��Ƶ��������������ԴĬ��Ϊvideo.gmf�������ļ�����������ʽ��Ƶ�ļ�����Ƶ�ļ��������ΪH.264��\n");
	//printf("6. ������֡����Ƶ�Ĳ��ţ�Ĭ�ϰ�30pfs���в��ţ�\n");
	//printf("7. ����ƵԴΪG711ʱ����������Ƶ���ݣ�\n");
	//printf("8. ģ���������ö�λ���ꣻ\n");
	printf("\n");
	system("pause");

	GxxGmApplication app;
	app.run();

	return 0;
}

