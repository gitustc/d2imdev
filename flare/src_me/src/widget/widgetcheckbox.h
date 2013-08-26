#ifndef WIDGET_CHECKBOX_H
#define WIDGET_CHECKBOX_H

class Widget;

class WidgetCheckBox : public Widget {
public:
	WidgetCheckBox (const std::string & fname);
	~WidgetCheckBox ();

	void Check ();
	void unCheck ();
	bool checkClick ();
	bool checkClick (int x, int y);
	bool isChecked () const;
	void render (SDL_Surface *target = NULL);

private:
	std::string imgFileName;
	SDL_Surface * cb;
	bool checked;
	bool pressed;
};

#endif

