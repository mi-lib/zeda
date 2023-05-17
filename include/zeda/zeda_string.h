/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_string.h
 * \brief string manipulations.
 * \author Zhidao
 */

#ifndef __ZEDA_STRING_H__
#define __ZEDA_STRING_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

#ifndef __KERNEL__

/*! \brief return the null string.
 *
 * zNullStr() returns a pointer to the null string "".
 * \return
 * a pointer to the null string.
 */
__ZEDA_EXPORT char *zNullStr(void);

/*! \brief copy a string without checking the buffer size.
 *
 * zStrCopyNC() copies a string \a src to a buffer pointed by
 * \a dest. It supposes that the destination area pointed by
 * \a dest has enough size.
 */
__ZEDA_EXPORT char *zStrCopyNC(char *dest, const char *src);

/*! \brief copy a string.
 *
 * zStrCopy() copies a string \a src including the terminating
 * null charactor to a buffer pointed by \a dest. The memory areas
 * for \a dest and \a src may overlap. \a size is the size of the
 * buffer pointed by \a dest. If the length of \a src is beyond
 * \a size, it is clamped and terminated by the null charactor.
 */
__ZEDA_EXPORT char *zStrCopy(char *dest, const char *src, size_t size);

/*! \brief clone a string.
 *
 * zStrClone() duplicates a string, namely, allocates memory for
 * a string \a str and copies it there.
 * \return
 * a pointer to the memory allocated.
 * When it fails to allocate memory or \a str is the null pointer,
 * the null pointer is returned.
 */
__ZEDA_EXPORT char *zStrClone(char *str);

/*! \brief concatenate a string to another.
 *
 * zStrCat() concatenates a string \a src to another \a dest.
 * \a size is the size of the memory area pointed by \a dest.
 * If the sum of lengths of \a src and \a dest is beyond \a size,
 * the resulted string is clamped.
 */
__ZEDA_EXPORT char *zStrCat(char *dest, const char *src, size_t size);

/*! \brief concatenate a string with a formatted string.
 *
 * zStrCatPrint() concatenates a formatted string \a fmt to
 * another string \a str. \a size is the maximum number of
 * charactors \a str can store. If the sum of lengths of \a src
 * and \a fmt is beyond \a size, the resulted string is clamped.
 *
 * The format of \a fmt and the following variable arguments
 * conform to printf() family.
 * \return
 * zStrCatPrint() returns a pointer \a str.
 * \note
 * zStrCatPrint() is not sufficiently secure since it internally
 * calls vsprintf(), in which the size of \a fmt is not checked.
 */
__ZEDA_EXPORT char *zStrCatPrint(char *str, size_t size, char *fmt, ...);

/*! \brief find a specified charactor in a string.
 *
 * zStrChar() finds the first occurence of a specified charactor
 * \a c in a string \s. \a size is the maximum size of the buffer
 * where \a s points.
 * \return
 * zStrChar() returns a pointer to the charactor where \a c
 * appears first in \a s. If \a c is not found in \a s, the null
 * pointer is returned.
 * \sa
 * zStrCharNul, zStrrChar
 */
__ZEDA_EXPORT char *zStrChar(const char *s, size_t size, int c);

/*! \brief find a specified charactor in a string.
 *
 * zStrCharNul() finds the first occurence of a specified charactor
 * \a c in a string \s. \a size is the maximum size of the buffer
 * where \a s points.
 * The functions is almost the same with zStrChar() except that
 * it returns the pointer to the last null charactor if \a c is
 * not found in \a s.
 * \return
 * zStrCharNul() returns a pointer to the charactor where \a c
 * appears first in \a s. If \a c is not found in \a s, the
 * pointer to the last null charactor is returned.
 * \sa
 * zStrChar, zStrrChar
 */
__ZEDA_EXPORT char *zStrCharNul(const char *s, size_t size, int c);

