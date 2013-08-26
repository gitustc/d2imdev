/*global d2im struct*/





int d2im_initilize(){
    system_initilize();
}

int d2im_main_loop(){

    event   pe;
    while(d2im_state()->state!=QUIT){
        event_source_pull(&pe);
        d2im_process(&pe);
    }
    return 0;

}


static int d2im_process(event *pe){
    if(d2im_widget_manager_process(pe)){
        d2im_state_logic_process(pe);
    }
    return 0;
}

static int d2im_widget_manager_process(event *pe){
    widget_manager_process(&(d2im_widget_manager()),pe);
    return 0
}

static int d2im_state_logic_process(event *pe){
    switch(d2im_state()){
        case D2IM_STATE_RUNNING:
            d2im_state_running_process(pe);
        default:

    }
    return 0;
}
static int d2im_state_running_process(event *pe){
    switch(pe->type){
        case QUIT:
            d2im_set_state(D2IM_STATE_QUIT);
        default:

}
