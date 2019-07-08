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
  char pathname[BUFSIZ];    /*!< pathname of a file */
  FILE *fp;                 /*!< a file pointer */
  struct _zFileStack *prev; /*!< a pointer to the caller file */
} zFileStack;

/* initialize a file stack. */
__EXPORT void zFileStackInit(zFileStack *stack);

/* check if the given file is already in a file stack, and if not,
 * open the file and push it. */
__EXPORT zFileStack *zFileStackPush(zFileStack *head, char *pathname);

/* pop the latest file from a file stack. */
__EXPORT zFileStack *zFileStackPop(zFileStack *head);

/* destroy a file stack. */
__EXPORT void zFileStackDestroy(zFileStack *head);

/* ********************************************************** */
/*! \struct ZTKDef
 * \brief definition of ZTK (a set of tag and keys).
 *//* ******************************************************* */
typedef struct{
  char *tag;        /*!< defined tag */
  zStrList keylist; /*!< defined set of keys */
} ZTKDef;

/* destroy a definition of ZTK. */
__EXPORT void ZTKDefDestroy(ZTKDef *def);

/* find a key in a definition of ZTK. */
__EXPORT bool ZTKDefFindKey(ZTKDef *def, char *key);

/* print out a definition of ZTK (for debug). */
__EXPORT void ZTKDefFPrint(FILE *fp, ZTKDef *def);

/* ********************************************************** */
/*! \struct ZTKDefList
 * \brief a definition list of ZTK.
 *//* ******************************************************* */
zListClass( ZTKDefList, ZTKDefListCell, ZTKDef );

/*! \brief destroy a definition list of ZTK. */
__EXPORT void ZTKDefListDestroy(ZTKDefList *list);

/*! \brief find tag in a definition list of ZTK. */
__EXPORT ZTKDefListCell *ZTKDefListFindTag(ZTKDefList *list, char *tag);

/*! \brief find tag in a definition list of ZTK, and if not, allocate a new definition. */
__EXPORT ZTKDefListCell *ZTKDefListFindAndAddTag(ZTKDefList *list, char *tag);

/*! \brief register a new definition of ZTK to a list. */
__EXPORT ZTKDefListCell *ZTKDefListRegOne(ZTKDefList *list, char *tag, char *key);

/*! \brief register a new definition of ZTK to a list. */
__EXPORT ZTKDefListCell *ZTKDefListReg(ZTKDefList *list, char *tag, char **key, int keynum);

/*! \brief print out a definition list of ZTK (for debug). */
__EXPORT void ZTKDefListFPrint(FILE *fp, ZTKDefList *list);

/* ********************************************************** */
/*! \struct ZTKKeyField
 * \brief key field of ZTK format.
 *//* ******************************************************* */
typedef struct{
  char *key;        /*!< parsed key */
  zStrList vallist; /*!< parsed list of strings */
} ZTKKeyField;

/* print out a key field of ZTK format. */
__EXPORT void ZTKKeyFieldFPrint(FILE *fp, ZTKKeyField *kf);

/* ********************************************************** */
/*! \struct ZTKKeyFieldList
 * \brief a list of key fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKKeyFieldList, ZTKKeyFieldListCell, ZTKKeyField );

/* insert a new key field of ZTK format to a list. */
__EXPORT ZTKKeyFieldListCell *ZTKKeyFieldListNew(ZTKKeyFieldList *list, char *key);

/* destroy a list of key fields of ZTK format. */
__EXPORT void ZTKKeyFieldListDestroy(ZTKKeyFieldList *list);

/* ********************************************************** */
/*! \struct ZTKTagField
 * \brief tagged field of ZTK format.
 *//* ******************************************************* */
typedef struct{
  char *tag;
  ZTKKeyFieldList kflist;
} ZTKTagField;

/* print out a tagged field of ZTK format (for debug). */
__EXPORT void ZTKTagFieldFPrint(FILE *fp, ZTKTagField *tf);

/* ********************************************************** */
/*! \struct ZTKTagFieldList
 * \brief list of tagged fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKTagFieldList, ZTKTagFieldListCell, ZTKTagField );

/* insert a new tagged field of ZTK format to a list. */
__EXPORT ZTKTagFieldListCell *ZTKTagFieldListNew(char buf[]);

/* destroy a list of tagged fields of ZTK format. */
__EXPORT void ZTKTagFieldListDestroy(ZTKTagFieldList *list);

/* print out a list of tagged fields of ZTK format (for debug). */
__EXPORT void ZTKTagFieldListFPrint(FILE *fp, ZTKTagFieldList *list);

/* ********************************************************** */
/*! \struct ZTK
 * \brief ZTK format processor.
 *//* ******************************************************* */
typedef struct{
  zFileStack fs;
  ZTKDefList deflist;
  ZTKDefListCell *def;
  ZTKTagFieldList tflist;
  ZTKTagFieldListCell *tf_cp;
  ZTKKeyFieldListCell *kf_cp;
  zStrListCell *val_cp;
} ZTK;

