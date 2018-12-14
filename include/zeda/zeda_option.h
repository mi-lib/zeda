/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_option.h
 * \brief command line option manipulation.
 * \author Zhidao
 */

#ifndef __ZEDA_OPTION_H__
#define __ZEDA_OPTION_H__

/*! command line option operations are not available in the
    kernel space */
#ifndef __KERNEL__

#include <ctype.h>
#include <zeda/zeda_strlist.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup option command line option manipulations.
 * \{ *//* ************************************************** */

/* ********************************************************** */
/*! \struct zOption
 * \brief command line option information.
 *
 * zOption class associates command line arguments with
 * specified options. It is also utilized for displaying
 * a command usage.
 *
 * zOption class instance has the following members.
 *  \a key:     a short-version key string lasting to '-'.
 *  \a longkey: a long-version key string lasting to '--'.
 *   Each option has to have at least either \a key or
 *   \a longkey.
 *  \a arg_guide: a guide of the argument following the
 *   corresponding option to be displayed in the usage.
 *  \a desc: description about the function of the option.
 *  \a arg: a pointer to be associated to the corresponding
 *   argument. It should be initialized for the null
 *   pointer.
 *  \a flag: the flag to check if the option is validated.
 *   It should be initialized for the false value.
 *
 * A typical notation is as follows:
 *
 *  zOption option[] = {
 *    { "x", "xarg", "<x val>", "x value", NULL, false },
 *    ...
 *    { NULL, NULL,     NULL, NULL, false },
 *  };
 *
 * The last instance filled with the null pointers is the
 * terminator of the command line option list, needed to
 * seek the end of options.
 *//* ******************************************************* */

typedef struct{
  const char *key;       /*!< option switch */
  const char *longkey;   /*!< long version option switch */
  const char *arg_guide; /*!< a guide for option displayed with usage */
  const char *desc;      /*!< description for displaying a usage */
  char *arg;             /*!< an associated argument */
  bool flag;             /*!< check if the option is validated */
} zOption;

/*! \brief display a command line help.
 *
 * zOptionHelp() displays a guidance of command line
 * options to the standard output in the following style:
 *
 *   -x, --xxx  [arg]  description
 *
 * where 'x' is the short-version option switch, and
 * 'xxx' is the long-version option switch.
 *
 * The information of option key, argument and description
 * are stored in \a opts. Refer zOptionRead() for more
 * information.
 * \sa
 * zOptionRead
 */
__EXPORT void zOptionHelp(zOption *opts);

/*! \brief diagnose option flags.
 *
 * zOptionDiag() diagnoses on/off status of each command
 * line option defined by \a opts. The statuses are
 * displayed to the standard error. This function is
 * prepared for debugs.
 * \sa
 * zOptionRead
 */
__EXPORT void zOptionDiag(zOption *opts);

/*! \brief read command line options.
 *
 * zOptionRead() reads command line arguments and parse the runtime
 * argument vector according to \a opts.
 *
 * The command line argument vector \a argv is parsed and detected
 * options are validated. If valid arguments following the options
 * are parsed, they are pointed by the \a arg member of the
 * correspoinding option instance in \a opts.
 *
 * When arguments which do not follow any options are picked up,
 * they are sequentially stored into a list of strings \a arglist.
 * \a arglist is automatically initialized in zOptionRead().
 *
 * Example:
 *
 *   zOption opts[] = {
 *     { "x",  NULL, "<x val>", "x value",  NULL, false },
 *     { "y",  NULL, "<y val>", "y value",  NULL, false },
 *     { "z",  NULL,      NULL, "z switch", NULL, false },
 *     { NULL, NULL,      NULL,      NULL,  NULL, false },
 *   };
 *   zStrList arglist;
 *
 *   zOptionRead( opts, argv, &arglist );
 *   ...
 *
 *   % command -x 1 -y -- -1 -z str1 str2
 *
 *   Then, the contents of \a opts varies as follows:
 *
 *   opts[] = {
 *     { "x",  NULL, "<x val>", "x value",  "1",  true },
 *     { "y",  NULL, "<y val>", "y value",  "-1", true },
 *     { "z",  NULL,      NULL, "z switch", NULL, true },
 *     { NULL, NULL,      NULL,      NULL,  NULL, false },
 *   };
 *
 *   In addition, "str1" and "str2" are stored in \a arglist
 *   from the head to the tail.
 *
 * Note that the string "--" disables the following string
 * to be parsed as an option even if it begins with '-'.
 */
__EXPORT bool zOptionRead(zOption *opts, char **argv, zStrList *arglist);

/* ********************************************************** */
/* integer range
 * ********************************************************** */

#define Z_INTRANGE_BORDER (-1)

/*! \brief parse integer range notation.
 *
 * zParseIntRange() parses a string \a expr which
 * expresses an integer range, and sets \a from and \a to
 * for the smallest and the largest values, respectively.
 *
 * The followings are examples of strings.

 *   "3"   -> from 3 to 3

 *   "3-7" -> from 3 to 7

 *   "3-"  -> from 3 to the last

 *   "-7"  -> from the beginning to 7

 *   "-"  -> from the beginning to the last

 * Note that Z_INTRANGE_BORDER is set for \a from to
 * represent the beginning, or for \a to to represent
 * the last.
 */
__EXPORT void zParseIntRange(char *expr, int *from, int *to);

/* ********************************************************** */
/* commandline argument vector
 * ********************************************************** */

/*! \brief get the next command line argument.
 *
 * zGetArg() gets the next argument from commandline
 * arguments. \a argv is a pointer to the argument vector,
 * which is directly proceeded.
 * An associated usage of this function is as follows:
 *
 *  int main(int argc, char *argv[]){
 *    char *arg;
 *    while( ( arg = zGetArg( &argv ) ) ){
 *      ...
 *    }
 *  }
 *
 * In the above case, zGetArg() directly increments
 * \a argv. Note that one can never handle to the
 * previous arguments.
 * \return
 * a pointer to the next argument, if it exists, is
 * returned. Otherwise, the null pointer is returned.
 */
__EXPORT char *zGetArg(char ***argv);

/*! \brief check if an argument string is an option switch.
 *
 * zIsOption() checks if a string is an option switch
 * or not. If \a o begins with '-' or '/', it regards
 * \a o as a commandline option switch.
 * \return
 * zIsOption() returns the true value if \a option
 * is regarded as a commandline option switch.
 * Otherwise, the false value is returned.
 */
#ifdef __WINDOWS__
# define zIsOption(o) \
  ( ( (o)[0] == '-' || (o)[0] == '/' ) && !isdigit( (o)[1] ) && (o)[1] != '-' )
#else
#define zIsOption(o) \
  ( ( (o)[0] == '-' ) && !isdigit( (o)[1] ) && (o)[1] != '-' )
#endif /* __WINDOWS__ */

/*! \} */

__END_DECLS

#endif /* __KERNEL__ */

#endif /* __ZEDA_OPTION_H__ */
