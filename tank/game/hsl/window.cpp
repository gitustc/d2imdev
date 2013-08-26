#include <iostream>
#include <exception>
#include "image.hpp"
#include <sys/select.h>
#include <sys/signal.h>
#include <signal.h>
//#include <sys/time.h>
//#include <unistd.h>
#include "window.hpp"

extern Display* g_pdis;
extern int	    g_display_count;
int HSL::Window::s_window_count = 0;



HSL::Window::Window(){


	//获取pdis
	if(HSL::Window::s_window_count > 0)
		throw std::runtime_error("only one Window could be constructed... \n");


	if (g_display_count > 0){
		m_pdis = g_pdis;
		g_display_count ++;
	} else {
		g_pdis = XOpenDisplay((char*)getenv("DISPLAY"));
		if ( g_pdis == NULL) 
			throw std::runtime_error("Connect to X server failed ...\n");
		m_pdis = g_pdis ;
		g_display_count = 1;
	}




	//获取Window
	m_width = 640;
	m_height = 480;
	m_reverse_video = true;
	m_event_handler = NULL;


	m_win = XCreateSimpleWindow(
			m_pdis,
			RootWindow(m_pdis,DefaultScreen(m_pdis)),
			0,0,
			m_width,m_height,
			0,
			WhitePixel(m_pdis,DefaultScreen(m_pdis)),
			BlackPixel(m_pdis,DefaultScreen(m_pdis))
			);




//
//	m_title = "Default";
//	m_width = 640;
//	m_height = 480;
//	m_reverse_video = true;
//	m_attr.background_pixel = BlackPixel(m_pdis,DefaultScreen(m_pdis)),
//
//	m_win = XCreateWindow(m_pdis,
//			RootWindow(m_pdis,DefaultScreen(m_pdis)),
//			0,0,
//			m_width,m_height,
//			1,
//			DefaultDepth(m_pdis,DefaultScreen(m_pdis)),
//			CopyFromParent,
//			DefaultVisual(m_pdis,DefaultScreen(m_pdis)),
//			0,
//			&m_attr);
//




	{//create gc
		unsigned long valuemask = 0; /* which values in 'values' to */ 
								 /* check when creating the GC. */ 
		XGCValues values; /* initial values for the GC. */ 
		unsigned int line_width = 2; /* line width for the GC. */ 
		int line_style = LineSolid; /* style for lines drawing and */ 
		int cap_style = CapButt; /* style of the line's edje and */ 
		int join_style = JoinBevel; /* joined lines. */ 
		int screen_num = DefaultScreen(m_pdis); 
	
		m_gc = XCreateGC(m_pdis, m_win, valuemask, &values); 
		if (m_gc < 0)
			throw std::runtime_error("XCreateGC fail ... \n"); 
	
		/* allocate foreground and background colors for this GC. */ 
		if (!m_reverse_video) { 
			XSetForeground(m_pdis, m_gc, WhitePixel(m_pdis, screen_num)); 
			XSetBackground(m_pdis, m_gc, BlackPixel(m_pdis, screen_num)); 
		} else { 
			XSetForeground(m_pdis, m_gc, BlackPixel(m_pdis, screen_num)); 
			XSetBackground(m_pdis, m_gc, WhitePixel(m_pdis, screen_num)); 
		} 
	
		/* define the style of lines that will be drawn using this GC. */ 
		XSetLineAttributes(m_pdis, m_gc, 
							line_width, line_style, cap_style, join_style); 
	
		/* define the fill style for the GC. to be 'solid filling'. */ 
		XSetFillStyle(m_pdis, m_gc, FillSolid); 
	}//create gc


	HSL::Window::s_window_count = 1;
	map();
	flush();
}



void HSL::Window::close(){
	if(HSL::Window::s_window_count == 1){
		XDestroyWindow(m_pdis,m_win);
		HSL::Window::s_window_count = 0;
	}

}

void HSL::Window::destory(){
	close();
}
//descructor
HSL::Window::~Window(){
	//printf("in window des....\n");

	if(HSL::Window::s_window_count == 1){
		XDestroyWindow(m_pdis,m_win);
		HSL::Window::s_window_count = 0;
	}


	//printf("in window des,begin free gc....\n");
	XFreeGC(m_pdis,m_gc);
//	printf("in window des,succeed free gc....\n");
	
	m_pdis = NULL;
	g_display_count -- ;

	if (g_display_count == 0){
		XCloseDisplay(g_pdis);
		g_pdis = NULL;
	}


	//printf("out window des....\n");


}

void HSL::Window::draw(const HSL::Image& img,
		int src_x,int src_y,
		int src_width,int src_height,
		int dst_x,int dst_y){

	XPutImage(m_pdis,m_win,
			m_gc,
			img.ximage_ptr(),
			src_x,src_y,
			dst_x,dst_y,
			src_width,src_height);
}



void HSL::Window::draw(const HSL::Image& img){

	XPutImage(m_pdis,m_win,
			m_gc,
			img.ximage_ptr(),
			0,0,
			0,0,
			img.width(),img.height());

}




void HSL::Window::map(){
	XMapWindow(m_pdis,m_win);
	flush();
}



void HSL::Window::flush(){
	XFlush(m_pdis);
}




void HSL::Window::set_event_handler(HSL::EventHandler handler,void* parg){
	m_parg = parg;
	m_event_handler = handler;
}



void HSL::Window::handle_event(){
	m_event_handler(m_event,m_parg);
}




void HSL::Window::sync(bool clean){
	XSync(m_pdis,(int)clean);
}



int HSL::Window::pending(){
	return XPending(m_pdis);
}



void HSL::Window::set_event_mask(long mask){
	XSelectInput(m_pdis,m_win,mask);
}



void HSL::Window::next_event(){
	XNextEvent(m_pdis,&m_event);
}

