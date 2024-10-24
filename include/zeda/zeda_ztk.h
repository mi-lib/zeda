/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_ztk.h
 * \brief ZTK (Z's tag-and-key) file format.
 * \author Zhidao
 */

#ifndef __ZEDA_ZTK_H__
#define __ZEDA_ZTK_H__

/*! ZTK format parser is not available in the kernel space */
#ifndef __KERNEL__

#include <zeda/zeda_strlist.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zFileStack
 * \brief file stack.
 *
 * zFileStack class stores files which could include another file
 * in them in a stacking manner. If a file A includes B inside,
 * a stack cell for B is pushed on that for A.
 *//* ******************************************************* */
typedef struct _zFileStack{
  char *pathname;           /*!< pathname of a file */
  FILE *fp;                 /*!< a file pointer */
  struct _zFileStack *prev; /*!< a pointer to the caller file */
} zFileStack;

/* initialize a file stack. */
__ZEDA_EXPORT void zFileStackInit(zFileStack *stack);

/* check if the given file is already in a file stack, and if not,
 * open the file and push it. */
__ZEDA_EXPORT zFileStack *zFileStackPush(zFileStack *head, const char *pathname);

/* pop the latest file from a file stack. */
__ZEDA_EXPORT zFileStack *zFileStackPop(zFileStack *head);

/* destroy a file stack. */
__ZEDA_EXPORT void zFileStackDestroy(zFileStack *head);

/* ********************************************************** */
/*! \struct ZTKKeyField
 * \brief key field of ZTK format.
 *//* ******************************************************* */
typedef struct{
  char *key;        /*!< parsed key */
  zStrList vallist; /*!< parsed list of strings */
} ZTKKeyField;

/* print out a key field of ZTK format. */
__ZEDA_EXPORT void ZTKKeyFieldFPrint(FILE *fp, ZTKKeyField *kf);

/* ********************************************************** */
/*! \struct ZTKKeyFieldList
 * \brief a list of key fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKKeyFieldList, ZTKKeyFieldListCell, ZTKKeyField );

/* insert a new key field of ZTK format to a list. */
__ZEDA_EXPORT ZTKKeyFieldListCell *ZTKKeyFieldListNew(ZTKKeyFieldList *list, const char *key);

/* destroy a list of key fields of ZTK format. */
__ZEDA_EXPORT void ZTKKeyFieldListDestroy(ZTKKeyFieldList *list);

/* ********************************************************** */
/*! \struct ZTKTagField
 * \brief tagged field of ZTK format.
 *//* ******************************************************* */
typedef struct{
  char *tag;
  ZTKKeyFieldList kflist;
} ZTKTagField;

/* print out a tagged field of ZTK format (for debug). */
__ZEDA_EXPORT void ZTKTagFieldFPrint(FILE *fp, ZTKTagField *tf);

/* ********************************************************** */
/*! \struct ZTKTagFieldList
 * \brief list of tagged fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKTagFieldList, ZTKTagFieldListCell, ZTKTagField );

/* insert a new tagged field of ZTK format to a list. */
__ZEDA_EXPORT ZTKTagFieldListCell *ZTKTagFieldListNew(const char *buf);

/* destroy a list of tagged fields of ZTK format. */
__ZEDA_EXPORT void ZTKTagFieldListDestroy(ZTKTagFieldList *list);

/* print out a list of tagged fields of ZTK format (for debug). */
__ZEDA_EXPORT void ZTKTagFieldListFPrint(FILE *fp, ZTKTagFieldList *list);

/* ********************************************************** */
/*! \struct ZTK
 * \brief ZTK format processor.
 *//* ******************************************************* */
typedef struct{
  zFileStack fs;
  ZTKTagFieldList tflist;
  ZTKTagFieldListCell *tf_cp;
  ZTKKeyFieldListCell *kf_cp;
  zStrListCell *val_cp;
} ZTK;

/*! \brief initialize a ZTK format processor. */
__ZEDA_EXPORT ZTK *ZTKInit(ZTK *ztk);

/*! \brief destroy a ZTK format processor. */
__ZEDA_EXPORT void ZTKDestroy(ZTK *ztk);

/*! \brief add a new tag to a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddTag(ZTK *ztk, const char *buf);
/*! \brief add a new key to the current tag field of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddKey(ZTK *ztk, const char *key);
/*! \brief add a new value to the current key field of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddVal(ZTK *ztk, const char *val);
/*! \brief add a new integer value to the current key field of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddInt(ZTK *ztk, const int val);
/*! \brief add a new double-precision floating-point value value to the current key field of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddDouble(ZTK *ztk, const double val);

/*! \brief scan and parse a file stream into a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKParseFP(ZTK *ztk, FILE *fp);

/*! \brief scan a file and parse it into a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKParse(ZTK *ztk, const char *path);

/*! \brief count the number of tagged fields with a specified tag in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT int ZTKCountTag(ZTK *ztk, const char *tag);

/*! \brief count the number of key fields with a specified key of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT int ZTKCountKey(ZTK *ztk, const char *key);

/*! \brief return a pointer to the current value string of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKValPtr(ztk) (ztk)->val_cp
/*! \brief return the current value string of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKVal(ztk) ( (ztk)->val_cp ? (const char *)(ztk)->val_cp->data : "" )
/*! \brief move to the next value string in the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT zStrListCell *ZTKValNext(ZTK *ztk);
/*! \brief rewind the list of value strings of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT zStrListCell *ZTKValRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current value string of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKValCmp(ztk,str) ( strcmp( ZTKVal(ztk), str ) == 0 )

/*! \brief return the current key of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKKey(ztk) ( (ztk)->kf_cp ? (const char *)(ztk)->kf_cp->data.key : "" )
/*! \brief return the number of values of the current key field of a ZTK format processor. */
#define ZTKKeyFieldSize(ztk) ( (ztk)->kf_cp ? zListSize(&(ztk)->kf_cp->data.vallist): 0 )
/*! \brief move to the next key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT ZTKKeyFieldListCell *ZTKKeyNext(ZTK *ztk);
/*! \brief rewind the list of key fields of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT ZTKKeyFieldListCell *ZTKKeyRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current key of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKKeyCmp(ztk,str) ( strcmp( ZTKKey(ztk), str ) == 0 )

/*! \brief return the current tag in a tag-and-key list of a ZTK format processor. */
#define ZTKTag(ztk) ( (ztk)->tf_cp ? (const char *)(ztk)->tf_cp->data.tag : "" )
/*! \brief move to the next tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT ZTKTagFieldListCell *ZTKTagNext(ZTK *ztk);
/*! \brief rewind the list of tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT ZTKTagFieldListCell *ZTKTagRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current tag in a tag-and-key list of a ZTK format processor. */
#define ZTKTagCmp(ztk,str) ( strcmp( ZTKTag(ztk), str ) == 0 )

/*! \brief rewind a tag-and-key list of a ZTK format processor. */
#define ZTKRewind(ztk) ZTKTagRewind( ztk )

/*! \brief retrieve an integer value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT int ZTKInt(ZTK *ztk);
/*! \brief retrieve a real value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__ZEDA_EXPORT double ZTKDouble(ZTK *ztk);

/*! \brief print out ZTK to a file. */
__ZEDA_EXPORT void ZTKFPrint(FILE *fp, ZTK *ztk);
#define ZTKPrint(ztk) ZTKFPrint( stdout, ztk )

/* ********************************************************** */
/*! \struct ZTKPrp
 * \brief properties of a class described by a set of tag/key string and call-back functions.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, ZTKPrp ){
  const char *str; /*!< a string for a tag/key */
  int num;   /*!< the number of tags/keys */
  void *(* _eval)(void *, int, void *, ZTK *); /*!< ZTK evaluation function */
  bool (* _fprint)(FILE *, int, void *); /*!< print out function */
};

/*! \brief duplicate an array of ZTKPrp. */
__ZEDA_EXPORT ZTKPrp *_ZTKPrpDup(ZTKPrp *src, int num);
#define ZTKPrpDup(src) _ZTKPrpDup( src, sizeof(src)/sizeof(ZTKPrp) )

/* evaluate a key field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void *_ZTKEvalKey(void *obj, void *arg, ZTK *ztk, ZTKPrp prp[], int num);
#define ZTKEvalKey(obj,arg,ztk,prp) _ZTKEvalKey( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a key field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void _ZTKPrpKeyFPrint(FILE *fp, void *obj, ZTKPrp prp[], int num);
#define ZTKPrpKeyFPrint(fp,obj,prp) _ZTKPrpKeyFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* evaluate a tag field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void *_ZTKEvalTag(void *obj, void *arg, ZTK *ztk, ZTKPrp prp[], int num);
#define ZTKEvalTag(obj,arg,ztk,prp) _ZTKEvalTag( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a tag field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void _ZTKPrpTagFPrint(FILE *fp, void *obj, ZTKPrp prp[], int num);
#define ZTKPrpTagFPrint(fp,obj,prp) _ZTKPrpTagFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_ZTK_H__ */
