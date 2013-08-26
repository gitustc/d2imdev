#include <iostream>
#include "hsl.hpp"

bool flag;
int cou;

void handle_event(HSL::Event& e,void* parg){
	switch(e.type){
		case KeyPress:
			cou ++;
			printf("Key Press...,val = %d \n",HSL::KeyVal(e));
		default:
			break;
	}
}
int main(int argc,char** argv){

	HSL::Window win;

	flag = true;
	cou = 0;

	win.set_event_mask(KeyPressMask);
	win.set_event_handler(handle_event,NULL);
	while(flag){
		while(win.pending()){
			win.next_event();
			win.handle_event();
		}
		usleep(20);
	}
	win.flush();
	win.sync(false);

	sleep(10);

}
