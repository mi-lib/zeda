/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_stream.h
 * \brief generalized I/O stream.
 * \author Zhidao
 *
 * This module is not available in the kernel space,
 * since it requires the file pointer.
 */

#ifndef __ZEDA_STREAM_H__
#define __ZEDA_STREAM_H__

#include <zeda/zeda_misc.h>

#ifndef __KERNEL__

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup stream generalized I/O stream.
 * \{ *//* ************************************************** */

/* ********************************************************** */
/*! \struct zStream
 * \brief abstracted stream manipulation class.
 *
 * zStream class abstracts a data flow as a stream, and provides
 * a unified methods to manipulate it.
 * Either a file or memory space is assigned to the stream.
 *//* ******************************************************* */

typedef struct __zstream{
  union{
    FILE *fp;
    struct{
      byte *buf, *cur;
      size_t size;
    } mem;
  } src;
  void (* rewind)(struct __zstream *str);
  int (* seek)(struct __zstream *str, long offset);
  long (* tell)(struct __zstream *str);
  size_t (* read)(struct __zstream *str, byte *dest, size_t size, size_t nmemb);
  size_t (* write)(struct __zstream *str, byte *src, size_t size, size_t nmemb);
  int (* close)(struct __zstream *str);
} zStream;

/*! \brief attach a file to a stream.
 *
 * zStreamAttachFile() attaches a file stream \a fp to
 * an abstracted stream \a str.
 */
__EXPORT void zStreamAttachFile(zStream *str, FILE *fp);

/*! \brief attach a memory space to a stream.
 *
 * zStreamAttachMem() attaches a memory buffer pointed
 * by \a mem with the size \a size to an abstracted
 * stream \a str.
 */
__EXPORT void zStreamAttachMem(zStream *str, byte *mem, size_t size);

/*! \brief open a file and attach to a stream.
 *
 * zStreamOpenFile() opens a file \a filename with the
 * mode \a mode, and attaches it to a stream \a str.
 * \return
 * a pointer \a str is returned if succeeding to open
 * the file. Otherwise, the null pointer is returned.
 */
__EXPORT zStream *zStreamOpenFile(zStream *str, char filename[], char *mode);

/*! \brief allocate a memory space and attach to a stream.
 *
 * zStreamOpenMem() allocates a memory buffer with the
 * size \a size, and attaches it to a stream \a str.
 * \return
 * a pointer \a str is returned if succeeding to allocate
 * the memory space. Otherwise, the null pointer is returned.
 */
__EXPORT zStream *zStreamOpenMem(zStream *str, size_t size);

/*!
 * \def zStreamRewind
 * \brief set the current position to the head of a stream.
 * \def zStreamSeek
 * \brief set the current position to the specified location in a stream.
 * \def zStreamTell
 * \brief tell the current position in a stream.
 * \def zStreamRead
 * \brief read \a n data from the current position of \a s,
 * and copy it to the memory pointed by \a d. \a u is the
 * size of a data cell.
 * \def zStreamWrite
 * \brief write \a n data stored at \a m to the current
 * position of \a s. \a s is the size of a data cell.
 * \def zStreamClose
 * \brief close a stream.
 */
#define zStreamRewind(s)      (*(s)->rewind)( (s) )
#define zStreamSeek(s,p)      (*(s)->seek)( (s), (p) )
#define zStreamTell(s)        (*(s)->tell)( (s) )
#define zStreamRead(s,d,u,n)  (*(s)->read)( (s), (d), (u), (n) )
#define zStreamWrite(s,m,u,n) (*(s)->write)( (s), (m), (u), (n) )
#define zStreamClose(s)       (*(s)->close)( (s) )

/*! \} */

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_STREAM_H__ */
