#ifndef font_h
#define font_h

#include <string>
#include "image.hpp"

namespace HSL{
class Font :public HSL::Image{
	private:

	public:
		Font();
		~Font();
		int put_char(HSL::Image&,char,int,int);
		void put_str(HSL::Image&,const std::string&,int,int);
		

};




};
#endif

