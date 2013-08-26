
widget_manager_remove(widget_manager pwm, widget *pw){
    blist_node  *current;
    blist       *pb;

    pb=&(pwm->widget_list);
    current=blist_head(pb);
    while(current){
        if(current==pw){
            blist_remove(pb, pw);
            return pw;
        }else{
            current=blist_next(pb, current);
        }
    }
    return NULL;
}

widget_manager_add(widget_manager *pwm, widget *pw){
    pw->active=0;
    pwm->widget_list.push(pw);
}

widget_manager_process(widget_manager *pwm, event *pe){

    blist_node  *current;
    blist       *pb;

    pb=&(pwm->widget_list);
    current=blist_head(pb);
    while(current){
        if(current->process(current, pe)){
            current=blist_next(pb, current);
        }else{
            return NULL;
        }
    }
    return pe;
}