/*! \brief find a specified charactor from the last in a string.
 *
 * zStrrChar() finds the last occurence of a specified charactor
 * \a c in a string \s. \a size is the maximum size of the buffer
 * where \a s points.
 * \return
 * zStrrChar() returns a pointer to the charactor where \a c
 * appears the last in \a s. If \a c is not found in \a s, the
 * null pointer is returned.
 * \sa
 * zStrChar, zStrCharNul
 */
__ZEDA_EXPORT char *zStrrChar(const char *s, size_t size, int c);

/*! \brief concatenate a string and a charactor.
 *
 * zStrAddChar() appends a charactor \a c to a string
 * \a str directly. \a size is the size of a buffer
 * assigned to the string.
 * \return
 * a pointer \a str is returned if successful.
 * If the buffer is already exhausted, the null pointer
 * is returned.
 */
__ZEDA_EXPORT char *zStrAddChar(char *str, size_t size, char c);

/*! \brief insert a charactor to a string.
 *
 * zStrInsChar() inserts a charactor \a c to a string
 * \a str. \a size is the size of a buffer assigned to
 * the string. \a cur is the location where \a c is to
 * be inserted.
 * If \a cur is over the length of the string, \a c is
 * appended to the string.
 * \return
 * a pointer to the inserted charactor is returned if
 * successful.
 * If the buffer is already exhausted, the null pointer
 * is returned.
 */
__ZEDA_EXPORT char *zStrInsChar(char *str, size_t size, uint cur, char c);

/*! \brief override a charactor to a string.
 *
 * zStrOvrChar() overrides a charactor \a c to a string
 * \a str. \a size is the size of a buffer assigned to
 * the string. \a cur is the location where \a c is to
 * be overrided.
 * If \a cur is over the length of the string, \a c is
 * appended to the string.
 * \return
 * a pointer to the overridden charactor is returned if
 * successful.
 */
__ZEDA_EXPORT char *zStrOvrChar(char *str, size_t size, uint cur, char c);

/*! \brief delete a charactor from a string.
 *
 * zStrDelChar() deletes a charactor from a string \a str.
 * \a cur is the location where a charactor is to be
 * deleted.
 * \return
 * a pointer to the deleted location is returned.
 */
__ZEDA_EXPORT char *zStrDelChar(char *str, uint cur);

/*! \brief backspace a charactor from a string.
 *
 * zStrBSChar() backspaces a charactor from a string \a str.
 * \a cur is the location where a charactor is to be
 * withdrawn.
 * \return
 * a pointer to the withdrawn location is returned.
 */
__ZEDA_EXPORT char *zStrBSChar(char *str, int cur);

/*! \brief cut a newline charactor.
 *
 * zCutNL() directly cuts a newline charactor at the
 * end of a string pointed by \a str, and puts the
 * null charactor '\\0' there instead.
 * \return
 * a pointer \a str is returned.
 */
__ZEDA_EXPORT char *zCutNL(char *str);

/*! \brief convert a string to the uppercase set.
 *
 * zStrToUpper() copies a string \a str to \a dest with all lowercase alphabets
 * included in \a str converted to uppercase charactors.
 * \return
 * zStrToUpper() returns a pointer \a dest.
 */
__ZEDA_EXPORT char *zStrToUpper(char *src, size_t size, char *dest);

/*! \brief convert a string to the lowercase set.
 *
 * zStrToLower() copies a string \a str to \a dest with all uppercase alphabets
 * included in \a str converted to lowercase charactors.
 * \return
 * zStrToLower() returns a pointer \a dest.
 */
__ZEDA_EXPORT char *zStrToLower(char *src, size_t size, char *dest);

/* ********************************************************** */
/*! \defgroup token tokenization.
 * \{ *//* ************************************************** */

/*! \brief specify a delimiter set.
 *
 * zSetDelimiter() specifies a delimiter set for \a s.
 * It works for tokenization functions.
 * To reset the delimiter set, call zResetDelimiter().
 * \sa
 * zResetDelimiter, zIsDelimiter,
 * zFSkipDelimiter, zSSkipDelimiter
 */
