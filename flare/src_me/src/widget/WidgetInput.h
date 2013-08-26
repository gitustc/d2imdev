/**
 * class WidgetInput
 *
 * A simple text box with a label above it.
 * It has two images - one for focused and one for unfocused.
 */


#ifndef WIDGETINPUT_H
#define WIDGETINPUT_H

#include "Utils.h"
#include "Widget.h"
#include <SDL.h>
#include <string>

class WidgetInput : public Widget {

protected:

	void loadGraphics(const std::string& filename);

	SDL_Surface *background;

	bool enabled;
	bool pressed;
	bool hover;

	std::string text; // the text that has been typed into the box
	unsigned int max_characters;
	int cursor_frame;

	Point font_pos;

	SDL_Color color_normal;

public:
	WidgetInput();
	~WidgetInput();

	void logic();
	bool logic(int x, int y);
	void render(SDL_Surface *target = NULL);
	bool checkClick();
	std::string getText() { return text; }
	void setText(std::string _text) { text = _text; }
	void setPosition(int x, int y);

	bool inFocus;
};

#endif
