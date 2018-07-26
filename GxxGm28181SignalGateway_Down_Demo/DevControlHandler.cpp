#include "DevControlHandler.h"
#include "GB28181AgentSDK.h"

SIP_REPSOND_CODE DevControlHandler::Handler(const char * czSrvGBCode, const StruDevCtrlCmd * stuCmd, void * pUserData)
{
	//////////////////////////////////////////////////////////////////////////
	// �豸��������
	switch (stuCmd->eType)
	{
	case EnumDevCtrlType::eCTRL_PZT:
		// ��̨����
		break;
	case EnumDevCtrlType::eCTRL_TELEBOOT:
		// Զ������
		break;
	case EnumDevCtrlType::eCTRL_RECORD:
		// ¼�����
		break;
	case EnumDevCtrlType::eCTRL_GUARD:
		// ��������
		break;
	case EnumDevCtrlType::eCTRL_RESET_ALARM:
		// ������λ
		break;
	case EnumDevCtrlType::eCTRL_ZOOMIN:
		// ����Ŵ�
		break;
	case EnumDevCtrlType::eCTRL_ZOOMOUT:
		// ������С
		break;
	case EnumDevCtrlType::eCTRL_CONFIGSET:
		// �������ò���
		break;
	case EnumDevCtrlType::eCTRL_IFRAME:
		// ǿ�ƹؼ�֡
		break;
	case EnumDevCtrlType::eCTRL_HOMEPOSITION:
		// ���ؿ���
		break;
	default:
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}