__ZEDA_EXPORT void zSetDelimiter(char s[]);

/*! \brief reset a delimiter set.
 *
 * zResetDelimiter() resets a delimiter set.
 * The default delimiter set includes:
 *  whitespace,
 *  tab \\t,
 *  new line \\n,
 *  carriage return \\r,
 *  comma,
 *  colon :,
 *  semi-colon ;,
 *  vertical bar |,
 *  parenthesis (),
 *  brace {},
 *  null character \\0 and
 *  end of file EOF.
 *
 * They work for tokenization functions.
 * \sa
 * zSetDelimiter, zIsDelimiter,
 * zFSkipDelimiter, zSSkipDelimiter
 */
__ZEDA_EXPORT void zResetDelimiter(void);

/*! \brief specify an operator set.
 *
 * zSetOperator() specifies an operator set for \a s.
 * It works for tokenization functions.
 * To reset the operator set, call zResetOperator().
 * \sa
 * zResetOperator, zIsOperator
 */
__ZEDA_EXPORT void zSetOperator(char s[]);

/*! \brief reset an operator set.
 *
 * zResetOperator() resets an operator set.
 * The default operator set includes:
 *  exclamation !,
 *  percent %,
 *  ampasand &,
 *  asterisk *,
 *  plus +,
 *  minus -,
 *  slash /,
 *  more than <,
 *  equal =,
 *  less than >,
 *  question ?,
 *  at-mark @,
 *  back slash \,
 *  carat ^ and
 *  tilde ~
 *
 * They work for tokenization functions.
 * \sa
 * zSetOperator, zIsOperator
 */
__ZEDA_EXPORT void zResetOperator(void);

/*! \brief check if a charactor is included in a specified set.
 *
 * zIsIncludedChar() checks if a charactor \a c is
 * included in a charactor set \a s.
 * \return if \a c is included in \a s, the true value
 * is returned. Otherwise, the false value is returned.
 * \note
 * The charactor set \a s must be terminated by the null
 * charactor '\\0'. Also, note that \a s is automatically
 * terminated by the null charactor if it is a string.
 */
__ZEDA_EXPORT bool zIsIncludedChar(char c, char *s);

/*! \brief check if a charactor is the whitespace.
 *
 * zIsWS() is a boolean. It is the true value if
 * a charactor \a c is either the whitespace or the
 * tab charactor.
 */
#define zIsWS(c) ( (c) == ' ' || (c) == '\t' )

/*! \brief check if a charactor is a delimiter.
 *
 * zIsDelimiter() checks if a charactor \a c is a
 * delimiter, namely, if \a c is included in the
 * delimiter set defined by zSetDelimiter().
 * \return if \a c is a delimiter, the true value is
 * returned. Otherwise, the false value is returned.
 * \sa zSetDelimiter, zResetDelimiter
 */
__ZEDA_EXPORT bool zIsDelimiter(char c);

/*! \brief check if a charactor is an operator.
 *
 * zIsOperator() checks if a charactor \a c is an
 * operator, namely, if \a c is included in the operator
 * set defined by zSetOperator().
 * \return if \a c is an operator, the true value is
 * returned. Otherwise, the false value is returned.
 * \sa zSetOperator, zResetOperator
 */
__ZEDA_EXPORT bool zIsOperator(char c);

/*! \brief check if a charactor is a quotation mark.
 *
 * zIsQuotation() is a boolean. It is the true value
 * if \a c is either ' or ".
 */
#define zIsQuotation(c) ( (c) == '\'' || (c) == '\"' )

/*! \brief check if a string represents a hexadecimal number.
 *
 * zStrIsHex() checks if a string pointed by \a str
 * represents a hexadecimal number, namely, if \a str
 * consists only of numbers and a-f alphabets.
 * \return the result is returned as a boolean.
 * \note
 * zStrIsHex() does not recognize a signed value in
 * hex-style. Namely, a string beginning with a sign
 * such as -ffff is not regarded as a number.
 */
