#include "ExtendRqeustHandler.h"
#include "GB28181AgentSDK.h"

SIP_REPSOND_CODE ExtendRqeustHandler::Handler(SESSION_HANDLE hSession, EnumExtendType eType, const char * czTargetDevID, void * pMsg, void * pUserData)
{
	switch (eType)
	{
	case EnumExtendType::eEX_TRANSDATA:
		// ͸�������յ�͸�������ֱ���ӵ�������
		break;
	default:
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}