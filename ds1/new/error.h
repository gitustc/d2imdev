#ifndef _ERROR_H_

#define _ERROR_H_


#define FATAL_EXIT(...)      do{fprintf( stderr, __VA_ARGS__ ); fflush(stderr); exit(255); }while(0)
#define DEBUG_MESSAGE(...)   do{fprintf( stderr, __VA_ARGS__ ); fflush(stderr);            }while(0)
#define WARNING_MESSAGE(...) do{fprintf( stderr, __VA_ARGS__ ); fflush(stderr);            }while(0)

#endif
