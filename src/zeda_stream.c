/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_stream - generalized I/O stream.
 */

#include <zeda/zeda_stream.h>

#ifndef __KERNEL__

/* abstracts of standard UNIX file */

void zStreamRewindFile(zStream *str)
{
  rewind( str->src.fp );
}

int zStreamSeekFile(zStream *str, long offset)
{
  return fseek( str->src.fp, offset, SEEK_SET );
}

long zStreamTellFile(zStream *str)
{
  return ftell( str->src.fp );
}

size_t zStreamReadFile(zStream *str, byte *dest, size_t size, size_t nmemb)
{
  return fread( dest, size, nmemb, str->src.fp );
}

size_t zStreamWriteFile(zStream *str, byte *src, size_t size, size_t nmemb)
{
  return fwrite( src, size, nmemb, str->src.fp );
}

int zStreamCloseFile(zStream *str)
{
  return fclose( str->src.fp );
}

/* zStreamAttachFile
 * - attachment of file to stream.
 */
void zStreamAttachFile(zStream *str, FILE *fp)
{
  /* stream source */
  str->src.fp = fp;
  /* methods */
  str->rewind = zStreamRewindFile;
  str->seek   = zStreamSeekFile;
  str->tell   = zStreamTellFile;
  str->read   = zStreamReadFile;
  str->write  = zStreamWriteFile;
  str->close  = zStreamCloseFile;
}

/* zStreamOpenFile
 * - file open and attachment to stream.
 */
zStream *zStreamOpenFile(zStream *str, char filename[], char *mode)
{
  FILE *fp;

  if( !( fp = fopen( filename, mode ) ) ){
    ZOPENERROR( filename );
    return NULL;
  }
  zStreamAttachFile( str, fp );
  return str;
}

/* abstracts of memory buffer */

void zStreamRewindBuf(zStream *str)
{
  str->src.mem.cur = str->src.mem.buf;
}

int zStreamSeekBuf(zStream *str, long offset)
{
  str->src.mem.cur = str->src.mem.buf + offset;
  return ( str->src.mem.cur - str->src.mem.buf ) > (int)str->src.mem.size ? -1 : 0;
}

long zStreamTellBuf(zStream *str)
{
  return (long)str->src.mem.cur;
}

size_t zStreamReadBuf(zStream *str, byte *dest, size_t size, size_t nmemb)
{
  size_t rs, rm;

  rm = str->src.mem.size - ( str->src.mem.cur - str->src.mem.buf );
  if( rm <= 0 ) return 0;
  if( ( rs = size * nmemb ) > rm ) rs = rm;
  memcpy( dest, str->src.mem.cur, rs );
  str->src.mem.cur += rs;
  return rs;
}

size_t zStreamWriteBuf(zStream *str, byte *src, size_t size, size_t nmemb)
{
  size_t rs, rm;

  rm = str->src.mem.size - ( str->src.mem.cur - str->src.mem.buf );
  if( rm <= 0 ) return 0;
  if( ( rs = size * nmemb ) > rm ) rs = rm;
  memcpy( str->src.mem.cur, src, rs );
  str->src.mem.cur += rs;
  return rs;
}

int zStreamCloseBuf(zStream *str)
{
  free( str->src.mem.buf ); return 1;
}

/* zStreamAttachMem
 * - attachment of memory buffer to stream.
 */
void zStreamAttachMem(zStream *str, byte *mem, size_t size)
{
  /* stream source */
  str->src.mem.buf = mem;
  str->src.mem.cur = mem;
  str->src.mem.size = size;
  /* methods */
  str->rewind = zStreamRewindBuf;
  str->seek   = zStreamSeekBuf;
  str->tell   = zStreamTellBuf;
  str->read   = zStreamReadBuf;
  str->write  = zStreamWriteBuf;
  str->close  = zStreamCloseBuf;
}

/* zStreamOpenMem
 * - memory allocation and attachment to stream.
 */
zStream *zStreamOpenMem(zStream *str, size_t size)
{
  byte *buf;

  if( !( buf = (byte *)calloc( size, 1 ) ) ){
    ZALLOCERROR();
    return str;
  }
  zStreamAttachMem( str, buf, size );
  return NULL;
}

#endif /* __KERNEL__ */
