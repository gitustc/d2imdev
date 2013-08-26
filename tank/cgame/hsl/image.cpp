#include <X11/Xlib.h>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "image.hpp"
#include <stdio.h>
#include <stdlib.h>

extern Display* g_pdis;
extern int		g_display_count;

using namespace HSL;

static int highbit(unsigned long ul){
	int i; unsigned long hb;
	hb = 0x8000; hb = (hb<<16);
	for (i=31; ((ul & hb) == 0) && i>=0; i--, ul<<=1);
	return i;
}


Image::Image(){
	m_pximage = NULL;
}

Image::Image(const char* path){
	open(path);
}


//Image& Image::operator=(const Image& src){
//	if(this == &src)return *this;
//
//
//
//}
bool Image::open(const char* path){


	if (g_display_count == 0){
		g_pdis = XOpenDisplay((char*)getenv("DISPLAY"));
		if ( g_pdis == NULL){
			throw std::runtime_error("fail to connect x server ...\n");
		}
		m_pdis = g_pdis;
		g_display_count = 1;

	}else{
		m_pdis = g_pdis;
		g_display_count ++;
	}


	m_pximage = NULL;
	FILE* fp = NULL;
	char* buffer = NULL;
	

	if( (fp = fopen(path,"rb")) == NULL){
		fprintf(stderr,"fail to open file \" %s \"...\n",path);
		return false;
	}

	BMP_FILE:
	{ 


		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER bmih;

		fread(&bmfh,1,sizeof(bmfh),fp);
		fread(&bmih,1,sizeof(bmih),fp);

		if (bmfh.bfType != 19778){
			fprintf(stderr,"Not a BMP file ...\n");
			goto JPG_FILE;
		}

		if( ( buffer = (char*)malloc(bmih.biSizeImage)) == NULL ){
			fprintf(stderr,"malloc fail ...\n");
			return false;
		}

		fseek(fp,bmfh.bfOffBytes,SEEK_SET);
		fread(buffer,1,bmih.biSizeImage,fp);
		fclose(fp);


		m_pximage = XCreateImage(m_pdis,
				DefaultVisual(m_pdis,DefaultScreen(m_pdis)),
				DefaultDepth(m_pdis,DefaultScreen(m_pdis)),
				ZPixmap,
				0,
				NULL,
				bmih.biWidth,bmih.biHeight,
				32,
				0);

		if (m_pximage == NULL){
			fprintf(stderr,"malloc error ...\n");
			return false;
		}

		m_pximage->data = ( char*)malloc(m_pximage->bytes_per_line*bmih.biHeight);
		if (m_pximage->data == NULL){
			fprintf(stderr,"malloc for Image->data error ...\n");
			return false;
		}


		{ //load data
		  //need to improve for speed
			unsigned long r, g, b, rmask, gmask, bmask, xcol; 
			int rshift, gshift, bshift;
			unsigned long bperpix, bperline, border; 
			char *ip, *pp; 
			rmask = DefaultVisual(m_pdis,DefaultScreen(m_pdis))->red_mask; 
			gmask = DefaultVisual(m_pdis,DefaultScreen(m_pdis))->green_mask; 
			bmask = DefaultVisual(m_pdis,DefaultScreen(m_pdis))->blue_mask; 
			
			rshift = 7 - highbit(rmask); 
			gshift = 7 - highbit(gmask); 
			bshift = 7 - highbit(bmask); 
			
			bperline = m_pximage->bytes_per_line; 
			bperpix = m_pximage->bits_per_pixel; 
			border = m_pximage->byte_order; 
			

			pp = buffer; 
			
			for (long i=0; i<bmih.biHeight; i++) { 
				for (long j=0; j<bmih.biWidth; j++) { 
					b = *pp++; g = *pp++; r = *pp++; 
					//pp++; 
			
					if (rshift<0) r = r << (-rshift); 
					else r = r >> rshift; 
			
					if (gshift<0) g = g << (-gshift); 
					else g = g >> gshift; 
					
					if (bshift<0) b = b << (-bshift); 
					else b = b >> bshift; 
					
					r = r & rmask; 
					g = g & gmask; 
					b = b & bmask; 
					
					xcol = r | g | b; 
					
					
					ip = ( m_pximage->data + (bmih.biHeight-1-i)*bperline + j*bperpix/8); 

					//以后改进为24
					if (bperpix == 32) { 
					//	printf("32\n");
						if (border == MSBFirst) { 
							*ip++ = (xcol>>24) & 0xff; 
							*ip++ = (xcol>>16) & 0xff; 
							*ip++ = (xcol>>8) & 0xff; 
							*ip++ = xcol & 0xff; 
						} else { /* LSBFirst */ 
							*ip++ = xcol & 0xff; 
							*ip++ = (xcol>>8) & 0xff; 
							*ip++ = (xcol>>16) & 0xff; 
							*ip++ = (xcol>>24) & 0xff; 
						} 
					} else if (bperpix == 24) { 
						if (border == MSBFirst) { 
							*ip++ = (xcol>>16) & 0xff; 
							*ip++ = (xcol>>8) & 0xff; 
							*ip++ = xcol & 0xff; 
						} else { /* LSBFirst */ 
							*ip++ = xcol & 0xff; 
							*ip++ = (xcol>>8) & 0xff; 
							*ip++ = (xcol>>16) & 0xff; 
						} 
					} else if (bperpix == 16) { 
						if (border == MSBFirst) { 
							*ip++ = (xcol>>8) & 0xff; 
							*ip++ = xcol & 0xff; 
						} else { /* LSBFirst */ 
							*ip++ = xcol & 0xff; 
							*ip++ = (xcol>>8) & 0xff; 
						} 
					} else if (bperpix == 8) { 
							*ip++ = xcol & 0xff; 
					} else {
						return false;
					}
				}//endfor
			}//endfor
		}//end_load_data
		free (buffer);
	}//BMP_FILE

	JPG_FILE:
	{ 
	}

	return true;
}

