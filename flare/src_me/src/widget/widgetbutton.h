#define  BUTTON_NORMAL          0
#define  BUTTON_PRESSED         1
#define  BUTTON_HOVER           2
#define  BUTTON_DISABLED        3


typedef struct {

    void                    *self;

    widget                  m_widget;
    event_client            m_event_client;


    uint32_t                state;



}widget_button;
class WidgetButton : public Widget {
private:

	std::string fileName; // the path to the buttons background image

	SDL_Surface *buttons;
	Mix_Chunk *click;

	WidgetLabel wlabel;

	SDL_Color color_normal;
	SDL_Color color_disabled;

	TooltipData tip_buf;
	TooltipData tip_new;
	WidgetTooltip *tip;
	TooltipData checkTooltip(Point mouse);

public:
	WidgetButton(const std::string& _fileName);
	~WidgetButton();

	void loadArt();
	bool checkClick();
	bool checkClick(int x, int y);
	void render(SDL_Surface *target = NULL);
	void refresh();

	std::string label;
	std::string tooltip;
	bool enabled;
	bool pressed;
	bool hover;
};

#endif
