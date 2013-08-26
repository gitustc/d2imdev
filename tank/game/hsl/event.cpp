#include "event.hpp"

extern Display* g_pdis;
extern int      g_display_count;

KeySym HSL::KeyValue(HSL::Event& event){
	return XKeycodeToKeysym(g_pdis,event.xkey.keycode,0);
}

KeySym HSL::KeyVal(HSL::Event& event){
	return XKeycodeToKeysym(g_pdis,event.xkey.keycode,0);
}
