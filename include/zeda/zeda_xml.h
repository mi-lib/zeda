/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_xml.h
 * \brief XML file operations.
 * \author Zhidao
 */

#ifndef __ZEDA_XML_H__
#define __ZEDA_XML_H__

#ifndef __KERNEL__

#include <zeda/zeda_misc.h>
#include <libxml/parser.h>

__BEGIN_DECLS

/* check if the compiled and linked versions match. */
#define zXMLInit()                LIBXML_TEST_VERSION

/* strings */

#define zXMLCheckStr(str1,str2)   ( strcmp( (char*)(str1), (str2) ) == 0 )

#define zXMLGetString(node)       ( (char *)xmlNodeListGetString( (node)->doc, node, 1 ) )
#define zXMLGetInt(node)          atoi( zXMLGetString( node ) )
#define zXMLGetContent(node)      ( (char *)xmlNodeGetContent( node ) )

/* nodes & elements */

#define zXMLForEachNode(node,np) \
  for( (np)=(node); (np); (np)=(np)->next )

#define zXMLCheckElement(node,str) \
  ( (node)->type == XML_ELEMENT_NODE && zXMLCheckStr( (node)->name, str ) )

#define zXMLCheckElementAndExec(node,name,op) \
  if( zXMLCheckElement(node,name) ){ if( !op ) return false; }

__EXPORT xmlNode *zXMLFindNodeElement(xmlNode *node, const char *name);
#define zXMLFindNodeAttr(node,name) ( (char *)xmlGetProp( node, (xmlChar*)name ) )
#define zXMLAddNodeAttr(node,name,val)     xmlNewProp( node, (xmlChar *)name, (xmlChar *)val )
#define zXMLReplaceNodeAttr(node,name,val) xmlSetProp( node, (xmlChar *)name, (xmlChar *)val )

/* attributes */

#define zXMLGetAttrVal(attr)         ( (char *)xmlNodeListGetString( (attr)->doc, (attr)->children, 1 ) )
#define zXMLGetAttrInt(attr)         atoi( zXMLGetAttrVal( attr ) )
#define zXMLGetAttrDouble(attr)      atof( zXMLGetAttrVal( attr ) )

#define zXMLForEachAttr(node,attr) \
  for( (attr)=(node)->properties; (attr); (attr)=(attr)->next )

#define zXMLCheckAttr(attr,str)      zXMLCheckStr( (attr)->name, str )

#define zXMLCheckAttrAndExec(attr,str,op) \
  if( zXMLCheckStr( (attr)->name, str ) ){ (op); }

__EXPORT bool zXMLCheckAttrVal(xmlAttr *attr, char *name, char *str);

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_XML_H__ */
