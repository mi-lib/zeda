/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_errmsg.h
 * \brief error messages.
 * \author Zhidao
 */

#ifndef __ZEDA_ERRMSG_H__
#define __ZEDA_ERRMSG_H__

/* NOTE: never include this header file in user programs. */

/* warning message */

#define ZEDA_WARN_ENDIAN_NOT_SUPPORTED "middle endian not supported."
#define ZEDA_WARN_INVALID_INTSIZ       "invalid (or unsupported) size of int %d."
#define ZEDA_WARN_INVALID_LNGSIZ       "invalid (or unsupported) size of long int %d."
#define ZEDA_WARN_INT_SHRTSIZ          "size of int in ZBD file = %d, whilc sizeof(int) = %ld; might lose information."
#define ZEDA_WARN_LNG_SHRTSIZ          "size of long in ZBD file = %d, whilc sizeof(long) = %ld; might lose information."

#define ZEDA_WARN_INVHEX               "invalid hexadecimal notation: %c"

#define ZEDA_WARN_TOOLNG_BITWIDTH      "too large bit width. the result might be wrong."

#define ZEDA_WARN_BUF_EXHAUSTED        "exhausted buffer for string"

#define ZEDA_WARN_TOOLNG_STR           "too long string"
#define ZEDA_WARN_TOOLNG_TKN           "too long string to tokenize"
#define ZEDA_WARN_TOOLNG_NUM           "too long number"

#define ZEDA_WARN_ZTK_INCLUDE_DUP      "%s: duplicate file inclusion, skipped."
#define ZEDA_WARN_ZTK_NOT_TAGGED       "not in a tagged field, skipped."
#define ZEDA_WARN_ZTK_DUPDEF           "duplicate entry of tag [%s], skipped."

#define ZEDA_WARN_ZTK_TOOMANY_TAGS     "too many tag %s specified, skipped."
#define ZEDA_WARN_ZTK_TOOMANY_KEYS     "too many key %s specified, skipped."

#define ZEDA_WARN_UNKNOWNOPT           "unknown option: %s"

/* error messages */

#define ZEDA_ERR_NOT_ZBD               "not a ZBD file."
#define ZEDA_ERR_ENDIAN_CHK_NOT_FOUND  "endian checker not found."
#define ZEDA_ERR_VERSION_NOT_FOUND     "version info not found."
#define ZEDA_ERR_INTSIZ_NOT_FOUND      "size of int not found."
#define ZEDA_ERR_LNGSIZ_NOT_FOUND      "size of long not found."
#define ZEDA_ERR_UNKNOWN_ENDIAN        "unknown endian."

#define ZEDA_ERR_FATAL                 "fatal error! - please report to the author"

#endif /* __ZEDA_ERRMSG_H__ */