__ZEDA_EXPORT bool zStrIsHex(char *str);

/*! \brief skip whitespaces in a file.
 *
 * zFSkipWS() skips whitespaces in a file \a fp from
 * the current position.
 * \return
 * zFSkipWS() returns the first charactor which is
 * not a whitespace appeared in \a fp. If the file
 * reaches EOF, the null charactor '\\0' is returned.
 */
__ZEDA_EXPORT char zFSkipWS(FILE *fp);

/*! \brief skip whitespaces in a string.
 *
 * zSSkipWS() skips whitespaces in a string \a str
 * from the current position.
 * \return
 * zSSkipWS() returns a pointer to the first charactor
 * which is not a whitespace appeared in \a str. If
 * all the rest charactors of \a str are whitespaces,
 * the null pointer is returned.
 */
__ZEDA_EXPORT char *zSSkipWS(char *str);

/*! \brief skip a certain charactors in a stream.
 *
 * zFSkipIncludedChar() skips charactors in a specified
 * set \a s from the current position of a file referred
 * by \a fp.
 * \return
 * zFSkipIncludedChar() returns the first charactor which
 * is not included \a s. If the file reaches EOF, the null
 * charactor '\\0' is returned.
 */
__ZEDA_EXPORT char zFSkipIncludedChar(FILE *fp, char *s);

/*! \brief skip a certain charactors in a string.
 *
 * zSSkipIncludedChar() skips charactors in a specified
 * set \a s from the current position of a string \a str.
 * \return
 * zSSkipIncludedChar() returns a pointer to the first
 * charactor which is not included in \a s. If all the
 * rest charactors of \a str are whitespaces, the null
 * pointer is returned.
 */
__ZEDA_EXPORT char *zSSkipIncludedChar(char *str, char *s);

/*! \brief skip delimiter in a file.
 *
 * zFSkipDelimiter() skips delimiters in a file \a fp
 * from the current position.
 * \return
 * zFSkipDelimiter() returns the first charactor which
 * is not a delimiter appeared in \a fp. If the file
 * reaches EOF, the null charactor '\\0' is returned.
 */
__ZEDA_EXPORT char zFSkipDelimiter(FILE *fp);

/*! \brief skip delimiter in a string.
 *
 * zSSkipDelimiter() skips delimiters in a string
 * \a str from the current position.
 * \return
 * zSSkipDelimiter() returns a pointer to the first
 * charactor which is not a delimiter appeared in
 * \a str. If all the rest charactors of \a str are
 * whitespaces, the null pointer is returned.
 */
__ZEDA_EXPORT char *zSSkipDelimiter(char *str);

#define ZDEFAULT_COMMENT_IDENT '%'

/*! \brief specify the comment identifier. */
__ZEDA_EXPORT void zSetCommentIdent(char ident);

/*! \brief reset the comment identifier. */
__ZEDA_EXPORT void zResetCommentIdent(void);

/*! \brief skip comments.
 *
 * zFSkipComment() skips comments, i.e., a one-line string which
 * begins with a specified identifier, in a file \a fp.
 * Vague lines are also skipped.
 * The identifier can be set by zSetCommentIdent() and be reset
 * by zResetCommentIdent().
 * \return
 * zFSkipComment() returns the next charactor to the last comment,
 * which is put back to the file.
 * If the file reaches EOF, the null charactor is returned.
 * \sa
 * zSetCommentIdent, zResetCommentIdent
 */
__ZEDA_EXPORT char zFSkipComment(FILE *fp);

/*! \brief tokenize a file.
 *
 * zFToken() acquires a first token in a file \a fp
 * appearing from the current position, and copies it
 * where \a tkn points. Token is a charactor set
 * segmented by a delimiter, charactors enclosed
 * in quotation marks or double quotation marks.
 * The token size is limited up to \a size.
 * If the first quotation mark does not have a
 * corresponding closing mark, the token buffer is
 * filled with the rest charactors in the file.
 * \return
 * zFToken() returns a pointer \a tkn.
 * If a token is not found, the null pointer is
 * returned.
 * \sa zSetDelimiter, zIsDelimiter, zSToken
 * \note
 * If the size of buffer pointed by \a tkn is less
 * than \a size, anything might happen.
 */
