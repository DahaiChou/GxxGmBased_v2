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
#include "Poco/ASCIIEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/DateTime.h"

//#include "json/json.h"

//#ifdef _DEBUG
//#pragma comment(lib, "jsoncppd.lib")
//#else
//#pragma comment(lib, "jsoncpp.lib")
//#endif

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

void GxxGmWSSimulator::SetFileUploadInfo(std::string file_domain, std::string dsj_id, int file_size,
			std::string file_type, int file_duration, std::string tag_info, std::string org_code, 
			std::string org_name, std::string police_code, std::string police_name, std::string storage_path, 
			std::string local_path, std::string play_path, std::string storage_server, std::string thumb)
{
	file_domain_ = file_domain;			// �ļ�������
	dsj_id_ = dsj_id;					// ִ���ǹ���ID
	police_name_ = police_name;			// ������
	file_size_ = file_size;				// �ļ���С
	file_type_ = file_type;				// �ļ�����
	file_duration_ = file_duration;		// �ļ�ʱ��
	tag_info_ = tag_info;				// ��ע��Ϣ
	org_code_ = org_code;				// ��λ��Ż��ű��
	org_name_ = org_name;				// ��λ����
	police_code_ = police_code;			// ��Ա���
	police_name_ = police_name;			// ��Ա����
	storage_path_ = storage_path;		// �洢λ��
	local_path_ = local_path;			// ����λ�ã��ɼ�վ��ԭ�ļ������洢·��
	play_path_ = play_path;				// ����λ�ã�HTTP����·��
	storage_server_ = storage_server;	// �洢������
	thumb_ = thumb;						// �ɼ�վ����ͼ
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

		Poco::JSON::Object json_object;
		json_object.set("gzz_xh", workstaion_id_);
		json_object.set("gzz_ipdz", workstaion_ip_);
		json_object.set("zxzt", "1");
		json_object.set("qizt", "1");
		json_object.set("cczrl", disk_total);
		json_object.set("syzrl", used_total);
		json_object.set("cpu", cpu_usage);
		json_object.set("ram", memory_usage);
		json_object.set("raid_zt", 101);
		json_object.set("bjlx", 0);
		json_object.set("version", "3.4.5");

		std::stringstream  jsnString;
		json_object.stringify(jsnString, 3);
		std::cout << jsnString.str() << std::endl;
		std::string body = jsnString.str();

		//Json::Value root;
		//Json::Value heart_beat_json_object;
		//heart_beat_json_object["gzz_xh"] = workstaion_id_.c_str();
		//heart_beat_json_object["gzz_ipdz"] = workstaion_ip_.c_str();
		//heart_beat_json_object["zxzt"] = "1";
		//heart_beat_json_object["qizt"] = "1";
		//heart_beat_json_object["cczrl"] = disk_total;
		//heart_beat_json_object["syzrl"] = used_total;
		//heart_beat_json_object["cpu"] = cpu_usage;
		//heart_beat_json_object["ram"] = memory_usage;
		//heart_beat_json_object["raid_zt"] = 101;
		//heart_beat_json_object["bjlx"] = 0;
		//heart_beat_json_object["version"] = "3.4.5";
		//root.append(heart_beat_json_object);

		//std::string json_value = root.toStyledString();

		//char body[4096] = {0};
		//sprintf_s(body, 4096,
		//	"{"
		//		"\"gzz_xh\":\"%s\","		// ����վID
		//		"\"gzz_ipdz\":\"%s\","		// ����վIP
		//		"\"zxzt\":\"1\","			// ����״̬
		//		"\"qizt\":\"1\","			// ����״̬
		//		"\"cczrl\":%d,"				// �洢������
		//		"\"syzrl\":%d,"				// ��ʹ��������
		//		"\"cpu\":%d,"				// CPUռ����
		//		"\"ram\":%d,"				// �ڴ�ʹ����
		//		"\"raid_zt\":101,"			// RAID��״̬
		//		"\"bjlx\":0,"				// ��������
		//		"\"version\":\"3.4.5\""		// ��ǰ�汾
		//	"}",
		//	workstaion_id_.c_str(), workstaion_ip_.c_str(), disk_total, used_total, cpu_usage, memory_usage
		//);

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

		// �������صĽ�����������SUCCESS����ȡ�����ű�ţ�bmbh���Ͳ������ƣ�bmmc�����Զ�����ģ������Ӧ�ֶ�
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

		Poco::Dynamic::Var body_json = jsonObject->get("body");
		Poco::JSON::Object::Ptr body_jsonObject = body_json.extract<Poco::JSON::Object::Ptr>();

		Poco::Dynamic::Var org_code = body_jsonObject->get("bmbh");
		workstaion_org_code_ = org_code.toString();

		Poco::Dynamic::Var org_name = body_jsonObject->get("bmmc");
		workstaion_org_name_ = org_name.toString();

		// �ɼ�վ������������ת��ΪGBK
		Poco::UTF8Encoding utf8Encoding;
		Poco::ASCIIEncoding asciiEncoding;
		Poco::TextConverter converter2(utf8Encoding,asciiEncoding);
		std::string workstaion_org_name_ascii;
		int errors = converter2.convert(workstaion_org_name_, workstaion_org_name_ascii); // ����ת���������� ����?��
		std::cout<<workstaion_org_name_ascii.c_str()<<std::endl;
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
	sprintf_s(file_id, 4096, "%s_%s%d%02d%02d%02d%02d%02d0000%d%02d%02d%02d%02d%04d",
		file_domain_.c_str(), dsj_id_.c_str(), current_datetime.year(), current_datetime.month(), current_datetime.day(),
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

	// �ϴ�ʱ�䣬��ʽΪ��yyyy-MM-dd HH:mm:ss
	char upload_time[4096] = {0};
	strcpy_s(upload_time, 4096, camera_time);

	try
	{
		Poco::JSON::Array json_array;
		Poco::JSON::Object json_object;
		Poco::JSON::Object json_object_array;
		json_object.set("wjbh", file_id);
		json_object.set("wjbm", file_name);
		json_object.set("pssj", camera_time);
		json_object.set("wjdx", file_size_);
		json_object.set("wjlx", file_type_);
		json_object.set("wjsc", file_duration_);
		json_object.set("bzlx", tag_info_);
		json_object.set("jgdm", org_code_);
		json_object.set("dwmc", org_name_);
		json_object.set("jybh", police_code_);
		json_object.set("jy_xm", police_name_);
		json_object.set("cpxh", dsj_id_);
		json_object.set("gzz_xh", workstaion_id_);
		json_object.set("scsj", upload_time);
		json_object.set("ccwz", storage_path_);
		json_object.set("wlwz", local_path_);
		json_object.set("bfwz", play_path_);
		json_object.set("ccfwq", storage_server_);
		json_object.set("sltxdwz", thumb_);

		json_array.add(json_object);

		std::stringstream  jsnString;
		json_array.stringify(jsnString, 3);
		std::cout << jsnString.str() << std::endl;
		std::string body = jsnString.str();

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

int GxxGmWSSimulator::GetOrgInfo()
{
	int errCode = 0;
	std::string errStr;

	try
	{
		char uri_string[4096] = {0};
		sprintf_s(uri_string, 4096, 
			"/openapi/workstation/v3/suborg?domain=2431&gzz_xh=%s&authkey=1234&sjbmbh=%s",
			workstaion_id_.c_str(), workstaion_org_code_.c_str());

		// ��������
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri_string, Poco::Net::HTTPRequest::HTTP_1_1);
		request.add("Content-Type", "application/json; charset=utf-8");


		// ��������
		session_->sendRequest(request);

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
			"body": [
				{ "bmbh": "44010401", "bmmc": "һ������", "sjbmbh": null },
				{ "bmbh": "22222222", "bmmc": "org2", "sjbmbh": "44010401" },
				{ "bmbh": "44010402", "bmmc": "org3", "sjbmbh": "44010401" },
				{ "bmbh": "12345678910", "bmmc": "org4", "sjbmbh": "44010401" },
				{ "bmbh": "1234567891011", "bmmc": "org5", "sjbmbh": "44010401" }
			]
		}
		*/

		// �������صĽ�����������SUCCESS����ȡ�����ű�ţ�bmbh���Ͳ������ƣ�bmmc�����Զ�����ģ������Ӧ�ֶ�
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
	catch(Poco::Exception e)
	{
		errCode = e.code();
		errStr = e.displayText();
	}

	return errCode;
}

int GxxGmWSSimulator::GetUserInfo()
{
	int errCode = 0;
	std::string errStr;

	return errCode;
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

	int heartbeat_count = simulater->hearbeat_rate_;
	int fileupload_count = simulater->fileupload_rate_;
	int locationupload_count = simulater->locationupload_rate_;

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