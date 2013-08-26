#include "sound.hpp"
#include <unistd.h>
using namespace HSL;


pthread_mutex_t g_mutex;
int g_dsp_fd;

void* write_dsp(void* parg){
//	int res;
//	Channel* p = (Channel*)parg;
//	res = write(g_dsp_fd,
//				p->data,
//				p->size);
//	pthread_mutex_unlock(&g_mutex);
//	if(res == -1){
//		fprintf(stderr,"Fail to play channel\n");
//	}
//	return NULL;
}


HSL::Audio::Audio(){
//	int arg;
//	int res;
//
//	m_handler = open("/dev/dsp",O_WRONLY|O_SYNC);
//	g_dsp_fd = m_handler;
//
//	arg = Sample_Rate;
//	res = ioctl(m_handler,SOUND_PCM_WRITE_RATE,&arg);
//	if(res == -1){
//		throw std::runtime_error("Error from SOUND_PCM_WRITE_RATE to ioctl\n");
//	}
//
//
//	arg = Sample_Size;
//	res = ioctl(m_handler,SOUND_PCM_WRITE_BITS,&arg);
//	if(res == -1){
//		throw std::runtime_error("Error from SOUND_PCM_WRITE_BITS to ioctl\n");
//	}
//
//	pthread_mutex_init(&g_mutex,NULL);
//
//	for(int i=0;i<g_max_channel;i++){
//		m_channel[i].data = NULL;
//		m_channel[i].size = 0;
//	}

}

HSL::Audio::~Audio(){
//
//	pthread_mutex_destroy(&g_mutex);
//	for(int i=0;i<g_max_channel;i++){
//		if(!m_channel[i].data){
//			free(m_channel[i].data);
//		}
//	}
//	close(m_handler);

}
void HSL::Audio::play_sound(int id){
//	pthread_t pid;
//	if(!pthread_mutex_trylock(&g_mutex)){
//		pthread_create(&pid,NULL,write_dsp,(void*)(&m_channel[id]));
//	//	pthread_mutex_unlock(&g_mutex);
////	}else{
////		fprintf(stderr,"busy...\n");
//	}

}

void HSL::Audio::add_sound(int ind,const std::string& path){
//	int fd;
//	struct stat stat_buf;
//
//	fd = open(path.c_str(),O_RDONLY);
//	if(fd<0){
//		fprintf(stderr,"fail to open %s\n",path.c_str());
//		return;
//	}
//
//	fstat(fd,&stat_buf);
//	if(m_channel[ind].data)free(m_channel[ind].data);
//	m_channel[ind].data = malloc(stat_buf.st_size);
//	if(!m_channel[ind].data){
//		close(fd);
//		fprintf(stderr,"fail to malloc for %s\n",path.c_str());
//		return;
//	}
//	if(read(fd,m_channel[ind].data,stat_buf.st_size)<0){
//		free(m_channel[ind].data);
//		close(fd);
//		return ;
//	}
//	m_channel[ind].size = stat_buf.st_size;
//	close(fd);
}
