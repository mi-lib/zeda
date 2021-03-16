#include <zeda/zeda.h>

char *generate_string_one(int len)
{
  int i;
  char *str;

  len = zRandI( 1, len );
  if( !( str = malloc( sizeof(char)*len ) ) ) return NULL;
  for( i=0; i<len; i++ ){
    str[i] = zRandI( '!', '~' );
  }
  str[i] = '\0';
  return str;
}

bool generate_strings(char *sp[], int n, int len)
{
  int i;

  for( i=0; i<n; i++ ){
    if( !( sp[i] = generate_string_one( len ) ) ) return false;
  }
  return true;
}

void destroy_strings(char *sp[], int n)
{
  int i;

  for( i=0; i<n; i++ ) free( sp[i] );
}

void assert_strlist_add(zStrList *list, char *sp[], int n)
{
  int i;

  zListInit( list );
  for( i=0; i<n; i++ ){
    zStrListAdd( list, sp[i] );
  }
  zAssert( zStrListAdd, zListSize(list) == n );
}

void assert_strlist_find(zStrList *list, char *sp[], int n)
{
  int i;
  bool result = true;
  char dummy[] = "dummy";

  for( i=0; i<n; i++ )
    if( !zStrListFind( list, sp[i] ) ) result = false;
  if( zStrListFind( list, dummy ) ) result = false;
  if( zStrListFind( list, NULL ) ) result = false;
  zAssert( zStrListFind, result );
}

void assert_strlist_getptr(zStrList *list, char *sp[], int n)
{
  char *str1, *str2, *str3;

  if( n < 3 ){
    ZRUNWARN( "less number of strings to be tested" );
    return;
  }
  zStrListGetPtr( list, 3, &str1, &str2, &str3 );
  zAssert( zStrListGetPtr, !strcmp( str1, sp[0] ) && !strcmp( str2, sp[1] ) & !strcmp( str3, sp[2] ) );
}

#define N   100
#define LEN  10

int main(void)
{
  zStrList list;
  char *sp[N];

  if( !generate_strings( sp, N, LEN ) ){
    ZALLOCERROR();
    return 1;
  }
  assert_strlist_add( &list, sp, N );
  assert_strlist_find( &list, sp, N );
  assert_strlist_getptr( &list, sp, N );

  zStrListDestroy( &list );
  destroy_strings( sp, N );
  return 0;
}
