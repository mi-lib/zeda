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

#define ZEDA_RUNTIME_ERROR "run-time error: "
#define ZEDA_RUNTIME_WARN  "warning: "

/* warning message */

#define ZEDA_WARN_ENDIAN_NOT_SUPPORTED     "middle endian not supported."
#define ZEDA_WARN_INVALID_INTSIZ           "invalid (or unsupported) size of int %d."
#define ZEDA_WARN_INVALID_LNGSIZ           "invalid (or unsupported) size of long int %d."
#define ZEDA_WARN_INT_SHORTSIZE            "size of int in ZBD file = %d, while sizeof(int) = %lu; might lose information."
#define ZEDA_WARN_LNG_SHORTSIZE            "size of long in ZBD file = %d, while sizeof(long) = %lu; might lose information."

#define ZEDA_WARN_INVALID_HEX              "invalid hexadecimal notation: %c"

#define ZEDA_WARN_TOOLONG_BITWIDTH         "too large bit width. the result might be wrong."

#define ZEDA_WARN_BUF_EXHAUSTED            "exhausted buffer for string"

#define ZEDA_WARN_TOOLONG_STRING           "too long string"
#define ZEDA_WARN_TOOLONG_TOKEN            "too long string to tokenize"
#define ZEDA_WARN_TOOLONG_NUM              "too long number"

#define ZEDA_WARN_INDEX_OVERSIZE           "index has only %d components, while specified size is %d"
#define ZEDA_WARN_INDEX_INVALID_POS        "invalid position %d, must be within [0,%d]"

#define ZEDA_WARN_CSV_FIELD_EMPTY          "field empty in a CSV file"

#define ZEDA_WARN_ZTK_PRP_NOTFOUND         "manipulator of field %s not found"
#define ZEDA_WARN_ZTK_TOOMANY_TAGS         "too many tags %s specified, skipped."
#define ZEDA_WARN_ZTK_TOOMANY_KEYS         "too many keys %s specified, skipped."
#define ZEDA_WARN_ZTK_INCLUDE_DUP          "%s: duplicate file inclusion, skipped."
#define ZEDA_WARN_ZTK_NOT_TAGGED           "not in a tagged field, skipped."
#define ZEDA_WARN_ZTK_DUPDEF               "duplicate entry of tag=%s, skipped."
#define ZEDA_WARN_ZTK_UNKNOWN_VAL          "unknown value %s in the field of tag=%s key=%s"

#define ZEDA_WARN_UNKNOWNOPT               "unknown option: %s"

/* error messages */

#define ZEDA_ERR_CANNOTOPENFILE            "cannot open file: %s"
#define ZEDA_ERR_CANNOTALLOCMEM            "cannot allocate memory"
#define ZEDA_ERR_CANNOTGETFILESTAT         "cannot get file status"
#define ZEDA_ERR_CANNOTGETFILEHANDLE       "cannot get file handle"
#define ZEDA_ERR_CANNOTGETFILEINFO         "cannot get file information by handle"

#define ZEDA_ERR_NOT_ZBD                   "not a ZBD file."
#define ZEDA_ERR_ENDIAN_CHK_NOT_FOUND      "endian checker not found."
#define ZEDA_ERR_VERSION_NOT_FOUND         "version info not found."
#define ZEDA_ERR_INTSIZ_NOT_FOUND          "size of int not found."
#define ZEDA_ERR_LNGSIZ_NOT_FOUND          "size of long not found."
#define ZEDA_ERR_UNKNOWN_ENDIAN            "unknown endian."

#define ZEDA_ERR_EMPTY_STRING              "empty string"

#define ZEDA_ERR_QUICKSORT_NO_COMPARATOR   "comparator not assigned for quicksort / quickselect"
#define ZEDA_ERR_QUICKSELECT_INVALID_ORDER "invalid order %d, must be within [0, %d] for quickselect"
#define ZEDA_ERR_QUICKSELECT_EMPTY         "empty array assigned"

#define ZEDA_ERR_INDEX_SIZENOTSPECIFIED    "index size not specified"
#define ZEDA_ERR_INDEX_SIZEMISMATCH        "size mismatch of indices"

#define ZEDA_ERR_INDEX_BUF_OCCUPIED       "buffer for the index vector already occupied"

#define ZEDA_ERR_LIST2INDEX_FAILED        "cannot create an integer vector from an empty list"

#define ZEDA_ERR_CSV_INVALID              "invalid CSV file"
#define ZEDA_ERR_CSV_INVALID_LINE         "out-of-range line number %d specified"

#define ZEDA_ERR_ZTK_PRP_UNMODIFIABLE     "not permitted to modify the number of fields %s"
#define ZEDA_ERR_ZTK_UNKNOWN_VAL          "unknown value %s in the field of tag=%s key=%s"

#define ZEDA_ERR_FATAL                    "fatal error! - please report to the author"

#endif /* __ZEDA_ERRMSG_H__ */
