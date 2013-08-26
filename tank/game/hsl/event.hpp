#ifndef event_h
#define event_h
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern Display* g_pdis;
extern int      g_display_count;



#define   KEY_Return XK_Return
#define   KEY_Esc XK_Escape
#define   KEY_Space (32)
#define   KEY_Up XK_Up
#define   KEY_Down XK_Down
#define   KEY_w XK_w
#define   KEY_a XK_a
#define   KEY_s XK_s
#define   KEY_j XK_j
#define   KEY_d XK_d
#define   KEY_Up XK_Up
#define   KEY_Down XK_Down
#define   KEY_Left XK_Left
#define   KEY_Right XK_Right



namespace HSL{

typedef XEvent Event;
typedef void (*EventHandler)(Event&,void*);


KeySym KeyValue(HSL::Event&);
KeySym KeyVal(HSL::Event&);


};

#endif

