// Emacs style mode select -*- C++ -*-
// ----------------------------------------------------------------------------
//
// Copyright(C) 2009 Stephen McGranahan
//
// This file is part of Gooey a4
//
// Gooey a4 is free software: you can redistribute it and/or modify
// it under the terms of the GNU Limited General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// ----------------------------------------------------------------------------
//
// Gooey a4 is a C/SDL-based Graphic User Interface (GUI) library which
// aims to provide a viable cross-platform user interface API which is simple,
// elegant, and enables rapid application development.
//
// ----------------------------------------------------------------------------

#ifndef GFOUNDATION_H
#define GFOUNDATION_H


// ----------------------------------------------------------------------------
// Helper functions

// gMouseOnRect
// returns true if a point is within a gScreenRect
bool gMouseOnRect(int x, int y, gScreenRect *sr);


// gClipScreenRect
// Clips r1 to be within r2
void gClipScreenRect(gScreenRect *r1, const gScreenRect *r2);



// ----------------------------------------------------------------------------
// gWidget
typedef enum
{
   // Ah! The Illusive Focus. This can only be assigned by a parent widget.
   // This is mouse focus. When a widget is clicked on, that widget should
   // continue to get first access to mouse events until the mouse lets up.
   stateMouseFocus = 0x1,

   // Has focus (input focus)
   stateHasFocus = 0x2,

   // Mouse is currently over the screen rect
   stateMouseOver = 0x4, 

   // Mouse is currently clicked on the object
   stateLeftDown = 0x8,

   // Mouse right button is currently clicked on the object
   stateRightDown = 0x10,

   // Mouse middle button is currently clicked on the object
   stateMiddleDown = 0x20,
} gWidgetState_e;


#define stateAnyDown (stateLeftDown|stateRightDown|stateMiddleDown)


typedef enum
{
   // Widget-level flags
   // Widget is rendered from the right-hand side of the parent rect
   behaviorFromRight = 0x1,
   // Widget is rendered from the bottom of the parent rect
   behaviorFromBottom = 0x2,
   // Widget resizes width-wise with the parent
   behaviorExpandW = 0x4,
   // Widget resizes height-wise with the parent
   behaviorExpandH = 0x8,
   // Widget updates the screen when the cursor enters or leaves the widget
   behaviorUpdateMouseOver = 0x10,
   // Widget updates the screen when the mouse clicks on the widget
   behaviorUpdateMouseOn = 0x20,
   // Widget updates when focus changes
   behaviorUpdateFocus = 0x40,
   // Widget should keep focus if the user clicks on the container but not
   // another widget. (eg. text box keeps input focus when a window is dragged)
   behaviorKeepFocus = 0x80,
   // Widget is not clipped to parent's rect
   behaviorNoClip = 0x100,
   // Widget is invisible (does not render)
   behaviorInvisible = 0x200,
   // Widget is dormant (does not respond to events)
   behaviorDisabled = 0x400,
} gWidgetBehavior_e;

#define behaviorNoEvent (behaviorInvisible|behaviorDisabled)


typedef void (*gDrawFunc)(gWidget *self);
typedef void (*gEventHandlerFunc)(gWidget *self, SDL_Event *event, bool *claimed);
typedef void (*gSignalHandlerFunc)(gWidget *self, gSignal *sig);
typedef void (*gSetRectFunc)(gWidget *self, int x, int y, unsigned int w, unsigned int h);
typedef void (*gUpdateSizeFunc)(gWidget *self);
typedef void (*gCalcScreenFunc)(gWidget *self);
typedef void (*gSetParentRectFunc)(gWidget *self, gScreenRect *prect);
typedef void (*gSetFactoryFunc)(gWidget *self, gFactory *factory);
typedef void (*gSetInFocusFunc)(gWidget *self, bool infocus);
typedef void (*gUpdateFunc)(gWidget *self);
typedef void (*gDeleteFunc)(gWidget *self);
typedef bool (*gIDFuncParam)();
typedef bool (*gIDFunc)(gIDFuncParam);
typedef gWidget *(*gTopFunction)(gWidget *self, int x, int y);
typedef gSkinCursor *(*gCursorFunc)(gWidget *self, int x, int y);
typedef void (*gSetBehaviorFunc)(gWidget *self, int behavior);




