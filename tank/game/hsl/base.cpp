#include <X11/Xlib.h>
#include <iostream>
volatile Display* g_pdis = NULL;
volatile int g_display_count = 0;
volatile void* g_timer_parg = NULL;

