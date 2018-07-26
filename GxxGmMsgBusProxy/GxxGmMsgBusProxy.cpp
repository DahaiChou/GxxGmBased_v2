#include "GxxGmMsgBusProxy.h"

GxxGmMsgBusProxy::GxxGmMsgBusProxy(GxxGmMsgBusRecevicer *recevicer)
: recevicer_(recevicer)
, connection_(NULL)
{

}

GxxGmMsgBusProxy::~GxxGmMsgBusProxy()
{

}

int GxxGmMsgBusProxy::Open(const char *bus_ip, unsigned int bus_port, const char *send_queue, const char *recevice_queue, const char *username /* = NULL */, const char *password /* = NULL */)
{
	char url[4096] = {0};
	sprintf(url, "amqp:tcp:%s:%d", bus_ip, bus_port);
	
	connection_ = new qpid::messaging::Connection(url, "");
	if (connection_ == NULL)
		return -1;

	// �������ӵ��û�������
	if ((username != NULL) && (password != NULL))
	{
		connection_->setOption("username", username);
		connection_->setOption("password", password);
	}

	// ���ӵ�qpid
	try {
		connection_->open();

		// �����Ự
		session_ = connection_->createSession();

		// ����ָ���ķ�������Ϣ����
		qpid::messaging::Address send_address("amq.topic");
		send_address.setSubject(send_queue);

		// ����������
		sender_ = session_.createSender(send_address);

		// ����ָ���Ľ�������Ϣ����
		qpid::messaging::Address recevice_address("amq.topic");
	}
}