#define gWidgetBase  \
   gIDFunc     id;\
   gRect       rect;\
   gScreenRect screenrect;\
   int         fromx2, fromy2;\
   gScreenRect visiblerect;\
   int         state;\
   int         behavior;\
   gDrawFunc            drawWidget;\
   gEventHandlerFunc    eventHandler;\
   gSignalHandlerFunc   signalHandler;\
   gSetRectFunc         setRect;\
   gUpdateSizeFunc      updateSize;\
   gCalcScreenFunc      calcScreen;\
   gSetFactoryFunc      setFactory;\
   gSetParentRectFunc   setParentRect;\
   gSetBehaviorFunc     setBehavior;\
   gDeleteFunc          deleteWidget;\
   gTopFunction         getTopWidget;\
   gCursorFunc          getCursor;\
   gSetInFocusFunc      setInFocus;\
   gUpdateFunc          update;\
   gContainer     *parent;\
   gScreenRect    *prect;\
   gFactory       *factory;\
   gList          *responders;\
   gWidget        *next;\
   gWidget        *prev;\



struct gWidget
{
   gWidgetBase
/*   // ID function used to identify widgets
   gIDFunc     id;

   // Location of the widget.
   gRect       rect;
   gScreenRect screenrect;
   int         fromx2, fromy2;

   // Total screen area visible (with parental clipping)
   gScreenRect visiblerect;

   // State flags
   int         state;
   // Behavior flags
   int         behavior;

   // Widget functions (can be replaced)
   gDrawFunc            drawWidget;
   gEventHandlerFunc    eventHandler;
   gSetRectFunc         setRect;
   gUpdateSizeFunc      updateSize;
   gCalcScreenFunc      calcScreen;
   gSetFactoryFunc      setFactory;
   gSetParentRectFunc   setParentRect;
   gDeleteFunc          deleteWidget;
   gTopFunction         getTopWidget;
   gCursorFunc          getCursor;
   gSetInFocusFunc      setInFocus;
   gUpdateFunc          update;

   // Parent?
   gContainer    *parent;
   gScreenRect   *prect;

   // Factory
   gFactory      *factory;

   // Signal responder list. I decided to do a linear list because that allows 
   // for multiple responders for the same event.
   gList         *responders;

   // Links
   gWidget *next;
   gWidget *prev;
   */
};



// gWidgetID
// Identity function for a basic unextended gWidget object
bool gWidgetID(gIDFuncParam);

// gWidgetDraw
// NOP
void gWidgetDraw(gWidget *self);

// gWidgetEventHandler
// Handles several common events (mouse motion events setting
// the gMouseOver flag, ect)
void gWidgetEventHandler(gWidget *self, SDL_Event *event, bool *claimed);


// gWidgetSignalHandler
// Handles and responds to signals (see: gevent.h)
void gWidgetSignalHandler(gWidget *self, gSignal *sig);

// gWidgetSetRect
// Changes the size and position of the widget and calls the calcScreen
// function.
void gWidgetSetRect(gWidget *self, int x, int y, unsigned int w, unsigned int h);

// gWidgetUpdateSize
// Called by the parent when the parent changes size. This function checks
// flags such as behaviorFromRight and behaviorFromBottom and adjusts the 
// location of the widget accordingly.
void gWidgetUpdateSize(gWidget *self);

// gWidgetCalcScreen
// Calculates the screen coordinates of the widget based on the widget rect,
// parent rect, and behavior flags (such as right or bottom justified)
void gWidgetCalcScreen(gWidget *self);

// gWidgetSetSkin
// Sets the skin for the widget.
void gWidgetSetFactory(gWidget *self, gFactory *f);

// gNewWidget
// Creates and initializes a new gWidget object.
gWidget *gNewWidget(void);

// gWidgetGetTop
// if the mouse cursor is on top of the widget, returns itself, otherwise
// returns null
gWidget *gWidgetGetTopWidget(gWidget *self, int mousex, int mousey);

// gWidgetGetCursor
// returns null
gSkinCursor *gWidgetGetCursor(gWidget *self, int mousex, int mousey);

// gWidgetSetInFocus
// sets the flag, if flagged to do so, calls the update function
void gWidgetSetInFocus(gWidget *self, bool infocus);

// gWidgetUpdate
// Calls the factory to update the screen with a widget's mouse rect
void gWidgetUpdate(gWidget *self);

// gInitWidget
// Initializes a new gWidget object.
void gInitWidget(gWidget *w);

// gWidgetDelete
// Deletes and unlinks a widget.
void gWidgetDelete(gWidget *self);



// gAddResponder
// This is not a member function because there is no concievable reason
// why this would ever need to be overloaded. Adds a responder for the given
// type to the given widget.
void gAddResponder(gWidget *self, char *type, signalFunc func);

// gRemoveResponder
// This is not a member function because there is no concievable reason
// why this would ever need to be overloaded. Removes all occurrences of
// func from the given widget.
void gRemoveResponder(gWidget *self, signalFunc func);



// ----------------------------------------------------------------------------
// gContainer
// Provides all basic functionality required to hold objects within a single 
// object. This is said to "extend" the gWidget object "class". The object can
// be cast directly to a gWidget object and visa versa, although the id function
// should always be called first before casting from gWidget to gContainer.

