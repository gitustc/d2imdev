#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMP_MAX 16


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

#include "dccinfo.h"


char    cofname[8], token[3], animtype[3];
char    * dat_dir = "datas\\",
        * tok_dir = "tokens\\";
UBYTE   * cofmem = NULL, * coftbl = NULL;
UBYTE   act1_pal[256][3];
UBYTE   palshift[8][256];
UBYTE   trans_r=0xAA, trans_g=0xAA, trans_b=0xAA;
PALETTE final_pal;
int     cof_lay, cof_fpd, cof_dir;
char    comp_str[COMP_MAX][3] = {
           {"HD"}, {"TR"}, {"LG"}, {"RA"}, {"LA"}, {"RH"}, {"LH"}, {"SH"},
           {"S1"}, {"S2"}, {"S3"}, {"S4"}, {"S5"}, {"S6"}, {"S7"}, {"S8"}};
struct
{
   int   active;  // in ini
   int   present; // in cof
   char  name[4];
   char  wclass[4];
   char  filepath[30];
   UBYTE colormap[256]; // colormap to applied to this layer
   int   palette;
   char  cmapname[256];
} cof_comp[COMP_MAX];

DCC_S * dcc[COMP_MAX];


// ==========================================================================
void my_credits(void)
{
   printf("MERGE_DCC Freeware v. 1.8, by Paul SIRAMY, 24 May 2007\n"
          "DCC Decoder made with the precious help of Bilian BELCHEV\n"
          "=========================================================\n");
}


// ==========================================================================
int is_good_char(char c)
{
   if ((c >= 33) && (c <= 127))
      return TRUE;
   return FALSE;
}


// ==========================================================================
void load_cmap(int comp_idx, char * line)
{
   char name[256], nb[6] = "";
   int  i, len, done, pal;
   FILE * in;


   sprintf(name, "%s%s", dat_dir, line);
   len = (int) strlen(name);
   done = FALSE;
   for (i = len-1; (i >= 0) && (! done); i--)
   {
      if (name[i] == ':')
      {
         name[i] = 0;
         strcpy(nb, & name[i + 1]);
         done = TRUE;
      }
      else if (name[i] == '\\')
         done = TRUE;
   }
   pal = atoi(nb);
   in = fopen(name, "rb");
   if (in == NULL)
   {
      fprintf(stderr, "couldn't open %s. The process will continue without it\n", name);
      return;
   }
   if (fseek(in, 256 * pal, SEEK_SET))
   {
      fclose(in);
      return;
   }
   if (fread(cof_comp[comp_idx].colormap, 1, 256, in) != 256)
   {
      for (i=0; i<256; i++)
         cof_comp[comp_idx].colormap[i] = (UBYTE) i;
   }
   else
   {
      strcpy(cof_comp[comp_idx].cmapname, name);
      cof_comp[comp_idx].palette = pal;
   }
   cof_comp[comp_idx].colormap[0] = 0;
   fclose(in);
}