__ZEDA_EXPORT char *zFToken(FILE *fp, char *tkn, size_t size);

/*! \brief tokenize a string.
 *
 * zSToken() acquires the first token in a string \a str and
 * copies it where \a tkn points. Token is a charactor set
 * segmented by a delimiter, charactors enclosed in quotation
 * marks or double quotation marks. The token size is limited
 * up to \a size. If the first quotation mark does not have
 * a corresponding closing mark, the token buffer is filled
 * with the rest charactors in the string.
 * \a str is overridden by the remaining string destructively.
 *
 * zSTokenSkim() also acquires the first token in a string
 * \a str and copies it where \a tkn points. The difference
 * from zSToken() is that it returns a pointer immediately
 * after \a tkn in \a str and does not destroy \a str.
 * \return
 * zSToken() returns a pointer \a tkn. If a token is
 * not found, the null pointer is returned.
 *
 * zSTokenSkim() returns a pointer immediately after \a tkn,
 * which corresponds to a charactor in \a str.
 * \sa zSetDelimiter, zIsDelimiter, zFToken
 * \note
 * If the size of buffer pointed by \a tkn is less
 * than \a size, anything might happen.
 */
__ZEDA_EXPORT char *zSTokenSkim(char *str, char *tkn, size_t size);
__ZEDA_EXPORT char *zSToken(char *str, char *tkn, size_t size);

/*! \brief tokenize an integer value in a file.
 *
 * zFIntToken() tokenizes an integer value in a file
 * \a fp from the current position, and puts it where
 * \a tkn points. \a size is the size of \a tkn. The
 * charactors beyond \a size are truncated.
 * \return
 * the pointer \a tkn is returned.
 */
__ZEDA_EXPORT char *zFIntToken(FILE *fp, char *tkn, size_t size);

/*! \brief tokenize a real number in a file.
 *
 * zFNumToken() tokenizes a real number in a file
 * \a fp from the current position, and puts it where
 * \a tkn points. \a size is the size of \a tkn. The
 * charactors beyond \a size are truncated.
 * \return
 * the pointer \a tkn is returned.
 */
__ZEDA_EXPORT char *zFNumToken(FILE *fp, char *tkn, size_t size);

/*! \brief tokenize an integer value in a string.
 *
 * zSIntToken() tokenizes an integer value in a string
 * \a str, and puts it where \a tkn points. \a size is
 * the size of \a tkn. The charactors beyond \a size
 * are truncated.
 * \a str is overridden by the remaining string
 * destructively.
 * \return
 * the pointer \a tkn is returned.
 */
__ZEDA_EXPORT char *zSIntToken(char *str, char *tkn, size_t size);

/*! \brief tokenize a real number in a string.
 *
 * zSNumToken() tokenizes a real number in a string
 * \a str, and puts it where \a tkn points. \a size is
 * the size of \a tkn. The charactors beyond \a size
 * are truncated.
 * \a str is overridden by the remaining string
 * destructively.
 * \return
 * the pointer \a tkn is returned.
 */
__ZEDA_EXPORT char *zSNumToken(char *str, char *tkn, size_t size);

/*! \brief get an integer value from file.
 *
 * zFInt() acquires an integer value in a file \a fp
 * from the current position. If no integer value is
 * recognized at the current position, zero is returned.
 */
__ZEDA_EXPORT char *zFInt(FILE *fp, int *val);

/*! \brief get an integer value from a string.
 *
 * zSInt() acquires an integer value in a string \a str.
 * If no integer value is recognized at the head of the
 * string, zero is returned.
 */
