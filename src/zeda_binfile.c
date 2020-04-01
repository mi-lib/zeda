/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_binfile - binary data file manipulation
 */

#include <zeda/zeda_binfile.h>

/* endian-conversion-friendly I/O functions */

#define ZEDA_BINFILE_DEF_FREAD( bit ) \
ZEDA_BINFILE_DEF_FREAD_PROTOTYPE( bit ){\
  return fread( val, sizeof(int##bit##_t), 1, fp );\
}

#define ZEDA_BINFILE_DEF_FREAD_REV( bit ) \
ZEDA_BINFILE_DEF_FREAD_REV_PROTOTYPE( bit ){\
  int##bit##_t __tmp;\
  size_t size;\
  if( ( size = fread( &__tmp, sizeof(int##bit##_t), 1, fp ) ) > 0 )\
    *val = endian_reverse##bit( __tmp );\
  return size;\
}

#define ZEDA_BINFILE_DEF_FWRITE( bit ) \
ZEDA_BINFILE_DEF_FWRITE_PROTOTYPE( bit ){\
  return fwrite( val, sizeof(int##bit##_t), 1, fp );\
}

#define ZEDA_BINFILE_DEF_FWRITE_REV( bit ) \
ZEDA_BINFILE_DEF_FWRITE_REV_PROTOTYPE( bit ){\
  int##bit##_t __tmp;\
  __tmp = endian_reverse##bit( *val );\
  return fwrite( &__tmp, sizeof(int##bit##_t), 1, fp );\
}

ZEDA_BINFILE_DEF_FREAD(       8 )
ZEDA_BINFILE_DEF_FWRITE(      8 )

ZEDA_BINFILE_DEF_FREAD(      16 )
ZEDA_BINFILE_DEF_FREAD_REV(  16 )
ZEDA_BINFILE_DEF_FWRITE(     16 )
ZEDA_BINFILE_DEF_FWRITE_REV( 16 )

ZEDA_BINFILE_DEF_FREAD(      32 )
ZEDA_BINFILE_DEF_FREAD_REV(  32 )
ZEDA_BINFILE_DEF_FWRITE(     32 )
ZEDA_BINFILE_DEF_FWRITE_REV( 32 )

ZEDA_BINFILE_DEF_FREAD(      64 )
ZEDA_BINFILE_DEF_FREAD_REV(  64 )
ZEDA_BINFILE_DEF_FWRITE(     64 )
ZEDA_BINFILE_DEF_FWRITE_REV( 64 )

#define ZEDA_BINFILE_DEF_FREAD_TYPE( type, bit ) \
ZEDA_BINFILE_DEF_FREAD_TYPE_PROTOTYPE( type ){\
  type val;\
  return fread_int##bit( fp, (int##bit##_t *)&val ) > 0 ? val : 0;\
}

#define ZEDA_BINFILE_DEF_FREAD_REV_TYPE( type, bit ) \
ZEDA_BINFILE_DEF_FREAD_REV_TYPE_PROTOTYPE( type ){\
  type val;\
  return fread_int##bit##_rev( fp, (int##bit##_t *)&val ) > 0 ? val : 0;\
}

#define ZEDA_BINFILE_DEF_FWRITE_TYPE( type, bit ) \
ZEDA_BINFILE_DEF_FWRITE_TYPE_PROTOTYPE( type ){\
  return fwrite_int##bit( fp, (int##bit##_t *)&val );\
}

#define ZEDA_BINFILE_DEF_FWRITE_REV_TYPE( type, bit ) \
ZEDA_BINFILE_DEF_FWRITE_REV_TYPE_PROTOTYPE( type ){\
  return fwrite_int##bit##_rev( fp, (int##bit##_t *)&val );\
}

ZEDA_BINFILE_DEF_FREAD_TYPE(      byte, 8 )
ZEDA_BINFILE_DEF_FWRITE_TYPE(     byte, 8 )

ZEDA_BINFILE_DEF_FREAD_TYPE(      word, 16 )
ZEDA_BINFILE_DEF_FREAD_REV_TYPE(  word, 16 )
ZEDA_BINFILE_DEF_FWRITE_TYPE(     word, 16 )
ZEDA_BINFILE_DEF_FWRITE_REV_TYPE( word, 16 )

ZEDA_BINFILE_DEF_FREAD_TYPE(      dword, 32 )
ZEDA_BINFILE_DEF_FREAD_REV_TYPE(  dword, 32 )
ZEDA_BINFILE_DEF_FWRITE_TYPE(     dword, 32 )
ZEDA_BINFILE_DEF_FWRITE_REV_TYPE( dword, 32 )

ZEDA_BINFILE_DEF_FREAD_TYPE(      float, 32 )
ZEDA_BINFILE_DEF_FREAD_REV_TYPE(  float, 32 )
ZEDA_BINFILE_DEF_FWRITE_TYPE(     float, 32 )
ZEDA_BINFILE_DEF_FWRITE_REV_TYPE( float, 32 )

ZEDA_BINFILE_DEF_FREAD_TYPE(      double, 64 )
ZEDA_BINFILE_DEF_FREAD_REV_TYPE(  double, 64 )
ZEDA_BINFILE_DEF_FWRITE_TYPE(     double, 64 )
ZEDA_BINFILE_DEF_FWRITE_REV_TYPE( double, 64 )

/* binary-file manipulator */

#define ZBINFILE_DEF_INT_FREAD( bit, type ) \
static type _zBinFile_##type##_fread##bit(zBinFile *bf){\
  int##bit##_t val;\
  return bf->_fread_int##bit( bf->_fp, &val ) > 0 ? val : 0;\
}

#define ZBINFILE_DEF_INT_FWRITE( bit, type ) \
static size_t _zBinFile_##type##_fwrite##bit(zBinFile *bf, type val){\
  int##bit##_t __tmp;\
  __tmp = val;\
  return bf->_fwrite_int##bit( bf->_fp, &__tmp );\
}

ZBINFILE_DEF_INT_FREAD(  16, int )
ZBINFILE_DEF_INT_FWRITE( 16, int )
ZBINFILE_DEF_INT_FREAD(  32, int )
ZBINFILE_DEF_INT_FWRITE( 32, int )
ZBINFILE_DEF_INT_FREAD(  64, int )
ZBINFILE_DEF_INT_FWRITE( 64, int )

ZBINFILE_DEF_INT_FREAD(  16, long )
ZBINFILE_DEF_INT_FWRITE( 16, long )
ZBINFILE_DEF_INT_FREAD(  32, long )
ZBINFILE_DEF_INT_FWRITE( 32, long )
ZBINFILE_DEF_INT_FREAD(  64, long )
ZBINFILE_DEF_INT_FWRITE( 64, long )

byte zBinFileByteFRead(zBinFile *bf){ return fread_byte( bf->_fp ); }
size_t zBinFileByteFWrite(zBinFile *bf, byte val){ return fwrite_byte( bf->_fp, val ); }

static bool _zBinFileEndianIsSame(zBinFile *bf);;

static size_t _zBinFileEndianCheckerFWrite(zBinFile *bf);

bool zBinFileOpen(zBinFile *bf, char filename[], const char *mode)
{
  if( !( bf->_fp = fopen( filename, mode ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  return true;
}

int zBinFileClose(zBinFile *bf)
{
  if( bf->_fp ) return fclose( bf->_fp );
  return EOF;
}

bool _zBinFileEndianIsSame(zBinFile *bf)
{
  return bf->_endian_type == endian_check() ? true : false;
}

bool zBinFileHeaderFRead(zBinFile *bf)
{
  char id[BUFSIZ];
  int id_len;
  int16_t version;
  union{
    int8_t byte[4];
    int32_t val;
  } endian_checker;
  bool ret = true;

  /* check if the file is ZBD */
  id_len = strlen( ZBINFILE_ID );
  if( fread( id, sizeof(char), id_len, bf->_fp ) < 1 ) return false;
  if( strncmp( id, ZBINFILE_ID, id_len ) != 0 ){
    ZRUNERROR( ZEDA_ERR_NOT_ZBD );
    return false;
  }
  /* check endian */
  if( ( endian_checker.byte[0] = zBinFileByteFRead( bf ) ) == 0 ||
      ( endian_checker.byte[1] = zBinFileByteFRead( bf ) ) == 0 ||
      ( endian_checker.byte[2] = zBinFileByteFRead( bf ) ) == 0 ||
      ( endian_checker.byte[3] = zBinFileByteFRead( bf ) ) == 0 ){
    ZRUNERROR( ZEDA_ERR_ENDIAN_CHK_NOT_FOUND );
    return false;
  }
  if( endian_checker.val == ZBINFILE_ENDIAN_CHECKER ){
    bf->_endian_type = Z_ENDIAN_BIG;
  } else
  if( endian_checker.val == endian_reverse32( ZBINFILE_ENDIAN_CHECKER ) ){
    bf->_endian_type = Z_ENDIAN_LITTLE;
  } else{
    ZRUNWARN( ZEDA_WARN_ENDIAN_NOT_SUPPORTED );
    bf->_endian_type = Z_ENDIAN_UNKNOWN; /* middle endian not supported. */
    return false;
  }
  if( _zBinFileEndianIsSame( bf ) ){
    bf->_fread_int16 = fread_int16;
    bf->_fread_int32 = fread_int32;
    bf->_fread_int64 = fread_int64;
  } else{
    bf->_fread_int16 = fread_int16_rev;
    bf->_fread_int32 = fread_int32_rev;
    bf->_fread_int64 = fread_int64_rev;
  }
  /* read version and byte sizes of some types */
  if( bf->_fread_int16( bf->_fp, &version ) < 1 ){
    ZRUNERROR( ZEDA_ERR_VERSION_NOT_FOUND );
    return false;
  }
  if( bf->_fread_int16( bf->_fp, &bf->_size_int ) < 1 ){
    ZRUNERROR( ZEDA_ERR_INTSIZ_NOT_FOUND );
    return false;
  }
  if( bf->_fread_int16( bf->_fp, &bf->_size_long ) < 1 ){
    ZRUNERROR( ZEDA_ERR_LNGSIZ_NOT_FOUND );
    return false;
  }
  /* assign readers of int and long */
  switch( bf->_size_int ){
  case 2: bf->_fread_int = _zBinFile_int_fread16; break;
  case 4: bf->_fread_int = _zBinFile_int_fread32; break;
  case 8: bf->_fread_int = _zBinFile_int_fread64; break;
  default: ZRUNWARN( ZEDA_WARN_INVALID_INTSIZ, bf->_size_int ); ret = false;
  }
  if( bf->_size_int > sizeof(int) ){
    ZRUNWARN( ZEDA_WARN_INT_SHRTSIZ, bf->_size_int, sizeof(int) );
  }
  switch( bf->_size_long ){
  case 2: bf->_fread_long = _zBinFile_long_fread16; break;
  case 4: bf->_fread_long = _zBinFile_long_fread32; break;
  case 8: bf->_fread_long = _zBinFile_long_fread64; break;
  default: ZRUNWARN( ZEDA_WARN_INVALID_LNGSIZ, bf->_size_long ); ret = false;
  }
  if( bf->_size_long > sizeof(long) ){
    ZRUNWARN( ZEDA_WARN_LNG_SHRTSIZ, bf->_size_long, sizeof(long) );
  }
  return ret;
}

void zBinFileInfoSet(zBinFile *bf, int16_t version, int16_t endian_type, int16_t size_int, int16_t size_long)
{
  bf->version = version;
  bf->_endian_type = endian_type;
  bf->_size_int = size_int;
  bf->_size_long = size_long;
}

void zBinFileInfoSetThis(zBinFile *bf)
{
  zBinFileInfoSet( bf, ZBINFILE_CURRENT_VERSION, endian_check(), sizeof(int), sizeof(long) );
}

size_t _zBinFileEndianCheckerFWrite(zBinFile *bf)
{
  size_t size = 0;

  switch( bf->_endian_type ){
  case Z_ENDIAN_BIG:
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x000000ff ) >>  0 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x0000ff00 ) >>  8 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x00ff0000 ) >> 16 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0xff000000 ) >> 24 );
    break;
  case Z_ENDIAN_LITTLE:
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0xff000000 ) >> 24 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x00ff0000 ) >> 16 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x0000ff00 ) >>  8 );
    size += zBinFileByteFWrite( bf, ( ZBINFILE_ENDIAN_CHECKER & 0x000000ff ) >>  0 );
    break;
  case Z_ENDIAN_UNKNOWN:
  default:
    ZRUNWARN( ZEDA_WARN_ENDIAN_NOT_SUPPORTED );
    return 0;
  }
  return size;
}

size_t zBinFileHeaderFWrite(zBinFile *bf)
{
  size_t header_size = 0;
  bool ret = true;

  header_size += fwrite( ZBINFILE_ID, sizeof(char), strlen(ZBINFILE_ID), bf->_fp );
  header_size += _zBinFileEndianCheckerFWrite( bf );
  if( _zBinFileEndianIsSame( bf ) ){
    bf->_fwrite_int16 = fwrite_int16;
    bf->_fwrite_int32 = fwrite_int32;
    bf->_fwrite_int64 = fwrite_int64;
  } else{
    bf->_fwrite_int16 = fwrite_int16_rev;
    bf->_fwrite_int32 = fwrite_int32_rev;
    bf->_fwrite_int64 = fwrite_int64_rev;
  }
  header_size += bf->_fwrite_int16( bf->_fp, &bf->version );
  header_size += bf->_fwrite_int16( bf->_fp, &bf->_size_int );
  header_size += bf->_fwrite_int16( bf->_fp, &bf->_size_long );
  /* ssign writers of int and long */
  switch( bf->_size_int ){
  case 2: bf->_fwrite_int = _zBinFile_int_fwrite16; break;
  case 4: bf->_fwrite_int = _zBinFile_int_fwrite32; break;
  case 8: bf->_fwrite_int = _zBinFile_int_fwrite64; break;
  default: ZRUNWARN( ZEDA_WARN_INVALID_INTSIZ, bf->_size_int ); ret = false;
  }
  if( bf->_size_int < sizeof(int) ){
    ZRUNWARN( ZEDA_WARN_INT_SHRTSIZ, bf->_size_int, sizeof(int) );
  }
  switch( bf->_size_long ){
  case 2: bf->_fwrite_long = _zBinFile_long_fwrite16; break;
  case 4: bf->_fwrite_long = _zBinFile_long_fwrite32; break;
  case 8: bf->_fwrite_long = _zBinFile_long_fwrite64; break;
  default: ZRUNWARN( ZEDA_WARN_INVALID_LNGSIZ, bf->_size_long ); ret = false;
  }
  if( bf->_size_long < sizeof(long) ){
    ZRUNWARN( ZEDA_WARN_LNG_SHRTSIZ, bf->_size_long, sizeof(long) );
  }
  return ret ? header_size : 0;
}

/* readers / writers */

float zBinFileFloatFRead(zBinFile *bf){
  float val;
  return bf->_fread_int32( bf->_fp, (int32_t *)&val ) > 0 ? val : 0;
}

size_t zBinFileFloatFWrite(zBinFile *bf, float val){
  return bf->_fwrite_int32( bf->_fp, (int32_t *)&val );
}

double zBinFileDoubleFRead(zBinFile *bf){
  double val;
  return bf->_fread_int64( bf->_fp, (int64_t *)&val ) > 0 ? val : 0;
}

size_t zBinFileDoubleFWrite(zBinFile *bf, double val){
  return bf->_fwrite_int64( bf->_fp, (int64_t *)&val );
}
