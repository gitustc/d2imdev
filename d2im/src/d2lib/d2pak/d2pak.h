#ifndef _D2PAK_H_
#define _D2PAK_H_


typedef struct {
    uint32_t        binfo;
    uint32_t        capacity;

}d2pak_header;

typedef struct {
    uint32_t        crc1;

typedef struct {

    d2pak_header    d2phdr;

    uint32_t        seed1;
    uint32_t        seed2;
    uint32_t        seed3;

    uint32_t       *indices;
    uint32_t       *crctbl;
    uint32_t       *finfo;
}d2pak;


#if 0
typedef struct {
    uint32_t        start;
    uint32_t        length;
    uint32_t        binfo;
}d2finfo;
#endif

#define d2pak_fstart(f, index)      (const)(*(((f)->finfo)+(index)*3))
#define d2pak_flen(f, index)        (const)(*(((f)->finfo)+(index)*3+1))
#define d2pak_fbinfo(f, index)      (const)(*(((f)->finfo)+(index)*3+2))


#define d2pak_seek(f, index)        (fseek(f->fp, d2pak_fstart(f, index), SEEK_SET))
#define d2pak_read(f, buf, n)       (fread(buf, buf, n, 1, f->fp))




void str2hash1(const char *, uint32_t *);
void str2hash2(const char *, uint32_t *);
void str2hash3(const char *, uint32_t *);

extern d2pak *d2pak_open(const char *);
extern void   d2pak_free(const char *);




#endif