__ZEDA_EXPORT char *zSInt(char *str, int *val);

/*! \brief get a d-float value in a file.
 *
 * zFDouble() acquires a double-precision floating-point
 * value in a file \a fp from the current position.
 * If no value is recognized at the current position,
 * zero is returned.
 */
__ZEDA_EXPORT char *zFDouble(FILE *fp, double *val);

/*! \brief get a d-float value in a string.
 *
 * zSDouble() acquires a double-precision floating-point
 * value in a string \a str. If no value is recognized
 * at the head of the string, zero is returned.
 */
__ZEDA_EXPORT char *zSDouble(char *str, double *val);

/*! \} */

#define ZDEFAULT_TAG_BEGIN_IDENT '['
#define ZDEFAULT_TAG_END_IDENT   ']'

/*! \brief specify the tag identifiers. */
__ZEDA_EXPORT void zSetTagIdent(char begin_ident, char end_ident);

/*! \brief reset the tag identifiers. */
__ZEDA_EXPORT void zResetTagIdent(void);

/*! \brief check if a token is a tag.
 *
 * zTokenIsTag() checks if a token pointed by \a tkn
 * is a tag, namely, \a tkn is bracketed or not.
 * \return
 * a boolean value is returned.
 */
__ZEDA_EXPORT bool zTokenIsTag(char *tkn);

/*! \brief extract a tag.
 *
 * zExtractTag() extracts a keyword from a bracketted
 * token \a tag, and puts it where \a notag points.
 * ex. suppose \a tag is "[xxx]", then \a notag is "xxx".
 * \return
 * a pointer \a notag is returned.
 * \note
 * \a notag is allowed to be the same with \a tag.
 * Namely, zExtractTag( str, str ) is a valid expression.
 */
__ZEDA_EXPORT char *zExtractTag(char *tag, char *notag);

/*! \brief count a number of tag tokens in a file.
 *
 * zFCountTag() counts bracketed tokens with a keyword
 * \a tag in a file \a fp.
 * After counting, the current position of \a fp is
 * rewinded to the head of file.
 * \return
 * The number of the counted tags in the file is returned.
 * \sa
 * zFCountKey
 */
__ZEDA_EXPORT int zFCountTag(FILE *fp, char *tag);

/*! \brief scan tagged fields in a file.
 *
 * zTagFScan() scans all tagged fields in a file \a fp.
 * \a tag_fscan defines persing rules to accept some tags.
 * \a instance is a pointer to an instance of a specific class
 * to be processed.
 *
 * \a fp and \instance are given to \a tag_fscan as the first and
 * second arguments, respectively. The third argument of \a tag_fscan
 * is for a buffer that stores the token previously scanned, which
 * may be used to further tokenize in \a tag_fscan. It is up to
 * programmers whether they use the fourth argument. It can be
 * utilized to catch something illegal in \a tag_fscan and try
 * any exceptions. It is returned by zTagFScan().
 * \return
 * zTagFScan() returns the fourth value of \a tag_fscan, which may
 * catch something illegal in the persing process. At default,
 * the true value is returned.
 */
__ZEDA_EXPORT bool zTagFScan(FILE *fp, bool (* tag_fscan)(FILE*,void*,char*,bool*), void *instance);

#define ZDEFAULT_KEY_IDENT ':'

/*! \brief specify the key identifier. */
__ZEDA_EXPORT void zSetKeyIdent(char ident);

/*! \brief reset the key identifier. */
__ZEDA_EXPORT void zResetKeyIdent(void);

/*! \brief  check if the last token is a key. */
__ZEDA_EXPORT bool zFPostCheckKey(FILE *fp);

/*! \brief count keywords in a file.
 *
 * zFCountKey() counts keywords which coincide with
 * a string pointed by \a key in a file \a fp between a
 * tag and the next tag.
 * After counting, the current position of \a fp is
 * rewinded where it pointed before counting.
 * \return
 * The number of the counted keywords is returned.
 * \sa
 * zFCountTag
 */