int HSL::Image::height()const{
	return m_pximage->height;
}

int HSL::Image::width()const{
	return m_pximage->width;
}
XImage* Image::ximage_ptr()const{
	return m_pximage;
}

HSL::Image::~Image(){
	if ( m_pximage ){
		XDestroyImage(m_pximage);
	}
	g_display_count --;
	if (g_display_count == 0){
		XCloseDisplay(g_pdis);
		g_pdis = NULL;
		m_pdis = NULL;
	}

}

void HSL::Image::draw(HSL::Image& src,
			         int src_row,int src_col,
					 int src_width,int src_height,
					 int dst_row,int dst_col){
	char* ps = src.data_ptr()+(src_row*src.width()+src_col)*4;
	char* pd = this->m_pximage->data+(dst_row*(this->width())+dst_col)*4;

	
	for(unsigned long i=0;i<src_height;i++)
		memcpy(pd+this->width()*i*4,ps+src.width()*i*4,src_width*4);




}

void HSL::Image::draw(HSL::Image& src,
			         int src_row,int src_col,
					 int src_width,int src_height,
					 int dst_row,int dst_col,
					 HSL::Color col){

	uint32_t* ps = (uint32_t*)src.data_ptr()+(src_row*src.width()+src_col);
	uint32_t* pd = (uint32_t*)this->m_pximage->data+(dst_row*(this->width())+dst_col);
	uint32_t mask = col&0x00ffffff;



//	sleep(10);

	
	for(unsigned long i=0;i<src_height;i++){

		pd+=this->width();
		ps+=src.width();

		for(unsigned long j=0;j<src_width;j++){
			if( ((*(ps+j) & 0x00ffffff)) != mask ) *(pd+j) = *(ps+j);

		}
	}

}

			

char* HSL::Image::data_ptr()const{
	return m_pximage->data;
}

HSL::Color HSL::Image::pixel(int row,int col){

	return *( (uint32_t*)this->m_pximage->data+(row*(this->width())+col) );

}

void HSL::Image::set_pixel(HSL::Color co,int row,int col){

	 *( (uint32_t*)this->m_pximage->data+(row*(this->width())+col) ) = co;

}




