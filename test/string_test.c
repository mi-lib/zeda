#include <zeda/zeda.h>

void assert_strmanip(void)
{
  char str1[BUFSIZ], str2[BUFSIZ], *strp;

  zStrCopy( str1, "abcdefg", BUFSIZ );
  zStrCopy( str2, str1, BUFSIZ );
  zAssert( zStrCopy, strcmp( str1, str2 ) == 0 );
  strp = zStrClone( str1 );
  zAssert( zStrClone, strcmp( strp, str2 ) == 0 );
  free( strp );

  zStrCopy( str2, "hijklmn", BUFSIZ );
  zStrCat( str1, str2, BUFSIZ );
  zAssert( zStrCat, strcmp( str1, "abcdefghijklmn" ) == 0 );
  zStrAddChar( str1, BUFSIZ, 'o' );
  zAssert( zStrAddChar, strcmp( str1, "abcdefghijklmno" ) == 0 );
  zStrInsChar( str1, BUFSIZ, 7, '*' );
  zAssert( zStrInsChar, strcmp( str1, "abcdefg*hijklmno" ) == 0 );
  zStrOvrChar( str1, BUFSIZ, 7, '-' );
  zAssert( zStrOvrChar, strcmp( str1, "abcdefg-hijklmno" ) == 0 );
  zStrDelChar( str1, 7 );
  zAssert( zStrDelChar, strcmp( str1, "abcdefghijklmno" ) == 0 );
  zStrBSChar( str1, 100 );
  zAssert( zStrBSChar, strcmp( str1, "abcdefghijklmn" ) == 0 );
  zAssert( zIsIncludedChar, zIsIncludedChar('a',str1) && !zIsIncludedChar('z',str1) );
  zToUpper( str1, str2 );
  zAssert( zToUpper, strcmp( str2, "ABCDEFGHIJKLMN" ) == 0 );
  zToLower( str2, str1 );
  zAssert( zToLower, strcmp( str1, "abcdefghijklmn" ) == 0 );
}

#define TEST_TXT "string_test.txt"
#define WS_TEST_TXT "string_ws_test.txt"
#define NUM_TEST_TXT "string_num_test.txt"

void assert_token(void)
{
  FILE *fp;
  char buf[BUFSIZ];

  fp = fopen( TEST_TXT, "r" );
  zAssert( zFSkipDefaultComment,
    zFSkipDefaultComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string1" ) &&
    zFSkipDefaultComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string2" ) &&
    zFSkipDefaultComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "separated string 3" ) &&
    zFSkipDefaultComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string4" ) );
  fclose( fp );

  fp = fopen( WS_TEST_TXT, "r" );
  zAssert( zFSkipWS,
    zFSkipWS( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "abcde" ) &&
    zFSkipWS( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "fghij" ) &&
    zFSkipWS( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "klmno" ) &&
    zFSkipWS( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "p" ) );
  fclose( fp );

  zAssert( zStrIsHex,
    zStrIsHex( "0123456789abcdef" ) &&
    zStrIsHex( "0123456789ABCDEF" ) &&
    !zStrIsHex( "0 1 2 3 4 5 6 7 8 9 A B C D E F" ) &&
    !zStrIsHex( "abcdefg" ) );
  zAssert( zTokenIsTag, zTokenIsTag( "[tag]" ) );
  zAssert( zExtractTag, zExtractTag( "[tag]", buf ) && !strcmp( buf, "tag" ) );
}

void assert_num_token(void)
{
  FILE *fp;
  char buf[BUFSIZ], tkn[BUFSIZ];
  int i;
  bool result;
  double answer[] = {
    1234.0,
       0.01234,
       0.1234,
      -0.1234,
       0.0,
       0.0,
       0.12,
       0.12e34,
      -0.12e34,
      -0.1e-34,
       0.12e-34,
       0.12e-34,
      -0.12e-34,
      -0.12e-34,
      -0.12e-34,
       0.0,
       0.0,
       0.0,
    1234.0,
  };

  fp = fopen( NUM_TEST_TXT, "r" );
  for( result=true, i=0; ; i++ ){
    zFNumToken( fp, tkn, BUFSIZ );
    if( !fgets( buf, BUFSIZ, fp ) ) break;
    if( atof(tkn) != answer[i] ) result = false;
    if( !zFSkipDelimiter( fp ) ) break;
  }
  zAssert( zFNumToken, result );

  rewind( fp );
  for( result=true, i=0; ; i++ ){
    if( !fgets( buf, BUFSIZ, fp ) ) break;
    zCutNL( buf );
    zSNumToken( buf, tkn, BUFSIZ );
    if( atof(tkn) != answer[i] ) result = false;
  }
  zAssert( zSNumToken, result );

  fclose( fp );
}

void assert_pathname(void)
{
  char str1[BUFSIZ], str2[BUFSIZ];

  zAssert( zGetSuffix, strcmp( zGetSuffix( "path/test.dummy.suffix" ), "suffix" ) == 0 );
  zAssert( zAddSuffix, strcmp( zAddSuffix( "path/test.dummy", "suffix", str1, BUFSIZ ), "path/test.dummy.suffix" ) == 0 &&
                       strcmp( zAddSuffix( "path/test.suffix", "suffix", str2, BUFSIZ ), "path/test.suffix" ) == 0 );
  zAssert( zReplaceSuffix, strcmp( zReplaceSuffix( str2, "sfx", str1, BUFSIZ ), "path/test.sfx" ) == 0 );

  zGetBasename( "C:\\user\\foo\\baa.d\\woo\\yeah.sfx", str1, BUFSIZ );
  zGetBasename( "C:\\user\\foo\\baa.d\\woo\\yeah", str2, BUFSIZ );
  zAssert( zGetBasename, strcmp( str1, "yeah" ) == 0 && strcmp( str2, "yeah" ) == 0 );
  zStrCopy( str1, "path/test.dummy.suffix", BUFSIZ );
  zCutSuffix( str1 );
  zAssert( zCutSuffix, strcmp( str1, "path/test.dummy" ) == 0 );
}

void assert_strsearch(void)
{
  char *str = "abcdefgabcdefeabcedfg";

  zAssert( zStrSearchKMP,
    zStrSearchKMP( str, "def" ) == &str[3] &&
    zStrSearchKMP( str, "defe" ) == &str[10] &&
    zStrSearchKMP( str, "gabd" ) == NULL );
  zAssert( zStrSearchBM,
    zStrSearchBM( str, "def" ) == &str[3] &&
    zStrSearchBM( str, "defe" ) == &str[10] &&
    zStrSearchBM( str, "gabd" ) == NULL );
}

int main(void)
{
  assert_strmanip();
  assert_token();
  assert_num_token();
  assert_pathname();
  assert_strsearch();
  return EXIT_SUCCESS;
}
