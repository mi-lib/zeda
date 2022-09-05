/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 *
 * XML file operations.
 */

#include <zeda/zeda_xml.h>

xmlNode *zXMLFindNodeElement(xmlNode *node, const char *name)
{
  xmlNode *np;

  zXMLForEachNode( node, np )
    if( zXMLCheckElement( np, name ) ) return np;
  return NULL;
}

bool zXMLCheckAttrVal(xmlAttr *attr, const char *name, const char *str)
{
  return zXMLCheckAttr( attr, name ) && zXMLCheckStr( zXMLGetAttrVal(attr), str );
}
