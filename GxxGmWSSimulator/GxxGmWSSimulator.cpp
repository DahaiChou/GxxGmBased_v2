#include "GxxGmWSSimulator.h"

#include "Poco/Thread.h"
#include "Poco/Logger.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/PrintHandler.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/TextConverter.h"
#include "Poco/DateTime.h"

GxxGmWSSimulator::GxxGmWSSimulator()
: working_thread_(new Poco::Thread)
, is_need_stop_(false)
, hearbeat_rate_(5000)
, fileupload_rate_(5000)
, locationupload_rate_(5000)
{

}

GxxGmWSSimulator::~GxxGmWSSimulator()
{
	if (working_thread_)
	{
		delete working_thread_;
	}
}

int GxxGmWSSimulator::Initialize(const char *ws_id, const char *ws_ip, int heartbeat_rate, int fileupload_rate,
					const char *video_path, const char *audio_path, const char *image_path,	const char *gateway_ip,
					int gateway_port, const char *authkey, const char *domain)
{
	int errCode = 0;

	// ���ȱ����������
	workstaion_id_ = ws_id;
	workstaion_ip_ = ws_ip;
	hearbeat_rate_ = heartbeat_rate;
	fileupload_rate_ = fileupload_rate;

	video_path_ = video_path;
	audio_path_ = audio_path;
	image_path_ = image_path;

	gateway_ip_ = gateway_ip;
	gateway_port_ = gateway_port;
	authkey_ = authkey;
	domain_ = domain;

	session_ = new Poco::Net::HTTPClientSession(gateway_ip_, gateway_port_);

	// ���������߳�
	if (!working_thread_->isRunning())
	{
		working_thread_->start(GxxGmWSSimulator::WorkingThreadFunc, this);
	}

	return errCode;
}

void GxxGmWSSimulator::Close()
{
	// ���̱߳��Ϊֹͣ��Ȼ��ȴ��߳��˳�����1���
	is_need_stop_ = true;
	working_thread_->join(1000);
}

void GxxGmWSSimulator::SetLogger(Poco::Logger *logger)
{
	logger_ = logger;
}

