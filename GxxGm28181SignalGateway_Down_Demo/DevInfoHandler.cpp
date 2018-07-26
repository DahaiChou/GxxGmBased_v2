#include "DevInfoHandler.h"
#include "GB28181AgentSDK.h"

#include "GB28181DownImp.h"

SIP_REPSOND_CODE DevInfoHandler::Handler(SESSION_HANDLE hSession, const char * czSrvGBCode, StruQueryReqDescri * stuQuery, void * pUserData)
{
	GB28181DownImp *gb28181 = (GB28181DownImp *)pUserData;

	//////////////////////////////////////////////////////////////////////////
	// ���ȼ���ѯ���ͣ����ݲ�ѯ���ͽ������ŵ���ͬ�Ĵ�������ȥ
	switch (stuQuery->eType)
	{
	case EnumQueryType::eQUE_DEV_INFO:
		// �豸��Ϣ��ѯ
		{
			// ����Ϣ�����ߣ���ѯָ���豸���豸��Ϣ
			// ����ѯ�������͸���Ϣ���ߣ�����Ĳ�ѯ������������/�豸/ϵͳ�Ĺ���ID
			// ������֯һ��Json��Ȼ��ͨ����Ϣͨ�Žӿڷ�������
			char json_str[4096] = {0};
			sprintf(json_str, "{\"type\":\"devinfo\",\"gbcode\":\"%s\"}", stuQuery->czGBCode);
			
		}
		break;
	case EnumQueryType::eQUE_DEV_STATUS:
		// �豸״̬��ѯ
		break;
	case EnumQueryType::eQUE_DEV_CATALOG:
		// �豸Ŀ¼��ѯ
		{
			StruDeviceCatalog stuCatlog;
			gs_strncpy(stuCatlog.czGBCode, stuQuery->czGBCode, STR_GBCODE_LEN);

			// ����Ϣ�����ߣ���ѯ��֯�ܹ����豸�б�
			// ���������Ҫע�⣬������򼯳�����̫�࣬������Ҫ��ǰ����һЩ׼��
			// ���绺����֯�ܹ��б����߶�̬������֯�ܹ��б�
			stuCatlog.ptrCatalog = gb28181->device_catlog_mgr_->ToGB28181Format(stuCatlog.iSumNum);

			int errCode = GB28181Agent_RespondDevCatalog(hSession, &stuCatlog, 0, false);
			if (errCode == GS28181_ERR_SUCCESS)
			{
				// ���ͳɹ�
			}
			else
			{
				// ����ʧ��
			}

			gb28181->device_catlog_mgr_->ReleaseInfoBuffer(&stuCatlog.ptrCatalog);
		}
		break;
	case EnumQueryType::eQUE_DEV_RECORDINDEX:
		// �豸¼��������ѯ
		break;
	case EnumQueryType::eQUE_DEV_CONFIGDOWNLOAD:
		// �豸���ò�ѯ
		break;
	case EnumQueryType::eQUE_DEV_PRESET:
		// �豸Ԥ��λ��ѯ
		break;
	case EnumQueryType::eQUE_DEV_UNKNOW:
		// δ֪��ѯ
		break;
	default:
		break;
	}

	return SIP_RESPONSE_CODE_SUCCESS;
}