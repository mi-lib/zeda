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
  fsetpos( csv->fp, &csv->pos[i] );
  return zCSVGetLine( csv );
}

/* get a field from the current buffer of a CSV file. */
char *zCSVGetField(zCSV *csv, char *field, size_t size)
{
  uint i, len;

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

/* skip a field from the current buffer of a CSV file. */
char *zCSVSkipField(zCSV *csv)
{
  char buf[BUFSIZ];

  return zCSVGetField( csv, buf, BUFSIZ );
}

/* get an integer value from the current buffer of a CSV file. */
bool zCSVGetInt(zCSV *csv, int *val)
{
  char field[BUFSIZ];

  if( !zCSVGetField( csv, field, BUFSIZ ) ){
    ZRUNWARN( ZEDA_WARN_CSV_FIELD_EMPTY );
    return false;
  }
  *val = atoi( field );
  return true;
}

/* get multiple integer values from the current buffer of a CSV file. */
bool zCSVGetIntN(zCSV *csv, int val[], int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zCSVGetInt( csv, &val[i] ) ) return false;
  return true;
}

/* get a double-precision floating-point value from the current buffer of a CSV file. */
bool zCSVGetDouble(zCSV *csv, double *val)
{
  char field[BUFSIZ];

  if( !zCSVGetField( csv, field, BUFSIZ ) ){
    ZRUNWARN( ZEDA_WARN_CSV_FIELD_EMPTY );
    return false;
  }
  *val = atof( field );
  return true;
}

/* get multiple double-precision floating-point values from the current buffer of a CSV file. */
bool zCSVGetDoubleN(zCSV *csv, double val[], int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zCSVGetDouble( csv, &val[i] ) ) return false;
  return true;
}

/* rewind the stream of a CSV file. */
void zCSVRewind(zCSV *csv)
{
  fsetpos( csv->fp, &csv->pos[0] );
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
  int i;

  if( !( csv->fp = zOpenFile( filename, (char *)"csv", (char *)"rt" ) ) )
    return NULL;
  for( csv->nl=0; fgets( csv->buf, BUFSIZ, csv->fp ); )
    if( csv->buf[0] != '\%' ) csv->nl++;
  if( !( csv->pos = zAlloc( fpos_t, csv->nl ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  rewind( csv->fp );
  for( i=0; i<csv->nl; ){
    fgetpos( csv->fp, &csv->pos[i] );
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