int GxxGmWSSimulator::SendHeartBeat()
{
	int errCode = 0;
	std::string errStr;

	try
	{
		// ��ȡ��ǰ����������
		int disk_total = 1024 * 1024;

		// ��ȡ��ǰ����ʹ����
		int used_total = 500 * 1024;

		// ��ȡ��ǰCPUʹ����
		int cpu_usage = 30;

		// ��ȡ��ǰ�ڴ�ʹ����
		int memory_usage = 40;

		char body[4096] = {0};
		sprintf_s(body, 4096,
			"{"
				"\"gzz_xh\":\"%s\","		// ����վID
				"\"gzz_ipdz\":\"%s\","		// ����վIP
				"\"zxzt\":\"1\","			// ����״̬
				"\"qizt\":\"1\","			// ����״̬
				"\"cczrl\":%d,"				// �洢������
				"\"syzrl\":%d,"				// ��ʹ��������
				"\"cpu\":%d,"				// CPUռ����
				"\"ram\":%d,"				// �ڴ�ʹ����
				"\"raid_zt\":101,"			// RAID��״̬
				"\"bjlx\":0,"				// ��������
				"\"version\":\"3.4.5\""		// ��ǰ�汾
			"}",
			workstaion_id_.c_str(), workstaion_ip_.c_str(), disk_total, used_total, cpu_usage, memory_usage
		);

		char uri_string[4096] = {0};
		sprintf_s(uri_string, 4096, 
			"/openapi/workstation/v3/wsinfo/heartbeat?gzz_xh=%s&authkey=%s&domain=%s",
			workstaion_id_.c_str(), authkey_.c_str(), domain_.c_str());

		// ��������
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, uri_string, Poco::Net::HTTPRequest::HTTP_1_1);
		request.add("Content-Type", "application/json; charset=utf-8");

		std::string http_body(body);
		request.setContentLength((int)http_body.length());

		// ��������
		session_->sendRequest(request) << http_body;

		// ����Ӧ��
		Poco::Net::HTTPResponse response;
		std::istream &is = session_->receiveResponse(response);

		// �жϷ�����������Ϣ
		Poco::Net::HTTPResponse::HTTPStatus status = response.getStatus();
		if (Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK != status)
		{
			errCode = status;
			return errCode;
		}

		/*
		{
		"code": 0,
		"message": "SUCCESS",
		"body": {
		"gzz_xh": "44010401901281918586",
		"name": "���ϵĲ��Թ���վ001",
		"bmbh": "44010401",
		"bmmc": "һ������",
		"admin": "",
		"phone": "",
		"address": "�����칫��",
		"wsconf": "{\"device_rule\":{\"bLUETOOTH\":\"0\",\"cDROM\":\"0\",\"mODEM\":\"0\",\"uDISK\":\"0\",\"uSB_KEYBOARD\":\"0\"},\"dsj_register_rule\":1,\"export_rule\":{\"export_to_common\":\"0\",\"export_to_ga\":\"0\"},\"net_rule\":{\"netsmon\":\"0\"},\"sys_rule\":{\"process\":[]}}",
		"regtime": "2019-02-28 18:22:18",
		"svrtime": "2019-03-02 19:03:19"
		}
		}
		*/

		std::ostringstream ostr;
		Poco::StreamCopier::copyStream(is, ostr);

		std::string json_str = ostr.str();
		if (json_str.empty())
		{
			return -1;
		}

		// ���ַ���תΪUTF-8
		Poco::Latin1Encoding latin1;
		Poco::UTF8Encoding utf8;
		Poco::TextConverter converter(latin1, utf8);
		std::string strUtf8;
		converter.convert(json_str, strUtf8);
		json_str = strUtf8;

		// �������
		Poco::JSON::Parser parser;
		Poco::Dynamic::Var json = parser.parse(json_str);
		Poco::JSON::Object::Ptr jsonObject = json.extract<Poco::JSON::Object::Ptr>();

		//Poco::Dynamic::Var message = jsonObject->get("message");
		//jsonObject = message.extract<Poco::JSON::Object::Ptr>();

		Poco::Dynamic::Var result_code = jsonObject->get("code");
		errCode = atoi(result_code.toString().c_str());
		if (errCode != 0)
			return errCode;
		
	}
	catch (Poco::Exception &ex)
	{
		std::cerr << "ex: " << ex.displayText().c_str() << std::endl;
	}


	//logger_->debug("Send Heartbeat....");
	return 0;
}

