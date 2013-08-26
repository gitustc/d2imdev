#include <stdio.h>
#include <stdlib.h>

// Disable warning message "unreferenced inline function has been removed"
#pragma warning( disable : 4514 )

#define USE_CONSOLE

#ifdef WIN32
   #pragma warning (push)
   #pragma warning (disable  : 4312 )
#endif

#include <allegro.h>

#ifdef WIN32
   #pragma warning (pop)
#endif

#include "dcc_decoder.h"


// ==========================================================================
int main(int argc, char **argv)
{
   DCC_S * dcc_ptr;

   if (argc != 2)
   {
      printf("syntaxe : dcc_extract <file.dcc>\n");
      exit(0);
   }

   allegro_init();

   dcc_ptr = dcc_disk_load(argv[1]);
   if (dcc_ptr == NULL)
   {
      printf("Fatal error while loading %s :\n%s", argv[1], dcc_error);
      exit(1);
   }

   if (dcc_decode(dcc_ptr, -1))
   {
      printf("Fatal error while decoding %s :\n%s", argv[1], dcc_error);
      printf("\n");
      dcc_debug(dcc_ptr);
      dcc_destroy(dcc_ptr);
      exit(1);
   }

   dcc_debug(dcc_ptr);
   dcc_destroy(dcc_ptr);
}
END_OF_MAIN();

