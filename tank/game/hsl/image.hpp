#ifndef image_h
#define image_h
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>


//copy from msdn
typedef unsigned short int WORD; 
typedef unsigned int DWORD; 
typedef int LONG; 

struct tagRGBQUAD { 
unsigned char rgbBlue; 
unsigned char rgbGreen; 
unsigned char rgbRed; 
unsigned char rgbReserved; 
}__attribute__((packed)); 

typedef struct tagRGBQUAD RGBQUAD; 

struct tagBITMAPFILEHEADER { 
WORD bfType; 
DWORD bfSize; 
WORD bfReserved1; 
WORD bfReserved2; 
DWORD bfOffBytes; 
}__attribute__((packed)); 
typedef struct tagBITMAPFILEHEADER BITMAPFILEHEADER; 

struct tagBITMAPINFOHEADER{ 
DWORD biSize; 
LONG biWidth; 
LONG biHeight; 
WORD biPlanes; 
WORD biBitCount; 
DWORD biCompression; 
DWORD biSizeImage; 
LONG biXPelsPerMeter; 
LONG biYPelsPerMeter; 
DWORD biClrUsed; 
DWORD biClrImportant; 
}__attribute__((packed)); 




//struct _Color{
//	char b,g,r;
//	char unused;
//};

typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER; 



#define RGB_R(col) (((uint32_t)(col)&0x00ff0000)>>16)
#define RGB_G(col) (((uint32_t)(col)&0x0000ff00)>>8)
#define RGB_B(col) ( (uint32_t)(col)&0x000000ff)
#define RGB(r,g,b) (((uint32_t)(r)<<16) + ((uint32_t)(g)<<8) + ((uint32_t)(b)))

namespace HSL{

	

typedef uint32_t   Color;


class Image {
	public:
		Image();
//		Image(const std::string&);
		Image(const char*);
//		Image(const Image&);
		virtual ~Image();

//		bool open(const std::string&);
		bool open(const char*);
		bool close();
		void draw(HSL::Image&,  //src
				  int,int,     //src_row,src_col
				  int,int,		//src_width,src_height
				  int,int		//dst_row,dst_col
				  );

		void draw(HSL::Image&,  //src
				  int,int,     //src_row,src_col
				  int,int,		//src_width,src_height
				  int,int,		//dst_row,dst_col
				  HSL::Color    //mask
				  );

	public:
//		Image& operator=(const Image&);
//		Image& operator=(const char*);

	public:
		int height()const;
		int width()const;
		int depth();
		XImage* ximage_ptr()const;
		char* data_ptr()const;
		HSL::Color pixel(int,int);
		void set_pixel(HSL::Color,int,int);
	private:
		XImage* m_pximage;
		Display* m_pdis;


};






};//namespace HSL
#endif
