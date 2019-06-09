/* ZEDA - Elementary Data and Algorithms
 * Copyright (C) 1998 Tomomichi Sugihara (Zhidao)
 */
/*! \file zeda_bit.h
 * \brief bit operations.
 * \author Zhidao
 */

#ifndef __ZEDA_BIT_H__
#define __ZEDA_BIT_H__

#include <zeda/zeda_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \defgroup endian_op endian operations.
 * \{ *//* ************************************************** */

/* middle endian not supported. */
#define Z_ENDIAN_UNKNOWN 9999
#define Z_ENDIAN_LITTLE  1234
#define Z_ENDIAN_BIG     4321

/*! \brief check type of endian of the current architecture.
 *
 * endian_check() checks the type of endian of the current
 * architecture.
 * \return
 * It returns any of the following identifiers with respect to
 * the detected type:
 *  Z_ENDIAN_UNKNOWN ... unknown endian type (incl. middle endian)
 *  Z_ENDIAN_LITTLE  ... little endian
 *  Z_ENDIAN_BIG     ... big endian
 */
__EXPORT int endian_check(void);

/*! \brief convert 16-bit little/big endian to big/little endian.
 *
 * endian_reverse16() converts a little endian 16-bit value
 * to a bit endian value, and vice versa.
 * \return a value converted.
 * \note
 * PDP endian is not dealt with.
 */
__EXPORT uint16_t endian_reverse16(uint16_t val);

/*! \brief convert 32-bit little/big endian to big/little endian.
 *
 * endian_reverse32() converts a little endian 32-bit value
 * to a bit endian value, and vice versa.
 * \return a value converted.
 * \note
 * PDP endian is not dealt with.
 */
__EXPORT uint32_t endian_reverse32(uint32_t val);

/*! \brief convert 64-bit little/big endian to big/little endian.
 *
 * endian_reverse64() converts a little endian 64-bit value
 * to a bit endian value, and vice versa.
 * \return a value converted.
 * \note
 * PDP endian is not dealt with.
 */
__EXPORT uint64_t endian_reverse64(uint64_t val);

/*! \} */

/* ********************************************************** */
/*! \defgroup bit_op bit operations.
 * \{ *//* ************************************************** */

/*! \brief rotate a bit sequence.
 *
 * bit_rotate() rotates a bit sequence of a value \a val
 * with the bit width \a bitwidth.
 * \a d is a rotation displacement.
 * \return the value converted.
 */
__EXPORT ulong bit_rotate(ulong val, int bitwidth, int d);

/*! \brief reverse a bit sequence.
 *
 * bit_reverse() reverses a bit sequence \a val with
 * the bit width \a bitwidth.
 * \return the value converted.
 */
__EXPORT ulong bit_reverse(ulong val, int bitwidth);

/*! \brief convert a bit sequence to a string in binary format.
 *
 * sprintf_bit() converts a given value \a val to
 * a string in binary format. The string is stored
 * where a pointer \a str points.
 * \a width is the applied bit width.
 * For example, sprintf_bit( str, 0xf, 6 ) makes
 * \a str "001111".
 * \sa fprintf_bit
 */
__EXPORT void sprintf_bit(char *str, ulong val, int width);

#ifndef __KERNEL__
/*! \brief output a bit sequence to a file in binary format.
 *
 * fprintf_bit() outputs a given value \a val to the
 * current position of a file pointed by \a fp.
 * The output format is the same with that of sprintf_bit().
 * \note
 * fprintf_bit() is not available in the kernel space.
 */
__EXPORT void fprintf_bit(FILE *fp, ulong val, int width);
/*! \brief output a bit sequence to the standard output. */
#define printf_bit(v,w) fprintf_bit( stdout, (v), (w) )
#else
void printk_bit(ulong val, int width);
#endif /* __KERNEL__ */

/*! \} */

__END_DECLS

#endif /* __ZEDA_BIT_H__ */