__ZEDA_EXPORT int zFCountKey(FILE *fp, char *key);

/*! \brief scan a field in a file.
 *
 * zFieldFScan() scans a field in a file pointed by \a fp from the
 * current position. \a field_fscan defines persing rules to process
 * a specific class. \a instance is a pointer to an instance of the
 * specific class.
 *
 * \a fp and \instance are given to \a field_fscan as the first
 * and second arguments, respectively. The third argument of
 * \a field_fscan is for a buffer that stores the token previously
 * scanned, which may be used to further tokenize in \a field_fscan.
 * It is up to programmers whether they use the fourth argument.
 * It can be utilized to catch something illegal in \a field_fscan
 * and try any exceptions. It is returned by zFieldFScan().
 * \return
 * zFieldFScan() returns the fourth value of \a field_fscan, which
 * may catch something illegal in the persing process. At default,
 * the true value is returned.
 */
__ZEDA_EXPORT bool zFieldFScan(FILE *fp, bool (* field_fscan)(FILE*,void*,char*,bool*), void *instance);

#endif /* __KERNEL__ */

/*! \brief insert whitespaces before a string.
 *
 * zFIndent() inserts \a n whitespaces at the current
 * position of a file \a fp.
 * For an indentation of the output to the standard
 * output, use zIndent().
 */
#ifndef __KERNEL__
__ZEDA_EXPORT void zFIndent(FILE *fp, int n);
#define zIndent(n) zFIndent( stdout,(n) )
#else
void zIndent(int n);
#endif /* __KERNEL__ */

/*! \brief output a comma to the standard output.
 */
#ifndef __KERNEL__
#define zFComma(fp) fprintf( fp, ", " )
#define zComma()    zFComma( stdout )
#else
#define zComma()    printk( ", " )
#endif /* __KERNEL__ */

/*! \brief output a newline charactor to the standard output.
 */
#ifndef __KERNEL__
#define zFEndl(fp) fprintf( fp, "\n" )
#define zEndl()    zFEndl( stdout )
#else
#define zEndl() printk( "\n" )
#endif /* __KERNEL__ */

/* ********************************************************** */
/*! \defgroup pathname pathname operations.
 * \{ *//* ************************************************** */

#ifndef __KERNEL__
/*! \brief strip a directory from a filename.
 *
 * zGetBasename() acquires a filename from a path string \a path
 * with any leading directory names removed, and puts it where
 * \a name points.
 * ex. if \a path is "/home/user/dummy.suf", \a name will be
 * "dummy.suf".
 *
 * zGetBasenameDRC() overrides a given string \a name by the
 * stripped string.
 * \return
 * zGetBasename() and zGetBasenameDRC() return a pointer \a name.
 * \note
 * Both '/' and '\\' are accepted as delimiters of the path name.
 */
__ZEDA_EXPORT char *zGetBasename(char *path, char *name, size_t size);
__ZEDA_EXPORT char *zGetBasenameDRC(char *name);

/*! \brief get a directory name and a file name from a path.
 *
 * zGetDirFilename() acquires a directory name and a file name
 * from a path \a path, and puts them into \a dirname and \a filename,
 * respectively. More than or equal to \a size bytes have to be
 * allocated for all \a path, \a dirname and \a filename.
 * \return
 * zGetDirFilename() returns an integer value from 0 to 3. If
 * \a filename is non-empty, the least bit of the value turns 1.
 * If \a dirname is non-empty, the second least bit of the value
 * turns 1. Otherwise, the corresponding bit turns 0.
 */
__ZEDA_EXPORT int zGetDirFilename(char *path, char *dirname, char *filename, size_t size);

/*! \brief get a suffix from a filename.
 *
 * zGetSuffix() finds a suffix from a filename \a name.
 * The string pointed by \a name is not modified.
 * \return
 * a pointer to the head of suffix is returned.
 * ex. if \a name is "xxx.yyy.zzz", then the address
 * of the first "z" is returned.
 */
