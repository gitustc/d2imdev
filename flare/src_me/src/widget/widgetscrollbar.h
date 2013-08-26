#include <SDL_mixer.h>

#include <string>


class WidgetScrollBar : public Widget {
private:

	std::string fileName; // the path to the ScrollBars background image

	SDL_Surface *scrollbars;
	Mix_Chunk *click;

	int value;
	int bar_height;
	int maximum;

public:
	WidgetScrollBar(const std::string& _fileName);
	~WidgetScrollBar();

	void loadArt();
	int checkClick();
	int checkClick(int x, int y);
	void set();
	int getValue();
	void render(SDL_Surface *target = NULL);
	void refresh(int x, int y, int h, int val, int max);

	SDL_Rect pos_up;
	SDL_Rect pos_down;
	SDL_Rect pos_knob;
	bool pressed_up;
	bool pressed_down;
	bool pressed_knob;
};

#endif
