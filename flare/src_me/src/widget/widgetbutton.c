
int button_initialize(widget_button *pb, const char* configure, ...){


    pb->process=process;
}

static event *process(widget* p, event *p){
    p=p->self;
    switch(pe.type){
        case EVENT_MOUSE_PRESS:
        case EVENT_RELEASE:
    }
}

static int *redraw(widget* p){
    p=p->self;
    switch(p->state){
        case WIDGET_BUTTON_PRESSED:
        case WIDGET_BUTTON_NORMAL:
        case WIDGET_BUTTON_HOVER:
        case WIDGET_BUTTON_DISABLED:
    }

}

static event *widget_button_process_event( event *p ){

}

int widget_button_initialize( widget_button *p ){

    p->event_client.




}




void WidgetButton::loadArt() {

	// load button images
	SDL_Surface *cleanup = IMG_Load(fileName.c_str());

	if(!cleanup) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
		exit(1); // or abort ??
	}

	// optimize
	buttons = SDL_DisplayFormatAlpha(cleanup);
	SDL_FreeSurface(cleanup);
}

bool WidgetButton::checkClick() {
	if (checkClick(inpt->mouse.x,inpt->mouse.y))
		return true;
	else
		return false;
}

/**
 * Sets and releases the "pressed" visual state of the button
 * If press and release, activate (return true)
 */
bool WidgetButton::checkClick(int x, int y) {
	Point mouse(x,y);

	// Change the hover state
	if (isWithin(pos, mouse)) {
		hover = true;
	} else {
		hover = false;
	}

	// Check the tooltip
	tip_new = checkTooltip(mouse);

	// disabled buttons can't be clicked;
	if (!enabled) return false;

	// main button already in use, new click not allowed
	if (inpt->lock[MAIN1]) return false;

	// main click released, so the button state goes back to unpressed
	if (pressed && !inpt->lock[MAIN1]) {
		pressed = false;

		if (isWithin(pos, mouse)) {

			// activate upon release
			return true;
		}
	}

	pressed = false;

	// detect new click
	if (inpt->pressing[MAIN1]) {
		if (isWithin(pos, mouse)) {

			inpt->lock[MAIN1] = true;
			pressed = true;

		}
	}
	return false;

}

void WidgetButton::render(SDL_Surface *target) {
	if (target == NULL) {
		target = screen;
	}
	SDL_Rect src;
	src.x = 0;
	src.w = pos.w;
	src.h = pos.h;

	// the "button" surface contains button variations.
	// choose which variation to display.
	if (!enabled)
		src.y = BUTTON_GFX_DISABLED * pos.h;
	else if (pressed)
		src.y = BUTTON_GFX_PRESSED * pos.h;
	else if (hover)
		src.y = BUTTON_GFX_HOVER * pos.h;
	else
		src.y = BUTTON_GFX_NORMAL * pos.h;

	// create a temporary rect so we don't modify pos
	SDL_Rect offset = pos;

	if (render_to_alpha)
		SDL_gfxBlitRGBA(buttons, &src, target, &offset);
	else
		SDL_BlitSurface(buttons, &src, target, &offset);

	wlabel.render(target);

	// render the tooltip
	// TODO move this to menu rendering
	if (!tip_new.isEmpty()) {
		if (!tip_new.compare(&tip_buf)) {
			tip_buf.clear();
			tip_buf = tip_new;
		}
		tip->render(tip_buf, inpt->mouse, STYLE_FLOAT, target);
	}
}

/**
 * Create the text buffer
 */
void WidgetButton::refresh() {
	if (label != "") {

		int font_x = pos.x + (pos.w/2);
		int font_y = pos.y + (pos.h/2);

		if (enabled)
			wlabel.set(font_x, font_y, JUSTIFY_CENTER, VALIGN_CENTER, label, color_normal);
		else
			wlabel.set(font_x, font_y, JUSTIFY_CENTER, VALIGN_CENTER, label, color_disabled);
	}
}

/**
 * If mousing-over an item with a tooltip, return that tooltip data.
 *
 * @param mouse The x,y screen coordinates of the mouse cursor
 */
TooltipData WidgetButton::checkTooltip(Point mouse) {
	TooltipData _tip;

	if (isWithin(pos, mouse) && tooltip != "") {
		_tip.addText(tooltip);
	}

	return _tip;
}

WidgetButton::~WidgetButton() {
	SDL_FreeSurface(buttons);
	tip_buf.clear();
	delete tip;
}