__ZEDA_EXPORT char *zGetSuffix(char *name);

/*! \brief add a suffix to a filename.
 *
 * zAddSuffix() concatenates '.' + a string \a suffix
 * to the original filename \a org, if necessary.
 * The resultant pathname is put where \a dest points.
 * If \a org already has the same suffix with \a suffix,
 * \a org is simply copied to \a dest.
 * \a size is the size of the destination buffer.
 * \return
 * a pointer \a dest is returned.
 * \note
 * \a dest must have enough size for \a org, '.' and
 * the additional \a suffix.
 */
__ZEDA_EXPORT char *zAddSuffix(char *org, const char *suffix, char *dest, size_t size);

/*! \brief replace the suffix of a filename to a specified one.
 *
 * zReplaceSuffix() replaces the suffix of a string \a org, which is
 * supposed to be a name of file with '.' + a string \a suffix.
 * The resulted pathname is put where \a dest points.
 * If \a org does not have a suffix, it simply concatenates \a suffix
 * to \a org.
 * \a size is the size of the destination buffer.
 * \return
 * a pointer \a dest is returned.
 * \note
 * \a dest must have enough size for the suffix-exempt \a org, '.'
 * and \a suffix.
 */
__ZEDA_EXPORT char *zReplaceSuffix(char *org, char *suffix, char *dest, size_t size);

/*! \brief cut a suffix from a filename
 *
 * zCutSuffix() directly cuts a suffix from a filename
 * \a name. If \a name doesn't have a suffix, nothing
 * happens.
 * \return
 * a pointer to the cut suffix in the original \a name.
 * ex. if \a name is "xxx.yyy.zzz", then \a name will
 * be modified to "xxx.yyy", and a pointer to the first
 * "z" is returned.
 */
__ZEDA_EXPORT char *zCutSuffix(char *name);

/*! \brief open a file with default suffix.
 *
 * zOpenFile() opens a file named \a filename. \a mode
 * is an access mode to the file. The definition is the
 * same with that for fopen() in ANSI-C.
 * If it fails to open \a filename and \a suffix is not
 * the null pointer, it tries to open file named
 * '\a filename '.'\a suffix '.
 * \return
 * zOpenFile() returns a pointer to the opened file if
 * succeeds. Otherwise, the null pointer is returned.
 */
__ZEDA_EXPORT FILE *zOpenFile(char filename[], const char *suffix, const char *mode);

/*! \brief default suffix of .ztk (Z-Tag-and-Key) file format. */
#define ZEDA_ZTK_SUFFIX "ztk"

/*! \brief open a .ztk file. */
#define zOpenZTKFile(filename,mode) zOpenFile( filename, ZEDA_ZTK_SUFFIX, mode )

/*! \} */

/* ********************************************************** */
/*! \defgroup strsearch string search operations.
 * \{ *//* ************************************************** */

/*! \brief search string by Knuth-Morris-Pratt algorithm
 *
 * zStrSearchKMP() searches a string pattern \a pat in another
 * string \a text based on Knuth-Morris-Pratt algorithm.
 * \return
 * zStrSearchKMP() returns a pointer to the first charactor of
 * the pattern in \a text which coincides with \a pat. If \a pat
 * is not included in \a text, the null pointer is returned.
 */
__ZEDA_EXPORT char *zStrSearchKMP(char *text, char *pat);

/*! \brief string search by Boyer-Moore algorithm
 *
 * zStrSearchBM() searches a string pattern \a pat in another
 * string \a text based on Boyer-Moore algorithm.
 * \return
 * zStrSearchBM() returns a pointer to the first charactor of
 * the pattern in \a text which coincides with \a pat. If \a pat
 * is not included in \a text, the null pointer is returned.
 */
__ZEDA_EXPORT char *zStrSearchBM(char *text, char *pat);

/*! \} */

#endif /* __KERNEL__ */

__END_DECLS

#endif /* __ZEDA_STRING_H__ */
