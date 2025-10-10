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
__ZEDA_EXPORT char *zStrClone(const char *str);

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
__ZEDA_EXPORT char *zStrToUpper(const char *src, size_t size, char *dest);

/*! \brief convert a string to the lowercase set.
 *
 * zStrToLower() copies a string \a str to \a dest with all uppercase alphabets
 * included in \a str converted to lowercase charactors.
 * \return
 * zStrToLower() returns a pointer \a dest.
 */
__ZEDA_EXPORT char *zStrToLower(const char *src, size_t size, char *dest);

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
__ZEDA_EXPORT void zSetDelimiter(const char s[]);

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
__ZEDA_EXPORT void zSetOperator(const char s[]);

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
__ZEDA_EXPORT bool zIsIncludedChar(char c, const char *s);

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
__ZEDA_EXPORT bool zStrIsHex(const char *str);

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
__ZEDA_EXPORT char zFSkipIncludedChar(FILE *fp, const char *s);

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
__ZEDA_EXPORT char *zSSkipIncludedChar(char *str, const char *s);

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

/* for ZTK (Z's tag-and-key) format */

#define ZDEFAULT_TAG_BEGIN_IDENT '['
#define ZDEFAULT_TAG_END_IDENT   ']'

/*! \brief specify the tag identifiers. */
__ZEDA_EXPORT void zSetTagIdent(char begin_ident, char end_ident);

/*! \brief reset the tag identifiers. */
__ZEDA_EXPORT void zResetTagIdent(void);

/*! \brief check if a string is a tag.
 *
 * zStrIsTag() checks if a token pointed by \a str is a tag, namely, \a str is bracketed by [] or not.
 * \return
 * a boolean value is returned.
 */
__ZEDA_EXPORT bool zStrIsTag(const char *str);

/*! \brief extract a tag.
 *
 * zExtractTag() extracts a keyword from a bracketted token \a tag, and puts it where \a notag points.
 * ex. suppose \a tag is "[xxx]", then \a notag is "xxx".
 * \return
 * a pointer \a notag is returned.
 * \note
 * \a notag is allowed to be the same with \a tag.
 * Namely, zExtractTag( str, str ) is a valid expression.
 */
__ZEDA_EXPORT char *zExtractTag(char *tag, char *notag);

#define ZDEFAULT_KEY_IDENT ':'

/*! \brief specify the key identifier. */
__ZEDA_EXPORT void zSetKeyIdent(char ident);

/*! \brief reset the key identifier. */
__ZEDA_EXPORT void zResetKeyIdent(void);

/*! \brief  check if the last token is a key. */
__ZEDA_EXPORT bool zFPostCheckKey(FILE *fp);

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
 * zSToken() acquires the first token in a string \a str and copies it where \a tkn points.
 * Token is a set of charactors segmented by delimiters, or charactors enclosed in single/double
 * quotation marks. \a size is the upper bound of the size of \a tkn. If the first quotation
 * mark does not have the corresponding closing mark, the rest of charactors in \a str are
 * copied to \a tkn.
 * \a str is overridden by the remaining string destructively.
 *
 * zSTokenSkim() also acquires the first token in \a str and copies it where \a tkn points.
 * The difference from zSToken() is that it returns a pointer immediately after the token in
 * \a str and does not destroy \a str.
 * \return
 * zSToken() always returns a pointer \a tkn.
 *
 * zSTokenSkim() returns a pointer immediately after the token  in \a str.
 * \sa zSetDelimiter, zIsDelimiter, zFToken
 * \note
 * If the size of the buffer pointed by \a tkn is less than \a size, the token is truncated.
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
 * zFInt() reads an integer value from the current position of a file \a fp. The value is stored where
 * \a val points.
 * \return
 * zFInt() returns the true value if it succeeds to read a value. If no value is found, the false value
 * is returned.
 */
__ZEDA_EXPORT bool zFInt(FILE *fp, int *val);

/*! \brief get an integer value from a string.
 *
 * zSInt() reads an integer value from the current position of a string \a str. The value is stored where
 * \a val points. \a str is destructively updated to skip the corresponding value part.
 * \return
 * zSInt() returns the true value if it succeeds to read a value. If no value is found, the false value
 * is returned.
 */
__ZEDA_EXPORT bool zSInt(char *str, int *val);

/*! \brief get a double-precision floating-point value in a file.
 *
 * zFDouble() reads a double-precision floating-point value from the current position of a file \a fp.
 * The value is stored where \a val points.
 * \return
 * zFDouble() returns the true value if it succeeds to read a value. If no value is found, the false value
 * is returned.
 */
__ZEDA_EXPORT bool zFDouble(FILE *fp, double *val);

/*! \brief get a double-precision floating-point value in a string.
 *
 * zSDouble() reads a double-precision floating-point value from the current position of a string \a str.
 * The value is stored where \a val points. \a str is destructively updated to skip the corresponding
 * value part.
 * \return
 * zSDouble() returns the true value if it succeeds to read a value. If no value is found, the false value
 * is returned.
 */
__ZEDA_EXPORT bool zSDouble(char *str, double *val);

/*! \} */

/*! \brief byte order marker of UTF-8/16 encode files. */
typedef enum{
  ZUTF_TYPE_NONE    =-1,
  ZUTF_TYPE_UTF8    = 0,
  ZUTF_TYPE_UTF16BE = 1,
  ZUTF_TYPE_UTF16LE = 2,
} zUTFType;

/*! \brief check byte order marker of UTF-8/16 encode files. */
__ZEDA_EXPORT zUTFType zFCheckUTFBOM(FILE *fp);

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
/*! \defgroup strsearch string search operations.
 * \{ *//* ************************************************** */

#ifndef __KERNEL__

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
