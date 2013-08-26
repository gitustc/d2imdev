#include "timer.hpp"
#include <pthread.h>



void* g_timer_arg;//[g_max_timer_handler+1];
TimerHandler g_timerhandler;//[g_max_timer_handler+1];


void sys_call_timer(int no_use){
	g_timerhandler(g_timer_arg);
	signal(SIGALRM,sys_call_timer);
}

int HSL::SetTimer(int ms,TimerHandler th,void* parg){

	if(ms < 0)return false;

	g_timer_arg = parg;
	g_timerhandler = th;

	struct itimerval    value;
	value.it_value.tv_sec = ms/1000;
	value.it_value.tv_usec = (ms%1000)*1000;
	value.it_interval.tv_sec = ms/1000;
	value.it_interval.tv_usec = (ms%1000)*1000;

	signal(SIGALRM,sys_call_timer);

	setitimer(ITIMER_REAL,&value,NULL);


}

bool HSL::RemoveTimer(int timer_id){
}
