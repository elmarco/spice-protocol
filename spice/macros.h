/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2010 Red Hat, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* This file is to a large extent based on gmacros.h from glib
 * Which is LGPL and copyright:
 *
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef _H_SPICE_MACROS
#define _H_SPICE_MACROS

/* We include stddef.h to get the system's definition of NULL */
#include <stddef.h>

#include <spice/types.h>

#if    __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 96)
#define SPICE_GNUC_PURE __attribute__((__pure__))
#define SPICE_GNUC_MALLOC __attribute__((__malloc__))
#else
#define SPICE_GNUC_PURE
#define SPICE_GNUC_MALLOC
#endif

#if     __GNUC__ >= 4
#define SPICE_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#else
#define SPICE_GNUC_NULL_TERMINATED
#endif

#if     (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#define SPICE_GNUC_ALLOC_SIZE(x) __attribute__((__alloc_size__(x)))
#define SPICE_GNUC_ALLOC_SIZE2(x,y) __attribute__((__alloc_size__(x,y)))
#else
#define SPICE_GNUC_ALLOC_SIZE(x)
#define SPICE_GNUC_ALLOC_SIZE2(x,y)
#endif

#if     __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define SPICE_GNUC_PRINTF( format_idx, arg_idx ) __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#define SPICE_GNUC_SCANF( format_idx, arg_idx ) __attribute__((__format__ (__scanf__, format_idx, arg_idx)))
#define SPICE_GNUC_FORMAT( arg_idx ) __attribute__((__format_arg__ (arg_idx)))
#define SPICE_GNUC_NORETURN __attribute__((__noreturn__))
#define SPICE_GNUC_CONST __attribute__((__const__))
#define SPICE_GNUC_UNUSED __attribute__((__unused__))
#define SPICE_GNUC_NO_INSTRUMENT __attribute__((__no_instrument_function__))
#else   /* !__GNUC__ */
#define SPICE_GNUC_PRINTF( format_idx, arg_idx )
#define SPICE_GNUC_SCANF( format_idx, arg_idx )
#define SPICE_GNUC_FORMAT( arg_idx )
#define SPICE_GNUC_NORETURN
#define SPICE_GNUC_CONST
#define SPICE_GNUC_UNUSED
#define SPICE_GNUC_NO_INSTRUMENT
#endif  /* !__GNUC__ */

#if    __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#define SPICE_GNUC_DEPRECATED  __attribute__((__deprecated__))
#else
#define SPICE_GNUC_DEPRECATED
#endif /* __GNUC__ */

#if     __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
#  define SPICE_GNUC_MAY_ALIAS __attribute__((may_alias))
#else
#  define SPICE_GNUC_MAY_ALIAS
#endif

#if    __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#define SPICE_GNUC_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define SPICE_GNUC_WARN_UNUSED_RESULT
#endif /* __GNUC__ */


/* Guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
# define SPICE_BEGIN_DECLS  extern "C" {
# define SPICE_END_DECLS    }
#else
# define SPICE_BEGIN_DECLS
# define SPICE_END_DECLS
#endif

#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)     (((a) < 0) ? -(a) : (a))

/* Count the number of elements in an array. The array must be defined
 * as such; using this with a dynamically allocated array will give
 * incorrect results.
 */
#define SPICE_N_ELEMENTS(arr) (sizeof (arr) / sizeof ((arr)[0]))

#define SPICE_ALIGN(a, size) (((a) + ((size) - 1)) & ~((size) - 1))

/* Provide convenience macros for handling structure
 * fields through their offsets.
 */

#if defined(__GNUC__)  && __GNUC__ >= 4
#  define SPICE_OFFSETOF(struct_type, member) \
    ((long) offsetof (struct_type, member))
#else
#  define SPICE_OFFSETOF(struct_type, member)	\
    ((long) ((uint8_t*) &((struct_type*) 0)->member))
#endif

#define SPICE_CONTAINEROF(ptr, struct_type, member) \
    ((struct_type *)((uint8_t *)(ptr) - SPICE_OFFSETOF(struct_type, member)))

#define SPICE_MEMBER_P(struct_p, struct_offset)   \
    ((gpointer) ((guint8*) (struct_p) + (glong) (struct_offset)))
#define SPICE_MEMBER(member_type, struct_p, struct_offset)   \
    (*(member_type*) SPICE_STRUCT_MEMBER_P ((struct_p), (struct_offset)))

/* Provide simple macro statement wrappers:
 *   SPICE_STMT_START { statements; } SPICE_STMT_END;
 * This can be used as a single statement, like:
 *   if (x) SPICE_STMT_START { ... } SPICE_STMT_END; else ...
 * This intentionally does not use compiler extensions like GCC's '({...})' to
 * avoid portability issue or side effects when compiled with different compilers.
 */
#if !(defined (SPICE_STMT_START) && defined (SPICE_STMT_END))
#  define SPICE_STMT_START  do
#  define SPICE_STMT_END    while (0)
#endif

/*
 * The SPICE_LIKELY and SPICE_UNLIKELY macros let the programmer give hints to
 * the compiler about the expected result of an expression. Some compilers
 * can use this information for optimizations.
 *
 * The _SPICE_BOOLEAN_EXPR macro is intended to trigger a gcc warning when
 * putting assignments in g_return_if_fail ().
 */
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#define _SPICE_BOOLEAN_EXPR(expr)               \
 __extension__ ({                               \
   int _g_boolean_var_;                         \
   if (expr)                                    \
      _g_boolean_var_ = 1;                      \
   else                                         \
      _g_boolean_var_ = 0;                      \
   _g_boolean_var_;                             \
})
#define SPICE_LIKELY(expr) (__builtin_expect (_SPICE_BOOLEAN_EXPR(expr), 1))
#define SPICE_UNLIKELY(expr) (__builtin_expect (_SPICE_BOOLEAN_EXPR(expr), 0))
#else
#define SPICE_LIKELY(expr) (expr)
#define SPICE_UNLIKELY(expr) (expr)
#endif

#endif /* _H_SPICE_MACROS */
