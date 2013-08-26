ALLEGRO_DISPLAY         *glb_display;
ALLEGRO_TIMER           *glb_timer;
ALLEGRO_EVENT_QUEUE     *glb_event_queue;



int system_initilize(){

    srand(time(NULL));
    al_init();
    glb_event_queue=al_create_event_queue();

    al_install_keyboard();
    al_register_event_source(glb_event_queue,al_get_keyboard_event_source());


    al_install_audio();


    glb_timer=al_create_timer(1.0/60);
    al_register_event_source(glb_event_queue,al_get_timer_event_source(glb_timer));
    al_start_timer(glb_timer);

    glb_display=al_create_display(640,480);
    al_set_window_title(glb_display,"D2IM-V0.01");
}



event *system_pull_event(event *p){
    return al_wait_for_event(glb_event_queue, p);
}