// ==========================================================================
int ini_load(FILE * in, char * name, DCC_BOX_S * anim_box, int * have_box,
             int * image_type)
{
  char line[300], tmp[80];
  int  s, i, found, c, pal;
  long size;
  FILE * cf, * ps;

  // init
  * have_box = FALSE;
  * image_type = 0;
  
  // 'format' line
  fgets(line, sizeof(line), in);
  s = (int) strlen(line);
  for (i=0; i<s; i++)
  {
     if ((line[i] == 0x0A) || (line[i] == 0x0D))
        line[i] = 0;
  }
  strupr(line);

  if ((line[0] == 'F') && (line[1] == 'O') && (line[2] == 'R') &&
      (line[3] == 'M') && (line[4] == 'A') && (line[5] == 'T') &&
      (line[6] == '='))
  {
     if ((line[7] == 'P') && (line[8] == 'C') && (line[9] == 'X'))
        * image_type = 0;
     else if ((line[7] == 'B') && (line[8] == 'M') && (line[9] == 'P'))
        * image_type = 1;
     else
     {
        fprintf(stderr, "format must be either BMP or PCX\n"
                        "Line : %s", line);
        return 1;
     }
  }
  else
  {
     fprintf(stderr, "1st line of file %s must start by "
                     "'FORMAT='\nLine : %s", name, line);
     return 1;
  }

  // 'cof' line
  fgets(line, sizeof(line), in);
  s = (int) strlen(line);
  for (i=0; i<s; i++)
  {
     if ((line[i] == 0x0A) || (line[i] == 0x0D))
        line[i] = 0;
  }
  strupr(line);

  if ((line[0] == 'C') && (line[1] == 'O') && (line[2] == 'F') &&
      (line[3] == '='))
  {
     for (i=0; i<7; i++)
        cofname[i] = line[i+4];
     cofname[7] = 0;
     if (strlen(line) != 11)
     {
        fprintf(stderr, "COF name must be 7 characters (currently %i)\n"
                        "Line : %s", strlen(line) - 4, line);
        return 1;
     }
     token[0] = cofname[0];
     token[1] = cofname[1];
     token[2] = 0;
     animtype[0] = cofname[2];
     animtype[1] = cofname[3];
     animtype[2] = 0;
     sprintf(tmp, "%s%s\\COF\\%s.COF", tok_dir, token, cofname);
     cf = fopen(tmp, "rb");
     if (cf == NULL)
     {
        fprintf(stderr, "can't open %s", tmp);
        return 1;
     }
     fseek(cf, 0, SEEK_END);
     size = ftell(cf);
     fseek(cf, 0, SEEK_SET);
     cofmem = (UBYTE *) malloc(size);
     if (cofmem == NULL)
     {
        fprintf(stderr, "can't allocate %li bytes for %s", size, tmp);
        fclose(cf);
        return 1;
     }
     fread(cofmem, size, 1, cf);
     fclose(cf);
  }
  else
  {
     fprintf(stderr, "1st line of file %s must start by the "
                     "4 characters 'COF='\nLine : %s", name, line);
     return 1;
  }
  cof_lay = cofmem[0];
  cof_fpd = cofmem[1];
  cof_dir = cofmem[2];
  coftbl  = cofmem + 28;

  // palshift.dat
  sprintf(tmp, "%s%s\\COF\\palshift.dat", tok_dir, token);
  ps = fopen(tmp, "rb");
  if (ps)
  {
     fread(palshift, 8, 256, ps);
     fclose(ps);
  }
  else
  {
     // no colormap effect
     for (s=0; s<8; s++)
        for (i=0; i<256; i++)
           palshift[s][i] = (UBYTE) i;
  }

  // palette
  sprintf(tmp, "%sact1.dat", dat_dir);
  ps = fopen(tmp, "rb");
  if (ps)
  {
     fread(act1_pal, 256, 3, ps);
     fclose(ps);
     for (i=0; i<256; i++)
     {
        final_pal[i].r = act1_pal[i][2] >> 2;
        final_pal[i].g = act1_pal[i][1] >> 2;
        final_pal[i].b = act1_pal[i][0] >> 2;
     }

  }
  else
  {
     // no palette
     fprintf(stderr, "can't open %s", tmp);
     return 1;
  }


  // cof infos
  my_credits();
  printf("COF file               = %s\n"
         "# directions           = %i\n"
         "# frames per direction = %i\n"
         "# layers               = %i\n"
         "Layers list            =",
         cofname, cof_dir, cof_fpd, cof_lay
  );
  memset(cof_comp, 0, sizeof(cof_comp));

  // init the colormaps to 'no modifications'
  for (c=0; c<COMP_MAX; c++)
     for (i=0; i<256; i++)
        cof_comp[c].colormap[i] = (UBYTE) i;

  for (i=0; i<cof_lay; i++)
  {
     c = * coftbl;
     printf(" %s", comp_str[c]);
     cof_comp[c].present = TRUE;
     cof_comp[c].wclass[0] = * (coftbl + 5);
     cof_comp[c].wclass[1] = * (coftbl + 6);
     cof_comp[c].wclass[2] = * (coftbl + 7);
     cof_comp[c].wclass[3] = 0;
     strupr(cof_comp[c].wclass);
     coftbl += 9;
  }
  printf("\n");
  coftbl += cof_fpd;

  // layers in ini
  while (fgets(line, sizeof(line), in))
  {
     s = (int) strlen(line);
     for (i=0; i<s; i++)
     {
        if ((line[i] == 0x0A) || (line[i] == 0x0D))
           line[i] = 0;
     }
     strupr(line);

     if (line[0] == ';') // the line in the .ini is a comment
        continue;
        
     // box ?
     if ((line[0] == 'B') && (line[1] == 'O') && (line[2] == 'X'))
     {
        if (line[3] != '=')
        {
           fprintf(stderr, "4th character must be '='\nLine : %s", line);
           return 1;
        }
        if (sscanf( & line[4], "%li,%li,%li,%li",
              & anim_box->xmin, & anim_box->ymin,
              & anim_box->xmax, & anim_box->ymax
            ) != 4)
        {
           fprintf(stderr, "BOX must have 4 parameters\nLine : %s", line);
           return 1;
        }
        * have_box = TRUE;
        continue;
     }

     // trans ?
     if ((line[0] == 'T') && (line[1] == 'R') && (line[2] == 'A') &&
         (line[3] == 'N') && (line[4] == 'S'))
     {
        if (line[5] != '=')
        {
           fprintf(stderr, "6th character must be '='\nLine : %s", line);
           return 1;
        }
        if (sscanf( & line[6], "%u %u %u",
              & trans_r, & trans_g, & trans_b
            ) != 3)
        {
           fprintf(stderr, "TRANS must have 3 parameters\nLine : %s", line);
           return 1;
        }
        act1_pal[0][2] = trans_r;
        act1_pal[0][1] = trans_g;
        act1_pal[0][0] = trans_b;
        final_pal[0].r = trans_r >> 2;
        final_pal[0].g = trans_g >> 2;
        final_pal[0].b = trans_b >> 2;
        continue;
     }

     // layer ?
     if (line[2] != '=')
     {
        fprintf(stderr, "3rd character must be '='\nLine : %s", line);
        return 1;
     }
     found = FALSE;
     for (i=0; i<COMP_MAX; i++)
     {
        if ((line[0] == comp_str[i][0]) && (line[1] == comp_str[i][1]))
        {
           found = TRUE;
           if (cof_comp[i].present != TRUE)
           {
              fprintf(stderr, "this layer is not present in the COF\n"
                              "Line : %s", line);
              return 1;
           }
           cof_comp[i].active  = TRUE;
           cof_comp[i].name[0] = line[3];
           cof_comp[i].name[1] = line[4];
           cof_comp[i].name[2] = line[5];
           cof_comp[i].name[3] = 0;
           if (line[6] == ':')
           {
              if (line[8] == 0)
              {
                 // from palshift.dat
                 sprintf(
                    cof_comp[i].cmapname,
                    "%s%s\\COF\\palshift.dat",
                    tok_dir,
                    token
                 );
                 pal = line[7] - '0';
                 cof_comp[i].palette = pal;
                 for (c=0; c<256; c++)
                    cof_comp[i].colormap[c] = palshift[pal][c];
              }
              else
              {
                 // from another file
                 load_cmap(i, & line[7]);
              }
           }
        }
     }
     if ( ! found)
     {
        fprintf(stderr, "the layer code %.2s does not exists\n"
                        "Line : %s", line, line);
        return 1;
     }
  }
  return 0;
}


