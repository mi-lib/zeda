/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */

/*!
 * \mainpage

 ZEDA is a collection of frequently used structures and
 functions including:
 - bit operations
 - array operation
 - list operation
 - tree operation
 - round-robin table
 - string operations
 - command line option manipulation
 - generalized I/O stream
 - random number generator
 - ZTK format parser
 - CSV file manipulation
 - XML parser (wrapper of libxml2)
 */

#ifndef __ZEDA_H__
#define __ZEDA_H__

#include <zeda/zeda_binfile.h>

#include <zeda/zeda_option.h>

#include <zeda/zeda_ring.h>
#include <zeda/zeda_rrtab.h>
#include <zeda/zeda_stream.h>
#include <zeda/zeda_list.h>
#include <zeda/zeda_tree.h>

#include <zeda/zeda_string.h>

#include <zeda/zeda_rand.h>
#include <zeda/zeda_index.h>

#include <zeda/zeda_name.h>

#include <zeda/zeda_ztk.h>
#include <zeda/zeda_csv.h>

#ifdef __ZEDA_USE_LIBXML
#include <zeda/zeda_xml.h>
#endif /* __ZEDA_USE_LIBXML */

#endif /* __ZEDA_H__ */
