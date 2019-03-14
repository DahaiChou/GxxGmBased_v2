#include "GxxGmPoliceStatuAndCasePlugin.h"

// ����������cpp�ļ�����ʵ�֣�����û���嵥��Ϣ
POCO_BEGIN_MANIFEST(AbstractPlugin)
POCO_EXPORT_CLASS(GxxGmPoliceStatuAndCasePlugin)
POCO_END_MANIFEST

std::string GxxGmPoliceStatuAndCasePlugin::name() const
{
	return "GxxGmPoliceStatuAndCasePlugin";
}

void GxxGmPoliceStatuAndCasePlugin::start(Poco::Util::Application *app)
{
	// ����Ӧ������һ���߳�����������ҵ��
	if (!working_thread_.isRunning())
	{
		app_ = app;
		working_thread_.start(GxxGmPoliceStatuAndCasePlugin::WorkingThread, this);
	}
	
	std::cout << "GxxGmPoliceStatuAndCasePlugin::start()" << std::endl;
}

void GxxGmPoliceStatuAndCasePlugin::WorkingThread(void* param)
{
	GxxGmPoliceStatuAndCasePlugin *plugin = (GxxGmPoliceStatuAndCasePlugin *)param;

	plugin->app_->logger().debug("GxxGmPoliceStatuAndCasePlugin::WorkingThread() started...");

	// �������ļ��ж�ȡ��Ӧ����Ϣ������
	// ȡ����Ҫģ����豸����
	std::string gateway_ip = plugin->app_->config().getString("GATEWAY_INFO.IPADDRESS");
	int gateway_port = plugin->app_->config().getInt("GATEWAY_INFO.PORT");

	// �������ӵ��Խӷ�����
	Poco::Net::SocketAddress address(gateway_ip, gateway_port);
	Poco::Net::StreamSocket socket(address);
	Poco::Net::HTTPSession session(socket);

	while (try)
	{
		// ��֯
	}

	plugin->app_->logger().debug("GxxGmPoliceStatuAndCasePlugin::WorkingThread() stopped...");
	return ;
}

// optional set up and clean up functions
void pocoInitializeLibrary()
{
	std::cout << "PluginLibrary initializing" << std::endl;
}

void pocoUninitializeLibrary()
{
	std::cout << "PluginLibrary uninitializing" << std::endl;
}