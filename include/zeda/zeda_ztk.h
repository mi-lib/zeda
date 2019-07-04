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

/* print out a definition of ZTK (for debug). */
__EXPORT void ZTKDefFPrint(FILE *fp, ZTKDef *def);

/* ********************************************************** */
/*! \struct ZTKDefList
 * \brief a definition list of ZTK.
 *//* ******************************************************* */
zListClass( ZTKDefList, ZTKDefListCell, ZTKDef );

/* destroy a definition list of ZTK. */
__EXPORT void ZTKDefListDestroy(ZTKDefList *list);

/* register a new definition of ZTK to a list. */
__EXPORT ZTKDefListCell *ZTKDefListReg(ZTKDefList *list, char *tag, char **key, int keynum);

/* find tag in a definition list of ZTK. */
__EXPORT ZTKDefListCell *ZTKDefListFindTag(ZTKDefList *list, char *tag);

/* print out a definition list of ZTK (for debug). */
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
#define ZTKVal(ztk) ( (ztk)->val_cp ? (ztk)->val_cp->data : NULL )
/*! \brief rewind the list of value strings of the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT zStrListCell *ZTKValRewind(ZTK *ztk);
/*! \brief move to the next value string in the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT zStrListCell *ZTKValNext(ZTK *ztk);

/*! \brief return the current key of the current tagged field in a tag-and-key list of a ZTK format processor. */
#define ZTKKey(ztk) ( (ztk)->kf_cp ? (ztk)->kf_cp->data.key : NULL )
/*! \brief rewind the list of key fields of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKKeyFieldListCell *ZTKKeyRewind(ZTK *ztk);
/*! \brief move to the next key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKKeyFieldListCell *ZTKKeyNext(ZTK *ztk);

/*! \brief return the current tag in a tag-and-key list of a ZTK format processor. */
#define ZTKTag(ztk) ( (ztk)->tf_cp ? (ztk)->tf_cp->data.tag : NULL )
/*! \brief rewind the list of tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKTagFieldListCell *ZTKTagRewind(ZTK *ztk);
/*! \brief move to the next tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT ZTKTagFieldListCell *ZTKTagNext(ZTK *ztk);

/*! \brief retrieve an integer value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT int ZTKInt(ZTK *ztk);
/*! \brief retrieve a real value from the current key field of the current tagged field in a tag-and-key list of a ZTK format processor. */
__EXPORT double ZTKDouble(ZTK *ztk);

/*! \brief print out ZTK to a file. */
__EXPORT void ZTKFPrint(FILE *fp, ZTK *ztk);
#define ZTKPrint(ztk) ZTKFPrint( stdout, ztk )

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_ZTK_H__ */
