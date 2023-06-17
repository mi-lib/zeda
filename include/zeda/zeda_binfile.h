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

/* endian-conversion-friendly I/O functions */

#define ZEDA_BINFILE_DEF_FREAD_PROTOTYPE( bit ) \
  size_t fread_int##bit(FILE *fp, int##bit##_t *val)
#define ZEDA_BINFILE_DEF_FREAD_REV_PROTOTYPE( bit ) \
  size_t fread_int##bit##_rev(FILE *fp, int##bit##_t *val)
#define ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE( bit ) \
  size_t fwrite_int##bit(FILE *fp, int##bit##_t *val)
#define ZEDA_BINFILE_DEF_FWRITE_REV_PROTOTYPE( bit ) \
  size_t fwrite_int##bit##_rev(FILE *fp, int##bit##_t *val)

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_PROTOTYPE(       8 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE(      8 );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_PROTOTYPE(      16 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_PROTOTYPE(  16 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE(     16 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_PROTOTYPE( 16 );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_PROTOTYPE(      32 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_PROTOTYPE(  32 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE(     32 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_PROTOTYPE( 32 );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_PROTOTYPE(      64 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_PROTOTYPE(  64 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE(     64 );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_PROTOTYPE( 64 );

#define ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE( type ) \
  type fread_##type(FILE *fp)
#define ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE( type ) \
  type fread_##type##_rev(FILE *fp)
#define ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE( type ) \
  size_t fwrite_##type(FILE *fp, type val)
#define ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( type ) \
  size_t fwrite_##type##_rev(FILE *fp, type val)

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE(      byte );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE(     byte );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE(      word );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE(  word );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE(     word );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( word );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE(      dword );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE(  dword );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE(     dword );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( dword );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE(      float );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE(  float );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE(     float );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( float );

__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE(      double );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE(  double );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE(     double );
__ZEDA_EXPORT ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( double );

/* binary-file manipulator */

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

__ZEDA_EXPORT bool zBinFileOpen(zBinFile *bf, char filename[], const char *mode);
__ZEDA_EXPORT int zBinFileClose(zBinFile *bf);

__ZEDA_EXPORT byte zBinFileByteFRead(zBinFile *bf);
__ZEDA_EXPORT size_t zBinFileByteFWrite(zBinFile *bf, byte val);

__ZEDA_EXPORT bool zBinFileHeaderFRead(zBinFile *bf);
__ZEDA_EXPORT size_t zBinFileHeaderFWrite(zBinFile *bf);

__ZEDA_EXPORT void zBinFileInfoSet(zBinFile *bf, int16_t version, int16_t endian_type, int16_t size_int, int16_t size_long);
__ZEDA_EXPORT void zBinFileInfoSetThis(zBinFile *bf);

#define zBinFileIntFRead(bf)       ( (bf)->_fread_int( (bf) ) )
#define zBinFileIntFWrite(bf,val)  ( (bf)->_fwrite_int( (bf), (val) ) )

#define zBinFileLongFRead(bf)      ( (bf)->_fread_long( (bf) ) )
#define zBinFileLongFWrite(bf,val) ( (bf)->_fwrite_long( (bf), (val) ) )

__ZEDA_EXPORT float zBinFileFloatFRead(zBinFile *bf);
__ZEDA_EXPORT size_t zBinFileFloatFWrite(zBinFile *bf, float val);
__ZEDA_EXPORT double zBinFileDoubleFRead(zBinFile *bf);
__ZEDA_EXPORT size_t zBinFileDoubleFWrite(zBinFile *bf, double val);

__END_DECLS

#endif /* __ZEDA_BINFILE_H__ */
