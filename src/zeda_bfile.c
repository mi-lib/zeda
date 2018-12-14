/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_bfile - binary file I/O
 * contributer: 2018 Naoki Wakisaka and Takeharu Suzuki
 */
#include <zeda/zeda_bfile.h>

uint16_t zByteSwap16(uint16_t val)
{
  uint16_t ret;

  ret  = val << 8;
  ret |= val >> 8;
  return ret;
}

uint32_t zByteSwap32(uint32_t val)
{
  uint32_t ret;

  ret  = val                << 24;
  ret |= (val & 0x0000FF00) << 8;
  ret |= (val & 0x00FF0000) >> 8;
  ret |= val                >> 24;
  return ret;
}

uint64_t zByteSwap64(uint64_t val)
{
  uint64_t ret;

  ret  = val                        << 56;
  ret |= (val & 0x000000000000FF00) << 40;
  ret |= (val & 0x0000000000FF0000) << 24;
  ret |= (val & 0x00000000FF000000) << 8;
  ret |= (val & 0x000000FF00000000) >> 8;
  ret |= (val & 0x0000FF0000000000) >> 24;
  ret |= (val & 0x00FF000000000000) >> 40;
  ret |= val                        >> 56;
  return ret;
}

void zByteSwapAllBuf(void *val, void *ret, int size, char* buf)
{
  register int i;

  for( i=0; i<size; i++ )
    buf[i] = ((char*)val)[i];
  for( i=0; i<size; i++ )
    ((char*)ret)[i] = buf[size-1-i];
}

bool zByteSwapAll(void *val, void *ret, int size)
{
  char *tmp;

  if( (tmp = zAlloc(char, size)) == NULL ){
    ZALLOCERROR();
    return false;
  }
  zByteSwapAllBuf( val, ret, size, tmp );
  zFree(tmp);
  return true;
}



uint32_t zFileHeaderCalcHeaderSize(zHeader *h)
{
  uint32_t size = 0;

  size += sizeof(char) * ZHEADER_HEAD_SIZE; /* head */
  size += sizeof(uint32_t);                 /* size */
  size += sizeof(uint8_t);                  /* hver */

  size += sizeof(uint8_t);                  /* dver */
  size += sizeof(uint8_t);                  /* int_size */
  size += sizeof(uint8_t);                  /* lint_size */
  size += sizeof(int64_t);                  /* time */
  size += sizeof(uint32_t);                 /* tsign */

  /* suffix */
  size += sizeof(char);
  if( h->suffix != '\0' )
    size += sizeof(char) * strlen(h->suffix);

  return size;
}


void zFileHeaderInit(zHeader *h){
  h->state = ZHEADER_NOTHING;
  h->endian = ZHEADER_ENDIAN_EQUAL;

  strcpy( h->head, ZHEADER_HEAD );
  h->hver = ZHEADER_NEWEST_VERSION;
  h->size = zFileHeaderCalcHeaderSize( h );

  zStrCopyNC( h->suffix, "" );
  h->dver = 0;
  h->int_size = sizeof(int);
  h->lint_size = sizeof(long int);
  h->time = 0;
  h->tsign = ZHEADER_TAIL_SIGN;
}

void zFileHeaderSetSuffix(zHeader *h, char *suffix)
{
  zStrCopy( h->suffix, suffix, BUFSIZ );
  h->size = zFileHeaderCalcHeaderSize( h );
}


static uint32_t _zFileHeaderFReadB(FILE *fp, zHeader *h);
uint32_t _zFileHeaderFReadB(FILE *fp, zHeader *h)
{
  uint32_t size = 0;
  char buf[BUFSIZ];
  uint32_t n = 0;

  while( !feof(fp) ){
    size += sizeof(char)* fread( &buf[n], sizeof(char), 1, fp );
    if( buf[n] == '\0' ) break;
    if( ++n >= BUFSIZ ){
      rewind( fp );
      h->state = ZHEADER_BROKEN;
      ZRUNERROR("Header BROKEN !!");
      return 0;
    }
  }
  zStrCopyNC( h->suffix, buf );

  size += sizeof(uint8_t) * fread( &h->dver,      sizeof(uint8_t), 1, fp );
  size += sizeof(uint8_t) * fread( &h->int_size,  sizeof(uint8_t), 1, fp );
  size += sizeof(uint8_t) * fread( &h->lint_size, sizeof(uint8_t), 1, fp );
  size += sizeof(int64_t) * fread( &h->time,      sizeof(int64_t), 1, fp );
  h->tsign = 0;
  size += sizeof(uint32_t) * fread(&h->tsign, sizeof(uint32_t), 1, fp);

  /* check endianness */
  if( h->tsign != ZHEADER_TAIL_SIGN ){
    if( zByteSwap32(h->tsign) == ZHEADER_TAIL_SIGN ){
      h->endian = ZHEADER_ENDIAN_ANOTHER;
      h->size = zByteSwap32( h->size );
      h->time = zByteSwap64( h->time );
      ZRUNWARN("Another Endian !!");
    } else {
      rewind( fp );
      h->endian = ZHEADER_ENDIAN_STRANGE;
      h->state = ZHEADER_BROKEN;
      ZRUNERROR("Header BROKEN !!");
      ZRUNERROR("Or another Endianness !!");
      return 0;
    }
  } else
    h->endian = ZHEADER_ENDIAN_EQUAL;

  h->state = ZHEADER_CORRECT;
  return size;
}

