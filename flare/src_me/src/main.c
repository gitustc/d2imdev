#include<main.h>

int main(int argc, char** argv){
    d2im_initialize();
    d2im_main_loop();
    d2im_cleanup();

    return 0;
}
