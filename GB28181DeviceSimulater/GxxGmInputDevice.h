#ifndef _GxxGmInputDevice_H_
#define _GxxGmInputDevice_H_

#include <windows.h>  
#include <map>  
#include <dshow.h>

#include "Poco/Thread.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavutil/avutil.h"
#include "libavutil/rational.h"
#include "libavutil/pixdesc.h"
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#ifdef __cplusplus
};
#endif

#include "GxxGmQueue.h"	

#define MAX_FRIENDLY_NAME_LENGTH    128  
#define MAX_MONIKER_NAME_LENGTH     256  

typedef struct _TDeviceName  
{  
	WCHAR FriendlyName[MAX_FRIENDLY_NAME_LENGTH];   // �豸�Ѻ���  
	WCHAR MonikerName[MAX_MONIKER_NAME_LENGTH];     // �豸Moniker��  
} TDeviceName;

class GxxGmInputDevice
{
public:
	GxxGmInputDevice();
	~GxxGmInputDevice();

public:
	/**
	 * ö�ٱ��ؽ���������豸
	 */
	int EnumDevices();

private:
	int EnumInputDevices(std::map<int, TDeviceName> *devices, REFGUID guidValue);

public:
	std::map<int, TDeviceName> video_devices;
	std::map<int, TDeviceName> audio_devices;
};

//////////////////////////////////////////////////////////////////////////
//
//

class GxxGmDeviceObserver
{
public:
	virtual int MediaParam() = 0;
	virtual int MediaData(unsigned char *data, unsigned int len) = 0;
};

class GxxGmDevice
{
public:
	GxxGmDevice();
	~GxxGmDevice();

public:
	// ����۲��ߣ��۲������õ���Ƶ��������Ƶ�����
	int InsertObserver(GxxGmDeviceObserver *observer);
	// �Ƴ��۲���
	void RemoveObserver();

public:
	/**
	 * ���豸
	 * ������
	 *	@device_name	�豸����
	 *	@device_type	�豸���ͣ�0-video��1-audio
	 */ 
	int Open(const char *device_name, int device_type = 0);

	// ��ʼ�����ݰ�
	int Start();

	// ֹͣ�����ݰ�
	void Stop();

	// �ر��豸
	void Close();

public:
	// �豸����Ƶ���ݶ�ȡ�߳�
	Poco::Thread device_read_thread_;
	static void device_read_thread_fun(void *param);
	bool device_read_thread_exit_;

	// ����ת���߳�
	Poco::Thread recode_thread_;
	static void recode_thread_fun(void *param);
	bool recode_thread_exit_;

	// �۲��߷ַ�ģ��
	Poco::Thread observer_dispatch_thread_;
	static void observer_dispatch_thread_fun(void *param);
	bool observer_dispatch_thread_exit_;

public:
	GxxGmDeviceObserver *observer_;

public:
	AVFormatContext *input_format_ctx_;
	AVInputFormat *iformat_;
	int device_type_;
	std::string device_name_;

	// �������
	AVCodec *video_decoder_;
	AVCodec *audio_decoder_;
	AVCodecContext *video_decoder_ctx_;
	AVCodecContext *audio_decoder_ctx_;

	// �������
	AVCodec *video_encoder_;
	AVCodec *audio_encoder_;
	AVCodecContext *video_encoder_ctx_;
	AVCodecContext *audio_encoder_ctx_;

	int input_video_stream_id_;
	int input_audio_stream_id_;

public:
	// ԭʼ��Ƶ���������
	GxxGmQueue<AVPacket> original_video_packet_queue_;
	GxxGmQueue<AVPacket> original_audio_packet_queue_;
	// ԭʼͼ�����ݶ���
	// �ر����ı��������
	GxxGmQueue<AVPacket> recode_packet_queue_;
};

#endif//_GxxGmInputDevice_H_