int GxxGmWSSimulator::SendFileInfo()
{
	int errCode = 0;
	std::string errStr;

	// �ļ���ţ������ɹ���[������8λ][ִ����ID��20λ][�ļ�����ʱ�䣺14λ][�ļ����ͱ��룺2λ][�ļ���ע״̬��2λ][�ļ�����ʱ�䣺14λ][˳��ţ�4λ]
	Poco::DateTime current_datetime;

	// �ļ�ID
	char file_id[4096] = {0};
	sprintf_s(file_id, 4096, "%s%s%d%02d%02d%02d%02d%02d0000%d%02d%02d%02d%02d%04d",
		file_domain_.c_str(), current_datetime.year(), current_datetime.month(), current_datetime.day(),
		current_datetime.hour(), current_datetime.minute(), current_datetime.second(), current_datetime.year(),
		current_datetime.month(), current_datetime.day(), current_datetime.hour(), current_datetime.minute(),
		current_datetime.second(), current_datetime.microsecond());

	// �ļ�����
	char file_name[4096] = {0};
	sprintf_s(file_name, 4096, "%d%02d%02d%02d%02d%02d%03d.mp4", current_datetime.year(),
		current_datetime.month(), current_datetime.day(), current_datetime.hour(), current_datetime.minute(),
		current_datetime.second(), current_datetime.microsecond());

	// ����ʱ��
	char camera_time[4096] = {0};
	sprintf_s(camera_time, 4096, "%d-%02d-%02d %02d:%02d:%02d", current_datetime.year(),
		current_datetime.month(), current_datetime.day(), current_datetime.hour(), current_datetime.minute(),
		current_datetime.second());

	// �ļ���С���̶�500M
	int file_size = 500 * 1024 * 1024;

	// �ļ�����
	const char *file_type = "0";

	// �ļ�ʱ��
	int file_duration = 15 * 60;

	// ��ע��Ϣ
	int tag_info = 0;

	try
	{
		//char body[409600] = {0};
		//sprintf_s(body, 409600,
		//	"["
		//		"{"
		//			"\"wjbh\": \"%s\","		// �ļ����
		//			"\"wjbm\": \"%s\","		// �ļ�����
		//			"\"pssj\": \"%s\","		// ����ʱ��
		//			"\"wjdx\": \"%d\","		// �ļ���С
		//			"\"wjlx\": \"%s\","		// �ļ�����
		//			"\"wjsc\": \"%d\","		// �ļ�ʱ�����룩
		//			"\"bzlx\": \"%d\","		// ��ע��Ϣ��0����ͨ�ļ���1���ص����ļ�
		//			"\"jgdm\": \"%s\","		// ��λ��Ż��ű��
		//			"\"dwmc\": \"%s\","		// ��λ����
		//			"\"jybh\": \"%s\","		// ��Ա���
		//			"\"jy_xm\": \"%s\","	// ��Ա����
		//			"\"cpxh\": \"%s\","		// ִ���ǲ�Ʒ�ͺ�
		//			"\"gzz_xh\": \"%s\","	// ����վ���
		//			"\"scsj\": \"%s\","		// �ϴ�ʱ�䣬��ʽΪ��yyyy-MM-dd HH:mm:ss
		//			"\"ccwz\": \"%s\","		// �洢λ��
		//			"\"wlwz\": \"%s\""		// ����λ�ã��ɼ�վ��ԭ�ļ������洢·��
		//			"\"bfwz\": \"%s\","		// ����λ�ã�HTTP����·��
		//			"\"ccfwq\": \"%s\","	// �洢������
		//			"\"sltxdwz\": \"%s\","	// �ɼ�վ����ͼ
		//		"}"
		//	"]", file_id, file_name, camera_time, file_size, file_type, file_duration, tag_info)
	}
	catch (Poco::Exception e)
	{
		errCode = e.code();
		errStr = e.displayText();
	}
	//logger_->debug("Send FileInfo....");
	return 0;
}

int GxxGmWSSimulator::SendLocationInfo()
{
	//logger_->debug("Send LocationInfo....");
	return 0;
}

int GxxGmWSSimulator::GetDiskTotalSpace()
{
	return 512 * 1024;
}

int GxxGmWSSimulator::GetDiskUsedSpace()
{
	return 1 * 1024;
}

int GxxGmWSSimulator::GetCpuUsage()
{
	return 10;
}

int GxxGmWSSimulator::GetMemoryUsage()
{
	return 40;
}

void GxxGmWSSimulator::WorkingThreadFunc(void* param)
{
	GxxGmWSSimulator *simulater = (GxxGmWSSimulator *)param;
	
	simulater->is_need_stop_ = false;

	int heartbeat_count = 0;
	int fileupload_count = 0;
	int locationupload_count = 0;
	while (!(simulater->is_need_stop_))
	{
		if (heartbeat_count == simulater->hearbeat_rate_)
		{
			// ��������
			simulater->SendHeartBeat();
			heartbeat_count = 0;
		}

		if (fileupload_count == simulater->fileupload_rate_)
		{
			// �ϴ��ļ���Ϣ
			simulater->SendFileInfo();
			fileupload_count = 0;
		}

		if (locationupload_count == simulater->locationupload_rate_)
		{
			simulater->SendLocationInfo();
			locationupload_count = 0;
		}

		++heartbeat_count;
		++fileupload_count;
		++locationupload_count;
		Sleep(1);
	}

	return ;
}