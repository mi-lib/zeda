/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * CSV file operations.
 */

#include <zeda/zeda_csv.h>

#ifndef __KERNEL__

/* get a line from the current stream of a CSV file. */
char *zCSVGetLine(zCSV *csv)
{
  char *ret;

  ret = fgets( csv->buf, BUFSIZ, csv->fp );
  zCutNL( csv->buf );
  return ret;
}

/* go to a specified line in a CSV file. */
char *zCSVGoToLine(zCSV *csv, int i)
{
  if( i >= zCSVLineNum(csv) ){
    ZRUNERROR( ZEDA_ERR_CSV_INVALID_LINE, i );
    return NULL;
  }
  fseek( csv->fp, csv->pos[i], SEEK_SET );
  return zCSVGetLine( csv );
}

/* get a field from the current buffer of a CSV file. */
char *zCSVGetField(zCSV *csv, char *field, size_t size)
{
  register int i;
  int len;

  if( zCSVLineIsEmpty( csv ) )
    if( !zCSVGetLine( csv ) ) return NULL;
  len = strlen( csv->buf );
  for( i=0; i<size; i++ ){
    field[i] = csv->buf[i];
    if( field[i] == ',' || field[i] == '\0' ) break;
  }
  field[i] = '\0';
  if( csv->buf[i] == ',' ) i++;
  memmove( csv->buf, csv->buf+i, len-i+1 );
  return field;
}

/* get an integer value from the current buffer of a CSV file. */
int zCSVGetInt(zCSV *csv)
{
  char field[BUFSIZ];

  return zCSVGetField( csv, field, BUFSIZ ) ? atoi( field ) : 0;
}

/* get a double-precision floating-point value from the current buffer of a CSV file. */
double zCSVGetDouble(zCSV *csv)
{
  char field[BUFSIZ];

  return zCSVGetField( csv, field, BUFSIZ ) ? atof( field ) : 0;
}

/* rewind the stream of a CSV file. */
void zCSVRewind(zCSV *csv)
{
  fseek( csv->fp, csv->pos[0], SEEK_SET );
}

/* count the number of fields per line. */
static int _zCSVCountField(zCSV *csv)
{
  char field[BUFSIZ];

  zCSVRewind( csv );
  for( csv->nf=0; !zCSVLineIsEmpty(csv); csv->nf++ )
    zCSVGetField( csv, field, BUFSIZ );
  zCSVRewind( csv );
  return csv->nf;
}

/* open a CSV file. */
zCSV *zCSVOpen(zCSV *csv, char filename[])
{
  register int i;

  if( !( csv->fp = zOpenFile( filename, "csv", "rt" ) ) )
    return NULL;
  for( csv->nl=0; fgets( csv->buf, BUFSIZ, csv->fp ); )
    if( csv->buf[0] != '\%' ) csv->nl++;
  if( !( csv->pos = zAlloc( long, csv->nl ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  rewind( csv->fp );
  for( i=0; i<csv->nl; ){
    csv->pos[i] = ftell( csv->fp );
    if( !fgets( csv->buf, BUFSIZ, csv->fp ) ){
      ZRUNERROR( ZEDA_ERR_CSV_INVALID );
      return NULL;
    }
    if( csv->buf[0] != '\%' ) i++;
  }
  _zCSVCountField( csv );
  memset( csv->buf, 0, BUFSIZ ); /* clear internal buffer */
  return csv;
}

/* close a CSV file. */
void zCSVClose(zCSV *csv)
{
  free( csv->pos );
  fclose( csv->fp );
  csv->nl = csv->nf = 0;
}

#endif /* __KERNEL__ */