zHeader *zFileHeaderFReadB(FILE *fp, zHeader *h)
{
  uint32_t size = 0;

  zFileHeaderInit( h );
  size += sizeof(char) * fread( h->head, sizeof(char), ZHEADER_HEAD_SIZE, fp );
  if( h->head[ZHEADER_HEAD_SIZE-1] != '\0' || strcmp( h->head, ZHEADER_HEAD ) != 0 ){
    rewind( fp );
    h->state = ZHEADER_NOTHING;
    ZRUNWARN("Nothing Header. or Header Broken.");
    return h;
  }
  size += sizeof(uint32_t) * fread( &h->size, sizeof(uint32_t), 1, fp );
  size += sizeof(uint8_t)  * fread( &h->hver, sizeof(uint8_t),  1, fp );

  switch( h->hver ){
  case 1: size += _zFileHeaderFReadB( fp, h ); break;
  default:
    rewind( fp );
    h->state = ZHEADER_BROKEN;
    ZRUNERROR("Header version ERROR !!");
    return h;
  }

  if( h->size != size ){
    h->state = ZHEADER_BROKEN;
    ZRUNERROR("Header BROKEN !!");
  }
  return h;
}

zHeader* zFileHeaderFWriteB(FILE *fp, zHeader *h)
{
  int64_t t;

  if( zFileHeaderIsBroken( h ) ){
    ZRUNERROR("  Header is BROKEN !!");
    return NULL;
  }
  if( h->hver != ZHEADER_NEWEST_VERSION )
    ZRUNWARN("Write Header newest version !!");

  fwrite( h->head, sizeof(char), ZHEADER_HEAD_SIZE, fp );
  fwrite( &h->size, sizeof(uint32_t), 1, fp );
  fwrite( &h->hver, sizeof(uint8_t), 1, fp );

  if( h->suffix[0] != '\0' )
    fwrite( h->suffix, sizeof(char), strlen(h->suffix)+1, fp );
  else {
    fwrite( zNullStr(), sizeof(char), 1, fp );
    ZRUNERROR( "Nothing SUFFIX in header !!" );
  }
  fwrite( &h->dver, sizeof(uint8_t), 1, fp );
  fwrite( &h->int_size, sizeof(uint8_t), 1, fp );
  fwrite( &h->lint_size, sizeof(uint8_t), 1, fp );

  t = (int64_t)time(NULL);
  fwrite( &t, sizeof(int64_t), 1, fp );

  fwrite( &h->tsign, sizeof(uint32_t), 1, fp );
  return h;
}


void _zFileHeaderFWrite(FILE *fp, zHeader *h)
{
  time_t t;
  char tmp[BUFSIZ];

  fprintf( fp, "File type      : %s\n", h->suffix );
  fprintf( fp, "%-6s version : %d\n", h->suffix, h->dver );
  fprintf( fp, "int size       : %d [byte]\n", h->int_size );
  fprintf( fp, "long int size  : %d [byte]\n", h->lint_size );

  t = (time_t)(h->time);
  strftime(tmp, sizeof(tmp), "%Y/%m/%d %a %H:%M:%S", localtime(&t));
  fprintf( fp, "time stamp     : %s\n", tmp );
  fprintf( fp, "Endianness     : 0x%x (Correct 0x%x)\n", h->tsign, ZHEADER_TAIL_SIGN );
  fprintf( fp, "Endian state   : ");
  if( zFileHeaderEndianIsEqual( h ) )
    fprintf( fp, "Equal.\n");
  else if( zFileHeaderEndianIsAnother( h ) )
    fprintf( fp, "Another\n");
  else
    fprintf( fp, "Strange or Broken\n");

  fprintf( fp, "--End of header--\n" );
}


void zFileHeaderFWrite(FILE *fp, zHeader *h)
{
  if( zFileHeaderIsNothing( h ) ) {
    eprintf( "Header is Nothing !!\n" );
    return;
  }
  if( !zFileHeaderIsCorrect( h ) )
    eprintf( "Header is BROKEN !!\n" );

  fprintf( fp, "Header         : %s\n", h->head );
  fprintf( fp, "Header size    : %d [byte]\n", h->size );
  fprintf( fp, "Header version : %d\n", h->hver );

  switch( h->hver ){
  case 1: _zFileHeaderFWrite( fp, h ); break;

  default:
    eprintf( "  Header version is Strange !!\n" );
    _zFileHeaderFWrite( fp, h );
  }
}

