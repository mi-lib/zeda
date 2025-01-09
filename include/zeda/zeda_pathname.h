/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_pathname.h
 * \brief pathname manipulations.
 * \author Zhidao
 */

#ifndef __ZEDA_PATHNAME_H__
#define __ZEDA_PATHNAME_H__

#include <zeda/zeda_string.h>

__BEGIN_DECLS

#ifndef __KERNEL__

/* ********************************************************** */
/*! \defgroup pathname pathname operations.
 * \{ *//* ************************************************** */

/*! \brief get a suffix from a filename.
 *
 * zGetSuffix() finds a suffix from a filename \a name.
 * The string pointed by \a name is not modified.
 * \return
 * a pointer to the head of suffix is returned.
 * ex. if \a name is "xxx.yyy.zzz", then the address
 * of the first "z" is returned.
 */
__ZEDA_EXPORT char *zGetSuffix(const char *name);

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
__ZEDA_EXPORT char *zAddSuffix(const char *org, const char *suffix, char *dest, size_t size);

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
__ZEDA_EXPORT FILE *zOpenFile(const char filename[], const char *suffix, const char *mode);

/*! \} */

#endif /* __KERNEL__ */

__END_DECLS

#endif /* __ZEDA_PATHNAME_H__ */
