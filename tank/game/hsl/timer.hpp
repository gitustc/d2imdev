#ifndef timer_h
#define timer_h

#include <stdio.h>
#include <sys/select.h>
#include <sys/signal.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


#define g_max_timer_handler  128 
typedef void(*TimerHandler)(void*);

namespace HSL{

	int SetTimer(int,TimerHandler,void*);
	bool RemoveTimer(int);

};


#endif