/*! \brief register a definition of a set of tag and key to ZTK format processor. */
#define ZTKDefRegOne(ztk,tag,key) ZTKDefListRegOne( &(ztk)->deflist, tag, key )

/*! \brief register a definition of a set of tag and keys to ZTK format processor. */
#define ZTKDefReg(ztk,tag,keylist) ZTKDefListReg( &(ztk)->deflist, tag, keylist, sizeof(keylist)/sizeof(char*) )

/*! \brief initialize a ZTK format processor. */
__EXPORT ZTK *ZTKInit(ZTK *ztk);

/*! \brief destroy a ZTK format processor. */
__EXPORT void ZTKDestroy(ZTK *ztk);

/*! \brief scan a file and parse it into a tag-and-key list of a ZTK format processor. */
__EXPORT bool ZTKParse(ZTK *ztk, char *path);

/*! \brief count the number of tagged fields with a specified tag in a tag-and-key list of a ZTK format processor. */
__EXPORT int ZTKCountTag(ZTK *ztk, const char *tag);

/*! \brief count the number of key fields with a specified key of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT int ZTKCountKey(ZTK *ztk, const char *key);

/*! \brief return the current value string of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKVal(ztk) ( (ztk)->val_cp ? (ztk)->val_cp->data : "" )
/*! \brief move to the next value string in the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT zStrListCell *ZTKValNext(ZTK *ztk);
/*! \brief rewind the list of value strings of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT zStrListCell *ZTKValRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current value string of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKValCmp(ztk,str) ( strcmp( ZTKVal(ztk), str ) == 0 )

/*! \brief return the current key of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKKey(ztk) ( (ztk)->kf_cp ? (ztk)->kf_cp->data.key : "" )
/*! \brief move to the next key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKKeyFieldListCell *ZTKKeyNext(ZTK *ztk);
/*! \brief rewind the list of key fields of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKKeyFieldListCell *ZTKKeyRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current key of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKKeyCmp(ztk,str) ( strcmp( ZTKKey(ztk), str ) == 0 )

/*! \brief return the current tag in a tag-and-key list of a ZTK format processor. */
#define ZTKTag(ztk) ( (ztk)->tf_cp ? (ztk)->tf_cp->data.tag : "" )
/*! \brief move to the next tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKTagFieldListCell *ZTKTagNext(ZTK *ztk);
/*! \brief rewind the list of tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKTagFieldListCell *ZTKTagRewind(ZTK *ztk);
/*! \brief check if a string is the same with the current tag in a tag-and-key list of a ZTK format processor. */
#define ZTKTagCmp(ztk,str) ( strcmp( ZTKTag(ztk), str ) == 0 )

/*! \brief retrieve an integer value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT int ZTKInt(ZTK *ztk);
/*! \brief retrieve a real value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT double ZTKDouble(ZTK *ztk);

/*! \brief print out ZTK to a file. */
__EXPORT void ZTKFPrint(FILE *fp, ZTK *ztk);
#define ZTKPrint(ztk) ZTKFPrint( stdout, ztk )

/* ********************************************************** */
/*! \struct ZTKPrp
 * \brief properties of a class described by a set of tag/key string and call-back functions.
 *//* ******************************************************* */
typedef struct{
  char *str; /*!< a string for a tag/key */
  int num;   /*!< the number of tags/keys */
  void *(* encode)(void *, int, void *, ZTK *); /*!< ZTK encoder function */
  void (* fprintf)(FILE *, int, void *); /*!< print out function */
} ZTKPrp;

/* register a tag-and-key property to a ZTK format processor. */
__EXPORT bool ZTKDefListRegPrp(ZTKDefList *list, char *tag, ZTKPrp prp[], int num);
#define ZTKDefRegPrp(ztk,tag,prp) ZTKDefListRegPrp( &(ztk)->deflist, tag, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* encode a key field of a ZTK format processor based on a ZTK property. */
__EXPORT void *_ZTKEncodeKey(void *obj, void *arg, ZTK *ztk, ZTKPrp prp[], int num);
#define ZTKEncodeKey(obj,arg,ztk,prp) _ZTKEncodeKey( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a key field of a ZTK format processor based on a ZTK property. */
__EXPORT void _ZTKPrpKeyFPrint(FILE *fp, void *obj, ZTKPrp prp[], int num);
#define ZTKPrpKeyFPrint(fp,obj,prp) _ZTKPrpKeyFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* encode a tag field of a ZTK format processor based on a ZTK property. */
__EXPORT void *_ZTKEncodeTag(void *obj, void *arg, ZTK *ztk, ZTKPrp prp[], int num);
#define ZTKEncodeTag(obj,arg,ztk,prp) _ZTKEncodeTag( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a tag field of a ZTK format processor based on a ZTK property. */
__EXPORT void _ZTKPrpTagFPrint(FILE *fp, void *obj, ZTKPrp prp[], int num);
#define ZTKPrpTagFPrint(fp,obj,prp) _ZTKPrpTagFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_ZTK_H__ */
