#ifndef window_hpp
#define window_hpp
#include <X11/Xlib.h>
#include <string>
#include <stdexcept>
#include "event.hpp"
#include "timer.hpp"
#include <pthread.h>


namespace HSL{



class Image;
class Window {

	public:
		Window();
		~Window();

		void close();
		void destory();

		



	public:
		void map();
		void flush();
		void draw(const Image&,
				int,int,
				int,int,
				int,int);
		void draw(const Image&);


	public:

		void sync(bool clean = false);
		int  pending();
		void set_event_handler(HSL::EventHandler,void*);
		void handle_event();
		void set_event_mask(long);
		void next_event();


	private:
		static bool m_is_only;
		std::string m_title;
		Display*	m_pdis;
		XID 		m_win;
		GC			m_gc;
		XSetWindowAttributes m_attr;
		int 		m_height;
		int			m_width;
		bool		m_reverse_video;
		void* 		m_parg;
	private:
		HSL::EventHandler 		m_event_handler;
		HSL::Event				m_event;
	private:
	    static	int 			s_window_count;
	
	private:




};
};

#endif
