#ifndef sound_h
#define sound_h
#include <string>

#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<linux/soundcard.h>
#include<pthread.h>

#include<exception>
#include<stdexcept>

#define Sample_Size 8
#define Sample_Rate 14914
#define g_max_channel 10

typedef struct {
	void* data;
	long   size;
}Channel;


typedef struct WaveData{
	unsigned long sample_lenth;
	unsigned short rate;
	unsigned short channels;
	unsigned char time_constant;
	char bit_res;
	char *name_wav;
	char *sample;
}WAV;

typedef struct HeaderType{
	long riff; /*RIFF类资源文件头部*/
	unsigned long file_len; /*文件长度*/
	char wave[4]; /*"WAVE"标志*/
	char fmt [4]; /*"fmt"标志*/
	char NI1 [4]; /*过渡字节*/
	unsigned short format_type;/*格式类别(10H为PCM形式的声音数据)*/
	unsigned short Channels; /*Channels 1 = 单声道; 2 = 立体声*/
	long frequency; /*采样频率*/
	long trans_speed;/*音频数据传送速率*/
	char NI2 [2]; /*过渡字节*/
	short sample_bits;/*样本的数据位数(8/16)*/
	char data[4]; /*数据标记符"data"*/
	unsigned long wav_len; /*语音数据的长度*/
	char NI3 [4]; /*过渡字节*/
} HEAD_WAV;



namespace HSL{


class Sound{};
class Music{};

class Audio{
	public:
		Audio();
		virtual ~Audio();
	public:
		void add_sound(int,const std::string&);
		void play_sound(int);
		void stop_sound(int);
	private:
//		static pthread_mutex_t m_mutex;
		Channel  m_channel[g_max_channel];
		int 	m_handler;
};









};





#endif

