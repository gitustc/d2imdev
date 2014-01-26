#include "d2p.h"



/* mkpkg */
/*  */
/* open */
/* close */







D2PKG *d2p_open(const char *filename)
{

    D2PKG *p = malloc(sizeof(D2PKG));

    if(!p){
        return NULL;
    }

    p->fp = fopen(filename, "rb");
    p->hash_bits = MIN_HASH_BITS;

    return p;
}

D2PKG *d2p_close(D2PKG *p)
{
    fclose(p->fp);
    return p;
}


D2PKG *d2p_mkpkg(uint32_t chunkSize, uint32_t fileUserDataSize)
{




	fstream stream;
	locale loc = locale::global(locale(""));
	stream.open(filename, ios_base::out | ios_base::trunc | ios_base::binary);
	locale::global(loc);
	if (!stream.is_open())
	{
		return NULL;
	}
	PackageHeader header;
	header.sign = PACKAGE_FILE_SIGN;
	header.version = CURRENT_VERSION;
	header.headerSize = sizeof(PackageHeader);
	header.fileCount = 0;
	header.fileEntryOffset = sizeof(PackageHeader);
	header.filenameOffset = sizeof(PackageHeader);
	header.allFileEntrySize = 0;
	header.allFilenameSize = 0;
	header.originFilenamesSize = 0;
	header.chunkSize = chunkSize;
#ifdef ZP_USE_WCHAR
	header.flag = PACK_UNICODE;
#else
	header.flag = 0;
#endif
	header.fileEntrySize = sizeof(FileEntry) + fileUserDataSize;
	memset(header.reserved, 0, sizeof(header.reserved));

	stream.write((char*)&header, sizeof(header));
	stream.close();

	return open(filename, 0);
}

}
