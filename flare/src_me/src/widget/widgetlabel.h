#ifndef WIDGET_LABEL_H
#define WIDGET_LABEL_H

#include "widget.h"


#define  VALIGN_CENTER      0
#define  VALIGN_TOP         1
#define  VALIGN_BOTTOM      2


typedef struct {
    int         x;
    int         y;

    int         justify;
    int         valign;
    int         hidden;
    char[128]   font_style;

}label_info;


int         label_info_initilize( label_info*, const char*, ...);
int         label_info_clearup( label_info* );





typedef struct {
    void                    *self;
    widget                  m_widget;


    void                    *refresh;
    void                    *apply_offisets;
    char[256]               text;


}widget_label;



class WidgetLabel : public Widget {
private:

	void refresh();
	void applyOffsets();

	std::string text;
	SDL_Color color;
	int x_origin;
	int y_origin;
	int justify;
	int valign;
	std::string font_style;
	SDL_Surface *text_buffer;

public:
	WidgetLabel();
	~WidgetLabel();
	void render(SDL_Surface *target = NULL);
	void set(int _x, int _y, int _justify, int _valign, const std::string& _text, SDL_Color _color);
	void set(int _x, int _y, int _justify, int _valign, const std::string& _text, SDL_Color _color, std::string _font);
	void setX(int _x);
	void setY(int _y);
	int getX();
	int getY();
	void setJustify(int _justify);

	void set(const std::string& _text);
	std::string get() {return text;}

	SDL_Rect bounds;
};

#endif
