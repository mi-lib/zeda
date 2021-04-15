/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_csv.h
 * \brief CSV file operations.
 * \author Zhidao
 *
 * This module is not available in the kernel space,
 * since it requires the file pointer.
 */

#ifndef __ZEDA_CSV_H__
#define __ZEDA_CSV_H__

#include <zeda/zeda_string.h>

#ifndef __KERNEL__

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup CSV file operations.
 * \{ *//* ************************************************** */

/* ********************************************************** */
/*! \struct zCSV
 * \brief CSV manager class
 *
 * zCSV class handles a CSV file.
 *//* ******************************************************* */
typedef struct{
  FILE *fp;         /*!< file pointer */
  long pos0;        /*!< starting position in the file stream */
  int nf;           /*!< number of fields per line */
  char buf[BUFSIZ]; /*!< internal buffer */
} zCSV;

/*! \brief open a CSV file. */
__EXPORT zCSV *zCSVOpen(zCSV *csv, char filename[]);

/*! \brief close a CSV file. */
__EXPORT void zCSVClose(zCSV *csv);

/*! \brief rewind the stream of a CSV file. */
__EXPORT void zCSVRewind(zCSV *csv);

/*! \brief check if the stream is at the end-of-file. */
#define zCSVIsEOF(csv) feof( (csv)->fp )

/*! \brief check if the current buffer is exhausted. */
#define zCSVLineIsEmpty(csv) ( (csv)->buf[0] == '\0' )

/*! \brief get a line from the current stream of a CSV file. */
__EXPORT char *zCSVGetLine(zCSV *csv);

/*! \brief get a field from the current buffer of a CSV file. */
__EXPORT char *zCSVGetField(zCSV *csv, char *field, size_t size);

/*! \brief get an integer value from the current buffer of a CSV file. */
__EXPORT int zCSVGetInt(zCSV *csv);

/*! \brief get a double-precision floating-point value from the current buffer of a CSV file. */
__EXPORT double zCSVGetDouble(zCSV *csv);

/*! \} */

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_CSV_H__ */
