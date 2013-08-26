
class WidgetTabControl {

private:

	SDL_Surface *activeTabSurface;
	SDL_Surface *inactiveTabSurface;

	std::string *titles;  /** Titles of the tabs. */
	int activeTab;        /** Index of the currently active tab. */
	int tabsAmount;       /** Amount of tabs in the control. */

	SDL_Rect *tabs;       /** Rectangles for each tab title on the tab header. */
	SDL_Rect tabsArea;    /** Area the tab titles are displayed. */
	SDL_Rect contentArea; /** Area where the content of the {@link activeTab active tab} is displayed. */
	Point tabPadding;     /** Padding between tab titles. */
	SDL_Color color_normal;
	SDL_Color color_disabled;

	void loadGraphics();

	void renderTab(int number, SDL_Surface *target);

public:

	WidgetTabControl(int amount);
	WidgetTabControl(const WidgetTabControl &copy); // copy constructor not implemented
	~WidgetTabControl();

	void setTabTitle(int number, const std::string& title);
	void setMainArea(int x, int y, int width, int height);

	int getActiveTab();
	void setActiveTab(int tab) { activeTab = tab; }
	SDL_Rect getContentArea();
	int getTabHeight() { return activeTabSurface->h; }

	void updateHeader();

	void logic();
	void logic(int x, int y);
	void render(SDL_Surface *target = NULL);
};

#endif