// ==========================================================================
// cmap (colormap) must point to an array of 256 UBYTE
void remap_dcc_pixels(DCC_S * dcc, UBYTE * cmap)
{
   BITMAP * bmp;
   int    d, f, y, x;

   for (d=0; d < dcc->header.directions; d++)
   {
      for (f=0; f < dcc->header.frames_per_dir; f++)
      {
         bmp = dcc->frame[d][f].bmp;
         for (y=0; y<bmp->h; y++)
         {
            for (x=0; x<bmp->w; x++)
            {
               putpixel(bmp, x, y, cmap[getpixel(bmp, x, y)]);
            }
         }
      }
   }
}


// ==========================================================================
void prepare_box(DCC_BOX_S * anim_box)
{
   int i;
   
   for (i=0; i<COMP_MAX; i++)
   {
      if (dcc[i])
      {
         if (dcc[i]->box.xmin < anim_box->xmin)
            anim_box->xmin = dcc[i]->box.xmin;

         if (dcc[i]->box.ymin < anim_box->ymin)
            anim_box->ymin = dcc[i]->box.ymin;

         if (dcc[i]->box.xmax > anim_box->xmax)
            anim_box->xmax = dcc[i]->box.xmax;

         if (dcc[i]->box.ymax > anim_box->ymax)
            anim_box->ymax = dcc[i]->box.ymax;
      }
   }
}


