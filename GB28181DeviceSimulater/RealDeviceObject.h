#ifndef _RealDeviceObject_H_
#define _RealDeviceObject_H_

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
};

#include "Poco/Thread.h"

#include "PSFormat.h"

enum MediaDeviceType
{
	Dev_Camera,
	Dev_Mic
};

class RealDeviceObjectObserver
{
public:
	virtual int MediaData(StruPSFrameInfo *ps_frame) = 0;
};

class RealDeviceObject
{
public:
	RealDeviceObject(RealDeviceObjectObserver *observer);
	~RealDeviceObject();

public:
	int SetDeviceName(const char *device_name, enum MediaDeviceType dev_type = Dev_Camera, const char *open_type = "dshow");

public:
	/**
	 * ���豸
	 */
	int OpenRealDevice();

	/**
	 * �ر��豸
	 */
	void CloseRealDevice();

public:
	/**
	 * ��ʼת��
	 */
	int Start(enum AVCodecID dest_codec_id);

	/**
	 * ֹͣת��
	 */
	int Stop();

public:
	Poco::Thread stream_handler_;
	static void StreamHandler(void *param);
	bool is_stream_handler_stop_;

public:
	std::string device_name_;
	std::string device_symbolink_;
	enum MediaDeviceType device_type_;
	std::string open_type_;

	AVFormatContext *input_format_ctx_;
	AVInputFormat *iformat_;

	// �������
	AVCodec *video_decoder_;
	AVCodec *audio_decoder_;
	AVCodecContext *video_decoder_ctx_;
	AVCodecContext *audio_decoder_ctx_;

	// �������
	enum AVCodecID dest_codec_id_;
	AVCodec *video_encoder_;
	AVCodec *audio_encoder_;
	AVCodecContext *video_encoder_ctx_;
	AVCodecContext *audio_encoder_ctx_;

	int input_video_stream_id_;
	int input_audio_stream_id_;

public:
	RealDeviceObjectObserver *observer_;
};

#endif
