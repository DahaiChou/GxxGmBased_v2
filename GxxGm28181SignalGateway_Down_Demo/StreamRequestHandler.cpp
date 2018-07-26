#include "StreamRequestHandler.h"
#include "GB28181AgentSDK.h"

SIP_REPSOND_CODE StreamRequestHandler::Handler(STREAM_HANDLE hStream, const char * czSrvGBCode, EnumStreamRequest eRequest, StruMediaInfo * pInMedia, const StruStreamDescription * pDescri, void * pUserData)
{
	//////////////////////////////////////////////////////////////////////////
	//
	switch (eRequest)
	{
	case EnumStreamRequest::eSTREAM_REALPLAY:
		// ʵʱ��Ƶ����
		break;
	case EnumStreamRequest::eSTREAM_PLAYBACK:
		// ��ʷ����Ƶ�ط�
		break;
	case EnumStreamRequest::eSTREAM_DOWNLOAD:
		// ��ʷ����Ƶ����
		break;
	case EnumStreamRequest::eSTREAM_AUDIO:
		// ����������
		break;
	case EnumStreamRequest::eSTREAM_BYE:
		// ������
		break;
	case EnumStreamRequest::eSTREAM_TIMEOUT:
		// �첽������ʱ
		break;
	case EnumStreamRequest::eSTREAM_REJECT:
		// �첽�����󱻾ܾ�
		break;
	default:
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}