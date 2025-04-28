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
#include <zeda/zeda_pathname.h>

__BEGIN_DECLS

/*! \brief default suffix of .ztk (Z's Tag-and-Key) file format. */
#define ZEDA_ZTK_SUFFIX "ztk"

/*! \brief open a .ztk file. */
#define zOpenZTKFile(filename,mode) zOpenFile( filename, ZEDA_ZTK_SUFFIX, mode )

/* ********************************************************** */
/*! \struct zFileStack
 * \brief file stack.
 *
 * zFileStack class stores files which could include another file
 * in them in a stacking manner. If a file A includes B inside,
 * a stack cell for B is pushed on that for A.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, zFileStack ){
  char *pathname;   /*!< pathname of a file */
  FILE *fp;         /*!< a file pointer */
  zFileStack *prev; /*!< a pointer to the caller file */
};

/*! \brief initialize a file stack. */
__ZEDA_EXPORT void zFileStackInit(zFileStack *stack);

/*! \brief check if the given file is already in a file stack, and if not, open the file and push it. */
__ZEDA_EXPORT zFileStack *zFileStackPush(zFileStack *head, const char *pathname);

/*! \brief pop the latest file from a file stack. */
__ZEDA_EXPORT zFileStack *zFileStackPop(zFileStack *head);

/*! \brief destroy a file stack. */
__ZEDA_EXPORT void zFileStackDestroy(zFileStack *head);

/* ********************************************************** */
/*! \struct ZTKKeyField
 * \brief key field of ZTK format.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, ZTKKeyField ){
  char *key;        /*!< parsed key */
  zStrList vallist; /*!< parsed list of strings */
};

/*! \brief print out a key field of ZTK format. */
__ZEDA_EXPORT void ZTKKeyFieldFPrint(FILE *fp, ZTKKeyField *kf);

/* ********************************************************** */
/*! \struct ZTKKeyFieldList
 * \brief a list of key fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKKeyFieldList, ZTKKeyFieldListCell, ZTKKeyField );

/*! \brief insert a new key field of ZTK format to a list. */
__ZEDA_EXPORT ZTKKeyFieldListCell *ZTKKeyFieldListNew(ZTKKeyFieldList *list, const char *key);

/*! \brief destroy the head of a list of key fields of ZTK format. */
__ZEDA_EXPORT void ZTKKeyFieldListHeadDestroy(ZTKKeyFieldList *list);

/*! \brief destroy a list of key fields of ZTK format. */
__ZEDA_EXPORT void ZTKKeyFieldListDestroy(ZTKKeyFieldList *list);

/* ********************************************************** */
/*! \struct ZTKTagField
 * \brief tagged field of ZTK format.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, ZTKTagField ){
  char *tag;
  ZTKKeyFieldList kflist;
};

/*! \brief print out a tagged field of ZTK format (for debug). */
__ZEDA_EXPORT void ZTKTagFieldFPrint(FILE *fp, ZTKTagField *tf);

/* ********************************************************** */
/*! \struct ZTKTagFieldList
 * \brief list of tagged fields of ZTK format.
 *//* ******************************************************* */
zListClass( ZTKTagFieldList, ZTKTagFieldListCell, ZTKTagField );

/*! \brief insert a new tagged field of ZTK format to a list. */
__ZEDA_EXPORT ZTKTagFieldListCell *ZTKTagFieldListNew(const char *buf);

/*! \brief destroy a list of tagged fields of ZTK format. */
__ZEDA_EXPORT void ZTKTagFieldListDestroy(ZTKTagFieldList *list);

/*! \brief print out a list of tagged fields of ZTK format (for debug). */
__ZEDA_EXPORT void ZTKTagFieldListFPrint(FILE *fp, ZTKTagFieldList *list);

/* ********************************************************** */
/*! \struct ZTK
 * \brief ZTK format processor.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, ZTK ){
  zFileStack fs;
  ZTKTagFieldList tflist;
  ZTKTagFieldListCell *tf_cp;
  ZTKKeyFieldListCell *kf_cp;
  zStrListCell *val_cp;
#ifdef __cplusplus
 public:
  ZTK();
  ~ZTK();
  bool addTag(const char *tag);
  bool addKey(const char *key);
  void delKey();
  bool addVal(const char *val);
  bool addInt(const int val);
  bool addDouble(const double val);
  bool parseFP(FILE *fp);
  bool parse(const char *path);
  int countTag(const char *tag);
  int countKey(const char *key);
  zStrListCell *valptr();
  const char *val();
  zStrListCell *nextVal();
  zStrListCell *rewindVal();
  bool cmpVal(const char *str);
  const char *key();
  int keyFieldSize();
  ZTKKeyFieldListCell *nextKey();
  ZTKKeyFieldListCell *rewindKey();
  bool cmpKey(const char *str);
  const char *tag();
  ZTKTagFieldListCell *nextTag();
  ZTKTagFieldListCell *rewindTag();
  bool cmpTag(const char *tag);
  void rewind();
  int getInt();
  double getDouble();
  void fprintf(FILE *fp);
  void printf();
#endif
};

/*! \brief initialize a ZTK format processor. */
__ZEDA_EXPORT ZTK *ZTKInit(ZTK *ztk);

/*! \brief destroy a ZTK format processor. */
__ZEDA_EXPORT void ZTKDestroy(ZTK *ztk);

