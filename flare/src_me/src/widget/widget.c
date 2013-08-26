#include "widget.h"

int widget_initialize( WIDGET *p ){
    p->self.clearup = widget_clearup;
    return 0;
}
















static int widget_x(widget *p){
    widget *parent;
    parent=(p->self).parent;
    if(parent){
        return (parent->widget_x(parent))+(p->x);
    }else{
        return p->x;
    }
}
