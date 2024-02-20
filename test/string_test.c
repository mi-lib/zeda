#ifndef __WINDOWS__
#include <unistd.h>
#endif /* __WINDOWS__ */

#include <zeda/zeda.h>

void assert_strchr(void)
{
  const char *str = ".,;:/abcde.,;:/abcde.,;:/abcde";

  zAssert( zStrChar (true case), strcmp( zStrChar( str, strlen(str), 'a' ), "abcde.,;:/abcde.,;:/abcde" ) == 0 );
  zAssert( zStrChar (false case1), !zStrChar( str, 5, 'a' ) );
  zAssert( zStrChar (false case2), !zStrChar( str, strlen(str), '+' ) );
  zAssert( zStrrChar (true case), strcmp( zStrrChar( str, strlen(str), 'a' ), "abcde" ) == 0 );
  zAssert( zStrrChar (false case1), !zStrrChar( str, 5, 'a' ) );
  zAssert( zStrrChar (false case2), !zStrrChar( str, strlen(str), '+' ) );
  zAssert( zStrCharNul (true case), strcmp( zStrCharNul( str, strlen(str), 'a' ), "abcde.,;:/abcde.,;:/abcde" ) == 0 );
  zAssert( zStrCharNul (false case), !*zStrCharNul( str, strlen(str), '+' ) );
}

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
  zStrToUpper( str1, BUFSIZ, str2 );
  zAssert( zStrToUpper, strcmp( str2, "ABCDEFGHIJKLMN" ) == 0 );
  zStrToLower( str2, BUFSIZ, str1 );
  zAssert( zStrToLower, strcmp( str1, "abcdefghijklmn" ) == 0 );
}

#define TEST_TXT "string_test.txt"
#define WS_TEST_TXT "string_ws_test.txt"
#define NUM_TEST_TXT "string_num_test.txt"

void assert_token(void)
{
  FILE *fp;
  char buf[BUFSIZ];

  fp = fopen( TEST_TXT, "r" );
  zAssert( zFSkipComment,
    zFSkipComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string1" ) &&
    zFSkipComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string2" ) &&
    zFSkipComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "separated string 3" ) &&
    zFSkipComment( fp ) && zFToken( fp, buf, BUFSIZ ) && !strcmp( buf, "string4" ) );
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

void assert_check_utf_bom(void)
{
#ifndef __WINDOWS__
#define NKF_PATH              "/usr/bin/nkf"

#define UTF_BOM_TEST_STRING           "あいうえお"
#define UTF_BOM_TEST_FILENAME_ORG     "test_ja.txt"
#define UTF_BOM_TEST_FILENAME_UTF8    "test_ja_utf8.txt"
#define UTF_BOM_TEST_FILENAME_UTF16B  "test_ja_utf16b.txt"
#define UTF_BOM_TEST_FILENAME_UTF16L  "test_ja_utf16l.txt"
  char cmd[BUFSIZ];
  FILE *fp;

  if( access( NKF_PATH, X_OK ) != 0 ){
    ZRUNWARN( "%s is not found. skipped", NKF_PATH );
    return;
  }
  /* generate test files */
  sprintf( cmd, "echo %s > %s", UTF_BOM_TEST_STRING, UTF_BOM_TEST_FILENAME_ORG );
  if( system( cmd ) );
  sprintf( cmd, "%s -w8 %s > %s", NKF_PATH, UTF_BOM_TEST_FILENAME_ORG, UTF_BOM_TEST_FILENAME_UTF8 );
  if( system( cmd ) );
  sprintf( cmd, "%s -w16B %s > %s", NKF_PATH, UTF_BOM_TEST_FILENAME_ORG, UTF_BOM_TEST_FILENAME_UTF16B );
  if( system( cmd ) );
  sprintf( cmd, "%s -w16L %s > %s", NKF_PATH, UTF_BOM_TEST_FILENAME_ORG, UTF_BOM_TEST_FILENAME_UTF16L );
  if( system( cmd ) );
  /* assert */
  fp = fopen( UTF_BOM_TEST_FILENAME_UTF8, "r" );
  zAssert( zFCheckUTFBOM (UTF-8), zFCheckUTFBOM( fp ) == ZUTF_TYPE_UTF8 );
  fclose( fp );
  fp = fopen( UTF_BOM_TEST_FILENAME_UTF16B, "r" );
  zAssert( zFCheckUTFBOM (UTF-16 big endian), zFCheckUTFBOM( fp ) == ZUTF_TYPE_UTF16BE );
  fclose( fp );
  fp = fopen( UTF_BOM_TEST_FILENAME_UTF16L, "r" );
  zAssert( zFCheckUTFBOM (UTF-16 little endian), zFCheckUTFBOM( fp ) == ZUTF_TYPE_UTF16LE );
  fclose( fp );
  fp = fopen( UTF_BOM_TEST_FILENAME_ORG, "r" );
  zAssert( zFCheckUTFBOM (unknown case), zFCheckUTFBOM( fp ) == ZUTF_TYPE_NONE );
  fclose( fp );
  /* delete test files */
  unlink( UTF_BOM_TEST_FILENAME_ORG );
  unlink( UTF_BOM_TEST_FILENAME_UTF8 );
  unlink( UTF_BOM_TEST_FILENAME_UTF16B );
  unlink( UTF_BOM_TEST_FILENAME_UTF16L );
#endif /* __WINDOWS__ */
}

bool test_getdirfilename(char *pathname, char *dir, char *file, int ret)
{
  char dirname[BUFSIZ], filename[BUFSIZ];
  int retval;

  retval = zGetDirFilename( pathname, dirname, filename, BUFSIZ );
  return strncmp( dirname, dir, BUFSIZ ) == 0 && strncmp( filename, file, BUFSIZ ) == 0 && retval == ret;
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
  zStrCopy( str1, "C:\\user\\foo\\baa.d\\woo\\yeah.sfx", BUFSIZ );
  zGetBasenameDRC( str1 );
  zAssert( zGetBasenameDRC, strcmp( str1, "yeah" ) == 0 );
  zAssert( zGetDirFilename,
    test_getdirfilename( "C:\\abcde\\fghij\\klmno\\pqr.stu", "C:\\abcde\\fghij\\klmno\\", "pqr.stu", 3 ) &&
    test_getdirfilename( "/usr/local/src/", "/usr/local/src/", "", 2 ) &&
    test_getdirfilename( "/etc/passwd", "/etc/", "passwd", 3 ) &&
    test_getdirfilename( "/root", "/", "root", 3 ) &&
    test_getdirfilename( "vmlinuz-5.4.0-100-generic", "", "vmlinuz-5.4.0-100-generic", 1 ) &&
    test_getdirfilename( "", "", "", 0 ) );
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
  assert_strchr();
  assert_strmanip();
  assert_token();
  assert_num_token();
  assert_check_utf_bom();
  assert_pathname();
  assert_strsearch();
  return EXIT_SUCCESS;
}
