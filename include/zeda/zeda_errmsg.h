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

#define ZEDA_WARN_INVHEX          "invalid hexadecimal notation: %c"

#define ZEDA_WARN_TOOLNG_BITWIDTH "too large bit width. the result might be wrong."

#define ZEDA_WARN_BUF_EXHAUSTED   "exhausted buffer for string"

#define ZEDA_WARN_TOOLNG_STR      "too long string"
#define ZEDA_WARN_TOOLNG_TKN      "too long string to tokenize"
#define ZEDA_WARN_TOOLNG_NUM      "too long number"

#define ZEDA_WARN_UNKNOWNOPT      "unknown option: %s"

/* error messages */

#define ZEDA_ERR_FATAL            "fatal error! - please report to the author"

#endif /* __ZEDA_ERRMSG_H__ */
