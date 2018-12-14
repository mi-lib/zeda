#include <zeda/zeda_string.h>

typedef struct{
  char name[BUFSIZ];
  int ival[3];
  double dval;
} typeA_t;

typedef struct{
  char name[BUFSIZ];
  int ival;
  double dval[3];
  char msg[BUFSIZ];
} typeB_t;

typedef struct{
  typeA_t a;
  typeB_t b;
} type_t;

bool typea_fread(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "name" ) == 0 ){
    if( !zFToken( fp, ((typeA_t*)instance)->name, BUFSIZ ) ){
      *success = false;
      return false;
    }
  } else
  if( strcmp( buf, "ival" ) == 0 ){
    ((typeA_t*)instance)->ival[0] = zFInt(fp);
    ((typeA_t*)instance)->ival[1] = zFInt(fp);
    ((typeA_t*)instance)->ival[2] = zFInt(fp);
  } else
  if( strcmp( buf, "dval" ) == 0 ){
    ((typeA_t*)instance)->dval = zFDouble(fp);
  } else{
    eprintf( "unknown field: %s\n", buf );
    return false;
  }
  return true;
}

bool typeb_fread(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "name" ) == 0 ){
    if( !zFToken( fp, ((typeB_t*)instance)->name, BUFSIZ ) ){
      *success = false;
      return false;
    }
  } else
  if( strcmp( buf, "ival" ) == 0 ){
    ((typeB_t*)instance)->ival = zFInt(fp);
  } else
  if( strcmp( buf, "dval" ) == 0 ){
    ((typeB_t*)instance)->dval[0] = zFDouble(fp);
    ((typeB_t*)instance)->dval[1] = zFDouble(fp);
    ((typeB_t*)instance)->dval[2] = zFDouble(fp);
  } else
  if( strcmp( buf, "msg" ) == 0 ){
    if( !zFToken( fp, ((typeB_t*)instance)->msg, BUFSIZ ) ){
      *success = false;
      return false;
    }
  } else{
    eprintf( "unknown field: %s\n", buf );
    return false;
  }
  return true;
}

bool type_fread(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "typeA" ) == 0 ){
    if( !zFieldFRead( fp, typea_fread, &((type_t*)instance)->a ) )
      *success = false;
  } else
  if( strcmp( buf, "typeB" ) == 0 ){
    if( !zFieldFRead( fp, typeb_fread, &((type_t*)instance)->b ) )
      *success = false;
  } else
    return false;
  return true;
}

void output(type_t *type)
{
  printf( "<A>\n" );
  printf( " name: %s\n", type->a.name );
  printf( " ival: %d %d %d\n", type->a.ival[0], type->a.ival[1], type->a.ival[2] );
  printf( " dval: %g\n", type->a.dval );
  printf( "<B>\n" );
  printf( " name: %s\n", type->b.name );
  printf( " ival: %d\n", type->b.ival );
  printf( " dval: %g %g %g\n", type->b.dval[0], type->b.dval[1], type->b.dval[2] );
  printf( " msg : %s\n", type->b.msg );
}

int main(void)
{
  FILE *fp;
  type_t instance;

  if( !( fp = fopen( "tag.txt", "r" ) ) ){
    ZOPENERROR( "tag.txt" );
    return 1;
  }
  if( !zTagFRead( fp, type_fread, &instance ) ){
    ZRUNWARN( "persing failure: illegal description of the file" );
  } else
    eprintf( "persing succeeds.\n" );
  fclose( fp );
  output( &instance );
  return 0;
}
