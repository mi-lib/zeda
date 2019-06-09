/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_binfile.h
 * \brief binary data file manipulation.
 * \author Zhidao
 */

#ifndef __ZEDA_BINFILE_H__
#define __ZEDA_BINFILE_H__

#include <zeda/zeda_bit.h>

__BEGIN_DECLS

#define ZBINFILE_CURRENT_VERSION 0x00000001

#define ZBINFILE_ID "ZBD "
#define ZBINFILE_ENDIAN_CHECKER 0xaabbccdd

#define Z_ENDIAN_LITTLE  1234
#define Z_ENDIAN_BIG     4321
#define Z_ENDIAN_UNKNOWN 9999

typedef struct _zBinFile{
  int16_t version;      /*!< ZBD file version */
  /*! \cond */
  int16_t _endian_type; /* endian type */
  int16_t _size_int;    /* byte size of int */
  int16_t _size_long;   /* byte size of long */
  FILE *_fp;            /* file stream */
  /* I/O methods */
  size_t (* _fread_int16)(FILE*,int16_t*);
  size_t (* _fread_int32)(FILE*,int32_t*);
  size_t (* _fread_int64)(FILE*,int64_t*);
  int (* _fread_int)(struct _zBinFile*);
  long (* _fread_long)(struct _zBinFile*);

  size_t (* _fwrite_int16)(FILE*,int16_t*);
  size_t (* _fwrite_int32)(FILE*,int32_t*);
  size_t (* _fwrite_int64)(FILE*,int64_t*);
  size_t (* _fwrite_int)(struct _zBinFile*,int);
  size_t (* _fwrite_long)(struct _zBinFile*,long);
  /*! \endcond */
} zBinFile;

bool zBinFileOpen(zBinFile *bf, char filename[], const char *mode);
int zBinFileClose(zBinFile *bf);

bool zBinFileHeaderFRead(zBinFile *bf);
size_t zBinFileHeaderFWrite(zBinFile *bf);

void zBinFileInfoSet(zBinFile *bf, int16_t version, int16_t endian_type, int16_t size_int, int16_t size_long);
void zBinFileInfoSetThis(zBinFile *bf);

byte zBinFileByteFRead(zBinFile *bf);
size_t zBinFileByteFWrite(zBinFile *bf, byte val);

#define zBinFileIntFRead(bf)       ( (bf)->_fread_int( (bf) ) )
#define zBinFileIntFWrite(bf,val)  ( (bf)->_fwrite_int( (bf), (val) ) )

#define zBinFileLongFRead(bf)      ( (bf)->_fread_long( (bf) ) )
#define zBinFileLongFWrite(bf,val) ( (bf)->_fwrite_long( (bf), (val) ) )

float zBinFileFloatFRead(zBinFile *bf);
size_t zBinFileFloatFWrite(zBinFile *bf, float val);
double zBinFileDoubleFRead(zBinFile *bf);
size_t zBinFileDoubleFWrite(zBinFile *bf, double val);

__END_DECLS

#endif /* __ZEDA_BINFILE_H__ */