// ==========================================================================
// allegro lost the 2 lowest bits, so hex-edit the PCX to put back the
// exact palette
void modify_pcx_palette(char * name)
{
   FILE * in;
   int  i;

   in = fopen(name, "rb+");
   if (in == NULL)
      return;
   fseek(in, -768, SEEK_END);
   for (i=0; i<256; i++)
   {
      fputc(act1_pal[i][2], in);
      fputc(act1_pal[i][1], in);
      fputc(act1_pal[i][0], in);
   }
   fclose(in);
}


// ==========================================================================
// allegro lost the 2 lowest bits, so hex-edit the BMP to put back the
// exact palette
void modify_bmp_palette(char * name)
{
   FILE          * in;
   int           i;
   unsigned long offset;

   in = fopen(name, "rb+");
   if (in == NULL)
      return;
   fseek(in, 14, SEEK_SET);
   fread( & offset, 4, 1, in);
   fseek(in, offset - 5, SEEK_CUR);
   for (i=0; i<256; i++)
   {
      fputc(0, in);
      fputc(act1_pal[i][0], in);
      fputc(act1_pal[i][1], in);
      fputc(act1_pal[i][2], in);
   }
   fclose(in);
}


// ==========================================================================
int main (void)
{
   BITMAP    * anim_bmp;
   DCC_BOX_S anim_box;
   FILE      * in;
   char      txtname[]="merge_dcc.ini", tmp[80];
   int       i, ok = TRUE, d, f, c, stop, have_box, image_type,
             new_dir1[1]   = { 0},
             new_dir4[4]   = { 0,  1,  2,  3},
             new_dir8[8]   = { 4,  0,  5,  1,  6,  2,  7,  3},

             new_dir16[16] = { 4,  8,  0,  9,  5, 10,  1, 11,
                               6, 12,  2, 13,  7, 14,  3, 15},

             new_dir32[32] = { 4,  8,  0,  9,  5, 10,  1, 11,
                               6, 12,  2, 13,  7, 14,  3, 15,
                              16, 17, 18, 19, 20, 21, 22, 23,
                              24, 25, 26, 27, 28, 29, 30, 31},
             new_dir[32];
   char      it_ext[2][5] = {{".PCX"}, {".BMP"}};
   struct al_ffblk ff;


   anim_box.xmin = anim_box.ymin = 0x7FFFFFFFL;
   anim_box.xmax = anim_box.ymax = 0x80000000L;

   allegro_init();
   set_color_depth(8);
   
   // delete all .pcx files
   sprintf(tmp, "D%s-(%s)-F%s.pcx", "??", "??", "???");
   stop = al_findfirst(tmp, &ff, -1);
   while ( ! stop)
   {
      remove(ff.name);
      stop = al_findnext( & ff);
   }
   al_findclose( & ff);

   sprintf(tmp, "(%s)-D%s-F%s.pcx", "??", "??", "???");
   stop = al_findfirst(tmp, &ff, -1);
   while ( ! stop)
   {
      remove(ff.name);
      stop = al_findnext( & ff);
   }
   al_findclose( & ff);

   // delete all .bmp files
   sprintf(tmp, "D%s-(%s)-F%s.bmp", "??", "??", "???");
   stop = al_findfirst(tmp, &ff, -1);
   while ( ! stop)
   {
      remove(ff.name);
      stop = al_findnext( & ff);
   }
   al_findclose( & ff);
   

   sprintf(tmp, "(%s)-D%s-F%s.bmp", "??", "??", "???");
   stop = al_findfirst(tmp, &ff, -1);
   while ( ! stop)
   {
      remove(ff.name);
      stop = al_findnext( & ff);
   }
   al_findclose( & ff);

   // new stderr & stdout files
   remove("stderr.txt");
   remove("stdout.txt");
   freopen("stderr.txt", "wt", stderr);
   freopen("stdout.txt", "wt", stdout);
   
   in = fopen(txtname, "rb");
   if (in == NULL)
   {
      fprintf(stderr, "can't open %s\n", txtname);
      return 1;
   }
   if (ini_load(in, txtname, & anim_box, & have_box, & image_type))
   {
      fclose(in);
      if (cofmem != NULL)
         free(cofmem);
      return 1;
   }
   fclose(in);
   memset(new_dir, 0, sizeof(new_dir));
   for (i=0; i<cof_dir; i++)
   {
      switch (cof_dir)
      {
         case  1 : new_dir[i] = new_dir1[i]; break;
         case  4 : new_dir[i] = new_dir4[i]; break;
         case  8 : new_dir[i] = new_dir8[i]; break;
         case 16 : new_dir[i] = new_dir16[i]; break;
         case 32 : new_dir[i] = new_dir32[i]; break;
      }
   }

   for (i=0; i<COMP_MAX; i++)
   {
      if (cof_comp[i].present && cof_comp[i].active)
      {
         sprintf(tmp, "%s%s\\%s\\%s%s%s%s%s.DCC",
            tok_dir,
            token, comp_str[i],
            token, comp_str[i], cof_comp[i].name, animtype,
            cof_comp[i].wclass
         );
         if ( ! file_exists(tmp, -1, NULL))
         {
            fprintf(stderr, "%s not found\n", tmp);
            ok = FALSE;
         }
         else
         {
            strcpy(cof_comp[i].filepath, tmp);
            printf("* %s : %s (%s, block %i)\n",
               comp_str[i], cof_comp[i].filepath,
               cof_comp[i].cmapname[0] ? cof_comp[i].cmapname : "none",
               cof_comp[i].palette);
         }
      }
   }
   if (ok)
   {
      for (i=0; i<COMP_MAX; i++)
      {
         if (strlen(cof_comp[i].filepath))
         {
            dcc[i] = dcc_disk_load(cof_comp[i].filepath);
            if (dcc[i] == NULL)
            {
               fprintf(stderr,
                  "Fatal error while loading %s :\n%s",
                  cof_comp[i].filepath, dcc_error
               );
            }
            else
            {
               if (dcc_decode(dcc[i], -1))
               {
                  fprintf(stderr,
                     "Fatal error while decoding %s :\n%s",
                     cof_comp[i].filepath, dcc_error
                  );
               }
               else
                  remap_dcc_pixels(dcc[i], cof_comp[i].colormap);
            }
         }
      }
      if (have_box == FALSE)
         prepare_box(& anim_box);
      anim_box.width  = anim_box.xmax - anim_box.xmin + 1;
      anim_box.height = anim_box.ymax - anim_box.ymin + 1;
      printf("box = (%i, %i) - (%i, %i) = %i * %i pixels\n",
         anim_box.xmin, anim_box.ymin,
         anim_box.xmax, anim_box.ymax,
         anim_box.width, anim_box.height
      );
      anim_bmp = create_bitmap(anim_box.width, anim_box.height);
      if (anim_bmp == NULL)
      {
         fprintf(stderr, "anim_bmp = NULL\n%");
         if (cofmem != NULL)
            free(cofmem);
         return 1;
      }
      for (d=0; d<cof_dir; d++)
      {
         for (f=0; f<cof_fpd; f++)
         {
            clear(anim_bmp);
            for (i=0; i<cof_lay; i++)
            {
               c = * coftbl;
               if (dcc[c])
               {
                  if (dcc[c]->frame[new_dir[d]][f].bmp != NULL)
                  {
                     draw_sprite(anim_bmp,
                        dcc[c]->frame[new_dir[d]][f].bmp,
                        dcc[c]->direction[new_dir[d]].box.xmin - anim_box.xmin,
                        dcc[c]->direction[new_dir[d]].box.ymin - anim_box.ymin
                     );
                  }
               }
               coftbl++;
            }
            //sprintf(tmp, "D%02i-(%02i)-F%03i%s",
            sprintf(tmp, "(%02i)-D%02i-F%03i%s",
               new_dir[d], d, f, it_ext[image_type]);
            save_bitmap(tmp, anim_bmp, final_pal);
            switch(image_type)
            {
               case 0 : modify_pcx_palette(tmp); break;
               case 1 : modify_bmp_palette(tmp); break;
            }
         }
      }
      destroy_bitmap(anim_bmp);
   }

   // end
   for (i=0; i<COMP_MAX; i++)
   {
      if (strlen(cof_comp[i].filepath))
         dcc_destroy(dcc[i]);
   }
   if (cofmem != NULL)
      free(cofmem);
   return 0;
}
END_OF_MAIN();

