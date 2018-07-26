#include "NotifyInfoHandler.h"
#include "GB28181AgentSDK.h"

SIP_REPSOND_CODE NotifyInfoHandler::Handler(EnumNotifyType eType, const char * czSrvGBCode, void * pMsg, void * pUserData)
{
	switch (eType)
	{
		//////////////////////////////////////////////////////////////////////////
		//����˽��յ���֪ͨ
	case EnumNotifyType::eNOTIFY_HEARTBEAT:
		// ����֪ͨ
		break;
	case EnumNotifyType::eNOTIFY_MEDIASTATUS:
		// ý��״̬
		break;
	case EnumNotifyType::eNOTIFY_ALARM:
		// ����
		break;
	case EnumNotifyType::eNOTIFY_ALARMFORSUBS:
		// ��Զ��Ķ����͵ı���
		break;
	case EnumNotifyType::eNOTIFY_CATALOG:
		// Ŀ¼֪ͨ
		break;
	case EnumNotifyType::eNOTIFY_MOBILEPOS:
		// �ƶ��豸λ����Ϣ֪ͨ
		break;
	case EnumNotifyType::eNOTIFY_TRANSDATA:
		// ����˷�����͸������
		break;

		//////////////////////////////////////////////////////////////////////////
		//����˽��յ���֪ͨ
	case EnumNotifyType::eNOTIFY_BROADCAST:
		// �����㲥
		break;
	case EnumNotifyType::eNOTIFY_CATASUBS:
		// �յ�Ŀ¼����֪ͨ
		break;
	case EnumNotifyType::eNOTIFY_ALARMSUBS:
		// �յ���������֪ͨ
		break;
	case EnumNotifyType::eNOTIFY_MOBILEPOSSUB:
		// �յ��ƶ��豸λ�ö���֪ͨ
		break;

		//////////////////////////////////////////////////////////////////////////
		//Э���ڲ�֪ͨ��Ϣ
	case EnumNotifyType::eNOTIFY_SUBSEXPIRED:
		// ������ֹ
		break;
	default:
		break;
	}
	return SIP_RESPONSE_CODE_SUCCESS;
}