typedef void (*gContainerAddChildFunc)(gContainer *self, gWidget *child);
typedef void (*gContainerRemoveChildFunc)(gContainer *self, gWidget *child);
typedef void (*gContainerDeleteChildfunc)(gContainer *self, gWidget *child);
typedef void (*gContainerPromoteChildFunc)(gContainer *self, gWidget *child);
typedef void (*gContainerImplantWidgetFunc)(gContainer *self, gWidget *widget);


#define gContainerBase \
   gWidgetBase\
   gWidget *head;\
   unsigned int topbuffer, bottombuffer, leftbuffer, rightbuffer;\
   gScreenRect childrect;\
   bool    sortchildren;\
   gWidget *embhead;\
   gWidget *mfocus;\
   gWidget *kfocus;\
   gContainerAddChildFunc      addChild;\
   gContainerRemoveChildFunc   removeChild;\
   gContainerDeleteChildfunc   deleteChild;\
   gContainerPromoteChildFunc  promoteChild;\
   gContainerImplantWidgetFunc implantWidget;\

struct gContainer
{
   gContainerBase
   /*
   // The main child head. This ring of widgets is contained within the child
   // rect on the screen and events are funneled to them by the container.
   gWidget *head;

   // Buffers for the child rect
   unsigned int topbuffer, bottombuffer, leftbuffer, rightbuffer;
   gScreenRect childrect;

   // If true, the container will re-arrange the children order when
   // mouse-click events occur.
   bool    sortchildren;


   // Embedded widgets are used by specific subclasses of container. They are
   // not added or removed like normal widgets but the contianer itself
   // decides when they should be used. This is for things like the titlebar
   // buttons for a window. These widgets are not constrained to the childrect
   // and events are funneled in much the same way as the main child ring.
   gWidget *embhead;


   gWidget *focus;


   gContainerAddChildFunc      addChild;
   gContainerRemoveChildFunc   removeChild;
   gContainerDeleteChildfunc   deleteChild;
   gContainerPromoteChildFunc  promoteChild;
   gContainerImplantWidgetFunc implantWidget;*/
};



// -----------------------------------------------
// Overloads of gWidget functions

bool gIsContainer(gWidget *widget);

// gContainerID
// Container type identity function
bool gContainerID(gIDFuncParam);

// gContainerDraw
// This function will set up clipping for child widgets and call the draw
// functions for them. It is useful as a template function for container sub
// classes.
void gContainerDraw(gWidget *self);

// gContainerEventHandler
// Basic routing for child widgets,
void gContainerEventHandler(gWidget *self, SDL_Event *event, bool *claimed);

// gContainerCalcScreen
// This will re-calculate the screen coordinates for the container
// and call the calcScreen functions of all children.
void gContainerCalcScreen(gWidget *self);

// gWidgetUpdateSize
// Called by the parent when it changes size.
void gContainerUpdateSize(gWidget *self);

// gContainerSetRect
// Called to set the size of the widget. This variation calls the updatesize
// function of all child and imbedded widgets.
void gContainerSetRect(gWidget *self, int x, int y, unsigned int w, unsigned int h);

// gContainerSetFactory
// Sets the factory for the container and calls setFactory methods of all child
// widgets.
void gContainerSetFactory(gWidget *self, gFactory *f);

// gContainerSetInFocus
// same as gWidgetSetInFocus but also clears or sets the current in-focus child 
// widget.
void gContainerSetInFocus(gWidget *self, bool infocus);

// gContainerUpdate
// Calls the factory to update the screen with a container's visible rect and also
// calls the update functions of all child widgets.
void gContainerUpdate(gWidget *self);

// gInitContainer
// Initializes a new gContainer widget.
void gInitContainer(gContainer *c);

// gNewContainer
// Creates and initializes a new gContainer widget.
gContainer *gNewContainer(void);

// gContainerDelete
// Deletes the instance of the container object, freeing all child widgets.
void gContainerDelete(gWidget *self);

// gContainerGetTopWidget
// calls the getTopWidget method of all child widgets. If one returns not null, 
// it returns the value, otherwise it returns gWidgetGetTop(self, mousex, mousey).
gWidget *gContainerGetTopWidget(gWidget *self, int mousex, int mousey);



// -----------------------------------------------
// gContainer-specific functions


void gContainerAddChild(gContainer *self, gWidget *child);

void gContainerRemoveChild(gContainer *self, gWidget *child);

void gContainerDeleteChild(gContainer *self, gWidget *child);

void gContainerPromoteChild(gContainer *self, gWidget *child);


// gContainerImplantWidget
// Adds an embedded widget to a container
void gContainerImplantWidget(gContainer *self, gWidget *w);

#endif

