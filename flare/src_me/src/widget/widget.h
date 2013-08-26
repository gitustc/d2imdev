#ifndef WIDGET_H
#define WIDGET_H
typedef struct {
    void                *self;

    widget              *parent;

    event_client        m_event_client;

}widget;



int widget_initialize( widget*, const char*, ... );
int widget_render( Widget* );

#endif

