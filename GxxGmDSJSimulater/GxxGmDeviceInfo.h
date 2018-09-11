#ifndef _GxxGmDeviceInfo_H_
#define _GxxGmDeviceInfo_H_

#include <string>

// �豸������Ϣ
typedef struct _DEVICE_BASE_INFO_
{
	std::string carrieroperator_;	// ��Ӫ��
	std::string net_type_;			// ��������
	std::string signal_;			// �ź�ǿ��
	std::string battery_;			// ʣ�����
	std::string storage_;			// ʣ������
	std::string cpu_;				// CPUռ����
	std::string version_;			// ϵͳ�汾��
	std::string local_record_;		// ����¼��״̬
} DEVICE_BASE_INFO, *PDEVICE_BASE_INFO;

// �豸��λ��Ϣ
typedef struct _DEVICE_LOCATION_INFO_
{
	std::string division_ns_;
	std::string division_ew_;
	std::string radius_;
	std::string longitude_;
	std::string latitude_;
	std::string direction_;
	std::string speed_;
	std::string height_;
	std::string location_time_;
	std::string satellites_;
	std::string location_available_;
} DEVICE_LOCATION_INFO, *PDEVICE_LOCATION_INFO;

// �豸�쳣��Ϣ
typedef struct _DEVICE_EXCEPTION_INFO_
{
	std::string storage_;
	std::string battery_;
	std::string ccd_;
	std::string mic_;
} DEVICE_EXCEPTION_INFO, *PDEVICE_EXCEPTION_INFO;

#endif//_GxxGmDeviceInfo_H_
