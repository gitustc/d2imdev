#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H



typedef struct {
    void*                   self;
    event_client            m_event_client;

    dlist                   widgets;

}widget_manager;



#define         widget_manager_instance()   (&g_widget_manager)
int             widget_manager_init( widget_manager*, const char*, ...);
int             widget_manager_clearup();

widget*         widget_manager_delete( widget* );
int             widget_manager_add( Widget* );

#endif
