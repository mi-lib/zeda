/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * zeda_bfile - binary file I/O
 * contributer: 2018 Naoki Wakisaka and Takeharu Suzuki
 */

#ifndef __ZEDA_BFILE_H__
#define __ZEDA_BFILE_H__

#include <zeda/zeda_string.h>
#include <time.h>
#include <stdint.h>

__BEGIN_DECLS

__EXPORT uint16_t zByteSwap16(uint16_t val);
__EXPORT uint32_t zByteSwap32(uint32_t val);
__EXPORT uint64_t zByteSwap64(uint64_t val);
__EXPORT void zByteSwapAllBuf(void *val, void *ret, int size, char* buf);
__EXPORT bool zByteSwapAll(void *val, void *ret, int size);

#define zByteSwapAllBufDRC(v,s,b) zByteSwapAllBuf(v,v,s,b)
#define zByteSwapAllDRC(v,s) zByteSwapAll(v,v,s)

#define zByteSwap(type,val,ret) do{           \
  switch( sizeof(type) ){                     \
  case 4:                                     \
    ret = (type)zByteSwap32( val );           \
    break;                                    \
  case 8:                                     \
    ret = (type)zByteSwap64( val );           \
    break;                                    \
  case 2:                                     \
    ret = (type)zByteSwap16( val );           \
    break;                                    \
  default:                                    \
    zByteSwapAll( &val, &ret, sizeof(type) ); \
  }                                           \
}while(0);

#define zByteSwapDRC(t,v) zByteSwap(t,v,v)



#define ZHEADER_NEWEST_VERSION 1

#define ZHEADER_HEAD_SIZE 13
#define ZHEADER_HEAD "ZFile Header"
#define ZHEADER_TAIL_SIGN 0xaabbccdd
/* TODO: Tail -> Term or Terminator? */

typedef enum{ ZHEADER_CORRECT, ZHEADER_BROKEN, ZHEADER_NOTHING } zHeaderState;
typedef enum{ ZHEADER_ENDIAN_EQUAL, ZHEADER_ENDIAN_ANOTHER, ZHEADER_ENDIAN_STRANGE } zHeaderEndianness;

typedef struct{
  zHeaderState state;       /*!< loaded header state */
  zHeaderEndianness endian; /*!< loaded endiannness */

  char head[ZHEADER_HEAD_SIZE];   /*!< identification string "ZFile Header" */
  uint32_t size;    /*!< size of header */
  uint8_t hver;     /*!< version of header */

  char suffix[BUFSIZ]; /*!< suffix of the data format */
  uint8_t dver;        /*!< version of the data format */
  uint8_t int_size;    /*!< size of int [byte] */
  uint8_t lint_size;   /*!< size of long [byte] */
  int64_t time;        /*!< timestamp at writing */
  uint32_t tsign;      /*!< terminator, for checking endianness [0xaabbccdd] */
} zHeader;


#define zFileHeaderDVer(h) ( (h)->dver )
#define zFileHeaderIsCorrect(h) ( (h)->state == ZHEADER_CORRECT )
#define zFileHeaderIsBroken(h)  ( (h)->state == ZHEADER_BROKEN )
#define zFileHeaderIsNothing(h) ( (h)->state == ZHEADER_NOTHING )
#define zFileHeaderEndianIsEqual(h)   ( (h)->endian == ZHEADER_ENDIAN_EQUAL )
#define zFileHeaderEndianIsAnother(h) ( (h)->endian == ZHEADER_ENDIAN_ANOTHER )
#define zFileHeaderEndianIsStrange(h) ( (h)->endian == ZHEADER_ENDIAN_STRANGE )


__EXPORT void zFileHeaderInit(zHeader *h);
__EXPORT uint32_t zFileHeaderCalcHeaderSize(zHeader *h);
__EXPORT void zFileHeaderSetSuffix(zHeader *h, char *suffix);

__EXPORT zHeader* zFileHeaderFReadB(FILE *fp, zHeader *h);
__EXPORT zHeader* zFileHeaderFWriteB(FILE *fp, zHeader *h);

__EXPORT void zFileHeaderFWrite(FILE *fp, zHeader *h);
#define zFileHeaderWrite(h) ( zFileHeaderFWrite( stdout, (h) ) );

__END_DECLS

#endif /* __ZEDA_BFILE_H__ */
