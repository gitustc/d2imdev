#include <stdio.h>
#include <stdlib.h>

#define UBYTE unsigned char


void main(int argc, char ** argv)
{
   FILE  * in;
   UBYTE layers, frames_per_dir, directions, c, comp_idx;
   int   total_frames, i, x, y;
   long  dword;
   char  composit[16][3] = {
            {"HD"}, {"TR"}, {"LG"}, {"RA"}, {"LA"}, {"RH"}, {"LH"}, {"SH"},
            {"S1"}, {"S2"}, {"S3"}, {"S4"}, {"S5"}, {"S6"}, {"S7"}, {"S8"}
         };

   if (argc != 2)
   {
      printf("syntaxe : cofinfo <file.cof>\n");
      exit(0);
   }
   in = fopen(argv[1], "rb");
   if (in == NULL)
   {
      printf("can't open %s\n", argv[1]);
      exit(0);
   }

   // read it
   layers         = fgetc(in);
   frames_per_dir = fgetc(in);
   directions     = fgetc(in);

   printf("==============\n%s\n==============\n", argv[1]);
   printf("# layers         = %u\n", layers);
   printf("# frames_per_dir = %u\n", frames_per_dir);
   printf("# directions     = %u\n", directions);

   printf("\ntotal frames     = %i\n", directions * frames_per_dir);

   // unknown
   printf("\nunknown :\n   ");
   for (i=0; i<5; i++)
   {
      c = fgetc(in);
      printf(" %02X", c);
   }
   printf("\n");

   for (i=0; i<4; i++)
   {
      printf("   ");
      for (x=0; x<4; x++)
      {
         c = fgetc(in);
         printf(" %02X", c);
      }
      fseek(in, -4, SEEK_CUR);
      fread(& dword, 4, 1, in);
      printf ("     (%5li if signed, else 1st byte = %3u)\n", dword, dword & 0xFF);
   }
   printf("   ");
   for (i=0; i<4; i++)
   {
      c = fgetc(in);
      printf(" %02X", c);
   }
   printf("\n\n");
   
   
   // 9 bytes strings
   for (i=0; i<layers; i++)
   {
      printf("layer %2i data : ", i);
      for (x=0; x<9; x++)
      {
         c = fgetc(in);
         printf(" %02X", c);
         if (x == 0)
         comp_idx = c;
      }

      printf("  (");
      fseek(in, -9, SEEK_CUR);
      for (x=0; x<9; x++)
      {
         c = fgetc(in);
         printf("%c", c > 32 ? c : 32);
      }
      printf(")  --> %s\n", composit[comp_idx]);
   }

   // unknown frames_per_dir data
   printf("\nframes per direction unknown data (frame that hurt ?) :\n ");
   for (i=0; i<frames_per_dir; i++)
   {
      c = fgetc(in);
      printf(" %02X", c);
   }
   printf("\n\n");
   
   // layers priority
   printf("\nlayers priority for all frames of all directions :\n", i);
   for (i=0; i<directions; i++)
   {
      printf("\ndirection %i\n", i);
      for (x=0; x<frames_per_dir; x++)
      {
         printf("   frame %3i : ", x);
         for (y=0; y<layers; y++)
         {
            c = fgetc(in);
            printf(" %02X", c);
         }

         printf(" =");
         fseek(in, -layers, SEEK_CUR);
         for (y=0; y<layers; y++)
         {
            c = fgetc(in);
            printf(" %s", composit[c]);
         }
         printf("\n");
      }
   }

   // end
   printf("\n");
   fclose(in);
}
