#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>





/*
 * tad -u8   x   file
 * tad -u16  x   file
 * tad -u32  x   file
 *               file
 * tad -i8   x   file
 * tad -i16  x   file
 * tad -i32  x   file
 *               file
 */

int main(int argc, char* argv[])
{

    FILE            *fp;
    uint8_t         u8;
    uint16_t        u16;
    uint32_t        u32;
    int8_t          i8;
    int16_t         i16;
    int32_t         i32;


    if(argc != 4){
        printf(
                " tad -u8   x   file\n"
                " tad -u16  x   file\n"
                " tad -u32  x   file\n"
                "                   \n"
                " tad -i8   x   file\n"
                " tad -i16  x   file\n"
                " tad -i32  x   file\n"
             );


        return 1;
    }


    if(!strcmp(argv[1],"-u8")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        u8 = (uint8_t)strtoul(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&u8, 1, 1, fp);
        fclose(fp);
        return 0;
    }


    if(!strcmp(argv[1],"-u16")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        u16 = (uint16_t)strtoul(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&u16, 1, 2, fp);
        fclose(fp);
        return 0;
    }


    if(!strcmp(argv[1],"-u32")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        u32 = (uint32_t)strtoul(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&u32, 1, 4, fp);
        fclose(fp);
        return 0;
    }

    if(!strcmp(argv[1],"-i8")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        i8 = (int8_t)strtol(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&i8, 1, 1, fp);
        fclose(fp);
        return 0;
    }


    if(!strcmp(argv[1],"-i16")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        i16 = (int16_t)strtol(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&i16, 1, 2, fp);
        fclose(fp);
        return 0;
    }


    if(!strcmp(argv[1],"-i32")){
        fp = fopen(argv[3], "ab");
        if(!fp){
            return 2;
        }

        u32 = (int32_t)strtol(argv[2],NULL,10);
        fseek(fp, 0, SEEK_END);
        fwrite(&i32, 1, 4, fp);
        fclose(fp);
        return 0;
    }
    return 3;
}
