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

#include <zeda/zeda_pathname.h>

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
  int nl;           /*!< number of lines */
  fpos_t *pos;      /*!< array of starting positions of lines in the file stream */
  int nf;           /*!< number of fields per line */
  char buf[BUFSIZ]; /*!< internal buffer */
} zCSV;

/*! \brief number of lines of a CSV file. */
#define zCSVLineNum(csv) (csv)->nl

/*! \brief open a CSV file. */
__ZEDA_EXPORT zCSV *zCSVOpen(zCSV *csv, char filename[]);

/*! \brief close a CSV file. */
__ZEDA_EXPORT void zCSVClose(zCSV *csv);

/*! \brief rewind the stream of a CSV file. */
__ZEDA_EXPORT void zCSVRewind(zCSV *csv);

/*! \brief check if the stream is at the end-of-file. */
#define zCSVIsEOF(csv) feof( (csv)->fp )

/*! \brief check if the current buffer is exhausted. */
#define zCSVLineIsEmpty(csv) ( (csv)->buf[0] == '\0' )

/*! \brief get a line from the current stream of a CSV file. */
__ZEDA_EXPORT char *zCSVGetLine(zCSV *csv);

/*! \brief go to a specified line in a CSV file. */
__ZEDA_EXPORT char *zCSVGoToLine(zCSV *csv, int i);

/*! \brief get a field from the current buffer of a CSV file. */
__ZEDA_EXPORT char *zCSVGetField(zCSV *csv, char *field, size_t size);
/*! \brief skip a field from the current buffer of a CSV file. */
__ZEDA_EXPORT char *zCSVSkipField(zCSV *csv);

/*! \brief get an integer value from the current buffer of a CSV file. */
__ZEDA_EXPORT bool zCSVGetInt(zCSV *csv, int *val);
/*! \brief get multiple integer values from the current buffer of a CSV file. */
__ZEDA_EXPORT bool zCSVGetIntN(zCSV *csv, int val[], int n);

/*! \brief get a double-precision floating-point value from the current buffer of a CSV file. */
__ZEDA_EXPORT bool zCSVGetDouble(zCSV *csv, double *val);
/*! \brief get multiple double-precision floating-point values from the current buffer of a CSV file. */
__ZEDA_EXPORT bool zCSVGetDoubleN(zCSV *csv, double val[], int n);

/*! \} */

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_CSV_H__ */
