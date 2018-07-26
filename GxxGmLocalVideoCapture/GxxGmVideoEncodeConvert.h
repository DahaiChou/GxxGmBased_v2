#ifndef _GxxGmVideoEncodeConvert_H_
#define _GxxGmVideoEncodeConvert_H_

//////////////////////////////////////////////////////////////////////////
//
// ֧�ֵı����ʽ

// ��Ƶ����
#define VIDEO_ENCODE_H264	0x00000001
#define VIDEO_ENCODE_HEVC	0x00000002

// ��Ƶ����
#define AUDIO_ENCODE_AAC	0x0000F001
#define AUDIO_ENCODE_MP3	0x0000F002
#define AUDIO_ENCODE_G711u	0x0000F003
#define AUDIO_ENCODE_G711a	0x0000F004


//////////////////////////////////////////////////////////////////////////
//
// ��Ƶ����ת��
class GxxGmVideoEncodeConvert
{
public:
	GxxGmVideoEncodeConvert();
	~GxxGmVideoEncodeConvert();

public:
	int InitVideoEncoder(int video_encoder);
	int InitAudioEncoder(int audio_encoder);

public:
	int ConvertVideo();
	int ConvertAudio();

private:
	int video_encoder_;
	int audio_encoder_;
};

#endif//_GxxGmVideoEncodeConvert_H_
