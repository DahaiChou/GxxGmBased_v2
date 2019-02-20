// GxxGmRestfulServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HttpServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/StreamCopier.h"

// Ĭ�϶˿�
#define DEFAULT_SERVICE_PORT	9902

class GxxGmRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		int errCode = 0;
		std::string errStr = "";
		try
		{
			// �������յ��ⲿ����������
			std::string uri = request.getURI();
			//std::string host = request.getHost();
			std::string method = request.getMethod();

			// ��ȡ������������
			// ��ȡ����
			std::istream &is = request.stream();

			std::ostringstream ostr;
			std::istream *ptr_rs = &is;
			Poco::StreamCopier::copyStream(*ptr_rs, ostr);

			std::string request_body = ostr.str();

			std::cout<<"�յ�����"<<std::endl;
			std::cout<<method.c_str()<<" "/*<<host.c_str()*/<<uri.c_str()<<std::endl;

			if (request_body.size() > 0)
				std::cout<<request_body.c_str()<<std::endl;

			std::cout<<std::endl;

			// ��Ӧ��Ϣ
			response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
			std::ostream &out = response.send();
		}
		catch(Poco::Exception &ex)
		{
			errCode = ex.code();
			errStr = ex.displayText();

			std::cout<<"��������ʱ�����쳣�������룺"<<errCode<<"��������Ϣ��"<<errStr.c_str()<<std::endl;
		}
	}
};

class GxxGmRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
	{
		return new GxxGmRequestHandler;
	}
};

class GxxGmServerApp : public Poco::Util::ServerApplication
{
protected:
	int main(const std::vector<std::string>& args)
	{
		// ���Ȼ�õ�ǰ����Ŀ¼
		std::string current_working_dir = Poco::Path::current();
		Poco::Path config_path(current_working_dir);
		config_path.append("config.ini");
		this->loadConfiguration()
		int port = DEFAULT_SERVICE_PORT;
		if (args.size() > 0)
			port = atoi(args[0].c_str());

		Poco::Net::HTTPServer server(new GxxGmRequestHandlerFactory, Poco::Net::ServerSocket(port), new Poco::Net::HTTPServerParams);
		server.start();
		waitForTerminationRequest();
		server.stop();

		return Poco::Util::Application::EXIT_OK;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	GxxGmServerApp app;
	return app.run(argc, argv);
}