/*! \brief add a new tag to a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddTag(ZTK *ztk, const char *tag);
/*! \brief add a new key to the current tag field of a ZTK format processor. */
__ZEDA_EXPORT bool ZTKAddKey(ZTK *ztk, const char *key);
/*! \brief delete a key at the head of the current tag field of a ZTK format processor. */
__ZEDA_EXPORT void ZTKDelKey(ZTK *ztk);
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

#ifdef __cplusplus
inline ZTK::ZTK(){ ZTKInit( this ); }
inline ZTK::~ZTK(){ ZTKDestroy( this ); }
inline bool ZTK::addTag(const char *tag){ return ZTKAddTag( this, tag ); }
inline bool ZTK::addKey(const char *key){ return ZTKAddKey( this, key ); }
inline void ZTK::delKey(){ ZTKDelKey( this ); }
inline bool ZTK::addVal(const char *val){ return ZTKAddVal( this, val ); }
inline bool ZTK::addInt(const int val){ return ZTKAddInt( this, val ); }
inline bool ZTK::addDouble(const double val){ return ZTKAddDouble( this, val ); }
inline bool ZTK::parseFP(FILE *fp){ return ZTKParseFP( this, fp ); }
inline bool ZTK::parse(const char *path){ return ZTKParse( this, path ); }
inline int  ZTK::countTag(const char *tag){ return ZTKCountTag( this, tag ); }
inline int  ZTK::countKey(const char *key){ return ZTKCountKey( this, key ); }
inline zStrListCell *ZTK::valptr(){ return ZTKValPtr( this ); }
inline const char *ZTK::val(){ return ZTKVal( this ); }
inline zStrListCell *ZTK::nextVal(){ return ZTKValNext( this ); }
inline zStrListCell *ZTK::rewindVal(){ return ZTKValRewind( this ); }
inline bool ZTK::cmpVal(const char *str){ return ZTKValCmp( this, str ); }
inline const char *ZTK::key(){ return ZTKKey( this ); }
inline int ZTK::keyFieldSize(){ return ZTKKeyFieldSize( this ); }
inline ZTKKeyFieldListCell *ZTK::nextKey(){ return ZTKKeyNext( this ); }
inline ZTKKeyFieldListCell *ZTK::rewindKey(){ return ZTKKeyRewind( this ); }
inline bool ZTK::cmpKey(const char *str){ return ZTKKeyCmp( this, str ); }
inline const char *ZTK::tag(){ return ZTKTag( this ); }
inline ZTKTagFieldListCell *ZTK::nextTag(){ return ZTKTagNext( this ); }
inline ZTKTagFieldListCell *ZTK::rewindTag(){ return ZTKTagRewind( this ); }
inline bool ZTK::cmpTag(const char *str){ return ZTKTagCmp( this, str ); }
inline void ZTK::rewind(){ rewindTag(); }
inline int ZTK::getInt(){ return ZTKInt( this ); }
inline double ZTK::getDouble(){ return ZTKDouble( this ); }
inline void ZTK::fprintf(FILE *fp){ ZTKFPrint( fp, this ); }
inline void ZTK::printf(){ ZTKPrint( this ); }
#endif

/* ********************************************************** */
/*! \struct ZTKPrp
 * \brief properties of a class described by a set of tag/key string and call-back functions.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEDA_CLASS_EXPORT, ZTKPrp ){
  const char *str; /*!< a string for a tag/key */
  int num; /*!< the number of tags/keys */
  void *(* _eval)(void *, int, void *, ZTK *); /*!< ZTK evaluation function */
  bool (* _fprint)(FILE *, int, void *); /*!< print out function */
};

/*! \brief print out information of a list of ZTK property (for debug). */
__ZEDA_EXPORT void _ZTKPrpFPrint(FILE *fp, const ZTKPrp *prp, size_t size);
#define ZTKPrpFPrint(fp,prp) _ZTKPrpFPrint( fp, prp, sizeof(prp)/sizeof(ZTKPrp) )

/*! \brief duplicate an array of ZTKPrp. */
__ZEDA_EXPORT ZTKPrp *_ZTKPrpDup(const ZTKPrp *src, size_t size);
#define ZTKPrpDup(src) _ZTKPrpDup( src, sizeof(src)/sizeof(ZTKPrp) )
/*! \brief set number of a ZTK property with the specified string. */
__ZEDA_EXPORT bool _ZTKPrpSetNum(ZTKPrp *prp, size_t size, const char *str, int num);
#define ZTKPrpSetNum(prp,str,num) _ZTKPrpSetNum( prp, sizeof(prp)/sizeof(ZTKPrp), str, num )

/* evaluate a key field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void *_ZTKEvalKey(void *obj, void *arg, ZTK *ztk, const ZTKPrp prp[], size_t size);
#define ZTKEvalKey(obj,arg,ztk,prp) _ZTKEvalKey( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a key field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void _ZTKPrpKeyFPrint(FILE *fp, void *obj, const ZTKPrp prp[], size_t size);
#define ZTKPrpKeyFPrint(fp,obj,prp) _ZTKPrpKeyFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* evaluate a tag field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void *_ZTKEvalTag(void *obj, void *arg, ZTK *ztk, const ZTKPrp prp[], size_t size);
#define ZTKEvalTag(obj,arg,ztk,prp) _ZTKEvalTag( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKPrp) )

/* print out a tag field of a ZTK format processor based on a ZTK property. */
__ZEDA_EXPORT void _ZTKPrpTagFPrint(FILE *fp, void *obj, const ZTKPrp prp[], size_t size);
#define ZTKPrpTagFPrint(fp,obj,prp) _ZTKPrpTagFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKPrp) )

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_ZTK_H__ */
