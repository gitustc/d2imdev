

class WidgetListBox : public Widget {
private:

	std::string fileName; // the path to the ListBoxs background image

	SDL_Surface *listboxs;
	Mix_Chunk *click;

	int list_amount;
	int list_height;
	int cursor;
	bool has_scroll_bar;
	int non_empty_slots;
	bool any_selected;
	std::string *values;
	std::string *tooltips;
	WidgetLabel *vlabels;
	SDL_Rect *rows;
	WidgetTooltip *tip;
	WidgetScrollBar *scrollbar;
	SDL_Color color_normal;
	SDL_Color color_disabled;

public:
	WidgetListBox(int amount, int height, const std::string& _fileName);
	~WidgetListBox();

	void loadArt();
	bool checkClick();
	bool checkClick(int x, int y);
	TooltipData checkTooltip(Point mouse);
	void append(std::string value, std::string tooltip);
	void set(int index, std::string value, std::string tooltip);
	void remove(int index);
	void clear();
	void shiftUp();
	void shiftDown();
	int getSelected();
	std::string getValue();
	std::string getValue(int index);
	std::string getTooltip(int index);
	int getSize();
	void scrollUp();
	void scrollDown();
	void render(SDL_Surface *target = NULL);
	void refresh();

	SDL_Rect pos_scroll;
	bool pressed;
	bool *selected;
	bool multi_select;
	bool can_deselect;
	bool can_select;
	int scrollbar_offset;
};

#endif
