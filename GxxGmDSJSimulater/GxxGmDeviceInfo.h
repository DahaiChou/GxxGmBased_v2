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

#define BASE_INFO_Carrieroperator_WIFI	"0"		// �豸������Ϣ>>>����������Ӫ��>>>WIFI
#define BASE_INFO_Carrieroperator_CMCC 	"1"		// �豸������Ϣ>>>����������Ӫ��>>>�й��ƶ�
#define BASE_INFO_Carrieroperator_CUCC	"2"		// �豸������Ϣ>>>����������Ӫ��>>>�й���ͨ
#define BASE_INFO_Carrieroperator_CTCC	"3"		// �豸������Ϣ>>>����������Ӫ��>>>�й�����

#define BASE_INFO_Nettype_2G			"2"		// �豸������Ϣ>>>������������>>>2G
#define BASE_INFO_Nettype_3G			"3"		// �豸������Ϣ>>>������������>>>3G
#define BASE_INFO_Nettype_4G			"4"		// �豸������Ϣ>>>������������>>>4G
#define BASE_INFO_Nettype_5G			"5"		// �豸������Ϣ>>>������������>>>5G

#define BASE_INFO_LocalRecord_YES		"YES"	// �豸������Ϣ>>>����¼��״̬>>>¼����
#define BASE_INFO_LocalRecord_NO		"NO"	// �豸������Ϣ>>>����¼��״̬>>>δ¼��

#define LOCATION_INFO_DivisionN			"N"		// ������
#define LOCATION_INFO_DivisionS			"S"		// �ϰ���
#define LOCATION_INFO_DivisionE			"E"		// ������
#define LOCATION_INFO_DivisionW			"W"		// ������

#endif//_GxxGmDeviceInfo_H_
