#include "PlayControlHandler.h"
#include "GB28181AgentSDK.h"

SIP_REPSOND_CODE PlayControlHandler::Handler(STREAM_HANDLE hStream, const StruPlayCtrlCmd * stuCmd, void * pUserData)
{
	//////////////////////////////////////////////////////////////////////////
	// ���ݲ��ſ������ͽ��д���
	switch (stuCmd->eType)
	{
	case EnumPlayCtrlType::ePLAY_START:
		// ��ʼ
		break;
	case EnumPlayCtrlType::ePLAY_PAUSE:
		// ��ͣ
		break;
	case EnumPlayCtrlType::ePLAY_FAST:
		// ���
		break;
	case EnumPlayCtrlType::ePLAY_SLOW:
		// ����
		break;
	case EnumPlayCtrlType::ePLAY_DRAG:
		// ����϶�
		break;
	default:
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}