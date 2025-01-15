       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

// <utility> -*- C++ -*-

// Copyright (C) 2001-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file include/utility
 *  This is a Standard C++ Library header.
 */



       

/**
 * @defgroup utilities Utilities
 *
 * Basic function and class templates used with the rest of the library.
 * Includes pair, swap, forward/move helpers, declval, integer_sequence.
 */

// Predefined symbols and macros -*- C++ -*-

// Copyright (C) 1997-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/c++config.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{version}
 */



       

// The major release number for the GCC release the C++ library belongs to.

// The datestamp of the C++ library in compressed ISO date format.

// Macros for various attributes.
//   _GLIBCXX_PURE
//   _GLIBCXX_CONST
//   _GLIBCXX_NORETURN
//   _GLIBCXX_NOTHROW
//   _GLIBCXX_VISIBILITY









// See below for C++






// Macros for visibility attributes.
//   _GLIBCXX_HAVE_ATTRIBUTE_VISIBILITY
//   _GLIBCXX_VISIBILITY








// Macros for deprecated attributes.
//   _GLIBCXX_USE_DEPRECATED
//   _GLIBCXX_DEPRECATED
//   _GLIBCXX_DEPRECATED_SUGGEST( string-literal )
//   _GLIBCXX11_DEPRECATED
//   _GLIBCXX11_DEPRECATED_SUGGEST( string-literal )
//   _GLIBCXX14_DEPRECATED
//   _GLIBCXX14_DEPRECATED_SUGGEST( string-literal )
//   _GLIBCXX17_DEPRECATED
//   _GLIBCXX17_DEPRECATED_SUGGEST( string-literal )
//   _GLIBCXX20_DEPRECATED
//   _GLIBCXX20_DEPRECATED_SUGGEST( string-literal )
//   _GLIBCXX23_DEPRECATED
//   _GLIBCXX23_DEPRECATED_SUGGEST( string-literal )








































// Macros for ABI tag attributes.



// Macro to warn about unused results.









// Macro for constexpr, to support in mixed 03/0x mode.











































// Macro for noexcept, to support in mixed 03/0x mode.


























// Macro for extern template, ie controlling template linkage via use
// of extern keyword on template declaration. As documented in the g++
// manual, it inhibits all implicit instantiations and is used
// throughout the library to avoid multiple weak definitions for
// required types that are already explicitly instantiated in the
// library binary. This substantially reduces the binary size of
// resulting executables.
// Special case: _GLIBCXX_EXTERN_TEMPLATE == -1 disallows extern
// templates only in basic_string, thus activating its debug-mode
// checks even at -O0.

/*
  Outline of libstdc++ namespaces.

  namespace std
  {
    namespace __debug { }
    namespace __parallel { }
    namespace __cxx1998 { }

    namespace __detail {
      namespace __variant { }				// C++17
    }

    namespace rel_ops { }

    namespace tr1
    {
      namespace placeholders { }
      namespace regex_constants { }
      namespace __detail { }
    }

    namespace tr2 { }
    
    namespace decimal { }

    namespace chrono { }				// C++11
    namespace placeholders { }				// C++11
    namespace regex_constants { }			// C++11
    namespace this_thread { }				// C++11
    inline namespace literals {				// C++14
      inline namespace chrono_literals { }		// C++14
      inline namespace complex_literals { }		// C++14
      inline namespace string_literals { }		// C++14
      inline namespace string_view_literals { }		// C++17
    }
  }

  namespace abi { }

  namespace __gnu_cxx
  {
    namespace __detail { }
  }

  For full details see:
  http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/namespaces.html
*/
namespace std
{
  typedef __SIZE_TYPE__ 	size_t;
  typedef __PTRDIFF_TYPE__	ptrdiff_t;


  typedef decltype(nullptr)	nullptr_t;


  // This allows the library to terminate without including all of <exception>
  // and without making the declaration of std::terminate visible to users.
  extern "C++" __attribute__ ((__noreturn__, __always_inline__))
  inline void __terminate() _GLIBCXX_USE_NOEXCEPT
  {
    void terminate() _GLIBCXX_USE_NOEXCEPT __attribute__ ((__noreturn__,__cold__));
    terminate();
  }
}











namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}







// Non-zero if inline namespaces are used for versioning the entire library.

// Unused.
// Used to version individual components, e.g. std::_V2::error_category.


// In the case that we don't have a hosted environment, we can't provide the
// debugging mode.  Instead, we do our best and downgrade to assertions.





// Inline namespaces for special modes: debug, parallel.

// Macros for namespace scope. Either namespace std:: or the name
// of some nested namespace within it corresponding to the active mode.
// _GLIBCXX_STD_A
// _GLIBCXX_STD_C
//
// Macros for opening/closing conditional namespaces.
// _GLIBCXX_BEGIN_NAMESPACE_ALGO
// _GLIBCXX_END_NAMESPACE_ALGO
// _GLIBCXX_BEGIN_NAMESPACE_CONTAINER
// _GLIBCXX_END_NAMESPACE_CONTAINER
















// GLIBCXX_ABI Deprecated
// Define if compatibility should be provided for -mlong-double-64.

// Define if compatibility should be provided for alternative 128-bit long
// double formats. Not possible for Clang until __ibm128 is supported.



// Inline namespaces for long double 128 modes.












namespace std
{
  // Internal version of std::is_constant_evaluated().
  // This can be used without checking if the compiler supports the feature.
  // The macro _GLIBCXX_HAVE_IS_CONSTANT_EVALUATED can be used to check if
  // the compiler support is present to make this function work as expected.
  __attribute__((__always_inline__))
  _GLIBCXX_CONSTEXPR inline bool
  __is_constant_evaluated() _GLIBCXX_NOEXCEPT
  {




    return __builtin_is_constant_evaluated();



  }
}

// Debug Mode implies checking assertions.




// Disable std::string explicit instantiation declarations in order to assert.






// Assert.

namespace std
{
  // Don't use <cassert> because this should be unaffected by NDEBUG.
  extern "C++" _GLIBCXX_NORETURN
  void
  __glibcxx_assert_fail /* Called when a precondition violation is detected. */
    (const char* __file, int __line, const char* __function,
     const char* __condition)
  _GLIBCXX_NOEXCEPT;
}






// Only check assertions during constant evaluation.
namespace std
{
  __attribute__((__always_inline__,__visibility__("default")))
  inline void
  __glibcxx_assert_fail()
  { }
}










// Macro indicating that TSAN is in use.








// Macros for race detectors.
// _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) and
// _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A) should be used to explain
// atomic (lock-free) synchronization to race detectors:
// the race detector will infer a happens-before arc from the former to the
// latter when they share the same argument pointer.
//
// The most frequent use case for these macros (and the only case in the
// current implementation of the library) is atomic reference counting:
//   void _M_remove_reference()
//   {
//     _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(&this->_M_refcount);
//     if (__gnu_cxx::__exchange_and_add_dispatch(&this->_M_refcount, -1) <= 0)
//       {
//         _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(&this->_M_refcount);
//         _M_destroy(__a);
//       }
//   }
// The annotations in this example tell the race detector that all memory
// accesses occurred when the refcount was positive do not race with
// memory accesses which occurred after the refcount became zero.





// Macros for C linkage: define extern "C" linkage only when using C++.

















// First includes.

// Pick up any OS-specific definitions.
// Specific definitions for GNU/Linux  -*- C++ -*-

// Copyright (C) 2000-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/os_defines.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */



// file will come before all others.

// This keeps isalnum, et al from being propagated as macros.

/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */



/* These are defined by the user (or the compiler)
   to specify the desired environment:

   __STRICT_ANSI__	ISO Standard C.
   _ISOC99_SOURCE	Extensions to ISO C89 from ISO C99.
   _ISOC11_SOURCE	Extensions to ISO C99 from ISO C11.
   _ISOC23_SOURCE	Extensions to ISO C99 from ISO C23.
   _ISOC2X_SOURCE	Old name for _ISOC23_SOURCE.
   __STDC_WANT_LIB_EXT2__
			Extensions to ISO C99 from TR 27431-2:2010.
   __STDC_WANT_IEC_60559_BFP_EXT__
			Extensions to ISO C11 from TS 18661-1:2014.
   __STDC_WANT_IEC_60559_FUNCS_EXT__
			Extensions to ISO C11 from TS 18661-4:2015.
   __STDC_WANT_IEC_60559_TYPES_EXT__
			Extensions to ISO C11 from TS 18661-3:2015.
   __STDC_WANT_IEC_60559_EXT__
			ISO C23 interfaces defined only in Annex F.

   _POSIX_SOURCE	IEEE Std 1003.1.
   _POSIX_C_SOURCE	If ==1, like _POSIX_SOURCE; if >=2 add IEEE Std 1003.2;
			if >=199309L, add IEEE Std 1003.1b-1993;
			if >=199506L, add IEEE Std 1003.1c-1995;
			if >=200112L, all of IEEE 1003.1-2004
			if >=200809L, all of IEEE 1003.1-2008
   _XOPEN_SOURCE	Includes POSIX and XPG things.  Set to 500 if
			Single Unix conformance is wanted, to 600 for the
			sixth revision, to 700 for the seventh revision.
   _XOPEN_SOURCE_EXTENDED XPG things and X/Open Unix extensions.
   _LARGEFILE_SOURCE	Some more functions for correct standard I/O.
   _LARGEFILE64_SOURCE	Additional functionality from LFS for large files.
   _FILE_OFFSET_BITS=N	Select default filesystem interface.
   _ATFILE_SOURCE	Additional *at interfaces.
   _DYNAMIC_STACK_SIZE_SOURCE Select correct (but non compile-time constant)
			MINSIGSTKSZ, SIGSTKSZ and PTHREAD_STACK_MIN.
   _GNU_SOURCE		All of the above, plus GNU extensions.
   _DEFAULT_SOURCE	The default set of features (taking precedence over
			__STRICT_ANSI__).

   _FORTIFY_SOURCE	Add security hardening to many library functions.
			Set to 1, 2 or 3; 3 performs stricter checks than 2, which
			performs stricter checks than 1.

   _REENTRANT, _THREAD_SAFE
			Obsolete; equivalent to _POSIX_C_SOURCE=199506L.

   The `-ansi' switch to the GNU C compiler, and standards conformance
   options such as `-std=c99', define __STRICT_ANSI__.  If none of
   these are defined, or if _DEFAULT_SOURCE is defined, the default is
   to have _POSIX_SOURCE set to one and _POSIX_C_SOURCE set to
   200809L, as well as enabling miscellaneous functions from BSD and
   SVID.  If more than one of these are defined, they accumulate.  For
   example __STRICT_ANSI__, _POSIX_SOURCE and _POSIX_C_SOURCE together
   give you ISO C, 1003.1, and 1003.2, but nothing else.

   These are defined by this file and are used by the
   header files to decide what to declare or define:

   __GLIBC_USE (F)	Define things from feature set F.  This is defined
			to 1 or 0; the subsequent macros are either defined
			or undefined, and those tests should be moved to
			__GLIBC_USE.
   __USE_ISOC11		Define ISO C11 things.
   __USE_ISOC99		Define ISO C99 things.
   __USE_ISOC95		Define ISO C90 AMD1 (C95) things.
   __USE_ISOCXX11	Define ISO C++11 things.
   __USE_POSIX		Define IEEE Std 1003.1 things.
   __USE_POSIX2		Define IEEE Std 1003.2 things.
   __USE_POSIX199309	Define IEEE Std 1003.1, and .1b things.
   __USE_POSIX199506	Define IEEE Std 1003.1, .1b, .1c and .1i things.
   __USE_XOPEN		Define XPG things.
   __USE_XOPEN_EXTENDED	Define X/Open Unix things.
   __USE_UNIX98		Define Single Unix V2 things.
   __USE_XOPEN2K        Define XPG6 things.
   __USE_XOPEN2KXSI     Define XPG6 XSI things.
   __USE_XOPEN2K8       Define XPG7 things.
   __USE_XOPEN2K8XSI    Define XPG7 XSI things.
   __USE_LARGEFILE	Define correct standard I/O things.
   __USE_LARGEFILE64	Define LFS things with separate names.
   __USE_FILE_OFFSET64	Define 64bit interface as default.
   __USE_MISC		Define things from 4.3BSD or System V Unix.
   __USE_ATFILE		Define *at interfaces and AT_* constants for them.
   __USE_DYNAMIC_STACK_SIZE Define correct (but non compile-time constant)
			MINSIGSTKSZ, SIGSTKSZ and PTHREAD_STACK_MIN.
   __USE_GNU		Define GNU extensions.
   __USE_FORTIFY_LEVEL	Additional security measures used, according to level.

   The macros `__GNU_LIBRARY__', `__GLIBC__', and `__GLIBC_MINOR__' are
   defined by this file unconditionally.  `__GNU_LIBRARY__' is provided
   only for compatibility.  All new code should use the other symbols
   to test for features.

   All macros listed above as possibly being defined by this file are
   explicitly undefined if they are not explicitly defined.
   Feature-test macros that are not defined by the user or compiler
   but are implied by the other feature-test macros defined (or by the
   lack of any definitions) are defined by the file.

   ISO C feature test macros depend on the definition of the macro
   when an affected header is included, not when the first system
   header is included, and so they are handled in
   <bits/libc-header-start.h>, which does not have a multiple include
   guard.  Feature test macros that can be handled from the first
   system header included are handled here.  */


/* Undefine everything, so we get a clean slate.  */

/* Suppress kernel-name space pollution unless user expressedly asks
   for it.  */



/* Convenience macro to test the version of gcc.
   Use like this:
   ... code requiring gcc 2.8 or later ...
   Note: only works for GCC 2.0 and later, because __GNUC_MINOR__ was
   added in 2.0.  */






/* Similarly for clang.  Features added to GCC after version 4.2 may
   or may not also be available in clang, and clang's definitions of
   __GNUC(_MINOR)__ are fixed at 4 and 2 respectively.  Not all such
   features can be queried via __has_extension/__has_feature.  */






/* Whether to use feature set F.  */

/* _BSD_SOURCE and _SVID_SOURCE are deprecated aliases for
   _DEFAULT_SOURCE.  If _DEFAULT_SOURCE is present we do not
   issue a warning; the expectation is that the source is being
   transitioned to use the new macro.  */







/* Remap the old name _ISOC2X_SOURCE to _ISOC23_SOURCE.  */






/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */



/* If nothing (other than _GNU_SOURCE and _DEFAULT_SOURCE) is defined,
   define _DEFAULT_SOURCE.  */








/* This is to enable the ISO C23 extension.  */






/* This is to enable the ISO C11 extension.  */




/* This is to enable the ISO C99 extension.  */





/* This is to enable the ISO C90 Amendment 1:1995 extension.  */






/* This is to enable compatibility for ISO C++17.  */


/* This is to enable compatibility for ISO C++11.
   Check the temporary macro for now, too.  */




/* If none of the ANSI/POSIX macros are defined, or if _DEFAULT_SOURCE
   is defined, use POSIX.1-2008 (or another version depending on
   _XOPEN_SOURCE).  */







/* Some C libraries once required _REENTRANT and/or _THREAD_SAFE to be
   defined in all multithreaded code.  GNU libc has not required this
   for many years.  We now treat them as compatibility synonyms for
   _POSIX_C_SOURCE=199506L, which is the earliest level of POSIX with
   comprehensive support for multithreaded code.  Using them never
   lowers the selected level of POSIX conformance, only raises it.  */


















































/* Features part to handle 64-bit time_t support.
   Copyright (C) 2021-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* We need to know the word size in order to check the time size.  */
/* Determine the wordsize from the preprocessor defines.  */










/* Both x86-64 and x32 use the 64-bit system call interface.  */
/* Bit size of the time_t type at glibc build time, x86-64 and x32 case.
   Copyright (C) 2018-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* Determine the wordsize from the preprocessor defines.  */










/* Both x86-64 and x32 use the 64-bit system call interface.  */





/* For others, time size is word size.  */
































/* The function 'gets' existed in C89, but is impossible to use
   safely.  It has been removed from ISO C11 and ISO C++14.  Note: for
   compatibility with various implementations of <cstdio>, this test
   must consider only the value of __cplusplus when compiling C++.  */





/* GNU formerly extended the scanf functions with modified format
   specifiers %as, %aS, and %a[...] that allocate a buffer for the
   input using malloc.  This extension conflicts with ISO C99, which
   defines %a as a standalone format specifier that reads a floating-
   point number; moreover, POSIX.1-2008 provides the same feature
   using the modifier letter 'm' instead (%ms, %mS, %m[...]).

   We now follow C99 unless GNU extensions are active and the compiler
   is specifically in C89 or C++98 mode (strict or not).  For
   instance, with GCC, -std=gnu11 will have C99-compliant scanf with
   or without -D_GNU_SOURCE, but -std=c89 -D_GNU_SOURCE will have the
   old extension.  */








/* ISO C23 added support for a 0b or 0B prefix on binary constants as
   inputs to strtol-family functions (base 0 or 2).  This macro is
   used to condition redirection in headers to allow that redirection
   to be disabled when building those functions, despite _GNU_SOURCE
   being defined.  */





/* Get definitions of __STDC_* predefined macros, if the compiler has
   not preincluded this header automatically.  */


/* This macro indicates that the installed library is the GNU C Library.
   For historic reasons the value now is 6 and this will stay from now
   on.  The use of this variable is deprecated.  Use __GLIBC__ and
   __GLIBC_MINOR__ now (see below) when you want to test for a specific
   GNU C library version and use the values in <gnu/lib-names.h> to get
   the sonames of the shared libraries.  */

/* Major and minor version number of the GNU C library package.  Use
   these macros to test for features in specific releases.  */



/* This is here only because every header file already includes this one.  */


/* Copyright (C) 1992-2024 Free Software Foundation, Inc.
   Copyright The GNU Toolchain Authors.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */



/* We are almost always included from features.h. */




/* The GNU libc does not support any K&R compilers or the traditional mode
   of ISO C compilers anymore.  Check for some of the combinations not
   supported anymore.  */




/* Some user header file might have defined this before.  */

/* Compilers that lack __has_attribute may object to
   even though they do not need to evaluate the right-hand side of the &&.
   Similarly for __has_builtin, etc.  */

















/* All functions, except those with callbacks or those that
   synchronize memory, are leaf functions.  */






/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this only works with
   gcc 2.8.x and egcs.  For gcc 3.4 and up we even mark C functions
   as non-throwing using a function attribute since programs can use
   the -fexceptions options for C code as well.  */


























/* These two macros are not used in glibc anymore.  They are kept here
   only because some other projects expect the macros to be defined.  */

/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */


/* This is not a typedef so `const __ptr_t' does the right thing.  */


/* C++ needs to know that types and declarations are C, not C++.  */







/* The overloadable attribute was added on clang 2.6. */






/* Fortify support.  */

/* Use __builtin_dynamic_object_size at _FORTIFY_SOURCE=3 when available.  */








/* Compile time conditions to choose between the regular, _chk and _chk_warn
   variants.  These conditions should get evaluated to constant and optimized
   away.  */




/* Length is known to be safe at compile time if the __L * __S <= __OBJSZ
   condition can be folded to a constant and if it is true, or unknown (-1) */






/* Conversely, we know at compile time that the length is unsafe if the
   __L * __S <= __OBJSZ condition can be folded to a constant and if it is
   false.  */





/* To correctly instrument the fortify wrapper clang requires the
   pass_object_size attribute, and the attribute has the restriction that the
   argument needs to be 'const'.  Furthermore, to make it usable with C
   interfaces, clang provides the overload attribute, which provides a C++
   like function overload support.  The overloaded fortify wrapper with the
   pass_object_size attribute has precedence over the default symbol.

   Also, clang does not support __va_arg_pack, so variadic functions are
   expanded to issue va_arg implementations. The error function must not have
   bodies (address takes are expanded to nonfortified calls), and with
   __fortify_function compiler might still create a body with the C++
   mangling name (due to the overload attribute).  In this case, the function
   is defined with __fortify_function_error_function macro instead.

   The argument size check is also done with a clang-only attribute,
   __attribute__ ((__diagnose_if__ (...))), different than gcc which calls
   symbol_chk_warn alias with uses __warnattr attribute.

   The pass_object_size was added on clang 4.0, __diagnose_if__ on 5.0,
   and pass_dynamic_object_size on 9.0.  */





/* Fortify function f.  __f_alias, __f_chk and __f_chk_warn must be
   declared.  */














/* Fortify function f, where object size argument passed to f is the number of
   elements and not total size.  */























/* Support for flexible arrays.
   Headers that should use flexible arrays only if they're "real"
   (e.g. only if they won't affect sizeof()) should test




/* GCC 2.97 and clang support C99 flexible array members as an extension,
   even when in C89 mode or compiling C++ (any version).  */


/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */



















/*

*/


/* GCC and clang have various useful declarations that can be made with
   the '__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it.  */




/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */





/* Tell the compiler which arguments to an allocation function
   indicate the size of the allocation.  */






/* Tell the compiler which argument to an allocation function
   indicates the alignment of the allocation.  */






/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */





/* This declaration tells the compiler that the value is constant.  */










/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */






/* Since version 3.2, gcc allows marking deprecated functions.  */





/* Since version 4.5, gcc also allows one to specify the message printed
   when a deprecated function is used.  clang claims to be gcc 4.2, but
   may also support this feature.  */







/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */





/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */






/* The nonnull function attribute marks pointer parameters that
   must not be NULL.  This has the name __nonnull in glibc,
   and __attribute_nonnull__ in files shared with Gnulib to avoid
   collision with a different __nonnull in DragonFlyBSD 5.9.  */









/* The returns_nonnull function attribute marks the return type of the function
   as always being non-null.  */







/* If fortification mode, we warn about unused results of certain
   function calls which can lead to problems.  */











/* Forces a function to be always inlined.  */

/* The Linux kernel defines __always_inline in stddef.h (283d7573), and
   it conflicts with this definition.  Therefore undefine it first to
   allow either header to be included first.  */





/* Associate error messages with the source location of the call site rather
   than with the source location inside the function.  */





/* GCC 4.3 and above with -std=c99 or -std=gnu99 implements ISO C99
   inline semantics, unless -fgnu89-inline is used.  Using __GNUC_STDC_INLINE__
   or __GNUC_GNU_INLINE is not a good enough check for gcc because gcc versions
   older than 4.3 may define these macros and still not guarantee GNU inlining
   semantics.

   clang++ identifies itself as gcc-4.2, but has support for GNU inlining
   semantics, that can be checked for by using the __GNUC_STDC_INLINE_ and
   __GNUC_GNU_INLINE__ macro definitions.  */














/* GCC 4.3 and above allow passing all anonymous arguments of an
   __extern_always_inline function to some other vararg function.  */



/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */




/* __restrict is known in EGCS 1.2 and above, and in clang.
   It works also in C++ mode (outside of arrays), but only when spelled
   as '__restrict', not 'restrict'.  */








/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 and clang support this.
   This syntax is not usable in C++ mode.  */














/* Describes a char array whose address can safely be passed as the first
   argument to strncpy and strncat, as the char array is not necessarily
   a NUL-terminated string.  */




/* Undefine (also defined in libc-symbols.h).  */

/* Copies attributes from the declaration or type referenced by
   the argument.  */





/* Gnulib avoids including these, as they don't work on non-glibc or
   older glibc platforms.  */

/* Determine the wordsize from the preprocessor defines.  */










/* Both x86-64 and x32 use the 64-bit system call interface.  */
/* Properties of long double type.  ldbl-96 version.
   Copyright (C) 2016-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License  published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* long double is distinct from double, so there is nothing to
   define here.  */









/* __glibc_macro_warning (MESSAGE) issues warning MESSAGE.  This is
   intended for use in preprocessor macros.

   Note: MESSAGE must be a _single_ string; concatenation of string
   literals is not supported.  */






/* Generic selection (ISO C11) is a C-only feature, available in GCC
   since version 4.9.  Previous versions do not provide generic
   selection, even though they might set __STDC_VERSION__ to 201112L,
   when in -std=c11 mode.  Thus, we must check for !defined __GNUC__
   when testing __STDC_VERSION__ for generic selection support.
   On the other hand, Clang also defines __GNUC__, so a clang-specific
   check is required to enable the use of generic selection.  */










/* Designates a 1-based positional argument ref-index of pointer type
   that can be used to access size-index elements of the pointed-to
   array according to access mode, or at least one element when
   size-index is not provided:
     access (access-mode, <ref-index> [, <size-index>])  */
/* For _FORTIFY_SOURCE == 3 we use __builtin_dynamic_object_size, which may
   use the access attribute to get object sizes from function definition
   arguments, so we can't use them on functions we fortify.  Drop the access
   attribute for such functions.  */















/* Designates dealloc as a function to call to deallocate objects
   allocated by the declared function.  */






/* Specify that a function such as setjmp or vfork may return
   twice.  */





/* Mark struct types as aliasable.  Restricted to compilers that
   support forward declarations of structs in the presence of the
   attribute.  */







/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */







/* Decide whether we can define 'extern inline' functions in headers.  */






/* This is here only because every header file already includes this one.
   Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   that will always return failure (and set errno to ENOSYS).  */
/* This file is automatically generated.
   This file selects the right generated file of `__stub_FUNCTION' macros
   based on the architecture being compiled for.  */






/* This file is automatically generated.
   It defines a symbol `__stub_FUNCTION' for each function
   in the C library which is a stub, meaning it will fail
   every time called, usually setting errno to ENOSYS.  */









// Provide a declaration for the possibly deprecated gets function, as
// glibc 2.15 and later does not declare gets for ISO C11 when
// __GNU_SOURCE is defined.



// Glibc 2.23 removed the obsolete isinf and isnan declarations. Check the
// version dynamically in case it has changed since libstdc++ was configured.

// Glibc 2.26 on i?86/x86-64/ia64/ppc64le added *f128 support.
// Glibc 2.27 added it also on many other arches but those have IEEE quad
// long double.







// Enable __cxa_thread_atexit to rely on a (presumably libc-provided)
// __cxa_thread_atexit_impl, if it happens to be defined, even if
// configure couldn't find it during the build.


// The following libpthread properties only apply to Linux, not GNU/Hurd.


// Since glibc 2.27 pthread_self() is usable without linking to libpthread.


// Since glibc 2.34 all pthreads functions are usable without linking to
// libpthread.




// Pick up any CPU-specific definitions.
// Specific definitions for generic platforms  -*- C++ -*-

// Copyright (C) 2005-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/cpu_defines.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{iosfwd}
 */




// If platform uses neither visibility nor psuedo-visibility,
// specify empty default for namespace annotation macros.



// Certain function definitions that are meant to be overridable from
// user code are decorated with this macro.  For some targets, this
// macro causes these definitions to be weak.



// By default, we assume that __GXX_WEAK__ also means that there is support
// for declaring functions as weak while not defining such functions.  This
// allows for referring to functions provided by other libraries (e.g.,
// libitm) without depending on them if the respective features are not used.



// Conditionally enable annotations for the Transactional Memory TS on C++11.
// Most of the following conditions are due to limitations in the current
// implementation.










// In C++17 mathematical special functions are in namespace std.





// The remainder of the prewritten config is automatic; all the
// user hooks are listed above.

// Create a boolean flag to be used to determine if --fast-math is set.





// This marks string literals in header files to be extracted for eventual
// translation.  It is primarily used for messages in thrown exceptions; see
// src/functexcept.cc.  We use __N because the more traditional _N is used
// for something else under certain OSes (see BADNAMES).


// N.B. these _GLIBCXX_USE_C99_XXX macros are defined unconditionally












// Unless explicitly specified, enable char8_t extensions only if the core
// language char8_t feature macro is defined.









/* Define if __float128 is supported on this host.  */

/* For powerpc64 don't use __float128 when it's the same type as long double. */




// Define if float has the IEEE binary32 format.





// Define if double has the IEEE binary64 format.





// Define if long double has the IEEE binary128 format.







namespace __gnu_cxx
{
  typedef __decltype(0.0bf16) __bfloat16_t;
}



















// Returns 1 if _GLIBCXX_DO_NOT_USE_BUILTIN_TRAITS is not defined and the
// compiler has a corresponding built-in type trait, 0 otherwise.
// _GLIBCXX_DO_NOT_USE_BUILTIN_TRAITS can be defined to disable the use of
// built-in traits.





// Mark code that should be ignored by the compiler, but seen by Doxygen.

// PSTL configuration


// This header is not installed for freestanding:

// Preserved here so we have some idea which version of upstream we've pulled in

// For now this defaults to being based on the presence of Thread Building Blocks


// This section will need some rework when a new (default) backend type is added






// -*- C++ -*-
//===-- pstl_config.h -----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//



// The version is XYYZ, where X is major, YY is minor, and Z is patch (i.e. X.YY.Z)





// Check the user-defined macro for warnings




















// note that when ICC or Clang is in use, _PSTL_GCC_VERSION might not fully match
// the actual GCC version on the system.






// Enable SIMD for compilers that support OpenMP 4.0


















// Should be defined to 1 for environments with a vendor implementation of C++17 execution policies










































// Declaration of reduction functor, where
// NAME - the name of the functor
// OP - type of the callable object with the reduction operation
// omp_in - refers to the local partial result
// omp_out - refers to the final value of the combiner operator
// omp_priv - refers to the private copy of the initial value
// omp_orig - refers to the original variable to be reduced







// Check the user-defined macro to use non-temporal stores

















// broken macros












// End of prewritten config; the settings discovered at configure time follow.
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the `acosf' function. */

/* Define to 1 if you have the `acosl' function. */

/* Define to 1 if you have the `aligned_alloc' function. */

/* Define if arc4random is available in <stdlib.h>. */

/* Define to 1 if you have the <arpa/inet.h> header file. */

/* Define to 1 if you have the `asinf' function. */

/* Define to 1 if you have the `asinl' function. */

/* Define to 1 if the target assembler supports .symver directive. */

/* Define to 1 if you have the `atan2f' function. */

/* Define to 1 if you have the `atan2l' function. */

/* Define to 1 if you have the `atanf' function. */

/* Define to 1 if you have the `atanl' function. */

/* Defined if shared_ptr reference counting should use atomic operations. */

/* Define to 1 if you have the `at_quick_exit' function. */

/* Define if C99 float_t and double_t in <math.h> should be imported in
   <cmath> in namespace std for C++11. */

/* Define to 1 if the target assembler supports thread-local storage. */

/* Define to 1 if you have the `ceilf' function. */

/* Define to 1 if you have the `ceill' function. */

/* Define to 1 if you have the <complex.h> header file. */

/* Define to 1 if you have the `cosf' function. */

/* Define to 1 if you have the `coshf' function. */

/* Define to 1 if you have the `coshl' function. */

/* Define to 1 if you have the `cosl' function. */

/* Define to 1 if you have the declaration of `strnlen', and to 0 if you
   don't. */

/* Define to 1 if you have the <dirent.h> header file. */

/* Define if dirfd is available in <dirent.h>. */

/* Define to 1 if you have the <dlfcn.h> header file. */

/* Define to 1 if you have the <endian.h> header file. */

/* Define to 1 if GCC 4.6 supported std::exception_ptr for the target */

/* Define to 1 if you have the <execinfo.h> header file. */

/* Define to 1 if you have the `expf' function. */

/* Define to 1 if you have the `expl' function. */

/* Define to 1 if you have the `fabsf' function. */

/* Define to 1 if you have the `fabsl' function. */

/* Define to 1 if you have the <fcntl.h> header file. */

/* Define if fdopendir is available in <dirent.h>. */

/* Define to 1 if you have the <fenv.h> header file. */

/* Define to 1 if you have the `finite' function. */

/* Define to 1 if you have the `finitef' function. */

/* Define to 1 if you have the `finitel' function. */

/* Define to 1 if you have the <float.h> header file. */

/* Define to 1 if you have the `floorf' function. */

/* Define to 1 if you have the `floorl' function. */

/* Define to 1 if you have the `fmodf' function. */

/* Define to 1 if you have the `fmodl' function. */

/* Define to 1 if you have the `fpclass' function. */

/* Define to 1 if you have the <fp.h> header file. */

/* Define to 1 if you have the `frexpf' function. */

/* Define to 1 if you have the `frexpl' function. */

/* Define if getentropy is available in <unistd.h>. */

/* Define if _Unwind_GetIPInfo is available. */

/* Define if gets is available in <stdio.h> before C++14. */

/* Define to 1 if you have the `hypot' function. */

/* Define to 1 if you have the `hypotf' function. */

/* Define to 1 if you have the `hypotl' function. */

/* Define if you have the iconv() function and it works. */

/* Define to 1 if you have the <ieeefp.h> header file. */

/* Define to 1 if you have the <inttypes.h> header file. */

/* Define to 1 if you have the `isinf' function. */

/* Define to 1 if you have the `isinff' function. */

/* Define to 1 if you have the `isinfl' function. */

/* Define to 1 if you have the `isnan' function. */

/* Define to 1 if you have the `isnanf' function. */

/* Define to 1 if you have the `isnanl' function. */

/* Defined if iswblank exists. */

/* Define if LC_MESSAGES is available in <locale.h>. */

/* Define to 1 if you have the `ldexpf' function. */

/* Define to 1 if you have the `ldexpl' function. */

/* Define to 1 if you have the <libintl.h> header file. */

/* Only used in build directory testsuite_hooks.h. */

/* Only used in build directory testsuite_hooks.h. */

/* Only used in build directory testsuite_hooks.h. */

/* Only used in build directory testsuite_hooks.h. */

/* Only used in build directory testsuite_hooks.h. */

/* Define if link is available in <unistd.h>. */

/* Define to 1 if you have the <link.h> header file. */

/* Define if futex syscall is available. */

/* Define to 1 if you have the <linux/random.h> header file. */

/* Define to 1 if you have the <linux/types.h> header file. */

/* Define to 1 if you have the <locale.h> header file. */

/* Define to 1 if you have the `log10f' function. */

/* Define to 1 if you have the `log10l' function. */

/* Define to 1 if you have the `logf' function. */

/* Define to 1 if you have the `logl' function. */

/* Define if lseek is available in <unistd.h>. */

/* Define to 1 if you have the <machine/endian.h> header file. */

/* Define to 1 if you have the <machine/param.h> header file. */

/* Define if mbstate_t exists in wchar.h. */

/* Define to 1 if you have the `memalign' function. */

/* Define to 1 if you have the <memory.h> header file. */

/* Define to 1 if you have the `modf' function. */

/* Define to 1 if you have the `modff' function. */

/* Define to 1 if you have the `modfl' function. */

/* Define to 1 if you have the <nan.h> header file. */

/* Define to 1 if you have the <netdb.h> header file. */

/* Define to 1 if you have the <netinet/in.h> header file. */

/* Define to 1 if you have the <netinet/tcp.h> header file. */

/* Define if <math.h> defines obsolete isinf function. */

/* Define if <math.h> defines obsolete isnan function. */

/* Define if openat is available in <fcntl.h>. */

/* Define if poll is available in <poll.h>. */

/* Define to 1 if you have the <poll.h> header file. */

/* Define to 1 if you have the `posix_memalign' function. */

/* Define to 1 if POSIX Semaphores with sem_timedwait are available in
   <semaphore.h>. */

/* Define to 1 if you have the `powf' function. */

/* Define to 1 if you have the `powl' function. */

/* Define to 1 if you have the `qfpclass' function. */

/* Define to 1 if you have the `quick_exit' function. */

/* Define if readlink is available in <unistd.h>. */

/* Define to 1 if you have the `secure_getenv' function. */

/* Define to 1 if you have the `setenv' function. */

/* Define to 1 if you have the `sincos' function. */

/* Define to 1 if you have the `sincosf' function. */

/* Define to 1 if you have the `sincosl' function. */

/* Define to 1 if you have the `sinf' function. */

/* Define to 1 if you have the `sinhf' function. */

/* Define to 1 if you have the `sinhl' function. */

/* Define to 1 if you have the `sinl' function. */

/* Defined if sleep exists. */

/* Define to 1 if you have the `sockatmark' function. */

/* Define to 1 if you have the `sqrtf' function. */

/* Define to 1 if you have the `sqrtl' function. */

/* Define if the <stacktrace> header is supported. */

/* Define to 1 if you have the <stdalign.h> header file. */

/* Define to 1 if you have the <stdbool.h> header file. */

/* Define to 1 if you have the <stdint.h> header file. */

/* Define to 1 if you have the <stdlib.h> header file. */

/* Define if strerror_l is available in <string.h>. */

/* Define if strerror_r is available in <string.h>. */

/* Define to 1 if you have the <strings.h> header file. */

/* Define to 1 if you have the <string.h> header file. */

/* Define to 1 if you have the `strtof' function. */

/* Define to 1 if you have the `strtold' function. */

/* Define to 1 if `d_type' is a member of `struct dirent'. */

/* Define if strxfrm_l is available in <string.h>. */

/* Define if symlink is available in <unistd.h>. */

/* Define to 1 if the target runtime linker supports binding the same symbol
   to different versions. */

/* Define to 1 if you have the <sys/filio.h> header file. */

/* Define to 1 if you have the <sys/ioctl.h> header file. */

/* Define to 1 if you have the <sys/ipc.h> header file. */

/* Define to 1 if you have the <sys/isa_defs.h> header file. */

/* Define to 1 if you have the <sys/machine.h> header file. */

/* Define to 1 if you have the <sys/mman.h> header file. */

/* Define to 1 if you have the <sys/param.h> header file. */

/* Define to 1 if you have the <sys/resource.h> header file. */

/* Define to 1 if you have a suitable <sys/sdt.h> header file */

/* Define to 1 if you have the <sys/sem.h> header file. */

/* Define to 1 if you have the <sys/socket.h> header file. */

/* Define to 1 if you have the <sys/statvfs.h> header file. */

/* Define to 1 if you have the <sys/stat.h> header file. */

/* Define to 1 if you have the <sys/sysinfo.h> header file. */

/* Define to 1 if you have the <sys/time.h> header file. */

/* Define to 1 if you have the <sys/types.h> header file. */

/* Define to 1 if you have the <sys/uio.h> header file. */

/* Define if S_IFREG is available in <sys/stat.h>. */

/* Define if S_ISREG is available in <sys/stat.h>. */

/* Define to 1 if you have the `tanf' function. */

/* Define to 1 if you have the `tanhf' function. */

/* Define to 1 if you have the `tanhl' function. */

/* Define to 1 if you have the `tanl' function. */

/* Define to 1 if you have the <tgmath.h> header file. */

/* Define to 1 if you have the `timespec_get' function. */

/* Define to 1 if the target supports thread-local storage. */

/* Define if truncate is available in <unistd.h>. */

/* Define to 1 if you have the <uchar.h> header file. */

/* Define to 1 if you have the <unistd.h> header file. */

/* Define if unlinkat is available in <fcntl.h>. */

/* Define to 1 if you have the `uselocale' function. */

/* Defined if usleep exists. */

/* Define to 1 if you have the <utime.h> header file. */

/* Defined if vfwscanf exists. */

/* Defined if vswscanf exists. */

/* Defined if vwscanf exists. */

/* Define to 1 if you have the <wchar.h> header file. */

/* Defined if wcstof exists. */

/* Define to 1 if you have the <wctype.h> header file. */

/* Define to 1 if you have the <windows.h> header file. */

/* Define if writev is available in <sys/uio.h>. */

/* Define to 1 if you have the <xlocale.h> header file. */

/* Define to 1 if you have the `_aligned_malloc' function. */

/* Define to 1 if you have the `_wfopen' function. */

/* Define to 1 if you have the `__cxa_thread_atexit' function. */

/* Define to 1 if you have the `__cxa_thread_atexit_impl' function. */

/* Define as const if the declaration of iconv() needs const. */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */

/* Name of package */

/* Define to the address where bug reports for this package should be sent. */

/* Define to the full name of this package. */

/* Define to the full name and version of this package. */

/* Define to the one symbol short name of this package. */

/* Define to the home page for this package. */

/* Define to the version of this package. */

/* Define to 1 if you have the ANSI C header files. */

/* Version number of package */

/* Enable large inode numbers on Mac OS X 10.5.  */



/* Number of bits in a file offset, on hosts where this is settable. */

/* Define if C99 functions in <complex.h> should be used in <complex> for
   C++11. Using compiler builtins for these functions requires corresponding
   C99 library functions to be present. */

/* Define if C99 generic macros in <math.h> should be imported in <cmath> in
   namespace std for C++11. */

/* Define if C99 functions or macros in <stdio.h> should be imported in
   <cstdio> in namespace std for C++11. */

/* Define if C99 functions or macros in <stdlib.h> should be imported in
   <cstdlib> in namespace std for C++11. */

/* Define if C99 functions or macros in <wchar.h> should be imported in
   <cwchar> in namespace std for C++11. */

/* Define if C99 functions in <complex.h> should be used in <complex> for
   C++98. Using compiler builtins for these functions requires corresponding
   C99 library functions to be present. */

/* Define if C99 functions or macros in <math.h> should be imported in <cmath>
   in namespace std for C++98. */

/* Define if C99 functions or macros in <stdio.h> should be imported in
   <cstdio> in namespace std for C++98. */

/* Define if C99 functions or macros in <stdlib.h> should be imported in
   <cstdlib> in namespace std for C++98. */

/* Define if C99 functions or macros in <wchar.h> should be imported in
   <cwchar> in namespace std for C++98. */

/* Define if the compiler supports C++11 atomics. */

/* Define if global objects can be aligned to
   std::hardware_destructive_interference_size. */

/* Define to use concept checking code from the boost libraries. */

/* Define to 1 if a fully dynamic basic_string is wanted, 0 to disable,
   undefined for platform defaults */

/* Define if gthreads library is available. */

/* Define to 1 if a full hosted library is built, or 0 if freestanding. */

/* Define if compatibility should be provided for alternative 128-bit long
   double formats. */

/* Define if compatibility should be provided for -mlong-double-64. */

/* Define to the letter to which size_t is mangled. */

/* Define if C99 llrint and llround functions are missing from <math.h>. */

/* Defined if no way to sleep is available. */

/* Define if ptrdiff_t is int. */

/* Define if using setrlimit to set resource limits during "make check" */

/* Define if size_t is unsigned int. */

/* Define if static tzdata should be compiled into the library. */

/* Define to the value of the EOF integer constant. */

/* Define to the value of the SEEK_CUR integer constant. */

/* Define to the value of the SEEK_END integer constant. */

/* Define to use symbol versioning in the shared library. */

/* Define to use darwin versioning in the shared library. */

/* Define to use GNU versioning in the shared library. */

/* Define to use GNU namespace versioning in the shared library. */

/* Define to use Sun versioning in the shared library. */

/* Define if C11 functions in <uchar.h> should be imported into namespace std
   in <cuchar>. */

/* Define if C99 functions or macros from <wchar.h>, <math.h>, <complex.h>,
   <stdio.h>, and <stdlib.h> can be used or exposed. */

/* Define if C99 inverse trig functions in <complex.h> should be used in
   <complex>. Using compiler builtins for these functions requires
   corresponding C99 library functions to be present. */

/* Define if C99 functions in <complex.h> should be used in <tr1/complex>.
   Using compiler builtins for these functions requires corresponding C99
   library functions to be present. */

/* Define if C99 functions in <ctype.h> should be imported in <cctype> in
   namespace std for C++11. */

/* Define if C99 functions in <ctype.h> should be imported in <tr1/cctype> in
   namespace std::tr1. */

/* Define if C99 functions in <fenv.h> should be imported in <cfenv> in
   namespace std for C++11. */

/* Define if C99 functions in <fenv.h> should be imported in <tr1/cfenv> in
   namespace std::tr1. */

/* Define if C99 functions in <inttypes.h> should be imported in <cinttypes>
   in namespace std in C++11. */

/* Define if C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */

/* Define if wchar_t C99 functions in <inttypes.h> should be imported in
   <cinttypes> in namespace std in C++11. */

/* Define if wchar_t C99 functions in <inttypes.h> should be imported in
   <tr1/cinttypes> in namespace std::tr1. */

/* Define if C99 functions in <math.h> should be imported in <cmath> in
   namespace std for C++11. */

/* Define if C99 functions or macros in <math.h> should be imported in
   <tr1/cmath> in namespace std::tr1. */

/* Define if C99 types in <stdint.h> should be imported in <cstdint> in
   namespace std for C++11. */

/* Define if C99 types in <stdint.h> should be imported in <tr1/cstdint> in
   namespace std::tr1. */

/* Define if usable chdir is available in <unistd.h>. */

/* Define if usable chmod is available in <sys/stat.h>. */

/* Defined if clock_gettime syscall has monotonic and realtime clock support.
   */

/* Defined if clock_gettime has monotonic clock support. */

/* Defined if clock_gettime has realtime clock support. */

/* Define if copy_file_range is available in <unistd.h>. */

/* Define if ISO/IEC TR 24733 decimal floating point types are supported on
   this host. */

/* Define if /dev/random and /dev/urandom are available for
   std::random_device. */

/* Define if fchmod is available in <sys/stat.h>. */

/* Define if fchmodat is available in <sys/stat.h>. */

/* Define if fseeko and ftello are available. */

/* Define if usable getcwd is available in <unistd.h>. */

/* Defined if gettimeofday is available. */

/* Define if get_nprocs is available in <sys/sysinfo.h>. */

/* Define if init_priority should be used for iostream initialization. */

/* Define if LFS support is available. */

/* Define if code specialized for long long should be used. */

/* Define if lstat is available in <sys/stat.h>. */

/* Define if usable mkdir is available in <sys/stat.h>. */

/* Defined if nanosleep is available. */

/* Define if NLS translations are to be used. */

/* Define if nl_langinfo_l should be used for std::text_encoding. */

/* Define if pthreads_num_processors_np is available in <pthread.h>. */

/* Define if pthread_cond_clockwait is available in <pthread.h>. */

/* Define if pthread_mutex_clocklock is available in <pthread.h>. */

/* Define if pthread_rwlock_clockrdlock and pthread_rwlock_clockwrlock are
   available in <pthread.h>. */

/* Define if POSIX read/write locks are available in <gthr.h>. */

/* Define if /dev/random and /dev/urandom are available for the random_device
   of TR1 (Chapter 5.1). */

/* Define if usable realpath is available in <stdlib.h>. */

/* Defined if sched_yield is available. */

/* Define if _SC_NPROCESSORS_ONLN is available in <unistd.h>. */

/* Define if _SC_NPROC_ONLN is available in <unistd.h>. */

/* Define if sendfile is available in <sys/sendfile.h>. */

/* Define to restrict std::__basic_file<> to stdio APIs. */

/* Define if struct stat has timespec members. */

/* Define if sysctl(), CTL_HW and HW_NCPU are available in <sys/sysctl.h>. */

/* Define if obsolescent tmpnam is available in <stdio.h>. */

/* Define if c8rtomb and mbrtoc8 functions in <uchar.h> should be imported
   into namespace std in <cuchar> for C++20. */

/* Define if c8rtomb and mbrtoc8 functions in <uchar.h> should be imported
   into namespace std in <cuchar> for -fchar8_t. */

/* Define if utime is available in <utime.h>. */

/* Define if utimensat and UTIME_OMIT are available in <sys/stat.h> and
   AT_FDCWD in <fcntl.h>. */

/* Define if code specialized for wchar_t should be used. */

/* Defined if Sleep exists. */

/* Define if _get_osfhandle should be used for filebuf::native_handle(). */

/* Define to 1 if a verbose library is built, or 0 otherwise. */

/* Defined if as can handle rdrand. */

/* Defined if as can handle rdseed. */

/* Define if a directory should be searched for tzdata files. */

/* Define to 1 if mutex_timedlock is available. */

/* Define for large files, on AIX-style hosts. */

/* Define if all C++11 floating point overloads are available in <math.h>.  */



/* Define if all C++11 integral type overloads are available in <math.h>.  */



// std::rel_ops implementation -*- C++ -*-

// Copyright (C) 2001-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the, 2009 Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/** @file bits/stl_relops.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 *
 *  This file is only included by `<utility>`, which is required by the
 *  standard to define namespace `rel_ops` and its contents.
 */



namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  namespace rel_ops
  {
    /** @namespace std::rel_ops
     *  @brief  The generated relational operators are sequestered here.
     *
     *  Libstdc++ headers must not use the contents of `rel_ops`.
     *  User code should also avoid them, because unconstrained function
     *  templates are too greedy and can easily cause ambiguities.
     *
     *  C++20 default comparisons are a better solution.
     */

    /**
     *  @brief Defines @c != for arbitrary types, in terms of @c ==.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x != __y
     *
     *  This function uses @c == to determine its result.
     */
    template <class _Tp>
      inline bool
      operator!=(const _Tp& __x, const _Tp& __y)
      { return !(__x == __y); }

    /**
     *  @brief Defines @c > for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x > __y
     *
     *  This function uses @c < to determine its result.
     */
    template <class _Tp>
      inline bool
      operator>(const _Tp& __x, const _Tp& __y)
      { return __y < __x; }

    /**
     *  @brief Defines @c <= for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x <= __y
     *
     *  This function uses @c < to determine its result.
     */
    template <class _Tp>
      inline bool
      operator<=(const _Tp& __x, const _Tp& __y)
      { return !(__y < __x); }

    /**
     *  @brief Defines @c >= for arbitrary types, in terms of @c <.
     *  @param  __x  A thing.
     *  @param  __y  Another thing.
     *  @return   __x >= __y
     *
     *  This function uses @c < to determine its result.
     */
    template <class _Tp>
      inline bool
      operator>=(const _Tp& __x, const _Tp& __y)
      { return !(__x < __y); }
  } // namespace rel_ops

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std

// Pair implementation -*- C++ -*-

// Copyright (C) 2001-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file bits/stl_pair.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */




// C++11 <type_traits> -*- C++ -*-

// Copyright (C) 2007-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/type_traits
 *  This is a Standard C++ Library header.
 */



       







// Copyright (C) 2023-2024 Free Software Foundation, Inc.

// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// DO NOT EDIT THIS FILE   (version.h)
//
// It has been AutoGen-ed
// From the definitions    version.def
// and the template file   version.tpl

/** @file bits/version.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{version}
 */

// Usage guide:
//
// In your usual header, do something like:
//
//
// This will generate the FTMs you named, and let you use them in your code as
// if it was user code.  All macros are also exposed under __glibcxx_NAME even
// if unwanted, to permit bits and other FTMs to depend on them for condtional
// computation without exposing extra FTMs to user code.

       



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Tp>
    class reference_wrapper;

  /**
   * @defgroup metaprogramming Metaprogramming
   * @ingroup utilities
   *
   * Template utilities for compile-time introspection and modification,
   * including type classification traits, type property inspection traits
   * and type transformation traits.
   *
   * @since C++11
   *
   * @{
   */

  /// integral_constant
  template<typename _Tp, _Tp __v>
    struct integral_constant
    {
      static constexpr _Tp value = __v;
      using value_type = _Tp;
      using type = integral_constant<_Tp, __v>;
      constexpr operator value_type() const noexcept { return value; }


      constexpr value_type operator()() const noexcept { return value; }

    };






  /// @cond undocumented
  /// bool_constant for C++11
  template<bool __v>
    using __bool_constant = integral_constant<bool, __v>;
  /// @endcond

  /// The type used as a compile-time boolean with true value.
  using true_type =  __bool_constant<true>;

  /// The type used as a compile-time boolean with false value.
  using false_type = __bool_constant<false>;


  /// Alias template for compile-time boolean constant types.
  /// @since C++17
  template<bool __v>
    using bool_constant = __bool_constant<__v>;


  // Metaprogramming helper types.

  // Primary template.
  /// Define a member typedef `type` only if a boolean constant is true.
  template<bool, typename _Tp = void>
    struct enable_if
    { };

  // Partial specialization for true.
  template<typename _Tp>
    struct enable_if<true, _Tp>
    { using type = _Tp; };

  // __enable_if_t (std::enable_if_t for C++11)
  template<bool _Cond, typename _Tp = void>
    using __enable_if_t = typename enable_if<_Cond, _Tp>::type;

  template<bool>
    struct __conditional
    {
      template<typename _Tp, typename>
	using type = _Tp;
    };

  template<>
    struct __conditional<false>
    {
      template<typename, typename _Up>
	using type = _Up;
    };

  // More efficient version of std::conditional_t for internal use (and C++11)
  template<bool _Cond, typename _If, typename _Else>
    using __conditional_t
      = typename __conditional<_Cond>::template type<_If, _Else>;

  /// @cond undocumented
  template <typename _Type>
    struct __type_identity
    { using type = _Type; };

  template<typename _Tp>
    using __type_identity_t = typename __type_identity<_Tp>::type;

  namespace __detail
  {
    // A variadic alias template that resolves to its first argument.
    template<typename _Tp, typename...>
      using __first_t = _Tp;

    // These are deliberately not defined.
    template<typename... _Bn>
      auto __or_fn(int) -> __first_t<false_type,
				     __enable_if_t<!bool(_Bn::value)>...>;

    template<typename... _Bn>
      auto __or_fn(...) -> true_type;

    template<typename... _Bn>
      auto __and_fn(int) -> __first_t<true_type,
				      __enable_if_t<bool(_Bn::value)>...>;

    template<typename... _Bn>
      auto __and_fn(...) -> false_type;
  } // namespace detail

  // Like C++17 std::dis/conjunction, but usable in C++11 and resolves
  // to either true_type or false_type which allows for a more efficient
  // implementation that avoids recursive class template instantiation.
  template<typename... _Bn>
    struct __or_
    : decltype(__detail::__or_fn<_Bn...>(0))
    { };

  template<typename... _Bn>
    struct __and_
    : decltype(__detail::__and_fn<_Bn...>(0))
    { };

  template<typename _Pp>
    struct __not_
    : __bool_constant<!bool(_Pp::value)>
    { };
  /// @endcond



  /// @cond undocumented
  template<typename... _Bn>
    inline constexpr bool __or_v = __or_<_Bn...>::value;
  template<typename... _Bn>
    inline constexpr bool __and_v = __and_<_Bn...>::value;

  namespace __detail
  {
    template<typename /* = void */, typename _B1, typename... _Bn>
      struct __disjunction_impl
      { using type = _B1; };

    template<typename _B1, typename _B2, typename... _Bn>
      struct __disjunction_impl<__enable_if_t<!bool(_B1::value)>, _B1, _B2, _Bn...>
      { using type = typename __disjunction_impl<void, _B2, _Bn...>::type; };

    template<typename /* = void */, typename _B1, typename... _Bn>
      struct __conjunction_impl
      { using type = _B1; };

    template<typename _B1, typename _B2, typename... _Bn>
      struct __conjunction_impl<__enable_if_t<bool(_B1::value)>, _B1, _B2, _Bn...>
      { using type = typename __conjunction_impl<void, _B2, _Bn...>::type; };
  } // namespace __detail
  /// @endcond

  template<typename... _Bn>
    struct conjunction
    : __detail::__conjunction_impl<void, _Bn...>::type
    { };

  template<>
    struct conjunction<>
    : true_type
    { };

  template<typename... _Bn>
    struct disjunction
    : __detail::__disjunction_impl<void, _Bn...>::type
    { };

  template<>
    struct disjunction<>
    : false_type
    { };

  template<typename _Pp>
    struct negation
    : __not_<_Pp>::type
    { };

  /** @ingroup variable_templates
   * @{
   */
  template<typename... _Bn>
    inline constexpr bool conjunction_v = conjunction<_Bn...>::value;

  template<typename... _Bn>
    inline constexpr bool disjunction_v = disjunction<_Bn...>::value;

  template<typename _Pp>
    inline constexpr bool negation_v = negation<_Pp>::value;
  /// @}



  // Forward declarations
  template<typename>
    struct is_reference;
  template<typename>
    struct is_function;
  template<typename>
    struct is_void;
  template<typename>
    struct remove_cv;
  template<typename>
    struct is_const;

  /// @cond undocumented
  template<typename>
    struct __is_array_unknown_bounds;

  // Helper functions that return false_type for incomplete classes,
  // incomplete unions and arrays of known bound from those.

  template <typename _Tp, size_t = sizeof(_Tp)>
    constexpr true_type __is_complete_or_unbounded(__type_identity<_Tp>)
    { return {}; }

  template <typename _TypeIdentity,
      typename _NestedType = typename _TypeIdentity::type>
    constexpr typename __or_<
      is_reference<_NestedType>,
      is_function<_NestedType>,
      is_void<_NestedType>,
      __is_array_unknown_bounds<_NestedType>
    >::type __is_complete_or_unbounded(_TypeIdentity)
    { return {}; }

  // __remove_cv_t (std::remove_cv_t for C++11).
  template<typename _Tp>
    using __remove_cv_t = typename remove_cv<_Tp>::type;
  /// @endcond

  // Primary type categories.

  /// is_void
  template<typename _Tp>
    struct is_void
    : public false_type { };

  template<>
    struct is_void<void>
    : public true_type { };

  template<>
    struct is_void<const void>
    : public true_type { };

  template<>
    struct is_void<volatile void>
    : public true_type { };

  template<>
    struct is_void<const volatile void>
    : public true_type { };

  /// @cond undocumented
  template<typename>
    struct __is_integral_helper
    : public false_type { };

  template<>
    struct __is_integral_helper<bool>
    : public true_type { };

  template<>
    struct __is_integral_helper<char>
    : public true_type { };

  template<>
    struct __is_integral_helper<signed char>
    : public true_type { };

  template<>
    struct __is_integral_helper<unsigned char>
    : public true_type { };

  // We want is_integral<wchar_t> to be true (and make_signed/unsigned to work)
  // even when libc doesn't provide working <wchar.h> and related functions,
  // so don't check _GLIBCXX_USE_WCHAR_T here.
  template<>
    struct __is_integral_helper<wchar_t>
    : public true_type { };







  template<>
    struct __is_integral_helper<char16_t>
    : public true_type { };

  template<>
    struct __is_integral_helper<char32_t>
    : public true_type { };

  template<>
    struct __is_integral_helper<short>
    : public true_type { };

  template<>
    struct __is_integral_helper<unsigned short>
    : public true_type { };

  template<>
    struct __is_integral_helper<int>
    : public true_type { };

  template<>
    struct __is_integral_helper<unsigned int>
    : public true_type { };

  template<>
    struct __is_integral_helper<long>
    : public true_type { };

  template<>
    struct __is_integral_helper<unsigned long>
    : public true_type { };

  template<>
    struct __is_integral_helper<long long>
    : public true_type { };

  template<>
    struct __is_integral_helper<unsigned long long>
    : public true_type { };

  // Conditionalizing on __STRICT_ANSI__ here will break any port that
  // uses one of these types for size_t.

  __extension__
  template<>
    struct __is_integral_helper<__GLIBCXX_TYPE_INT_N_0>
    : public true_type { };

  __extension__
  template<>
    struct __is_integral_helper<unsigned __GLIBCXX_TYPE_INT_N_0>
    : public true_type { };

  /// @endcond

  /// is_integral
  template<typename _Tp>
    struct is_integral
    : public __is_integral_helper<__remove_cv_t<_Tp>>::type
    { };

  /// @cond undocumented
  template<typename>
    struct __is_floating_point_helper
    : public false_type { };

  template<>
    struct __is_floating_point_helper<float>
    : public true_type { };

  template<>
    struct __is_floating_point_helper<double>
    : public true_type { };

  template<>
    struct __is_floating_point_helper<long double>
    : public true_type { };
































  template<>
    struct __is_floating_point_helper<__float128>
    : public true_type { };

  /// @endcond

  /// is_floating_point
  template<typename _Tp>
    struct is_floating_point
    : public __is_floating_point_helper<__remove_cv_t<_Tp>>::type
    { };

  /// is_array

  template<typename _Tp>
    struct is_array
    : public __bool_constant<__is_array(_Tp)>
    { };

  template<typename>
    struct __is_pointer_helper
    : public false_type { };

  template<typename _Tp>
    struct __is_pointer_helper<_Tp*>
    : public true_type { };

  /// is_pointer
  template<typename _Tp>
    struct is_pointer
    : public __is_pointer_helper<__remove_cv_t<_Tp>>::type
    { };

  /// is_lvalue_reference
  template<typename>
    struct is_lvalue_reference
    : public false_type { };

  template<typename _Tp>
    struct is_lvalue_reference<_Tp&>
    : public true_type { };

  /// is_rvalue_reference
  template<typename>
    struct is_rvalue_reference
    : public false_type { };

  template<typename _Tp>
    struct is_rvalue_reference<_Tp&&>
    : public true_type { };

  /// is_member_object_pointer

  template<typename _Tp>
    struct is_member_object_pointer
    : public __bool_constant<__is_member_object_pointer(_Tp)>
    { };


  /// is_member_function_pointer
  template<typename _Tp>
    struct is_member_function_pointer
    : public __bool_constant<__is_member_function_pointer(_Tp)>
    { };

  /// is_enum
  template<typename _Tp>
    struct is_enum
    : public __bool_constant<__is_enum(_Tp)>
    { };

  /// is_union
  template<typename _Tp>
    struct is_union
    : public __bool_constant<__is_union(_Tp)>
    { };

  /// is_class
  template<typename _Tp>
    struct is_class
    : public __bool_constant<__is_class(_Tp)>
    { };

  /// is_function

  template<typename _Tp>
    struct is_function
    : public __bool_constant<__is_function(_Tp)>
    { };


  /// is_null_pointer (LWG 2247).
  template<typename _Tp>
    struct is_null_pointer
    : public false_type { };

  template<>
    struct is_null_pointer<std::nullptr_t>
    : public true_type { };

  template<>
    struct is_null_pointer<const std::nullptr_t>
    : public true_type { };

  template<>
    struct is_null_pointer<volatile std::nullptr_t>
    : public true_type { };

  template<>
    struct is_null_pointer<const volatile std::nullptr_t>
    : public true_type { };

  /// __is_nullptr_t (deprecated extension).
  /// @deprecated Non-standard. Use `is_null_pointer` instead.
  template<typename _Tp>
    struct __is_nullptr_t
    : public is_null_pointer<_Tp>
    { } _GLIBCXX_DEPRECATED_SUGGEST("std::is_null_pointer");


  // Composite type categories.

  /// is_reference

  template<typename _Tp>
    struct is_reference
    : public __bool_constant<__is_reference(_Tp)>
    { };

  /// is_arithmetic
  template<typename _Tp>
    struct is_arithmetic
    : public __or_<is_integral<_Tp>, is_floating_point<_Tp>>::type
    { };

  /// is_fundamental
  template<typename _Tp>
    struct is_fundamental
    : public __or_<is_arithmetic<_Tp>, is_void<_Tp>,
		   is_null_pointer<_Tp>>::type
    { };

  /// is_object

  template<typename _Tp>
    struct is_object
    : public __bool_constant<__is_object(_Tp)>
    { };








  template<typename>
    struct is_member_pointer;

  /// is_scalar
  template<typename _Tp>
    struct is_scalar
    : public __or_<is_arithmetic<_Tp>, is_enum<_Tp>, is_pointer<_Tp>,
                   is_member_pointer<_Tp>, is_null_pointer<_Tp>>::type
    { };

  /// is_compound
  template<typename _Tp>
    struct is_compound
    : public __bool_constant<!is_fundamental<_Tp>::value> { };

  /// is_member_pointer

  template<typename _Tp>
    struct is_member_pointer
    : public __bool_constant<__is_member_pointer(_Tp)>
    { };

  template<typename, typename>
    struct is_same;

  /// @cond undocumented
  template<typename _Tp, typename... _Types>
    using __is_one_of = __or_<is_same<_Tp, _Types>...>;

  // Check if a type is one of the signed integer types.
  __extension__
  template<typename _Tp>
    using __is_signed_integer = __is_one_of<__remove_cv_t<_Tp>,
	  signed char, signed short, signed int, signed long,
	  signed long long

	  , signed __GLIBCXX_TYPE_INT_N_0










	  >;

  // Check if a type is one of the unsigned integer types.
  __extension__
  template<typename _Tp>
    using __is_unsigned_integer = __is_one_of<__remove_cv_t<_Tp>,
	  unsigned char, unsigned short, unsigned int, unsigned long,
	  unsigned long long

	  , unsigned __GLIBCXX_TYPE_INT_N_0










	  >;

  // Check if a type is one of the signed or unsigned integer types.
  template<typename _Tp>
    using __is_standard_integer
      = __or_<__is_signed_integer<_Tp>, __is_unsigned_integer<_Tp>>;

  // __void_t (std::void_t for C++11)
  template<typename...> using __void_t = void;
  /// @endcond

  // Type properties.

  /// is_const
  template<typename>
    struct is_const
    : public false_type { };

  template<typename _Tp>
    struct is_const<_Tp const>
    : public true_type { };

  /// is_volatile
  template<typename>
    struct is_volatile
    : public false_type { };

  template<typename _Tp>
    struct is_volatile<_Tp volatile>
    : public true_type { };

  /// is_trivial
  template<typename _Tp>
    struct is_trivial
    : public __bool_constant<__is_trivial(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_copyable
  template<typename _Tp>
    struct is_trivially_copyable
    : public __bool_constant<__is_trivially_copyable(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_standard_layout
  template<typename _Tp>
    struct is_standard_layout
    : public __bool_constant<__is_standard_layout(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /** is_pod
   * @deprecated Deprecated in C++20.
   * Use `is_standard_layout && is_trivial` instead.
   */
  // Could use is_standard_layout && is_trivial instead of the builtin.
  template<typename _Tp>
    struct
    _GLIBCXX20_DEPRECATED_SUGGEST("is_standard_layout && is_trivial")
    is_pod
    : public __bool_constant<__is_pod(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /** is_literal_type
   * @deprecated Deprecated in C++17, removed in C++20.
   * The idea of a literal type isn't useful.
   */
  template<typename _Tp>
    struct
    _GLIBCXX17_DEPRECATED
    is_literal_type
    : public __bool_constant<__is_literal_type(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_empty
  template<typename _Tp>
    struct is_empty
    : public __bool_constant<__is_empty(_Tp)>
    { };

  /// is_polymorphic
  template<typename _Tp>
    struct is_polymorphic
    : public __bool_constant<__is_polymorphic(_Tp)>
    { };


  /// is_final
  /// @since C++14
  template<typename _Tp>
    struct is_final
    : public __bool_constant<__is_final(_Tp)>
    { };


  /// is_abstract
  template<typename _Tp>
    struct is_abstract
    : public __bool_constant<__is_abstract(_Tp)>
    { };

  /// @cond undocumented
  template<typename _Tp,
	   bool = is_arithmetic<_Tp>::value>
    struct __is_signed_helper
    : public false_type { };

  template<typename _Tp>
    struct __is_signed_helper<_Tp, true>
    : public __bool_constant<_Tp(-1) < _Tp(0)>
    { };
  /// @endcond

  /// is_signed
  template<typename _Tp>
    struct is_signed
    : public __is_signed_helper<_Tp>::type
    { };

  /// is_unsigned
  template<typename _Tp>
    struct is_unsigned
    : public __and_<is_arithmetic<_Tp>, __not_<is_signed<_Tp>>>::type
    { };

  /// @cond undocumented
  template<typename _Tp, typename _Up = _Tp&&>
    _Up
    __declval(int);

  template<typename _Tp>
    _Tp
    __declval(long);
  /// @endcond

  template<typename _Tp>
    auto declval() noexcept -> decltype(__declval<_Tp>(0));

  template<typename>
    struct remove_all_extents;

  /// @cond undocumented
  template<typename _Tp>
    struct __is_array_known_bounds
    : public false_type
    { };

  template<typename _Tp, size_t _Size>
    struct __is_array_known_bounds<_Tp[_Size]>
    : public true_type
    { };

  template<typename _Tp>
    struct __is_array_unknown_bounds
    : public false_type
    { };

  template<typename _Tp>
    struct __is_array_unknown_bounds<_Tp[]>
    : public true_type
    { };

  // Destructible and constructible type properties.

  // In N3290 is_destructible does not say anything about function
  // types and abstract types, see LWG 2049. This implementation
  // describes function types as non-destructible and all complete
  // object types as destructible, iff the explicit destructor
  // call expression is wellformed.
  struct __do_is_destructible_impl
  {
    template<typename _Tp, typename = decltype(declval<_Tp&>().~_Tp())>
      static true_type __test(int);

    template<typename>
      static false_type __test(...);
  };

  template<typename _Tp>
    struct __is_destructible_impl
    : public __do_is_destructible_impl
    {
      using type = decltype(__test<_Tp>(0));
    };

  template<typename _Tp,
           bool = __or_<is_void<_Tp>,
                        __is_array_unknown_bounds<_Tp>,
                        is_function<_Tp>>::value,
           bool = __or_<is_reference<_Tp>, is_scalar<_Tp>>::value>
    struct __is_destructible_safe;

  template<typename _Tp>
    struct __is_destructible_safe<_Tp, false, false>
    : public __is_destructible_impl<typename
               remove_all_extents<_Tp>::type>::type
    { };

  template<typename _Tp>
    struct __is_destructible_safe<_Tp, true, false>
    : public false_type { };

  template<typename _Tp>
    struct __is_destructible_safe<_Tp, false, true>
    : public true_type { };
  /// @endcond

  /// is_destructible
  template<typename _Tp>
    struct is_destructible
    : public __is_destructible_safe<_Tp>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented

  // is_nothrow_destructible requires that is_destructible is
  // satisfied as well.  We realize that by mimicing the
  // implementation of is_destructible but refer to noexcept(expr)
  // instead of decltype(expr).
  struct __do_is_nt_destructible_impl
  {
    template<typename _Tp>
      static __bool_constant<noexcept(declval<_Tp&>().~_Tp())>
      __test(int);

    template<typename>
      static false_type __test(...);
  };

  template<typename _Tp>
    struct __is_nt_destructible_impl
    : public __do_is_nt_destructible_impl
    {
      using type = decltype(__test<_Tp>(0));
    };

  template<typename _Tp,
           bool = __or_<is_void<_Tp>,
                        __is_array_unknown_bounds<_Tp>,
                        is_function<_Tp>>::value,
           bool = __or_<is_reference<_Tp>, is_scalar<_Tp>>::value>
    struct __is_nt_destructible_safe;

  template<typename _Tp>
    struct __is_nt_destructible_safe<_Tp, false, false>
    : public __is_nt_destructible_impl<typename
               remove_all_extents<_Tp>::type>::type
    { };

  template<typename _Tp>
    struct __is_nt_destructible_safe<_Tp, true, false>
    : public false_type { };

  template<typename _Tp>
    struct __is_nt_destructible_safe<_Tp, false, true>
    : public true_type { };
  /// @endcond

  /// is_nothrow_destructible
  template<typename _Tp>
    struct is_nothrow_destructible
    : public __is_nt_destructible_safe<_Tp>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename... _Args>
    using __is_constructible_impl
      = __bool_constant<__is_constructible(_Tp, _Args...)>;
  /// @endcond

  /// is_constructible
  template<typename _Tp, typename... _Args>
    struct is_constructible
      : public __is_constructible_impl<_Tp, _Args...>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_default_constructible
  template<typename _Tp>
    struct is_default_constructible
    : public __is_constructible_impl<_Tp>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename = void>
    struct __add_lvalue_reference_helper
    { using type = _Tp; };

  template<typename _Tp>
    struct __add_lvalue_reference_helper<_Tp, __void_t<_Tp&>>
    { using type = _Tp&; };

  template<typename _Tp>
    using __add_lval_ref_t = typename __add_lvalue_reference_helper<_Tp>::type;
  /// @endcond

  /// is_copy_constructible
  template<typename _Tp>
    struct is_copy_constructible
    : public __is_constructible_impl<_Tp, __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename = void>
    struct __add_rvalue_reference_helper
    { using type = _Tp; };

  template<typename _Tp>
    struct __add_rvalue_reference_helper<_Tp, __void_t<_Tp&&>>
    { using type = _Tp&&; };

  template<typename _Tp>
    using __add_rval_ref_t = typename __add_rvalue_reference_helper<_Tp>::type;
  /// @endcond

  /// is_move_constructible
  template<typename _Tp>
    struct is_move_constructible
    : public __is_constructible_impl<_Tp, __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename... _Args>
    using __is_nothrow_constructible_impl
      = __bool_constant<__is_nothrow_constructible(_Tp, _Args...)>;
  /// @endcond

  /// is_nothrow_constructible
  template<typename _Tp, typename... _Args>
    struct is_nothrow_constructible
    : public __is_nothrow_constructible_impl<_Tp, _Args...>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_default_constructible
  template<typename _Tp>
    struct is_nothrow_default_constructible
    : public __is_nothrow_constructible_impl<_Tp>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_copy_constructible
  template<typename _Tp>
    struct is_nothrow_copy_constructible
    : public __is_nothrow_constructible_impl<_Tp, __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_move_constructible
  template<typename _Tp>
    struct is_nothrow_move_constructible
    : public __is_nothrow_constructible_impl<_Tp, __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename _Up>
    using __is_assignable_impl = __bool_constant<__is_assignable(_Tp, _Up)>;
  /// @endcond

  /// is_assignable
  template<typename _Tp, typename _Up>
    struct is_assignable
    : public __is_assignable_impl<_Tp, _Up>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_copy_assignable
  template<typename _Tp>
    struct is_copy_assignable
    : public __is_assignable_impl<__add_lval_ref_t<_Tp>,
				  __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_move_assignable
  template<typename _Tp>
    struct is_move_assignable
    : public __is_assignable_impl<__add_lval_ref_t<_Tp>, __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename _Up>
    using __is_nothrow_assignable_impl
      = __bool_constant<__is_nothrow_assignable(_Tp, _Up)>;
  /// @endcond

  /// is_nothrow_assignable
  template<typename _Tp, typename _Up>
    struct is_nothrow_assignable
    : public __is_nothrow_assignable_impl<_Tp, _Up>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_copy_assignable
  template<typename _Tp>
    struct is_nothrow_copy_assignable
    : public __is_nothrow_assignable_impl<__add_lval_ref_t<_Tp>,
					  __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_move_assignable
  template<typename _Tp>
    struct is_nothrow_move_assignable
    : public __is_nothrow_assignable_impl<__add_lval_ref_t<_Tp>,
					  __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename... _Args>
    using __is_trivially_constructible_impl
      = __bool_constant<__is_trivially_constructible(_Tp, _Args...)>;
  /// @endcond

  /// is_trivially_constructible
  template<typename _Tp, typename... _Args>
    struct is_trivially_constructible
    : public __is_trivially_constructible_impl<_Tp, _Args...>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_default_constructible
  template<typename _Tp>
    struct is_trivially_default_constructible
    : public __is_trivially_constructible_impl<_Tp>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  struct __do_is_implicitly_default_constructible_impl
  {
    template <typename _Tp>
    static void __helper(const _Tp&);

    template <typename _Tp>
    static true_type __test(const _Tp&,
                            decltype(__helper<const _Tp&>({}))* = 0);

    static false_type __test(...);
  };

  template<typename _Tp>
    struct __is_implicitly_default_constructible_impl
    : public __do_is_implicitly_default_constructible_impl
    {
      using type = decltype(__test(declval<_Tp>()));
    };

  template<typename _Tp>
    struct __is_implicitly_default_constructible_safe
    : public __is_implicitly_default_constructible_impl<_Tp>::type
    { };

  template <typename _Tp>
    struct __is_implicitly_default_constructible
    : public __and_<__is_constructible_impl<_Tp>,
		    __is_implicitly_default_constructible_safe<_Tp>>::type
    { };


  /// is_trivially_copy_constructible
  template<typename _Tp>
    struct is_trivially_copy_constructible
    : public __is_trivially_constructible_impl<_Tp, __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_move_constructible
  template<typename _Tp>
    struct is_trivially_move_constructible
    : public __is_trivially_constructible_impl<_Tp, __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  template<typename _Tp, typename _Up>
    using __is_trivially_assignable_impl
      = __bool_constant<__is_trivially_assignable(_Tp, _Up)>;
  /// @endcond

  /// is_trivially_assignable
  template<typename _Tp, typename _Up>
    struct is_trivially_assignable
    : public __is_trivially_assignable_impl<_Tp, _Up>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_copy_assignable
  template<typename _Tp>
    struct is_trivially_copy_assignable
    : public __is_trivially_assignable_impl<__add_lval_ref_t<_Tp>,
					    __add_lval_ref_t<const _Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_move_assignable
  template<typename _Tp>
    struct is_trivially_move_assignable
    : public __is_trivially_assignable_impl<__add_lval_ref_t<_Tp>,
					    __add_rval_ref_t<_Tp>>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_trivially_destructible
  template<typename _Tp>
    struct is_trivially_destructible
    : public __and_<__is_destructible_safe<_Tp>,
		    __bool_constant<__has_trivial_destructor(_Tp)>>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };


  /// has_virtual_destructor
  template<typename _Tp>
    struct has_virtual_destructor
    : public __bool_constant<__has_virtual_destructor(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };


  // type property queries.

  /// alignment_of
  template<typename _Tp>
    struct alignment_of
    : public integral_constant<std::size_t, alignof(_Tp)>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// rank
  template<typename>
    struct rank
    : public integral_constant<std::size_t, 0> { };

  template<typename _Tp, std::size_t _Size>
    struct rank<_Tp[_Size]>
    : public integral_constant<std::size_t, 1 + rank<_Tp>::value> { };

  template<typename _Tp>
    struct rank<_Tp[]>
    : public integral_constant<std::size_t, 1 + rank<_Tp>::value> { };

  /// extent
  template<typename, unsigned _Uint = 0>
    struct extent
    : public integral_constant<size_t, 0> { };

  template<typename _Tp, size_t _Size>
    struct extent<_Tp[_Size], 0>
    : public integral_constant<size_t, _Size> { };

  template<typename _Tp, unsigned _Uint, size_t _Size>
    struct extent<_Tp[_Size], _Uint>
    : public extent<_Tp, _Uint - 1>::type { };

  template<typename _Tp>
    struct extent<_Tp[], 0>
    : public integral_constant<size_t, 0> { };

  template<typename _Tp, unsigned _Uint>
    struct extent<_Tp[], _Uint>
    : public extent<_Tp, _Uint - 1>::type { };


  // Type relations.

  /// is_same

  template<typename _Tp, typename _Up>
    struct is_same
    : public __bool_constant<__is_same(_Tp, _Up)>
    { };

  /// is_base_of
  template<typename _Base, typename _Derived>
    struct is_base_of
    : public __bool_constant<__is_base_of(_Base, _Derived)>
    { };


  template<typename _From, typename _To>
    struct is_convertible
    : public __bool_constant<__is_convertible(_From, _To)>
    { };

  // helper trait for unique_ptr<T[]>, shared_ptr<T[]>, and span<T, N>
  template<typename _ToElementType, typename _FromElementType>
    using __is_array_convertible
      = is_convertible<_FromElementType(*)[], _ToElementType(*)[]>;


  // Const-volatile modifications.

  /// remove_const
  template<typename _Tp>
    struct remove_const
    { using type = _Tp; };

  template<typename _Tp>
    struct remove_const<_Tp const>
    { using type = _Tp; };

  /// remove_volatile
  template<typename _Tp>
    struct remove_volatile
    { using type = _Tp; };

  template<typename _Tp>
    struct remove_volatile<_Tp volatile>
    { using type = _Tp; };

  /// remove_cv

  template<typename _Tp>
    struct remove_cv
    { using type = __remove_cv(_Tp); };

  /// add_const
  template<typename _Tp>
    struct add_const
    { using type = _Tp const; };

  /// add_volatile
  template<typename _Tp>
    struct add_volatile
    { using type = _Tp volatile; };

  /// add_cv
  template<typename _Tp>
    struct add_cv
    { using type = _Tp const volatile; };


  /// Alias template for remove_const
  template<typename _Tp>
    using remove_const_t = typename remove_const<_Tp>::type;

  /// Alias template for remove_volatile
  template<typename _Tp>
    using remove_volatile_t = typename remove_volatile<_Tp>::type;

  /// Alias template for remove_cv
  template<typename _Tp>
    using remove_cv_t = typename remove_cv<_Tp>::type;

  /// Alias template for add_const
  template<typename _Tp>
    using add_const_t = typename add_const<_Tp>::type;

  /// Alias template for add_volatile
  template<typename _Tp>
    using add_volatile_t = typename add_volatile<_Tp>::type;

  /// Alias template for add_cv
  template<typename _Tp>
    using add_cv_t = typename add_cv<_Tp>::type;


  // Reference transformations.

  /// remove_reference

  template<typename _Tp>
    struct remove_reference
    { using type = __remove_reference(_Tp); };

  /// add_lvalue_reference
  template<typename _Tp>
    struct add_lvalue_reference
    { using type = __add_lval_ref_t<_Tp>; };

  /// add_rvalue_reference
  template<typename _Tp>
    struct add_rvalue_reference
    { using type = __add_rval_ref_t<_Tp>; };


  /// Alias template for remove_reference
  template<typename _Tp>
    using remove_reference_t = typename remove_reference<_Tp>::type;

  /// Alias template for add_lvalue_reference
  template<typename _Tp>
    using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;

  /// Alias template for add_rvalue_reference
  template<typename _Tp>
    using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;


  // Sign modifications.

  /// @cond undocumented

  // Utility for constructing identically cv-qualified types.
  template<typename _Unqualified, bool _IsConst, bool _IsVol>
    struct __cv_selector;

  template<typename _Unqualified>
    struct __cv_selector<_Unqualified, false, false>
    { using __type = _Unqualified; };

  template<typename _Unqualified>
    struct __cv_selector<_Unqualified, false, true>
    { using __type = volatile _Unqualified; };

  template<typename _Unqualified>
    struct __cv_selector<_Unqualified, true, false>
    { using __type = const _Unqualified; };

  template<typename _Unqualified>
    struct __cv_selector<_Unqualified, true, true>
    { using __type = const volatile _Unqualified; };

  template<typename _Qualified, typename _Unqualified,
	   bool _IsConst = is_const<_Qualified>::value,
	   bool _IsVol = is_volatile<_Qualified>::value>
    class __match_cv_qualifiers
    {
      using __match = __cv_selector<_Unqualified, _IsConst, _IsVol>;

    public:
      using __type = typename __match::__type;
    };

  // Utility for finding the unsigned versions of signed integral types.
  template<typename _Tp>
    struct __make_unsigned
    { using __type = _Tp; };

  template<>
    struct __make_unsigned<char>
    { using __type = unsigned char; };

  template<>
    struct __make_unsigned<signed char>
    { using __type = unsigned char; };

  template<>
    struct __make_unsigned<short>
    { using __type = unsigned short; };

  template<>
    struct __make_unsigned<int>
    { using __type = unsigned int; };

  template<>
    struct __make_unsigned<long>
    { using __type = unsigned long; };

  template<>
    struct __make_unsigned<long long>
    { using __type = unsigned long long; };


  __extension__
  template<>
    struct __make_unsigned<__GLIBCXX_TYPE_INT_N_0>
    { using __type = unsigned __GLIBCXX_TYPE_INT_N_0; };




















  // Select between integral and enum: not possible to be both.
  template<typename _Tp,
	   bool _IsInt = is_integral<_Tp>::value,
	   bool _IsEnum = __is_enum(_Tp)>
    class __make_unsigned_selector;

  template<typename _Tp>
    class __make_unsigned_selector<_Tp, true, false>
    {
      using __unsigned_type
	= typename __make_unsigned<__remove_cv_t<_Tp>>::__type;

    public:
      using __type
	= typename __match_cv_qualifiers<_Tp, __unsigned_type>::__type;
    };

  class __make_unsigned_selector_base
  {
  protected:
    template<typename...> struct _List { };

    template<typename _Tp, typename... _Up>
      struct _List<_Tp, _Up...> : _List<_Up...>
      { static constexpr size_t __size = sizeof(_Tp); };

    template<size_t _Sz, typename _Tp, bool = (_Sz <= _Tp::__size)>
      struct __select;

    template<size_t _Sz, typename _Uint, typename... _UInts>
      struct __select<_Sz, _List<_Uint, _UInts...>, true>
      { using __type = _Uint; };

    template<size_t _Sz, typename _Uint, typename... _UInts>
      struct __select<_Sz, _List<_Uint, _UInts...>, false>
      : __select<_Sz, _List<_UInts...>>
      { };
  };

  // Choose unsigned integer type with the smallest rank and same size as _Tp
  template<typename _Tp>
    class __make_unsigned_selector<_Tp, false, true>
    : __make_unsigned_selector_base
    {
      // With -fshort-enums, an enum may be as small as a char.
      using _UInts = _List<unsigned char, unsigned short, unsigned int,
			   unsigned long, unsigned long long>;

      using __unsigned_type = typename __select<sizeof(_Tp), _UInts>::__type;

    public:
      using __type
	= typename __match_cv_qualifiers<_Tp, __unsigned_type>::__type;
    };

  // wchar_t, char8_t, char16_t and char32_t are integral types but are
  // neither signed integer types nor unsigned integer types, so must be
  // transformed to the unsigned integer type with the smallest rank.
  // Use the partial specialization for enumeration types to do that.
  template<>
    struct __make_unsigned<wchar_t>
    {
      using __type
	= typename __make_unsigned_selector<wchar_t, false, true>::__type;
    };


  template<>
    struct __make_unsigned<char16_t>
    {
      using __type
	= typename __make_unsigned_selector<char16_t, false, true>::__type;
    };

  template<>
    struct __make_unsigned<char32_t>
    {
      using __type
	= typename __make_unsigned_selector<char32_t, false, true>::__type;
    };
  /// @endcond

  // Given an integral/enum type, return the corresponding unsigned
  // integer type.
  // Primary template.
  /// make_unsigned
  template<typename _Tp>
    struct make_unsigned
    { using type = typename __make_unsigned_selector<_Tp>::__type; };

  // Integral, but don't define.
  template<> struct make_unsigned<bool>;
  template<> struct make_unsigned<bool const>;
  template<> struct make_unsigned<bool volatile>;
  template<> struct make_unsigned<bool const volatile>;

  /// @cond undocumented

  // Utility for finding the signed versions of unsigned integral types.
  template<typename _Tp>
    struct __make_signed
    { using __type = _Tp; };

  template<>
    struct __make_signed<char>
    { using __type = signed char; };

  template<>
    struct __make_signed<unsigned char>
    { using __type = signed char; };

  template<>
    struct __make_signed<unsigned short>
    { using __type = signed short; };

  template<>
    struct __make_signed<unsigned int>
    { using __type = signed int; };

  template<>
    struct __make_signed<unsigned long>
    { using __type = signed long; };

  template<>
    struct __make_signed<unsigned long long>
    { using __type = signed long long; };


  __extension__
  template<>
    struct __make_signed<unsigned __GLIBCXX_TYPE_INT_N_0>
    { using __type = __GLIBCXX_TYPE_INT_N_0; };




















  // Select between integral and enum: not possible to be both.
  template<typename _Tp,
	   bool _IsInt = is_integral<_Tp>::value,
	   bool _IsEnum = __is_enum(_Tp)>
    class __make_signed_selector;

  template<typename _Tp>
    class __make_signed_selector<_Tp, true, false>
    {
      using __signed_type
	= typename __make_signed<__remove_cv_t<_Tp>>::__type;

    public:
      using __type
	= typename __match_cv_qualifiers<_Tp, __signed_type>::__type;
    };

  // Choose signed integer type with the smallest rank and same size as _Tp
  template<typename _Tp>
    class __make_signed_selector<_Tp, false, true>
    {
      using __unsigned_type = typename __make_unsigned_selector<_Tp>::__type;

    public:
      using __type = typename __make_signed_selector<__unsigned_type>::__type;
    };

  // wchar_t, char16_t and char32_t are integral types but are neither
  // signed integer types nor unsigned integer types, so must be
  // transformed to the signed integer type with the smallest rank.
  // Use the partial specialization for enumeration types to do that.
  template<>
    struct __make_signed<wchar_t>
    {
      using __type
	= typename __make_signed_selector<wchar_t, false, true>::__type;
    };


  template<>
    struct __make_signed<char16_t>
    {
      using __type
	= typename __make_signed_selector<char16_t, false, true>::__type;
    };

  template<>
    struct __make_signed<char32_t>
    {
      using __type
	= typename __make_signed_selector<char32_t, false, true>::__type;
    };
  /// @endcond

  // Given an integral/enum type, return the corresponding signed
  // integer type.
  // Primary template.
  /// make_signed
  template<typename _Tp>
    struct make_signed
    { using type = typename __make_signed_selector<_Tp>::__type; };

  // Integral, but don't define.
  template<> struct make_signed<bool>;
  template<> struct make_signed<bool const>;
  template<> struct make_signed<bool volatile>;
  template<> struct make_signed<bool const volatile>;


  /// Alias template for make_signed
  template<typename _Tp>
    using make_signed_t = typename make_signed<_Tp>::type;

  /// Alias template for make_unsigned
  template<typename _Tp>
    using make_unsigned_t = typename make_unsigned<_Tp>::type;


  // Array modifications.

  /// remove_extent
  template<typename _Tp>
    struct remove_extent
    { using type = _Tp; };

  template<typename _Tp, std::size_t _Size>
    struct remove_extent<_Tp[_Size]>
    { using type = _Tp; };

  template<typename _Tp>
    struct remove_extent<_Tp[]>
    { using type = _Tp; };

  /// remove_all_extents
  template<typename _Tp>
    struct remove_all_extents
    { using type = _Tp; };

  template<typename _Tp, std::size_t _Size>
    struct remove_all_extents<_Tp[_Size]>
    { using type = typename remove_all_extents<_Tp>::type; };

  template<typename _Tp>
    struct remove_all_extents<_Tp[]>
    { using type = typename remove_all_extents<_Tp>::type; };


  /// Alias template for remove_extent
  template<typename _Tp>
    using remove_extent_t = typename remove_extent<_Tp>::type;

  /// Alias template for remove_all_extents
  template<typename _Tp>
    using remove_all_extents_t = typename remove_all_extents<_Tp>::type;


  // Pointer modifications.

  /// remove_pointer

  template<typename _Tp>
    struct remove_pointer
    { using type = __remove_pointer(_Tp); };

  template<typename _Tp, typename = void>
    struct __add_pointer_helper
    { using type = _Tp; };

  template<typename _Tp>
    struct __add_pointer_helper<_Tp, __void_t<_Tp*>>
    { using type = _Tp*; };

  /// add_pointer
  template<typename _Tp>
    struct add_pointer
    : public __add_pointer_helper<_Tp>
    { };

  template<typename _Tp>
    struct add_pointer<_Tp&>
    { using type = _Tp*; };

  template<typename _Tp>
    struct add_pointer<_Tp&&>
    { using type = _Tp*; };


  /// Alias template for remove_pointer
  template<typename _Tp>
    using remove_pointer_t = typename remove_pointer<_Tp>::type;

  /// Alias template for add_pointer
  template<typename _Tp>
    using add_pointer_t = typename add_pointer<_Tp>::type;


  template<std::size_t _Len>
    struct __aligned_storage_msa
    {
      union __type
      {
	unsigned char __data[_Len];
	struct __attribute__((__aligned__)) { } __align;
      };
    };

  /**
   *  @brief Alignment type.
   *
   *  The value of _Align is a default-alignment which shall be the
   *  most stringent alignment requirement for any C++ object type
   *  whose size is no greater than _Len (3.9). The member typedef
   *  type shall be a POD type suitable for use as uninitialized
   *  storage for any object whose size is at most _Len and whose
   *  alignment is a divisor of _Align.
   *
   *  @deprecated Deprecated in C++23. Uses can be replaced by an
   *  array std::byte[_Len] declared with alignas(_Align).
  */
  template<std::size_t _Len, std::size_t _Align =
	   __alignof__(typename __aligned_storage_msa<_Len>::__type)>
    struct
    _GLIBCXX23_DEPRECATED
    aligned_storage
    {
      union type
      {
	unsigned char __data[_Len];
	struct __attribute__((__aligned__((_Align)))) { } __align;
      };
    };

  template <typename... _Types>
    struct __strictest_alignment
    {
      static const size_t _S_alignment = 0;
      static const size_t _S_size = 0;
    };

  template <typename _Tp, typename... _Types>
    struct __strictest_alignment<_Tp, _Types...>
    {
      static const size_t _S_alignment =
        alignof(_Tp) > __strictest_alignment<_Types...>::_S_alignment
	? alignof(_Tp) : __strictest_alignment<_Types...>::_S_alignment;
      static const size_t _S_size =
        sizeof(_Tp) > __strictest_alignment<_Types...>::_S_size
	? sizeof(_Tp) : __strictest_alignment<_Types...>::_S_size;
    };


  /**
   *  @brief Provide aligned storage for types.
   *
   *  [meta.trans.other]
   *
   *  Provides aligned storage for any of the provided types of at
   *  least size _Len.
   *
   *  @see aligned_storage
   *
   *  @deprecated Deprecated in C++23.
   */
  template <size_t _Len, typename... _Types>
    struct
    _GLIBCXX23_DEPRECATED
    aligned_union
    {
    private:
      static_assert(sizeof...(_Types) != 0, "At least one type is required");

      using __strictest = __strictest_alignment<_Types...>;
      static const size_t _S_len = _Len > __strictest::_S_size
	? _Len : __strictest::_S_size;
    public:
      /// The value of the strictest alignment of _Types.
      static const size_t alignment_value = __strictest::_S_alignment;
      /// The storage.
      using type = typename aligned_storage<_S_len, alignment_value>::type;
    };

  template <size_t _Len, typename... _Types>
    const size_t aligned_union<_Len, _Types...>::alignment_value;

  /// @cond undocumented

  // Decay trait for arrays and functions, used for perfect forwarding
  // in make_pair, make_tuple, etc.
  template<typename _Up>
    struct __decay_selector
    : __conditional_t<is_const<const _Up>::value, // false for functions
		      remove_cv<_Up>,		  // N.B. DR 705.
		      add_pointer<_Up>>		  // function decays to pointer
    { };

  template<typename _Up, size_t _Nm>
    struct __decay_selector<_Up[_Nm]>
    { using type = _Up*; };

  template<typename _Up>
    struct __decay_selector<_Up[]>
    { using type = _Up*; };

  /// @endcond

  /// decay
  template<typename _Tp>
    struct decay
    { using type = typename __decay_selector<_Tp>::type; };

  template<typename _Tp>
    struct decay<_Tp&>
    { using type = typename __decay_selector<_Tp>::type; };

  template<typename _Tp>
    struct decay<_Tp&&>
    { using type = typename __decay_selector<_Tp>::type; };

  /// @cond undocumented

  // Helper which adds a reference to a type when given a reference_wrapper
  template<typename _Tp>
    struct __strip_reference_wrapper
    {
      using __type = _Tp;
    };

  template<typename _Tp>
    struct __strip_reference_wrapper<reference_wrapper<_Tp> >
    {
      using __type = _Tp&;
    };

  // __decay_t (std::decay_t for C++11).
  template<typename _Tp>
    using __decay_t = typename decay<_Tp>::type;

  template<typename _Tp>
    using __decay_and_strip = __strip_reference_wrapper<__decay_t<_Tp>>;
  /// @endcond

  /// @cond undocumented

  // Helper for SFINAE constraints
  template<typename... _Cond>
    using _Require = __enable_if_t<__and_<_Cond...>::value>;

  // __remove_cvref_t (std::remove_cvref_t for C++11).
  template<typename _Tp>
    using __remove_cvref_t
     = typename remove_cv<typename remove_reference<_Tp>::type>::type;
  /// @endcond

  // Primary template.
  /// Define a member typedef @c type to one of two argument types.
  template<bool _Cond, typename _Iftrue, typename _Iffalse>
    struct conditional
    { using type = _Iftrue; };

  // Partial specialization for false.
  template<typename _Iftrue, typename _Iffalse>
    struct conditional<false, _Iftrue, _Iffalse>
    { using type = _Iffalse; };

  /// common_type
  template<typename... _Tp>
    struct common_type;

  // Sfinae-friendly common_type implementation:

  /// @cond undocumented

  // For several sfinae-friendly trait implementations we transport both the
  // result information (as the member type) and the failure information (no
  // member type). This is very similar to std::enable_if, but we cannot use
  // that, because we need to derive from them as an implementation detail.

  template<typename _Tp>
    struct __success_type
    { using type = _Tp; };

  struct __failure_type
  { };

  struct __do_common_type_impl
  {
    template<typename _Tp, typename _Up>
      using __cond_t
	= decltype(true ? std::declval<_Tp>() : std::declval<_Up>());

    // if decay_t<decltype(false ? declval<D1>() : declval<D2>())>
    // denotes a valid type, let C denote that type.
    template<typename _Tp, typename _Up>
      static __success_type<__decay_t<__cond_t<_Tp, _Up>>>
      _S_test(int);









    template<typename, typename>
      static __failure_type
      _S_test_2(...);

    template<typename _Tp, typename _Up>
      static decltype(_S_test_2<_Tp, _Up>(0))
      _S_test(...);
  };

  // If sizeof...(T) is zero, there shall be no member type.
  template<>
    struct common_type<>
    { };

  // If sizeof...(T) is one, the same type, if any, as common_type_t<T0, T0>.
  template<typename _Tp0>
    struct common_type<_Tp0>
    : public common_type<_Tp0, _Tp0>
    { };

  // If sizeof...(T) is two, ...
  template<typename _Tp1, typename _Tp2,
	   typename _Dp1 = __decay_t<_Tp1>, typename _Dp2 = __decay_t<_Tp2>>
    struct __common_type_impl
    {
      // If is_same_v<T1, D1> is false or is_same_v<T2, D2> is false,
      // let C denote the same type, if any, as common_type_t<D1, D2>.
      using type = common_type<_Dp1, _Dp2>;
    };

  template<typename _Tp1, typename _Tp2>
    struct __common_type_impl<_Tp1, _Tp2, _Tp1, _Tp2>
    : private __do_common_type_impl
    {
      // Otherwise, if decay_t<decltype(false ? declval<D1>() : declval<D2>())>
      // denotes a valid type, let C denote that type.
      using type = decltype(_S_test<_Tp1, _Tp2>(0));
    };

  // If sizeof...(T) is two, ...
  template<typename _Tp1, typename _Tp2>
    struct common_type<_Tp1, _Tp2>
    : public __common_type_impl<_Tp1, _Tp2>::type
    { };

  template<typename...>
    struct __common_type_pack
    { };

  template<typename, typename, typename = void>
    struct __common_type_fold;

  // If sizeof...(T) is greater than two, ...
  template<typename _Tp1, typename _Tp2, typename... _Rp>
    struct common_type<_Tp1, _Tp2, _Rp...>
    : public __common_type_fold<common_type<_Tp1, _Tp2>,
				__common_type_pack<_Rp...>>
    { };

  // Let C denote the same type, if any, as common_type_t<T1, T2>.
  // If there is such a type C, type shall denote the same type, if any,
  // as common_type_t<C, R...>.
  template<typename _CTp, typename... _Rp>
    struct __common_type_fold<_CTp, __common_type_pack<_Rp...>,
			      __void_t<typename _CTp::type>>
    : public common_type<typename _CTp::type, _Rp...>
    { };

  // Otherwise, there shall be no member type.
  template<typename _CTp, typename _Rp>
    struct __common_type_fold<_CTp, _Rp, void>
    { };

  template<typename _Tp, bool = __is_enum(_Tp)>
    struct __underlying_type_impl
    {
      using type = __underlying_type(_Tp);
    };

  template<typename _Tp>
    struct __underlying_type_impl<_Tp, false>
    { };
  /// @endcond

  /// The underlying type of an enum.
  template<typename _Tp>
    struct underlying_type
    : public __underlying_type_impl<_Tp>
    { };

  /// @cond undocumented
  template<typename _Tp>
    struct __declval_protector
    {
      static const bool __stop = false;
    };
  /// @endcond

  /** Utility to simplify expressions used in unevaluated operands
   *  @since C++11
   *  @ingroup utilities
   */
  template<typename _Tp>
    auto declval() noexcept -> decltype(__declval<_Tp>(0))
    {
      static_assert(__declval_protector<_Tp>::__stop,
		    "declval() must not be used!");
      return __declval<_Tp>(0);
    }

  /// result_of
  template<typename _Signature>
    struct result_of;

  // Sfinae-friendly result_of implementation:

  /// @cond undocumented
  struct __invoke_memfun_ref { };
  struct __invoke_memfun_deref { };
  struct __invoke_memobj_ref { };
  struct __invoke_memobj_deref { };
  struct __invoke_other { };

  // Associate a tag type with a specialization of __success_type.
  template<typename _Tp, typename _Tag>
    struct __result_of_success : __success_type<_Tp>
    { using __invoke_type = _Tag; };

  // [func.require] paragraph 1 bullet 1:
  struct __result_of_memfun_ref_impl
  {
    template<typename _Fp, typename _Tp1, typename... _Args>
      static __result_of_success<decltype(
      (std::declval<_Tp1>().*std::declval<_Fp>())(std::declval<_Args>()...)
      ), __invoke_memfun_ref> _S_test(int);

    template<typename...>
      static __failure_type _S_test(...);
  };

  template<typename _MemPtr, typename _Arg, typename... _Args>
    struct __result_of_memfun_ref
    : private __result_of_memfun_ref_impl
    {
      using type = decltype(_S_test<_MemPtr, _Arg, _Args...>(0));
    };

  // [func.require] paragraph 1 bullet 2:
  struct __result_of_memfun_deref_impl
  {
    template<typename _Fp, typename _Tp1, typename... _Args>
      static __result_of_success<decltype(
      ((*std::declval<_Tp1>()).*std::declval<_Fp>())(std::declval<_Args>()...)
      ), __invoke_memfun_deref> _S_test(int);

    template<typename...>
      static __failure_type _S_test(...);
  };

  template<typename _MemPtr, typename _Arg, typename... _Args>
    struct __result_of_memfun_deref
    : private __result_of_memfun_deref_impl
    {
      using type = decltype(_S_test<_MemPtr, _Arg, _Args...>(0));
    };

  // [func.require] paragraph 1 bullet 3:
  struct __result_of_memobj_ref_impl
  {
    template<typename _Fp, typename _Tp1>
      static __result_of_success<decltype(
      std::declval<_Tp1>().*std::declval<_Fp>()
      ), __invoke_memobj_ref> _S_test(int);

    template<typename, typename>
      static __failure_type _S_test(...);
  };

  template<typename _MemPtr, typename _Arg>
    struct __result_of_memobj_ref
    : private __result_of_memobj_ref_impl
    {
      using type = decltype(_S_test<_MemPtr, _Arg>(0));
    };

  // [func.require] paragraph 1 bullet 4:
  struct __result_of_memobj_deref_impl
  {
    template<typename _Fp, typename _Tp1>
      static __result_of_success<decltype(
      (*std::declval<_Tp1>()).*std::declval<_Fp>()
      ), __invoke_memobj_deref> _S_test(int);

    template<typename, typename>
      static __failure_type _S_test(...);
  };

  template<typename _MemPtr, typename _Arg>
    struct __result_of_memobj_deref
    : private __result_of_memobj_deref_impl
    {
      using type = decltype(_S_test<_MemPtr, _Arg>(0));
    };

  template<typename _MemPtr, typename _Arg>
    struct __result_of_memobj;

  template<typename _Res, typename _Class, typename _Arg>
    struct __result_of_memobj<_Res _Class::*, _Arg>
    {
      using _Argval = __remove_cvref_t<_Arg>;
      using _MemPtr = _Res _Class::*;
      using type = typename __conditional_t<__or_<is_same<_Argval, _Class>,
        is_base_of<_Class, _Argval>>::value,
        __result_of_memobj_ref<_MemPtr, _Arg>,
        __result_of_memobj_deref<_MemPtr, _Arg>
      >::type;
    };

  template<typename _MemPtr, typename _Arg, typename... _Args>
    struct __result_of_memfun;

  template<typename _Res, typename _Class, typename _Arg, typename... _Args>
    struct __result_of_memfun<_Res _Class::*, _Arg, _Args...>
    {
      using _Argval = typename remove_reference<_Arg>::type;
      using _MemPtr = _Res _Class::*;
      using type = typename __conditional_t<is_base_of<_Class, _Argval>::value,
        __result_of_memfun_ref<_MemPtr, _Arg, _Args...>,
        __result_of_memfun_deref<_MemPtr, _Arg, _Args...>
      >::type;
    };

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2219.  INVOKE-ing a pointer to member with a reference_wrapper
  //        as the object expression

  // Used by result_of, invoke etc. to unwrap a reference_wrapper.
  template<typename _Tp, typename _Up = __remove_cvref_t<_Tp>>
    struct __inv_unwrap
    {
      using type = _Tp;
    };

  template<typename _Tp, typename _Up>
    struct __inv_unwrap<_Tp, reference_wrapper<_Up>>
    {
      using type = _Up&;
    };

  template<bool, bool, typename _Functor, typename... _ArgTypes>
    struct __result_of_impl
    {
      using type = __failure_type;
    };

  template<typename _MemPtr, typename _Arg>
    struct __result_of_impl<true, false, _MemPtr, _Arg>
    : public __result_of_memobj<__decay_t<_MemPtr>,
				typename __inv_unwrap<_Arg>::type>
    { };

  template<typename _MemPtr, typename _Arg, typename... _Args>
    struct __result_of_impl<false, true, _MemPtr, _Arg, _Args...>
    : public __result_of_memfun<__decay_t<_MemPtr>,
				typename __inv_unwrap<_Arg>::type, _Args...>
    { };

  // [func.require] paragraph 1 bullet 5:
  struct __result_of_other_impl
  {
    template<typename _Fn, typename... _Args>
      static __result_of_success<decltype(
      std::declval<_Fn>()(std::declval<_Args>()...)
      ), __invoke_other> _S_test(int);

    template<typename...>
      static __failure_type _S_test(...);
  };

  template<typename _Functor, typename... _ArgTypes>
    struct __result_of_impl<false, false, _Functor, _ArgTypes...>
    : private __result_of_other_impl
    {
      using type = decltype(_S_test<_Functor, _ArgTypes...>(0));
    };

  // __invoke_result (std::invoke_result for C++11)
  template<typename _Functor, typename... _ArgTypes>
    struct __invoke_result
    : public __result_of_impl<
        is_member_object_pointer<
          typename remove_reference<_Functor>::type
        >::value,
        is_member_function_pointer<
          typename remove_reference<_Functor>::type
        >::value,
	_Functor, _ArgTypes...
      >::type
    { };

  // __invoke_result_t (std::invoke_result_t for C++11)
  template<typename _Fn, typename... _Args>
    using __invoke_result_t = typename __invoke_result<_Fn, _Args...>::type;
  /// @endcond

  template<typename _Functor, typename... _ArgTypes>
    struct result_of<_Functor(_ArgTypes...)>
    : public __invoke_result<_Functor, _ArgTypes...>
    { } _GLIBCXX17_DEPRECATED_SUGGEST("std::invoke_result");


  /// Alias template for aligned_storage
  template<size_t _Len, size_t _Align =
	    __alignof__(typename __aligned_storage_msa<_Len>::__type)>
    using aligned_storage_t _GLIBCXX23_DEPRECATED = typename aligned_storage<_Len, _Align>::type;

  template <size_t _Len, typename... _Types>
    using aligned_union_t _GLIBCXX23_DEPRECATED = typename aligned_union<_Len, _Types...>::type;

  /// Alias template for decay
  template<typename _Tp>
    using decay_t = typename decay<_Tp>::type;

  /// Alias template for enable_if
  template<bool _Cond, typename _Tp = void>
    using enable_if_t = typename enable_if<_Cond, _Tp>::type;

  /// Alias template for conditional
  template<bool _Cond, typename _Iftrue, typename _Iffalse>
    using conditional_t = typename conditional<_Cond, _Iftrue, _Iffalse>::type;

  /// Alias template for common_type
  template<typename... _Tp>
    using common_type_t = typename common_type<_Tp...>::type;

  /// Alias template for underlying_type
  template<typename _Tp>
    using underlying_type_t = typename underlying_type<_Tp>::type;

  /// Alias template for result_of
  template<typename _Tp>
    using result_of_t = typename result_of<_Tp>::type;



  /// A metafunction that always yields void, used for detecting valid types.
  template<typename...> using void_t = void;


  /// @cond undocumented

  // Detection idiom.
  // Detect whether _Op<_Args...> is a valid type, use default _Def if not.

  /// Implementation of the detection idiom (negative case).
  template<typename _Default, typename _AlwaysVoid,
	   template<typename...> class _Op, typename... _Args>
    struct __detector
    {
      using type = _Default;
      using __is_detected = false_type;
    };

  /// Implementation of the detection idiom (positive case).
  template<typename _Default, template<typename...> class _Op,
	    typename... _Args>
    struct __detector<_Default, __void_t<_Op<_Args...>>, _Op, _Args...>
    {
      using type = _Op<_Args...>;
      using __is_detected = true_type;
    };

  template<typename _Default, template<typename...> class _Op,
	   typename... _Args>
    using __detected_or = __detector<_Default, void, _Op, _Args...>;


  // _Op<_Args...> if that is a valid type, otherwise _Default.
  template<typename _Default, template<typename...> class _Op,
	   typename... _Args>
    using __detected_or_t
      = typename __detected_or<_Default, _Op, _Args...>::type;

  /**
   *  Use SFINAE to determine if the type _Tp has a publicly-accessible
   *  member type _NTYPE.
   */

  template <typename _Tp>
    struct __is_swappable;

  template <typename _Tp>
    struct __is_nothrow_swappable;

  template<typename>
    struct __is_tuple_like_impl : false_type
    { };

  // Internal type trait that allows us to sfinae-protect tuple_cat.
  template<typename _Tp>
    struct __is_tuple_like
    : public __is_tuple_like_impl<__remove_cvref_t<_Tp>>::type
    { };
  /// @endcond

  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR
    inline
    _Require<__not_<__is_tuple_like<_Tp>>,
	     is_move_constructible<_Tp>,
	     is_move_assignable<_Tp>>
    swap(_Tp&, _Tp&)
    noexcept(__and_<is_nothrow_move_constructible<_Tp>,
	            is_nothrow_move_assignable<_Tp>>::value);

  template<typename _Tp, size_t _Nm>
    _GLIBCXX20_CONSTEXPR
    inline
    __enable_if_t<__is_swappable<_Tp>::value>
    swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
    noexcept(__is_nothrow_swappable<_Tp>::value);

  /// @cond undocumented
  namespace __swappable_details {
    using std::swap;

    struct __do_is_swappable_impl
    {
      template<typename _Tp, typename
               = decltype(swap(std::declval<_Tp&>(), std::declval<_Tp&>()))>
        static true_type __test(int);

      template<typename>
        static false_type __test(...);
    };

    struct __do_is_nothrow_swappable_impl
    {
      template<typename _Tp>
        static __bool_constant<
          noexcept(swap(std::declval<_Tp&>(), std::declval<_Tp&>()))
        > __test(int);

      template<typename>
        static false_type __test(...);
    };

  } // namespace __swappable_details

  template<typename _Tp>
    struct __is_swappable_impl
    : public __swappable_details::__do_is_swappable_impl
    {
      using type = decltype(__test<_Tp>(0));
    };

  template<typename _Tp>
    struct __is_nothrow_swappable_impl
    : public __swappable_details::__do_is_nothrow_swappable_impl
    {
      using type = decltype(__test<_Tp>(0));
    };

  template<typename _Tp>
    struct __is_swappable
    : public __is_swappable_impl<_Tp>::type
    { };

  template<typename _Tp>
    struct __is_nothrow_swappable
    : public __is_nothrow_swappable_impl<_Tp>::type
    { };
  /// @endcond


  /// Metafunctions used for detecting swappable types: p0185r1

  /// is_swappable
  template<typename _Tp>
    struct is_swappable
    : public __is_swappable_impl<_Tp>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_swappable
  template<typename _Tp>
    struct is_nothrow_swappable
    : public __is_nothrow_swappable_impl<_Tp>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };


  /// is_swappable_v
  template<typename _Tp>
    _GLIBCXX17_INLINE constexpr bool is_swappable_v =
      is_swappable<_Tp>::value;

  /// is_nothrow_swappable_v
  template<typename _Tp>
    _GLIBCXX17_INLINE constexpr bool is_nothrow_swappable_v =
      is_nothrow_swappable<_Tp>::value;


  /// @cond undocumented
  namespace __swappable_with_details {
    using std::swap;

    struct __do_is_swappable_with_impl
    {
      template<typename _Tp, typename _Up, typename
               = decltype(swap(std::declval<_Tp>(), std::declval<_Up>())),
               typename
               = decltype(swap(std::declval<_Up>(), std::declval<_Tp>()))>
        static true_type __test(int);

      template<typename, typename>
        static false_type __test(...);
    };

    struct __do_is_nothrow_swappable_with_impl
    {
      template<typename _Tp, typename _Up>
        static __bool_constant<
          noexcept(swap(std::declval<_Tp>(), std::declval<_Up>()))
          &&
          noexcept(swap(std::declval<_Up>(), std::declval<_Tp>()))
        > __test(int);

      template<typename, typename>
        static false_type __test(...);
    };

  } // namespace __swappable_with_details

  template<typename _Tp, typename _Up>
    struct __is_swappable_with_impl
    : public __swappable_with_details::__do_is_swappable_with_impl
    {
      using type = decltype(__test<_Tp, _Up>(0));
    };

  // Optimization for the homogenous lvalue case, not required:
  template<typename _Tp>
    struct __is_swappable_with_impl<_Tp&, _Tp&>
    : public __swappable_details::__do_is_swappable_impl
    {
      using type = decltype(__test<_Tp&>(0));
    };

  template<typename _Tp, typename _Up>
    struct __is_nothrow_swappable_with_impl
    : public __swappable_with_details::__do_is_nothrow_swappable_with_impl
    {
      using type = decltype(__test<_Tp, _Up>(0));
    };

  // Optimization for the homogenous lvalue case, not required:
  template<typename _Tp>
    struct __is_nothrow_swappable_with_impl<_Tp&, _Tp&>
    : public __swappable_details::__do_is_nothrow_swappable_impl
    {
      using type = decltype(__test<_Tp&>(0));
    };
  /// @endcond

  /// is_swappable_with
  template<typename _Tp, typename _Up>
    struct is_swappable_with
    : public __is_swappable_with_impl<_Tp, _Up>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"first template argument must be a complete class or an unbounded array");
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Up>{}),
	"second template argument must be a complete class or an unbounded array");
    };

  /// is_nothrow_swappable_with
  template<typename _Tp, typename _Up>
    struct is_nothrow_swappable_with
    : public __is_nothrow_swappable_with_impl<_Tp, _Up>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"first template argument must be a complete class or an unbounded array");
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Up>{}),
	"second template argument must be a complete class or an unbounded array");
    };


  /// is_swappable_with_v
  template<typename _Tp, typename _Up>
    _GLIBCXX17_INLINE constexpr bool is_swappable_with_v =
      is_swappable_with<_Tp, _Up>::value;

  /// is_nothrow_swappable_with_v
  template<typename _Tp, typename _Up>
    _GLIBCXX17_INLINE constexpr bool is_nothrow_swappable_with_v =
      is_nothrow_swappable_with<_Tp, _Up>::value;




  /// @cond undocumented

  // __is_invocable (std::is_invocable for C++11)

  // The primary template is used for invalid INVOKE expressions.
  template<typename _Result, typename _Ret,
	   bool = is_void<_Ret>::value, typename = void>
    struct __is_invocable_impl
    : false_type
    {
      using __nothrow_conv = false_type; // For is_nothrow_invocable_r
    };

  // Used for valid INVOKE and INVOKE<void> expressions.
  template<typename _Result, typename _Ret>
    struct __is_invocable_impl<_Result, _Ret,
			       /* is_void<_Ret> = */ true,
			       __void_t<typename _Result::type>>
    : true_type
    {
      using __nothrow_conv = true_type; // For is_nothrow_invocable_r
    };

  // Used for INVOKE<R> expressions to check the implicit conversion to R.
  template<typename _Result, typename _Ret>
    struct __is_invocable_impl<_Result, _Ret,
			       /* is_void<_Ret> = */ false,
			       __void_t<typename _Result::type>>
    {
    private:
      // The type of the INVOKE expression.
      using _Res_t = typename _Result::type;

      // Unlike declval, this doesn't add_rvalue_reference, so it respects
      // guaranteed copy elision.
      static _Res_t _S_get() noexcept;

      // Used to check if _Res_t can implicitly convert to _Tp.
      template<typename _Tp>
	static void _S_conv(__type_identity_t<_Tp>) noexcept;

      // This overload is viable if INVOKE(f, args...) can convert to _Tp.
      template<typename _Tp,
	       bool _Nothrow = noexcept(_S_conv<_Tp>(_S_get())),
	       typename = decltype(_S_conv<_Tp>(_S_get())),

	       bool _Dangle = __reference_converts_from_temporary(_Tp, _Res_t)



	      >
	static __bool_constant<_Nothrow && !_Dangle>
	_S_test(int);

      template<typename _Tp, bool = false>
	static false_type
	_S_test(...);

    public:
      // For is_invocable_r
      using type = decltype(_S_test<_Ret, /* Nothrow = */ true>(1));

      // For is_nothrow_invocable_r
      using __nothrow_conv = decltype(_S_test<_Ret>(1));
    };

  template<typename _Fn, typename... _ArgTypes>
    struct __is_invocable
    : __is_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, void>::type
    { };

  template<typename _Fn, typename _Tp, typename... _Args>
    constexpr bool __call_is_nt(__invoke_memfun_ref)
    {
      using _Up = typename __inv_unwrap<_Tp>::type;
      return noexcept((std::declval<_Up>().*std::declval<_Fn>())(
	    std::declval<_Args>()...));
    }

  template<typename _Fn, typename _Tp, typename... _Args>
    constexpr bool __call_is_nt(__invoke_memfun_deref)
    {
      return noexcept(((*std::declval<_Tp>()).*std::declval<_Fn>())(
	    std::declval<_Args>()...));
    }

  template<typename _Fn, typename _Tp>
    constexpr bool __call_is_nt(__invoke_memobj_ref)
    {
      using _Up = typename __inv_unwrap<_Tp>::type;
      return noexcept(std::declval<_Up>().*std::declval<_Fn>());
    }

  template<typename _Fn, typename _Tp>
    constexpr bool __call_is_nt(__invoke_memobj_deref)
    {
      return noexcept((*std::declval<_Tp>()).*std::declval<_Fn>());
    }

  template<typename _Fn, typename... _Args>
    constexpr bool __call_is_nt(__invoke_other)
    {
      return noexcept(std::declval<_Fn>()(std::declval<_Args>()...));
    }

  template<typename _Result, typename _Fn, typename... _Args>
    struct __call_is_nothrow
    : __bool_constant<
	std::__call_is_nt<_Fn, _Args...>(typename _Result::__invoke_type{})
      >
    { };

  template<typename _Fn, typename... _Args>
    using __call_is_nothrow_
      = __call_is_nothrow<__invoke_result<_Fn, _Args...>, _Fn, _Args...>;

  // __is_nothrow_invocable (std::is_nothrow_invocable for C++11)
  template<typename _Fn, typename... _Args>
    struct __is_nothrow_invocable
    : __and_<__is_invocable<_Fn, _Args...>,
             __call_is_nothrow_<_Fn, _Args...>>::type
    { };

  struct __nonesuchbase {};
  struct __nonesuch : private __nonesuchbase {
    ~__nonesuch() = delete;
    __nonesuch(__nonesuch const&) = delete;
    void operator=(__nonesuch const&) = delete;
  };
  /// @endcond


  /// std::invoke_result
  template<typename _Functor, typename... _ArgTypes>
    struct invoke_result
    : public __invoke_result<_Functor, _ArgTypes...>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Functor>{}),
	"_Functor must be a complete class or an unbounded array");
      static_assert((std::__is_complete_or_unbounded(
	__type_identity<_ArgTypes>{}) && ...),
	"each argument type must be a complete class or an unbounded array");
    };

  /// std::invoke_result_t
  template<typename _Fn, typename... _Args>
    using invoke_result_t = typename invoke_result<_Fn, _Args...>::type;

  /// std::is_invocable
  template<typename _Fn, typename... _ArgTypes>
    struct is_invocable
    : __is_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, void>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Fn>{}),
	"_Fn must be a complete class or an unbounded array");
      static_assert((std::__is_complete_or_unbounded(
	__type_identity<_ArgTypes>{}) && ...),
	"each argument type must be a complete class or an unbounded array");
    };

  /// std::is_invocable_r
  template<typename _Ret, typename _Fn, typename... _ArgTypes>
    struct is_invocable_r
    : __is_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, _Ret>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Fn>{}),
	"_Fn must be a complete class or an unbounded array");
      static_assert((std::__is_complete_or_unbounded(
	__type_identity<_ArgTypes>{}) && ...),
	"each argument type must be a complete class or an unbounded array");
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Ret>{}),
	"_Ret must be a complete class or an unbounded array");
    };

  /// std::is_nothrow_invocable
  template<typename _Fn, typename... _ArgTypes>
    struct is_nothrow_invocable
    : __and_<__is_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, void>,
	     __call_is_nothrow_<_Fn, _ArgTypes...>>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Fn>{}),
	"_Fn must be a complete class or an unbounded array");
      static_assert((std::__is_complete_or_unbounded(
	__type_identity<_ArgTypes>{}) && ...),
	"each argument type must be a complete class or an unbounded array");
    };

  /// @cond undocumented
  // This checks that the INVOKE<R> expression is well-formed and that the
  // conversion to R does not throw. It does *not* check whether the INVOKE
  // expression itself can throw. That is done by __call_is_nothrow_ instead.
  template<typename _Result, typename _Ret>
    using __is_nt_invocable_impl
      = typename __is_invocable_impl<_Result, _Ret>::__nothrow_conv;
  /// @endcond

  /// std::is_nothrow_invocable_r
  template<typename _Ret, typename _Fn, typename... _ArgTypes>
    struct is_nothrow_invocable_r
    : __and_<__is_nt_invocable_impl<__invoke_result<_Fn, _ArgTypes...>, _Ret>,
             __call_is_nothrow_<_Fn, _ArgTypes...>>::type
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Fn>{}),
	"_Fn must be a complete class or an unbounded array");
      static_assert((std::__is_complete_or_unbounded(
	__type_identity<_ArgTypes>{}) && ...),
	"each argument type must be a complete class or an unbounded array");
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Ret>{}),
	"_Ret must be a complete class or an unbounded array");
    };



  /**
   * @defgroup variable_templates Variable templates for type traits
   * @ingroup metaprogramming
   *
   * Each variable `is_xxx_v<T>` is a boolean constant with the same value
   * as the `value` member of the corresponding type trait `is_xxx<T>`.
   *
   * @since C++17 unless noted otherwise.
   */

  /**
   * @{
   * @ingroup variable_templates
   */
template <typename _Tp>
  inline constexpr bool is_void_v = is_void<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_null_pointer_v = is_null_pointer<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_integral_v = is_integral<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_floating_point_v = is_floating_point<_Tp>::value;


template <typename _Tp>
  inline constexpr bool is_array_v = __is_array(_Tp);

template <typename _Tp>
  inline constexpr bool is_pointer_v = is_pointer<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_lvalue_reference_v = false;
template <typename _Tp>
  inline constexpr bool is_lvalue_reference_v<_Tp&> = true;
template <typename _Tp>
  inline constexpr bool is_rvalue_reference_v = false;
template <typename _Tp>
  inline constexpr bool is_rvalue_reference_v<_Tp&&> = true;


template <typename _Tp>
  inline constexpr bool is_member_object_pointer_v =
    __is_member_object_pointer(_Tp);







template <typename _Tp>
  inline constexpr bool is_member_function_pointer_v =
    __is_member_function_pointer(_Tp);






template <typename _Tp>
  inline constexpr bool is_enum_v = __is_enum(_Tp);
template <typename _Tp>
  inline constexpr bool is_union_v = __is_union(_Tp);
template <typename _Tp>
  inline constexpr bool is_class_v = __is_class(_Tp);
// is_function_v is defined below, after is_const_v.


template <typename _Tp>
  inline constexpr bool is_reference_v = __is_reference(_Tp);

template <typename _Tp>
  inline constexpr bool is_arithmetic_v = is_arithmetic<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_fundamental_v = is_fundamental<_Tp>::value;


template <typename _Tp>
  inline constexpr bool is_object_v = __is_object(_Tp);





template <typename _Tp>
  inline constexpr bool is_scalar_v = is_scalar<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_compound_v = !is_fundamental_v<_Tp>;


template <typename _Tp>
  inline constexpr bool is_member_pointer_v = __is_member_pointer(_Tp);





template <typename _Tp>
  inline constexpr bool is_const_v = false;
template <typename _Tp>
  inline constexpr bool is_const_v<const _Tp> = true;


template <typename _Tp>
  inline constexpr bool is_function_v = __is_function(_Tp);

template <typename _Tp>
  inline constexpr bool is_volatile_v = false;
template <typename _Tp>
  inline constexpr bool is_volatile_v<volatile _Tp> = true;

template <typename _Tp>
  inline constexpr bool is_trivial_v = __is_trivial(_Tp);
template <typename _Tp>
  inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(_Tp);
template <typename _Tp>
  inline constexpr bool is_standard_layout_v = __is_standard_layout(_Tp);
template <typename _Tp>
  _GLIBCXX20_DEPRECATED_SUGGEST("is_standard_layout_v && is_trivial_v")
  inline constexpr bool is_pod_v = __is_pod(_Tp);
template <typename _Tp>
  _GLIBCXX17_DEPRECATED
  inline constexpr bool is_literal_type_v = __is_literal_type(_Tp);
template <typename _Tp>
  inline constexpr bool is_empty_v = __is_empty(_Tp);
template <typename _Tp>
  inline constexpr bool is_polymorphic_v = __is_polymorphic(_Tp);
template <typename _Tp>
  inline constexpr bool is_abstract_v = __is_abstract(_Tp);
template <typename _Tp>
  inline constexpr bool is_final_v = __is_final(_Tp);

template <typename _Tp>
  inline constexpr bool is_signed_v = is_signed<_Tp>::value;
template <typename _Tp>
  inline constexpr bool is_unsigned_v = is_unsigned<_Tp>::value;

template <typename _Tp, typename... _Args>
  inline constexpr bool is_constructible_v = __is_constructible(_Tp, _Args...);
template <typename _Tp>
  inline constexpr bool is_default_constructible_v = __is_constructible(_Tp);
template <typename _Tp>
  inline constexpr bool is_copy_constructible_v
    = __is_constructible(_Tp, __add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_move_constructible_v
    = __is_constructible(_Tp, __add_rval_ref_t<_Tp>);

template <typename _Tp, typename _Up>
  inline constexpr bool is_assignable_v = __is_assignable(_Tp, _Up);
template <typename _Tp>
  inline constexpr bool is_copy_assignable_v
    = __is_assignable(__add_lval_ref_t<_Tp>, __add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_move_assignable_v
    = __is_assignable(__add_lval_ref_t<_Tp>, __add_rval_ref_t<_Tp>);

template <typename _Tp>
  inline constexpr bool is_destructible_v = is_destructible<_Tp>::value;

template <typename _Tp, typename... _Args>
  inline constexpr bool is_trivially_constructible_v
    = __is_trivially_constructible(_Tp, _Args...);
template <typename _Tp>
  inline constexpr bool is_trivially_default_constructible_v
    = __is_trivially_constructible(_Tp);
template <typename _Tp>
  inline constexpr bool is_trivially_copy_constructible_v
    = __is_trivially_constructible(_Tp, __add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_trivially_move_constructible_v
    = __is_trivially_constructible(_Tp, __add_rval_ref_t<_Tp>);

template <typename _Tp, typename _Up>
  inline constexpr bool is_trivially_assignable_v
    = __is_trivially_assignable(_Tp, _Up);
template <typename _Tp>
  inline constexpr bool is_trivially_copy_assignable_v
    = __is_trivially_assignable(__add_lval_ref_t<_Tp>,
				__add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_trivially_move_assignable_v
    = __is_trivially_assignable(__add_lval_ref_t<_Tp>,
				__add_rval_ref_t<_Tp>);

template <typename _Tp>
  inline constexpr bool is_trivially_destructible_v =
    is_trivially_destructible<_Tp>::value;


template <typename _Tp, typename... _Args>
  inline constexpr bool is_nothrow_constructible_v
    = __is_nothrow_constructible(_Tp, _Args...);
template <typename _Tp>
  inline constexpr bool is_nothrow_default_constructible_v
    = __is_nothrow_constructible(_Tp);
template <typename _Tp>
  inline constexpr bool is_nothrow_copy_constructible_v
    = __is_nothrow_constructible(_Tp, __add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_nothrow_move_constructible_v
    = __is_nothrow_constructible(_Tp, __add_rval_ref_t<_Tp>);

template <typename _Tp, typename _Up>
  inline constexpr bool is_nothrow_assignable_v
    = __is_nothrow_assignable(_Tp, _Up);
template <typename _Tp>
  inline constexpr bool is_nothrow_copy_assignable_v
    = __is_nothrow_assignable(__add_lval_ref_t<_Tp>,
			      __add_lval_ref_t<const _Tp>);
template <typename _Tp>
  inline constexpr bool is_nothrow_move_assignable_v
    = __is_nothrow_assignable(__add_lval_ref_t<_Tp>, __add_rval_ref_t<_Tp>);

template <typename _Tp>
  inline constexpr bool is_nothrow_destructible_v =
    is_nothrow_destructible<_Tp>::value;

template <typename _Tp>
  inline constexpr bool has_virtual_destructor_v
    = __has_virtual_destructor(_Tp);

template <typename _Tp>
  inline constexpr size_t alignment_of_v = alignment_of<_Tp>::value;

template <typename _Tp>
  inline constexpr size_t rank_v = 0;
template <typename _Tp, size_t _Size>
  inline constexpr size_t rank_v<_Tp[_Size]> = 1 + rank_v<_Tp>;
template <typename _Tp>
  inline constexpr size_t rank_v<_Tp[]> = 1 + rank_v<_Tp>;

template <typename _Tp, unsigned _Idx = 0>
  inline constexpr size_t extent_v = 0;
template <typename _Tp, size_t _Size>
  inline constexpr size_t extent_v<_Tp[_Size], 0> = _Size;
template <typename _Tp, unsigned _Idx, size_t _Size>
  inline constexpr size_t extent_v<_Tp[_Size], _Idx> = extent_v<_Tp, _Idx - 1>;
template <typename _Tp>
  inline constexpr size_t extent_v<_Tp[], 0> = 0;
template <typename _Tp, unsigned _Idx>
  inline constexpr size_t extent_v<_Tp[], _Idx> = extent_v<_Tp, _Idx - 1>;


template <typename _Tp, typename _Up>
  inline constexpr bool is_same_v = __is_same(_Tp, _Up);






template <typename _Base, typename _Derived>
  inline constexpr bool is_base_of_v = __is_base_of(_Base, _Derived);

template <typename _From, typename _To>
  inline constexpr bool is_convertible_v = __is_convertible(_From, _To);




template<typename _Fn, typename... _Args>
  inline constexpr bool is_invocable_v = is_invocable<_Fn, _Args...>::value;
template<typename _Fn, typename... _Args>
  inline constexpr bool is_nothrow_invocable_v
    = is_nothrow_invocable<_Fn, _Args...>::value;
template<typename _Ret, typename _Fn, typename... _Args>
  inline constexpr bool is_invocable_r_v
    = is_invocable_r<_Ret, _Fn, _Args...>::value;
template<typename _Ret, typename _Fn, typename... _Args>
  inline constexpr bool is_nothrow_invocable_r_v
    = is_nothrow_invocable_r<_Ret, _Fn, _Args...>::value;
/// @}



  /// has_unique_object_representations
  /// @since C++17
  template<typename _Tp>
    struct has_unique_object_representations
    : bool_constant<__has_unique_object_representations(
      remove_cv_t<remove_all_extents_t<_Tp>>
      )>
    {
      static_assert(std::__is_complete_or_unbounded(__type_identity<_Tp>{}),
	"template argument must be a complete class or an unbounded array");
    };


  /// @ingroup variable_templates
  template<typename _Tp>
    inline constexpr bool has_unique_object_representations_v
      = has_unique_object_representations<_Tp>::value;




  /// is_aggregate - true if the type is an aggregate.
  /// @since C++17
  template<typename _Tp>
    struct is_aggregate
    : bool_constant<__is_aggregate(remove_cv_t<_Tp>)>
    { };


  /** is_aggregate_v - true if the type is an aggregate.
   *  @ingroup variable_templates
   *  @since C++17
   */
  template<typename _Tp>
    inline constexpr bool is_aggregate_v = __is_aggregate(remove_cv_t<_Tp>);



  /** * Remove references and cv-qualifiers.
   * @since C++20
   * @{
   */










  /// @} group metaprogramming

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std



// Move, forward and identity for C++11 + swap -*- C++ -*-

// Copyright (C) 2007-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file bits/move.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 */










namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Used, in C++03 mode too, by allocators, etc.
  /**
   *  @brief Same as C++11 std::addressof
   *  @ingroup utilities
   */
  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR _Tp*
    __addressof(_Tp& __r) _GLIBCXX_NOEXCEPT
    { return __builtin_addressof(__r); }



  /**
   *  @addtogroup utilities
   *  @{
   */

  /**
   *  @brief  Forward an lvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   */
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }

  /**
   *  @brief  Forward an rvalue.
   *  @return The parameter cast to the specified type.
   *
   *  This function is used to implement "perfect forwarding".
   */
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
    {
      static_assert(!std::is_lvalue_reference<_Tp>::value,
	  "std::forward must not be used to convert an rvalue to an lvalue");
      return static_cast<_Tp&&>(__t);
    }


  /**
   *  @brief  Convert a value to an rvalue.
   *  @param  __t  A thing of arbitrary type.
   *  @return The parameter cast to an rvalue-reference to allow moving it.
  */
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr typename std::remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }


  template<typename _Tp>
    struct __move_if_noexcept_cond
    : public __and_<__not_<is_nothrow_move_constructible<_Tp>>,
                    is_copy_constructible<_Tp>>::type { };

  /**
   *  @brief  Conditionally convert a value to an rvalue.
   *  @param  __x  A thing of arbitrary type.
   *  @return The parameter, possibly cast to an rvalue-reference.
   *
   *  Same as std::move unless the type's move constructor could throw and the
   *  type is copyable, in which case an lvalue-reference is returned instead.
   */
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    constexpr
    __conditional_t<__move_if_noexcept_cond<_Tp>::value, const _Tp&, _Tp&&>
    move_if_noexcept(_Tp& __x) noexcept
    { return std::move(__x); }

  // declval, from type_traits.

  /**
   *  @brief Returns the actual address of the object or function
   *         referenced by r, even in the presence of an overloaded
   *         operator&.
   *  @param  __r  Reference to an object or function.
   *  @return   The actual address.
  */
  template<typename _Tp>
    _GLIBCXX_NODISCARD
    inline _GLIBCXX17_CONSTEXPR _Tp*
    addressof(_Tp& __r) noexcept
    { return std::__addressof(__r); }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2598. addressof works on temporaries
  template<typename _Tp>
    const _Tp* addressof(const _Tp&&) = delete;

  // C++11 version of std::exchange for internal use.
  template <typename _Tp, typename _Up = _Tp>
    _GLIBCXX20_CONSTEXPR
    inline _Tp
    __exchange(_Tp& __obj, _Up&& __new_val)
    {
      _Tp __old_val = std::move(__obj);
      __obj = std::forward<_Up>(__new_val);
      return __old_val;
    }

  /// @} group utilities







  /**
   *  @addtogroup utilities
   *  @{
   */

  /**
   *  @brief Swaps two values.
   *  @param  __a  A thing of arbitrary type.
   *  @param  __b  Another thing of arbitrary type.
   *  @return   Nothing.
  */
  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR
    inline

    typename enable_if<__and_<__not_<__is_tuple_like<_Tp>>,
			      is_move_constructible<_Tp>,
			      is_move_assignable<_Tp>>::value>::type



    swap(_Tp& __a, _Tp& __b)
    _GLIBCXX_NOEXCEPT_IF(__and_<is_nothrow_move_constructible<_Tp>,
				is_nothrow_move_assignable<_Tp>>::value)
    {




      _Tp __tmp = _GLIBCXX_MOVE(__a);
      __a = _GLIBCXX_MOVE(__b);
      __b = _GLIBCXX_MOVE(__tmp);
    }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // DR 809. std::swap should be overloaded for array types.
  /// Swap the contents of two arrays.
  template<typename _Tp, size_t _Nm>
    _GLIBCXX20_CONSTEXPR
    inline

    typename enable_if<__is_swappable<_Tp>::value>::type



    swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
    _GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Tp>::value)
    {
      for (size_t __n = 0; __n < _Nm; ++__n)
	swap(__a[__n], __b[__n]);
    }

  /// @} group utilities
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

// Utilities used throughout the library -*- C++ -*-

// Copyright (C) 2004-2024 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file include/bits/utility.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{utility}
 *
 *  This file contains the parts of `<utility>` needed by other headers,
 *  so they don't need to include the whole of `<utility>`.
 */



       






namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /// Finds the size of a given tuple type.
  template<typename _Tp>
    struct tuple_size;

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2313. tuple_size should always derive from integral_constant<size_t, N>
  // 2770. tuple_size<const T> specialization is not SFINAE compatible

  template<typename _Tp,
	   typename _Up = typename remove_cv<_Tp>::type,
	   typename = typename enable_if<is_same<_Tp, _Up>::value>::type,
	   size_t = tuple_size<_Tp>::value>
    using __enable_if_has_tuple_size = _Tp;

  template<typename _Tp>
    struct tuple_size<const __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> { };

  template<typename _Tp>
    struct tuple_size<volatile __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> { };

  template<typename _Tp>
    struct tuple_size<const volatile __enable_if_has_tuple_size<_Tp>>
    : public tuple_size<_Tp> { };


  template<typename _Tp>
    inline constexpr size_t tuple_size_v = tuple_size<_Tp>::value;


  /// Gives the type of the ith element of a given tuple type.
  template<size_t __i, typename _Tp>
    struct tuple_element;

  // Duplicate of C++14's tuple_element_t for internal use in C++11 mode
  template<size_t __i, typename _Tp>
    using __tuple_element_t = typename tuple_element<__i, _Tp>::type;

  template<size_t __i, typename _Tp>
    struct tuple_element<__i, const _Tp>
    {
      using type = const __tuple_element_t<__i, _Tp>;
    };

  template<size_t __i, typename _Tp>
    struct tuple_element<__i, volatile _Tp>
    {
      using type = volatile __tuple_element_t<__i, _Tp>;
    };

  template<size_t __i, typename _Tp>
    struct tuple_element<__i, const volatile _Tp>
    {
      using type = const volatile __tuple_element_t<__i, _Tp>;
    };



  // Return the index of _Tp in _Types, if it occurs exactly once.
  // Otherwise, return sizeof...(_Types).
  template<typename _Tp, typename... _Types>
    constexpr size_t
    __find_uniq_type_in_pack()
    {
      constexpr size_t __sz = sizeof...(_Types);
      constexpr bool __found[__sz] = { __is_same(_Tp, _Types) ... };
      size_t __n = __sz;
      for (size_t __i = 0; __i < __sz; ++__i)
	{
	  if (__found[__i])
	    {
	      if (__n < __sz) // more than one _Tp found
		return __sz;
	      __n = __i;
	    }
	}
      return __n;
    }


// The standard says this macro and alias template should be in <tuple> but we
// define them here, to be available in <array>, <utility> and <ranges> too.
// _GLIBCXX_RESOLVE_LIB_DEFECTS
// 3378. tuple_size_v/tuple_element_t should be available when
//       tuple_size/tuple_element are

  template<size_t __i, typename _Tp>
    using tuple_element_t = typename tuple_element<__i, _Tp>::type;


  // Stores a tuple of indices.  Used by tuple and pair, and by bind() to
  // extract the elements in a tuple.
  template<size_t... _Indexes> struct _Index_tuple { };

  // Builds an _Index_tuple<0, 1, 2, ..., _Num-1>.
  template<size_t _Num>
    struct _Build_index_tuple
    {







      // For GCC and other compilers, use __integer_pack instead.
      using __type = _Index_tuple<__integer_pack(_Num)...>;

    };



  /// Class template integer_sequence
  template<typename _Tp, _Tp... _Idx>
    struct integer_sequence
    {



      typedef _Tp value_type;
      static constexpr size_t size() noexcept { return sizeof...(_Idx); }
    };

  /// Alias template make_integer_sequence
  template<typename _Tp, _Tp _Num>
    using make_integer_sequence



      = integer_sequence<_Tp, __integer_pack(_Num)...>;


  /// Alias template index_sequence
  template<size_t... _Idx>
    using index_sequence = integer_sequence<size_t, _Idx...>;

  /// Alias template make_index_sequence
  template<size_t _Num>
    using make_index_sequence = make_integer_sequence<size_t, _Num>;

  /// Alias template index_sequence_for
  template<typename... _Types>
    using index_sequence_for = make_index_sequence<sizeof...(_Types)>;




  struct in_place_t {
    explicit in_place_t() = default;
  };

  inline constexpr in_place_t in_place{};

  template<typename _Tp> struct in_place_type_t
  {
    explicit in_place_type_t() = default;
  };

  template<typename _Tp>
    inline constexpr in_place_type_t<_Tp> in_place_type{};

  template<size_t _Idx> struct in_place_index_t
  {
    explicit in_place_index_t() = default;
  };

  template<size_t _Idx>
    inline constexpr in_place_index_t<_Idx> in_place_index{};

  template<typename>
    inline constexpr bool __is_in_place_type_v = false;

  template<typename _Tp>
    inline constexpr bool __is_in_place_type_v<in_place_type_t<_Tp>> = true;

  template<typename _Tp>
    using __is_in_place_type = bool_constant<__is_in_place_type_v<_Tp>>;

  template<typename>
    inline constexpr bool __is_in_place_index_v = false;

  template<size_t _Nm>
    inline constexpr bool __is_in_place_index_v<in_place_index_t<_Nm>> = true;




  template<size_t _Np, typename... _Types>
    struct _Nth_type
    { using type = __type_pack_element<_Np, _Types...>; };









_GLIBCXX_END_NAMESPACE_VERSION
} // namespace







namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @addtogroup utilities
   *  @{
   */


  /// Tag type for piecewise construction of std::pair objects.
  struct piecewise_construct_t { explicit piecewise_construct_t() = default; };

  /// Tag for piecewise construction of std::pair objects.
  _GLIBCXX17_INLINE constexpr piecewise_construct_t piecewise_construct =
    piecewise_construct_t();

  /// @cond undocumented

  // Forward declarations.
  template<typename _T1, typename _T2>
    struct pair;

  template<typename...>
    class tuple;

  // Declarations of std::array and its std::get overloads, so that
  // std::tuple_cat can use them if <tuple> is included before <array>.
  // We also declare the other std::get overloads here so that they're
  // visible to the P2165R4 tuple-like constructors of pair and tuple.
  template<typename _Tp, size_t _Nm>
    struct array;

  template<size_t...>
    struct _Index_tuple;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(pair<_Tp1, _Tp2>& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(pair<_Tp1, _Tp2>&& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(const pair<_Tp1, _Tp2>& __in) noexcept;

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(const pair<_Tp1, _Tp2>&& __in) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr __tuple_element_t<__i, tuple<_Elements...>>&
    get(tuple<_Elements...>& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr const __tuple_element_t<__i, tuple<_Elements...>>&
    get(const tuple<_Elements...>& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr __tuple_element_t<__i, tuple<_Elements...>>&&
    get(tuple<_Elements...>&& __t) noexcept;

  template<size_t __i, typename... _Elements>
    constexpr const __tuple_element_t<__i, tuple<_Elements...>>&&
    get(const tuple<_Elements...>&& __t) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr _Tp&
    get(array<_Tp, _Nm>&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr _Tp&&
    get(array<_Tp, _Nm>&&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr const _Tp&
    get(const array<_Tp, _Nm>&) noexcept;

  template<size_t _Int, typename _Tp, size_t _Nm>
    constexpr const _Tp&&
    get(const array<_Tp, _Nm>&&) noexcept;


  // Concept utility functions, reused in conditionally-explicit
  // constructors.
  // See PR 70437, don't look at is_constructible or
  // is_convertible if the types are the same to
  // avoid querying those properties for incomplete types.
  template <bool, typename _T1, typename _T2>
    struct _PCC
    {
      template <typename _U1, typename _U2>
      static constexpr bool _ConstructiblePair()
      {
	return __and_<is_constructible<_T1, const _U1&>,
		      is_constructible<_T2, const _U2&>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyConvertiblePair()
      {
	return __and_<is_convertible<const _U1&, _T1>,
		      is_convertible<const _U2&, _T2>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _MoveConstructiblePair()
      {
	return __and_<is_constructible<_T1, _U1&&>,
		      is_constructible<_T2, _U2&&>>::value;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyMoveConvertiblePair()
      {
	return __and_<is_convertible<_U1&&, _T1>,
		      is_convertible<_U2&&, _T2>>::value;
      }
    };

  template <typename _T1, typename _T2>
    struct _PCC<false, _T1, _T2>
    {
      template <typename _U1, typename _U2>
      static constexpr bool _ConstructiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyConvertiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _MoveConstructiblePair()
      {
	return false;
      }

      template <typename _U1, typename _U2>
      static constexpr bool _ImplicitlyMoveConvertiblePair()
      {
	return false;
      }
    };




  template<typename _U1, typename _U2> class __pair_base
  {

    template<typename _T1, typename _T2> friend struct pair;
    __pair_base() = default;
    ~__pair_base() = default;
    __pair_base(const __pair_base&) = default;
    __pair_base& operator=(const __pair_base&) = delete;

  };

  /// @endcond

 /**
   *  @brief Struct holding two objects of arbitrary type.
   *
   *  @tparam _T1  Type of first object.
   *  @tparam _T2  Type of second object.
   *
   *  <https://gcc.gnu.org/onlinedocs/libstdc++/manual/utilities.html>
   *
   * @headerfile utility
   */
  template<typename _T1, typename _T2>
    struct pair
    : public __pair_base<_T1, _T2>
    {
      typedef _T1 first_type;    ///< The type of the `first` member
      typedef _T2 second_type;   ///< The type of the `second` member

      _T1 first;                 ///< The first member
      _T2 second;                ///< The second member


      constexpr pair(const pair&) = default;	///< Copy constructor
      constexpr pair(pair&&) = default;		///< Move constructor

      template<typename... _Args1, typename... _Args2>
	_GLIBCXX20_CONSTEXPR
	pair(piecewise_construct_t, tuple<_Args1...>, tuple<_Args2...>);

      /// Swap the first members and then the second members.
      _GLIBCXX20_CONSTEXPR void
      swap(pair& __p)
      noexcept(__and_<__is_nothrow_swappable<_T1>,
		      __is_nothrow_swappable<_T2>>::value)
      {
	using std::swap;
	swap(first, __p.first);
	swap(second, __p.second);
      }


    private:
      template<typename... _Args1, size_t... _Indexes1,
	       typename... _Args2, size_t... _Indexes2>
	_GLIBCXX20_CONSTEXPR
	pair(tuple<_Args1...>&, tuple<_Args2...>&,
	     _Index_tuple<_Indexes1...>, _Index_tuple<_Indexes2...>);
    public:

      // C++11/14/17 implementation using enable_if, partially constexpr.

      /// @cond undocumented
      // Error if construction from _U1 and _U2 would create a dangling ref.








      /// @endcond

      /** The default constructor creates @c first and @c second using their
       *  respective default constructors.  */
      template <typename _U1 = _T1,
                typename _U2 = _T2,
                typename enable_if<__and_<
                                     __is_implicitly_default_constructible<_U1>,
                                     __is_implicitly_default_constructible<_U2>>
                                   ::value, bool>::type = true>
      constexpr pair()
      : first(), second() { }

      template <typename _U1 = _T1,
                typename _U2 = _T2,
                typename enable_if<__and_<
                       is_default_constructible<_U1>,
                       is_default_constructible<_U2>,
                       __not_<
                         __and_<__is_implicitly_default_constructible<_U1>,
                                __is_implicitly_default_constructible<_U2>>>>
                                   ::value, bool>::type = false>
      explicit constexpr pair()
      : first(), second() { }

      // Shortcut for constraining the templates that don't take pairs.
      /// @cond undocumented
      using _PCCP = _PCC<true, _T1, _T2>;
      /// @endcond

      /// Construct from two const lvalues, allowing implicit conversions.
      template<typename _U1 = _T1, typename _U2=_T2, typename
	       enable_if<_PCCP::template
			   _ConstructiblePair<_U1, _U2>()
	                 && _PCCP::template
			   _ImplicitlyConvertiblePair<_U1, _U2>(),
                         bool>::type=true>
      constexpr pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }

      /// Construct from two const lvalues, disallowing implicit conversions.
       template<typename _U1 = _T1, typename _U2=_T2, typename
		enable_if<_PCCP::template
			    _ConstructiblePair<_U1, _U2>()
	                  && !_PCCP::template
			    _ImplicitlyConvertiblePair<_U1, _U2>(),
                         bool>::type=false>
      explicit constexpr pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }

      // Shortcut for constraining the templates that take pairs.
      /// @cond undocumented
      template <typename _U1, typename _U2>
        using _PCCFP = _PCC<!is_same<_T1, _U1>::value
			    || !is_same<_T2, _U2>::value,
			    _T1, _T2>;
      /// @endcond

      template<typename _U1, typename _U2, typename
	       enable_if<_PCCFP<_U1, _U2>::template
			   _ConstructiblePair<_U1, _U2>()
	                 && _PCCFP<_U1, _U2>::template
			   _ImplicitlyConvertiblePair<_U1, _U2>(),
			  bool>::type=true>
	constexpr pair(const pair<_U1, _U2>& __p)
	: first(__p.first), second(__p.second)
	{ __glibcxx_no_dangling_refs(const _U1&, const _U2&); }

      template<typename _U1, typename _U2, typename
	       enable_if<_PCCFP<_U1, _U2>::template
			   _ConstructiblePair<_U1, _U2>()
			 && !_PCCFP<_U1, _U2>::template
			   _ImplicitlyConvertiblePair<_U1, _U2>(),
                         bool>::type=false>
	explicit constexpr pair(const pair<_U1, _U2>& __p)
	: first(__p.first), second(__p.second)
	{ __glibcxx_no_dangling_refs(const _U1&, const _U2&); }










    private:
      /// @cond undocumented

      // A type which can be constructed from literal zero, but not nullptr
      struct __zero_as_null_pointer_constant
      {
	__zero_as_null_pointer_constant(int __zero_as_null_pointer_constant::*)
	{ }
	template<typename _Tp,
		 typename = __enable_if_t<is_null_pointer<_Tp>::value>>
	__zero_as_null_pointer_constant(_Tp) = delete;
      };
      /// @endcond
    public:

      // Deprecated extensions to DR 811.
      // These allow construction from an rvalue and a literal zero,
      // in cases where the standard says the zero should be deduced as int
      template<typename _U1,
	       __enable_if_t<__and_<__not_<is_reference<_U1>>,
				    is_pointer<_T2>,
				    is_constructible<_T1, _U1>,
				    __not_<is_constructible<_T1, const _U1&>>,
				    is_convertible<_U1, _T1>>::value,
			     bool> = true>
	_GLIBCXX_DEPRECATED_PAIR_CTOR
	constexpr
	pair(_U1&& __x, __zero_as_null_pointer_constant, ...)
	: first(std::forward<_U1>(__x)), second(nullptr)
	{ __glibcxx_no_dangling_refs(_U1&&, std::nullptr_t); }

      template<typename _U1,
	       __enable_if_t<__and_<__not_<is_reference<_U1>>,
				    is_pointer<_T2>,
				    is_constructible<_T1, _U1>,
				    __not_<is_constructible<_T1, const _U1&>>,
				    __not_<is_convertible<_U1, _T1>>>::value,
			     bool> = false>
	_GLIBCXX_DEPRECATED_PAIR_CTOR
	explicit constexpr
	pair(_U1&& __x, __zero_as_null_pointer_constant, ...)
	: first(std::forward<_U1>(__x)), second(nullptr)
	{ __glibcxx_no_dangling_refs(_U1&&, std::nullptr_t); }

      template<typename _U2,
	       __enable_if_t<__and_<is_pointer<_T1>,
				    __not_<is_reference<_U2>>,
				    is_constructible<_T2, _U2>,
				    __not_<is_constructible<_T2, const _U2&>>,
				    is_convertible<_U2, _T2>>::value,
			     bool> = true>
	_GLIBCXX_DEPRECATED_PAIR_CTOR
	constexpr
	pair(__zero_as_null_pointer_constant, _U2&& __y, ...)
	: first(nullptr), second(std::forward<_U2>(__y))
	{ __glibcxx_no_dangling_refs(std::nullptr_t, _U2&&); }

      template<typename _U2,
	       __enable_if_t<__and_<is_pointer<_T1>,
				    __not_<is_reference<_U2>>,
				    is_constructible<_T2, _U2>,
				    __not_<is_constructible<_T2, const _U2&>>,
				    __not_<is_convertible<_U2, _T2>>>::value,
			     bool> = false>
	_GLIBCXX_DEPRECATED_PAIR_CTOR
	explicit constexpr
	pair(__zero_as_null_pointer_constant, _U2&& __y, ...)
	: first(nullptr), second(std::forward<_U2>(__y))
	{ __glibcxx_no_dangling_refs(std::nullptr_t, _U2&&); }


      template<typename _U1, typename _U2, typename
	       enable_if<_PCCP::template
			   _MoveConstructiblePair<_U1, _U2>()
			  && _PCCP::template
			   _ImplicitlyMoveConvertiblePair<_U1, _U2>(),
                         bool>::type=true>
	constexpr pair(_U1&& __x, _U2&& __y)
	: first(std::forward<_U1>(__x)), second(std::forward<_U2>(__y))
	{ __glibcxx_no_dangling_refs(_U1&&, _U2&&); }

      template<typename _U1, typename _U2, typename
	       enable_if<_PCCP::template
			   _MoveConstructiblePair<_U1, _U2>()
			  && !_PCCP::template
			   _ImplicitlyMoveConvertiblePair<_U1, _U2>(),
                         bool>::type=false>
	explicit constexpr pair(_U1&& __x, _U2&& __y)
	: first(std::forward<_U1>(__x)), second(std::forward<_U2>(__y))
	{ __glibcxx_no_dangling_refs(_U1&&, _U2&&); }


      template<typename _U1, typename _U2, typename
	       enable_if<_PCCFP<_U1, _U2>::template
			   _MoveConstructiblePair<_U1, _U2>()
			  && _PCCFP<_U1, _U2>::template
			   _ImplicitlyMoveConvertiblePair<_U1, _U2>(),
                         bool>::type=true>
	constexpr pair(pair<_U1, _U2>&& __p)
	: first(std::forward<_U1>(__p.first)),
	  second(std::forward<_U2>(__p.second))
	{ __glibcxx_no_dangling_refs(_U1&&, _U2&&); }

      template<typename _U1, typename _U2, typename
	       enable_if<_PCCFP<_U1, _U2>::template
			   _MoveConstructiblePair<_U1, _U2>()
			  && !_PCCFP<_U1, _U2>::template
			   _ImplicitlyMoveConvertiblePair<_U1, _U2>(),
                         bool>::type=false>
	explicit constexpr pair(pair<_U1, _U2>&& __p)
	: first(std::forward<_U1>(__p.first)),
	  second(std::forward<_U2>(__p.second))
	{ __glibcxx_no_dangling_refs(_U1&&, _U2&&); }


      pair&
      operator=(__conditional_t<__and_<is_copy_assignable<_T1>,
				       is_copy_assignable<_T2>>::value,
				const pair&, const __nonesuch&> __p)
      {
	first = __p.first;
	second = __p.second;
	return *this;
      }

      pair&
      operator=(__conditional_t<__and_<is_move_assignable<_T1>,
				       is_move_assignable<_T2>>::value,
				pair&&, __nonesuch&&> __p)
      noexcept(__and_<is_nothrow_move_assignable<_T1>,
		      is_nothrow_move_assignable<_T2>>::value)
      {
	first = std::forward<first_type>(__p.first);
	second = std::forward<second_type>(__p.second);
	return *this;
      }

      template<typename _U1, typename _U2>
	typename enable_if<__and_<is_assignable<_T1&, const _U1&>,
				  is_assignable<_T2&, const _U2&>>::value,
			   pair&>::type
	operator=(const pair<_U1, _U2>& __p)
	{
	  first = __p.first;
	  second = __p.second;
	  return *this;
	}

      template<typename _U1, typename _U2>
	typename enable_if<__and_<is_assignable<_T1&, _U1&&>,
				  is_assignable<_T2&, _U2&&>>::value,
			   pair&>::type
	operator=(pair<_U1, _U2>&& __p)
	{
	  first = std::forward<_U1>(__p.first);
	  second = std::forward<_U2>(__p.second);
	  return *this;
	}

    };

  /// @relates pair @{


  template<typename _T1, typename _T2> pair(_T1, _T2) -> pair<_T1, _T2>;


  /// Two pairs of the same type are equal iff their members are equal.
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first == __y.first && __x.second == __y.second; }

  /** Defines a lexicographical order for pairs.
   *
   * For two pairs of the same type, `P` is ordered before `Q` if
   * `P.first` is less than `Q.first`, or if `P.first` and `Q.first`
   * are equivalent (neither is less than the other) and `P.second` is less
   * than `Q.second`.
  */
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first < __y.first
	     || (!(__y.first < __x.first) && __x.second < __y.second); }

  /// Uses @c operator== to find the result.
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x == __y); }

  /// Uses @c operator< to find the result.
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __y < __x; }

  /// Uses @c operator< to find the result.
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__y < __x); }

  /// Uses @c operator< to find the result.
  template<typename _T1, typename _T2>
    inline _GLIBCXX_CONSTEXPR bool
    operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x < __y); }



  /** Swap overload for pairs. Calls std::pair::swap().
   *
   * @note This std::swap overload is not declared in C++03 mode,
   * which has performance implications, e.g. see https://gcc.gnu.org/PR38466
  */
  template<typename _T1, typename _T2>
    _GLIBCXX20_CONSTEXPR inline

    // Constrained free swap overload, see p0185r1
    typename enable_if<__and_<__is_swappable<_T1>,
                              __is_swappable<_T2>>::value>::type



    swap(pair<_T1, _T2>& __x, pair<_T1, _T2>& __y)
    noexcept(noexcept(__x.swap(__y)))
    { __x.swap(__y); }



  template<typename _T1, typename _T2>
    typename enable_if<!__and_<__is_swappable<_T1>,
			       __is_swappable<_T2>>::value>::type
    swap(pair<_T1, _T2>&, pair<_T1, _T2>&) = delete;



  /// @} relates pair

  /**
   *  @brief A convenience wrapper for creating a pair from two objects.
   *  @param  __x  The first object.
   *  @param  __y  The second object.
   *  @return   A newly-constructed pair<> object of the appropriate type.
   *
   *  The C++98 standard says the objects are passed by reference-to-const,
   *
   *  Since C++11 they have been passed by forwarding reference and then
   *  forwarded to the new members of the pair. To create a pair with a
   *  member of reference type, pass a `reference_wrapper` to this function.
   */
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 181.  make_pair() unintended behavior

  // NB: DR 706.
  template<typename _T1, typename _T2>
    constexpr pair<typename __decay_and_strip<_T1>::__type,
                   typename __decay_and_strip<_T2>::__type>
    make_pair(_T1&& __x, _T2&& __y)
    {
      typedef typename __decay_and_strip<_T1>::__type __ds_type1;
      typedef typename __decay_and_strip<_T2>::__type __ds_type2;
      typedef pair<__ds_type1, __ds_type2> 	      __pair_type;
      return __pair_type(std::forward<_T1>(__x), std::forward<_T2>(__y));
    }







  /// @}


  // Various functions which give std::pair a tuple-like interface.

  /// @cond undocumented
  template<typename _T1, typename _T2>
    struct __is_tuple_like_impl<pair<_T1, _T2>> : true_type
    { };
  /// @endcond

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_size<pair<_Tp1, _Tp2>>
    : public integral_constant<size_t, 2> { };

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_element<0, pair<_Tp1, _Tp2>>
    { typedef _Tp1 type; };

  /// Partial specialization for std::pair
  template<class _Tp1, class _Tp2>
    struct tuple_element<1, pair<_Tp1, _Tp2>>
    { typedef _Tp2 type; };

  // Forward declare the partial specialization for std::tuple
  // to work around modules bug PR c++/113814.
  template<size_t __i, typename... _Types>
    struct tuple_element<__i, tuple<_Types...>>;


  template<typename _Tp1, typename _Tp2>
    inline constexpr size_t tuple_size_v<pair<_Tp1, _Tp2>> = 2;

  template<typename _Tp1, typename _Tp2>
    inline constexpr size_t tuple_size_v<const pair<_Tp1, _Tp2>> = 2;

  template<typename _Tp>
    inline constexpr bool __is_pair = false;

  template<typename _Tp, typename _Up>
    inline constexpr bool __is_pair<pair<_Tp, _Up>> = true;


  /// @cond undocumented
  template<size_t _Int>
    struct __pair_get;

  template<>
    struct __pair_get<0>
    {
      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp1&
	__get(pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.first; }

      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp1&&
	__move_get(pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<_Tp1>(__pair.first); }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp1&
	__const_get(const pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.first; }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp1&&
	__const_move_get(const pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<const _Tp1>(__pair.first); }
    };

  template<>
    struct __pair_get<1>
    {
      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp2&
	__get(pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.second; }

      template<typename _Tp1, typename _Tp2>
	static constexpr _Tp2&&
	__move_get(pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<_Tp2>(__pair.second); }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp2&
	__const_get(const pair<_Tp1, _Tp2>& __pair) noexcept
	{ return __pair.second; }

      template<typename _Tp1, typename _Tp2>
	static constexpr const _Tp2&&
	__const_move_get(const pair<_Tp1, _Tp2>&& __pair) noexcept
	{ return std::forward<const _Tp2>(__pair.second); }
    };
  /// @endcond

  /** @{
   * std::get overloads for accessing members of std::pair
   */

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(pair<_Tp1, _Tp2>& __in) noexcept
    { return __pair_get<_Int>::__get(__in); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(pair<_Tp1, _Tp2>&& __in) noexcept
    { return __pair_get<_Int>::__move_get(std::move(__in)); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&
    get(const pair<_Tp1, _Tp2>& __in) noexcept
    { return __pair_get<_Int>::__const_get(__in); }

  template<size_t _Int, class _Tp1, class _Tp2>
    constexpr const typename tuple_element<_Int, pair<_Tp1, _Tp2>>::type&&
    get(const pair<_Tp1, _Tp2>&& __in) noexcept
    { return __pair_get<_Int>::__const_move_get(std::move(__in)); }



  template <typename _Tp, typename _Up>
    constexpr _Tp&
    get(pair<_Tp, _Up>& __p) noexcept
    { return __p.first; }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&
    get(const pair<_Tp, _Up>& __p) noexcept
    { return __p.first; }

  template <typename _Tp, typename _Up>
    constexpr _Tp&&
    get(pair<_Tp, _Up>&& __p) noexcept
    { return std::move(__p.first); }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&&
    get(const pair<_Tp, _Up>&& __p) noexcept
    { return std::move(__p.first); }

  template <typename _Tp, typename _Up>
    constexpr _Tp&
    get(pair<_Up, _Tp>& __p) noexcept
    { return __p.second; }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&
    get(const pair<_Up, _Tp>& __p) noexcept
    { return __p.second; }

  template <typename _Tp, typename _Up>
    constexpr _Tp&&
    get(pair<_Up, _Tp>&& __p) noexcept
    { return std::move(__p.second); }

  template <typename _Tp, typename _Up>
    constexpr const _Tp&&
    get(const pair<_Up, _Tp>&& __p) noexcept
    { return std::move(__p.second); }




  /// @}


_GLIBCXX_END_NAMESPACE_VERSION
} // namespace std




// std::initializer_list support -*- C++ -*-

// Copyright (C) 2008-2024 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/** @file initializer_list
 *  This is a Standard C++ Library header.
 */



       







namespace std _GLIBCXX_VISIBILITY(default)
{
  /// initializer_list
  template<class _E>
    class initializer_list
    {
    public:
      typedef _E 		value_type;
      typedef const _E& 	reference;
      typedef const _E& 	const_reference;
      typedef size_t 		size_type;
      typedef const _E* 	iterator;
      typedef const _E* 	const_iterator;

    private:
      iterator			_M_array;
      size_type			_M_len;

      // The compiler can call a private constructor.
      constexpr initializer_list(const_iterator __a, size_type __l)
      : _M_array(__a), _M_len(__l) { }

    public:
      constexpr initializer_list() noexcept
      : _M_array(0), _M_len(0) { }

      // Number of elements.
      constexpr size_type
      size() const noexcept { return _M_len; }

      // First element.
      constexpr const_iterator
      begin() const noexcept { return _M_array; }

      // One past the last element.
      constexpr const_iterator
      end() const noexcept { return begin() + size(); }
    };

  /**
   *  @brief  Return an iterator pointing to the first element of
   *          the initializer_list.
   *  @param  __ils  Initializer list.
   *  @relates initializer_list
   */
  template<class _Tp>
    constexpr const _Tp*
    begin(initializer_list<_Tp> __ils) noexcept
    { return __ils.begin(); }

  /**
   *  @brief  Return an iterator pointing to one past the last element
   *          of the initializer_list.
   *  @param  __ils  Initializer list.
   *  @relates initializer_list
   */
  template<class _Tp>
    constexpr const _Tp*
    end(initializer_list<_Tp> __ils) noexcept
    { return __ils.end(); }
}











// Copyright (C) 2023-2024 Free Software Foundation, Inc.

// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// DO NOT EDIT THIS FILE   (version.h)
//
// It has been AutoGen-ed
// From the definitions    version.def
// and the template file   version.tpl

/** @file bits/version.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{version}
 */

// Usage guide:
//
// In your usual header, do something like:
//
//
// This will generate the FTMs you named, and let you use them in your code as
// if it was user code.  All macros are also exposed under __glibcxx_NAME even
// if unwanted, to permit bits and other FTMs to depend on them for condtional
// computation without exposing extra FTMs to user code.

       































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION


  /// Assign @p __new_val to @p __obj and return its previous value.
  template <typename _Tp, typename _Up = _Tp>
    _GLIBCXX20_CONSTEXPR
    inline _Tp
    exchange(_Tp& __obj, _Up&& __new_val)
    noexcept(__and_<is_nothrow_move_constructible<_Tp>,
		    is_nothrow_assignable<_Tp&, _Up>>::value)
    { return std::__exchange(__obj, std::forward<_Up>(__new_val)); }



  template<typename _Tp>
    [[nodiscard]]
    constexpr add_const_t<_Tp>&
    as_const(_Tp& __t) noexcept
    { return __t; }

  template<typename _Tp>
    void as_const(const _Tp&&) = delete;





_GLIBCXX_END_NAMESPACE_VERSION
} // namespace




namespace ascip_details {

template<typename type, template<typename...>class tmpl> constexpr bool is_specialization_of = false;
template<template<typename...>class type, typename... args> constexpr bool is_specialization_of<type<args...>, type> = true;

template<typename,auto...> struct seq_type {};
template<typename t> struct type_holder { using type = t; t operator+() const ; };
template<typename t> constexpr auto type_c = type_holder<t>{};
template<typename t> constexpr auto type_dc = type_holder<std::decay_t<t>>{};
template<typename l, typename r> constexpr bool operator==(type_holder<l>, type_holder<r>) { return false; }
template<typename t> constexpr bool operator==(type_holder<t>, type_holder<t>) { return true; }

template<typename type, auto ind> struct tuple_value {
	type value; //NOTE: we cannot downcast for some reason in get method later, so we need in the fucky g methods
	template<auto i> constexpr type& g() requires (i==ind) { return value; }
	template<auto i> constexpr const type& g() const requires (i==ind) { return value; }
};
template<typename... types> struct tuple {
	consteval static auto mk_storage_type() {
		return []<typename fucky_clang,auto... inds>(seq_type<fucky_clang,inds...>){
			struct storage : tuple_value<types, inds>... { using tuple_value<types, inds>::g...; };
			return type_holder<storage>{};
		}(

			seq_type<unsigned, __integer_pack(sizeof...(types))...>{}



		);
	}

	using storage_type = decltype(+mk_storage_type());
	storage_type storage;

	constexpr tuple() =default ;
	constexpr tuple(tuple&& other) : storage(std::move(other.storage)) {}
	constexpr tuple(const tuple& other) : storage(other.storage) {}
	constexpr explicit tuple(auto&&... args) : storage{std::forward<decltype(args)>(args)...} {}

	template<auto ind> constexpr friend auto& get(tuple& t) { return t.storage.template g<ind>(); }
	template<auto ind> constexpr friend const auto& get(const tuple& t) { return t.storage.template g<ind>(); }
	friend constexpr auto size(const tuple&) { return sizeof...(types); }
};

template<typename... types> tuple(types&&...) -> tuple<std::decay_t<types>...>;

} // namespace ascip_details
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename type> concept vector = requires(const type& v){ v.size(); v[0]; v.back(); } && requires(type& v){ v.pop_back(); };
template<typename type> concept string = requires(type& s){ s += typename type::value_type{}; } && vector<type>;
template<typename type> concept string_view = requires(const type& s){ s.size(); s[0]; s.back(); };
template<typename type> concept empbackable = requires(type& r){ emplace_back(r); } || requires(type& r){ r.emplace_back(); };

} // namespace ascip_details

namespace ascip_details {

struct type_any_eq_allow {
	template<typename type> constexpr auto& operator =(const type&){ return *this; }
	template<typename type> constexpr auto& operator+=(const type&){ return *this; }
	template<typename type> constexpr auto& operator-=(const type&){ return *this; }
	template<typename type> constexpr auto& operator/=(const type&){ return *this; }
	template<typename type> constexpr auto& operator*=(const type&){ return *this; }
};

constexpr auto& pop_back(type_any_eq_allow& v){ return v; };
constexpr auto& emplace_back(type_any_eq_allow& v){ return v; };
constexpr void pop(auto& r) requires requires{ pop_back(r); } { pop_back(r); }
constexpr void pop(auto& r) requires requires{ r.pop_back(); } { r.pop_back(); }
constexpr void pop(auto& r) { }
template<typename type> constexpr auto& empback(type& r) {
	if constexpr(requires{ emplace_back(r); }) return emplace_back(r);
	else if constexpr(requires{ r.emplace_back(); }) return r.emplace_back();
	else if constexpr(string<type>) { r += typename decltype(auto(r))::value_type{}; return r.back(); }
	else return r;
}
constexpr void eq( auto& to, const auto& from) { empback(to) = from; }
constexpr void eq(bool ok, auto& to, const auto& from) {
  if constexpr (std::is_integral_v<std::decay_t<decltype(to)>>) to = from*ok + to*!ok;
	else (void)(ok && (eq(to, from),1));
}

template<auto cind> constexpr auto& variant_result(auto& result) {
	if constexpr (cind<0) return result;
	else if constexpr (requires{create<0>(result);}) return create<cind>(result);
	else if constexpr (requires{emplace<0>(result);}) return emplace<cind>(result);
	else if constexpr (requires{result.template emplace<0>();}) return result.template emplace<cind>();
	else return result;
}

}

namespace ascip_details {
struct skip_parser_tag{};
struct err_handler_tag{};
struct new_line_count_tag {}; //TODO: fix new line counter or remove it
//NOTE: store in context, copy in prev context if parser not failed
//      or store in parse method variable and put a pointer to the context, make resetter to restore value on fail

template<typename t, typename value_t> struct context_frame {
  constexpr static auto tag = type_holder<t>{};
  value_t value;
};
constexpr auto make_default_context() {
  return tuple{context_frame<new_line_count_tag, int>{1}};
}
constexpr static auto make_default_context(auto err_handler){ return make_ctx<err_handler_tag>(err_handler, make_default_context()); }

constexpr auto make_test_ctx() { return make_default_context(); }
constexpr static auto make_test_ctx(auto err_handler){ return make_default_context(std::move(err_handler)); }

template<template<typename...>class tuple, typename... types> constexpr decltype(auto) repack(tuple<types...>&& tup, auto&& fnc) {
  return [&]<auto... inds>(std::index_sequence<inds...>){
    return fnc(std::move(get<inds>(tup))...);
  }(std::make_index_sequence<sizeof...(types)>{});
}
template<typename tag, typename value, typename... frames> constexpr auto make_ctx(value&& val, tuple<frames...> prev_ctx) {
  using adding_t = context_frame<tag, std::decay_t<value>>;
  return repack(std::move(prev_ctx), [&](auto&&... prev) {
    return tuple<adding_t, frames...>{adding_t{std::forward<decltype(val)>(val)}, std::move(prev)...};
  });
}
template<typename tag, typename value> constexpr auto make_ctx(value&& val) {
  return make_ctx<tag>(std::forward<decltype(val)>(val), make_default_context());
}

constexpr struct ctx_not_found_type {} ctx_not_found ;
template<typename tag, auto cur, auto cur_ind, auto ind, auto sz> constexpr auto& _search_in_ctx(auto& ctx) {
  if constexpr (sz<=cur) return ctx_not_found;
  else {
    if constexpr (decltype(auto(get<cur>(ctx)))::tag != type_c<tag>) return _search_in_ctx<tag, cur+1, cur_ind, ind, sz>(ctx);
    else {
      if constexpr (cur_ind==ind) return get<cur>(ctx).value;
      else return _search_in_ctx<tag, cur+1, cur_ind+1, ind, sz>(ctx);
    }
  }
}
template<typename tag, auto ind=0, typename... frames> constexpr auto& search_in_ctx(tuple<frames...>& ctx) {
  return _search_in_ctx<tag, 0, 0, ind, sizeof...(frames)>(ctx);
}
template<typename tag, auto ind=0, typename... frames> constexpr const auto& search_in_ctx(const tuple<frames...>& ctx) {
  return _search_in_ctx<tag, 0, 0, ind, sizeof...(frames)>(const_cast<tuple<frames...>&>(ctx));
}
/*
template<typename tag, auto ind=0> constexpr auto& search_in_ctx(auto&& ctx) { // for tests
  return search_in_ctx<tag,ind>(ctx);
}
*/

template<typename tag, typename... frames> constexpr bool exists_in_ctx(const tuple<frames...>&) {
  return ((frames::tag == type_c<tag>) + ... );
}


template<typename tag, auto cur, auto cur_ind, auto ind, auto sz> constexpr auto _crop_ctx(auto ctx, auto&&... tail) {
  static_assert( cur <= sz, "required frame not found");
  if constexpr (decltype(ctx)::tag != type_c<tag>) return _crop_ctx<tag, cur+1, cur_ind, ind, sz>(std::move(tail)...);
  else {
    if constexpr (cur_ind==ind) return tuple<decltype(ctx), std::decay_t<decltype(tail)>...>{std::move(ctx), std::move(tail)...};
    else return _crop_ctx<tag, cur+1, cur_ind+1, ind, sz>(std::move(tail)...);
  }
}
template<auto ind, typename tag, typename... frames> constexpr auto crop_ctx(tuple<frames...> ctx) {
  return [&]<auto... inds>(std::index_sequence<inds...>) {
    return _crop_ctx<tag, 0, 0, ind, sizeof...(frames)>(get<inds>(ctx)...);
  }(std::make_index_sequence<sizeof...(frames)>{});
}

constexpr void count_new_line(bool result_ok, auto& ctx, auto sym, auto& r) {
	constexpr bool need_count_new_lines =
		   exists_in_ctx<new_line_count_tag>(decltype(auto(ctx)){})
		&& type_dc<decltype(r)> != type_c<type_any_eq_allow>
	;
	if constexpr (need_count_new_lines)
		search_in_ctx<new_line_count_tag>(ctx) += (sym == '\n') * result_ok;
}

constexpr auto new_line_count(auto& ctx) {
  auto& result = search_in_ctx<new_line_count_tag>(ctx);
  if constexpr (requires{result == 0;}) return result;
  else return 0;
}

} // namespace ascip_details

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

template<ascip_details::string_view type>
constexpr static auto make_source(type&& src) {
	struct strsrc {
		type src;
		unsigned ind = 0;
		constexpr auto operator()(){ return src[ind++]; }
		constexpr explicit operator bool() const { return ind < src.size(); }
		constexpr auto& operator += (int v) { ind+=v; return *this; }
	} ret{ std::forward<decltype(src)>(src) };
	return ret;
}

constexpr static auto make_source(std::integral auto sym) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(sym) val; bool where_is_more=true;
		constexpr auto operator()(){ where_is_more=false; return val; }
		constexpr explicit operator bool() const { return where_is_more; }
		constexpr auto& operator += (int v) { (void)( v==1 || (throw 1,1) ); where_is_more=false; return *this; }
	} ret{ sym };
	return ret;
}

constexpr auto strlen(const auto* vec){ unsigned ret = 0; while(vec[ret])++ret; return ++ret; }
constexpr static auto make_source(const auto* vec) {
	struct gcc_requires_name_for_tu_bug { // https://stackoverflow.com/a/66966679/24870413
		decltype(vec) val;
		unsigned sz;
		unsigned ind = 0;
		constexpr auto operator()(){ return val[ind++]; }
		constexpr explicit operator bool() const { return ind < sz-1; }
		constexpr auto& operator += (const int v) { ind+=v; return *this; }
	} ret{ vec, strlen(vec) };
	return ret;
}

constexpr auto pos(const auto& src) {
	//TODO: remove this function and use information from context
	if constexpr ( requires{ position(src); } ) return position(src);
	else return src.ind;
}

} // namespace ascip_details

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

template<typename char_type, auto str_size>
struct string_literal {
	constexpr string_literal(const char_type (&str)[str_size]) {
		for(auto i=0;i<str_size;++i) value[i] = str[i];
	}

	char_type value[str_size];

	constexpr auto size() const { static_assert( 1 <= str_size ); return str_size-1; }
	constexpr auto operator[](auto i) const { return value[i]; }

	consteval bool contains(char_type s) const {
		for (auto i=0;i<str_size;++i) if (value[i]==s) return true;
		return false;
	}

	constexpr void for_each(auto&& fnc) const {
		[&]<auto... i>(std::index_sequence<i...>){
			(fnc(value[i]), ...);
		}(std::make_index_sequence<str_size>{});
	}

	template<auto sz>
	constexpr bool operator==(const char_type(&r)[sz]) const {
		if constexpr (sz != str_size) return false;
		for(auto i=0;i<str_size;++i) if(value[i]!=r[i]) return false;
		return true;
	}
	template<typename char_t> requires (std::is_pointer_v<char_t>)
	constexpr bool operator==(char_t r) const {
		for(auto i=0;i<str_size;++i) {
			if(value[i]!=r[i]) return false;
		}
		return true;
	}
};

template<string_literal str> struct test_tmpl {
	constexpr bool is_eq(const char* v) const { return str == v; }
	constexpr auto size() const { return str.size(); }
};

constexpr void test_static_string() {
	static_assert( string_literal("cstr")[0] == 'c' );
	static_assert( string_literal("cstr")[4] == 0x00 );
	static_assert( string_literal("cstr").value[4] == 0x00 );
	static_assert( string_literal("cstr") == "cstr" );
	static_assert( !(string_literal("cstr") == "cstr\0ups") );

	static_assert( string_literal("cstr").size() == 4 );

	static_assert( test_tmpl<"test">{}.is_eq("test") );
	static_assert( !test_tmpl<"test">{}.is_eq("test_ups") );
	static_assert( test_tmpl<"test">{}.is_eq("test\0ups") );
	static_assert( test_tmpl<"test">{}.size() == 4 );
	static_assert( test_tmpl<"is">{}.size() == 2 );

	static_assert(  string_literal("abc").contains('b') );
	static_assert( !string_literal("abc").contains('d') );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


namespace ascip_details {
template<auto... inds, template<typename...>class wrapper, typename... parsers>
constexpr static bool exists_in_get(const wrapper<parsers...>* seq, const auto& checker, const auto& stop) {
	if constexpr (sizeof...(inds) == sizeof...(parsers)) return false;
	else return
		(!stop((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		&& (
		      checker((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr)
		   || exists_in((std::decay_t<decltype(get<sizeof...(inds)>(*seq))>*)nullptr, checker, stop)
		   )
		)
		|| exists_in_get<inds..., sizeof...(inds)>(seq, checker, stop)
		;
}
template<template<typename>class wrapper, typename parser>
constexpr static bool exists_in_derived(const wrapper<parser>* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	return exists_in(static_cast<const parser*>(src), checker, stop);
}

constexpr static bool exists_in(auto&& src, const auto& checker, const auto& stop) {
	return exists_in(&src, checker, stop);
}

constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires (!requires{typename std::decay_t<decltype(*src)>::type_in_base;}){
	return false;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) {
	if(stop(src)) return false;
	if(checker(src)) return true;
	if constexpr (std::is_same_v<typename std::decay_t<decltype(*src)>::type_in_base, std::decay_t<decltype(*src)>>)
		return checker(src);
	else return exists_in_derived(src, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->p; } {
	using type = std::decay_t<decltype(src->p)>;
	return stop(src) ? false : checker(src) || exists_in((type*)nullptr, checker, stop);
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->lp; } {
	using ltype = std::decay_t<decltype(src->lp)>;
	using rtype = std::decay_t<decltype(src->rp)>;
	if(stop(src)) return false;
	const bool lstop = stop((ltype*)nullptr);
	const bool rstop = stop((rtype*)nullptr);
	return checker(src)
		|| (!lstop&&checker((ltype*)nullptr))
		|| (!rstop&&checker((rtype*)nullptr))
		|| (!lstop&&exists_in((ltype*)nullptr, checker, stop))
		|| (!rstop&&exists_in((rtype*)nullptr, checker, stop))
		;
}
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->seq; } {
	using seq_t = decltype(src->seq);
	if(stop(src)) return false;
	return checker(src) || exists_in_get((seq_t*)nullptr, checker, stop); }
constexpr static bool exists_in(auto* src, const auto& checker, const auto& stop) requires requires{ src->s; src->b; } {
	//NOTE: it's for injected_parser, but without forward declaration
	if(stop(src)) return false;
	const auto* ptr = static_cast<const std::decay_t<decltype(src->b)>*>(nullptr);
	if(checker(ptr)) return true;
	return exists_in(ptr, checker, stop) ; }

template<typename p, template<auto>class t=p::template tmpl>
constexpr static bool test_exists_in() {
	auto checker = [](const auto* s){ return std::is_same_v<std::decay_t<decltype(*s)>, std::decay_t<decltype(t<'a'>::char_)>>; };
	auto pass = [](const auto* s){ return false; };
	auto stop = [](const auto* s){ return true; };
	static_assert(  exists_in(t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_, checker, stop) );
	static_assert(  exists_in(skip(t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(skip(t<'a'>::char_), checker, stop) );
	static_assert( !exists_in(skip(t<'b'>::char_), checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ | t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ | t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ | t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'b'>::char_ - t<'a'>::char_, checker, pass) );
	static_assert( !exists_in(t<'b'>::char_ - t<'a'>::char_, checker, stop) );
	static_assert( !exists_in(t<'b'>::char_ - t<'x'>::char_, checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ >> (t<'b'>::char_ - t<'a'>::char_), checker, pass) );
	static_assert(  exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, pass) );
	static_assert( !exists_in(t<'c'>::char_ - (t<'b'>::char_ >> t<'a'>::char_), checker, stop) );
	static_assert(  exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, pass) );
	static_assert( !exists_in(skip(t<'c'>::char_ >> t<'b'>::char_ >> t<'a'>::char_++), checker, stop) );
	return true;
}

} // namespace ascip_details
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_reflection {
/* Copyright (C) 2024 Hudyaev Alexey
 * SPDX-License-Identifier:    MIT
 */

template<typename t>
struct _decay {
	template<typename type> static const type& lref() ;
	template<typename u> struct _t{ using type = u; };
	template<typename u> static _t<u> impl(const u&) ;
	template<typename u, unsigned int sz> static _t<u*> impl(const u(&)[sz]) ;
	template<typename r, typename... p> static _t<r(*)(p...)> impl(r(&)(p...)) ;
	using type = typename decltype(impl(lref<t>()))::type;
};

template<typename t> using decay_t = typename _decay<t>::type;

template<typename t, t v>
struct integral_constant {
	static constexpr t value = v;
	using value_type = t;
	using type = integral_constant;
	constexpr operator value_type() const noexcept { return value; }
	constexpr value_type operator()() const noexcept { return value; }
};
struct true_type : integral_constant<bool, true> {}; 
struct false_type : integral_constant<bool, false> {}; 
template<typename t> struct is_rvalue_reference : false_type {};
template<typename t> struct is_rvalue_reference<t&&> : true_type {};
template<typename t> struct is_lvalue_reference : false_type {};
template<typename t> struct is_lvalue_reference<t&> : true_type {};
template<typename t> struct is_const : false_type {};
template<typename t> struct is_const<const t> : true_type {};
template<typename t> inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<t>::value;
template<typename t> inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<t>::value;
template<typename t> inline constexpr bool is_const_v = is_const<t>::value;

template<class t> struct remove_reference { typedef t type; };
template<class t> struct remove_reference<t&> { typedef t type; };
template<class t> struct remove_reference<t&&> { typedef t type; };
template< class t > using remove_reference_t = typename remove_reference<t>::type;

template<class t, class u>
constexpr auto&& forward_like(u&& x) noexcept
{
	constexpr bool is_adding_const = is_const_v<remove_reference_t<t>>;
	if constexpr (is_rvalue_reference_v<t&&>) {
		if constexpr (is_adding_const) return static_cast<const decay_t<u>&&>(x);
		else return static_cast<decay_t<u>&&>(x);
	}
	else {
		if constexpr (is_adding_const) return static_cast<const u&>(x);
		else return static_cast<u&>(x);
	}
}

/* Copyright (C) 2024 Hudyaev Alexey
 * SPDX-License-Identifier:    MIT
 */

constexpr struct {
	template<typename t> operator t() const ;
} any ;

template<typename type, auto... args> constexpr auto size_impl() {
	if constexpr ( requires{ type{ args..., any }; })
		return size_impl<type, args..., any>();
	else return sizeof...(args);
}

template<typename type> constexpr auto size_impl_dispatcher() {
	if constexpr (requires{type::struct_fields_count();}) return type::struct_fields_count();
	else return size_impl<type>();
}

template<typename t> constexpr auto size = size_impl_dispatcher<t>();

template<auto ind,auto cur=0> constexpr auto&& nth(auto& first, auto&... args) { if constexpr (cur==ind) return first; else return nth<ind,cur+1>(args...); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 1) { auto&& [f1]=r; return (forward_like<decltype(r)>)(nth<ind>(f1)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 2) { auto&&[f1,f2]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 3) { auto&&[f1,f2,f3]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 4) { auto&&[f1,f2,f3,f4]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 5) { auto&&[f1,f2,f3,f4,f5]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 6) { auto&&[f1,f2,f3,f4,f5,f6]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 7) { auto&&[f1,f2,f3,f4,f5,f6,f7]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 8) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 9) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 10) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 11) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 12) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 13) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 14) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 15) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 16) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 17) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 18) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 19) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 20) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 21) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 22) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 23) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 24) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 25) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 26) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 27) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 28) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 29) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 30) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 31) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 32) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 33) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 34) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 35) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 36) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 37) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 38) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 39) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 40) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 41) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 42) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 43) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 44) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 45) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 46) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 47) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 48) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 49) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 50) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 51) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 52) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 53) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 54) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 55) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 56) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 57) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 58) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 59) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 60) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 61) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 62) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 63) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 64) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 65) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 66) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 67) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 68) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 69) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 70) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 71) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 72) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 73) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 74) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 75) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 76) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 77) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 78) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 79) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 80) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 81) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 82) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 83) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 84) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 85) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 86) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 87) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 88) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 89) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 90) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 91) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 92) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 93) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 94) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 95) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 96) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 97) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 98) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> == 99) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==100) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==101) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==102) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==103) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==104) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==105) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==106) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==107) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==108) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==109) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==110) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==111) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==112) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==113) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==114) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==115) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==116) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==117) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==118) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==119) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==120) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==121) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==122) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==123) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==124) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==125) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==126) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==127) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127)); }
template<auto ind> constexpr auto&& get(auto&& r) requires (size<decay_t<decltype(r)>> ==128) { auto&&[f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128]=r; return (forward_like<decltype(r)>)(nth<ind>(f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,f33,f34,f35,f36,f37,f38,f39,f40,f41,f42,f43,f44,f45,f46,f47,f48,f49,f50,f51,f52,f53,f54,f55,f56,f57,f58,f59,f60,f61,f62,f63,f64,f65,f66,f67,f68,f69,f70,f71,f72,f73,f74,f75,f76,f77,f78,f79,f80,f81,f82,f83,f84,f85,f86,f87,f88,f89,f90,f91,f92,f93,f94,f95,f96,f97,f98,f99,f100,f101,f102,f103,f104,f105,f106,f107,f108,f109,f110,f111,f112,f113,f114,f115,f116,f117,f118,f119,f120,f121,f122,f123,f124,f125,f126,f127,f128)); }
constexpr void test () {
	struct tf1{ char f1; };
	struct tf2{ char f1, f2; };
	static_assert( []{ tf1 t{ 'a' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<0>(t); }() == 'a' );
	static_assert( []{ tf2 t{ 'a', 'b' }; return get<1>(t); }() == 'b' );
}
} // namespace ascip_reflection
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details {

template<typename type> struct forwarder{ type& o; constexpr forwarder(type& o) : o(o) {} };
template<typename type> forwarder(type&) -> forwarder<type>;
constexpr auto fwd(auto& o) { return forwarder( o ); }

}
       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

template<typename mutator>
constexpr static auto transform_apply(auto&& src, auto& ctx) {
	if constexpr (requires{ mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx ); }) {
		return mutator::template apply<std::decay_t<decltype(src)>>( std::forward<decltype(src)>(src), ctx );
	}
	else return src;
}

template<typename mutator, template<typename...>class result_t, auto... ind, template<typename...>class i_tuple, typename... tail>
constexpr static auto transform_apply_to_each(i_tuple<tail...>&& src, auto& ctx, auto&&... args) {
	if constexpr (sizeof...(ind) == sizeof...(tail))
		return result_t<
			std::decay_t<decltype(args)>...,
			std::decay_t<decltype(transform<mutator>(std::move(get<ind>(src)), ctx))>...>
			(
			 std::forward<decltype(args)>(args)...,
			 transform<mutator>(std::move(get<ind>(src)), ctx)...
			);
	else return  transform_apply_to_each<mutator, result_t, ind..., sizeof...(ind)>(std::move(src), ctx, std::forward<decltype(args)>(args)...);
}

template<typename mutator>
constexpr static auto transform(auto&& src) {
	auto ctx = mutator::create_ctx();
	return transform<mutator>(std::forward<decltype(src)>(src), ctx);
}

template<typename mutator>
constexpr static auto transform(auto&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	if constexpr(requires{ transform_special<mutator>(std::move(src), ctx); })
		return transform_special<mutator>(std::move(src), nctx);
	else return transform_apply<mutator>(std::forward<decltype(src)>(src), nctx);
}
template<typename mutator, template<typename>class wrapper, typename inner>
constexpr static auto transform(wrapper<inner>&& src, auto& ctx) requires (requires{ src.p; } && !requires{transform_special<mutator>(std::move(src), ctx);}){
	auto nctx = mutator::create_ctx(src, ctx);
	auto mp = transform<mutator>(std::move(src.p), nctx);
	if constexpr(requires{ wrapper{{}, std::move(mp)}; })
		return transform_apply<mutator>(wrapper{{}, std::move(mp)}, nctx);
	else
		return transform_apply<mutator>(wrapper{std::move(mp)}, nctx);
}
template<typename mutator, template<typename...>class semact_wrapper, typename parser, typename act_t, typename... tags>
constexpr static auto transform(semact_wrapper<parser, act_t, tags...>&& src, auto& ctx) requires requires{ src.act; src.p; } {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( semact_wrapper<std::decay_t<decltype(np)>, std::decay_t<decltype(src.act)>, tags...>{ {}, std::move(src.act), std::move(np) }, nctx );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

constexpr auto inject_skipping(auto&& to, auto&& what) ;

constexpr auto parse(auto&& parser, auto src) {
	type_any_eq_allow r;
	return parse(std::forward<decltype(parser)>(parser), src, r);
}

constexpr auto parse(auto&& parser, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	return parser.parse(make_default_context(), src, result);
}

constexpr auto parse(auto&& parser, const auto& skip, auto src, auto& result) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context() );
	return inject_skipping(auto(parser), std::forward<decltype(skip)>(skip)).parse(ctx, src, result);
}

constexpr auto parse(auto&& parser, auto&& skip, auto src, auto& result, const auto& err) {
	using parser_type = std::decay_t<decltype(parser)>;
	auto ctx = make_ctx<skip_parser_tag>( skip, make_default_context(&err) );
	return inject_skipping(auto(parser), std::move(skip)).parse(ctx, src, result);
}

constexpr auto parse_with_ctx(const auto& ctx, auto&& parser, auto src, auto& result) {
	auto err = search_in_ctx<err_handler_tag>(ctx);
	auto skip = search_in_ctx<skip_parser_tag>(ctx);
	constexpr bool skip_found = !std::is_same_v<ctx_not_found_type, decltype(skip)>;
	constexpr bool err_found = !std::is_same_v<ctx_not_found_type, decltype(err)>;
	if constexpr (!skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), src, result);
	else if constexpr (skip_found && !err_found) return parse(std::forward<decltype(parser)>(parser), skip, src, result);
	else return parse(std::forward<decltype(parser)>(parser), skip, src, result, err);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details {

template<typename char_type, auto sz>
constexpr auto& print_to(auto& os, const ascip_details::string_literal<char_type, sz>& what) { return os << what.value; }
constexpr auto& print_to(auto& os, const auto& what) { return os << what; }
constexpr void write_out_error_msg(
		auto& os,
		auto fn,
		auto msg,
		auto expt,
		auto src,
		auto ln
		) {
	print_to(os, msg) << '\n';
	print_to(os, fn) << ':' << ln << ' ';
	auto shift = 0;
	do{ ++shift; src += -2; } while(src()!='\n');
	os << '\n';
	while(src) {
		auto cur = src();
		if(cur == '\n') break;
		os << cur;
	}
	os << '\n';
	for(auto i=0;i<shift;++i) os << '-';
	os << "^\n";
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

struct adl_tag {};
struct seq_tag {};
using parse_result = decltype(-1);

namespace prs { template<typename parser> struct base_parser; }

template<typename type> concept parser = std::is_base_of_v<prs::base_parser<std::decay_t<type>>, std::decay_t<type>> ;
template<typename type> concept nonparser = !parser<type>;

} // namespace ascip_details

namespace ascip_details::prs {

template<typename parser> struct base_parser : adl_tag {
	using type_in_base = parser;

	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert( requires(const parser& p){ p.p; }, "child parser should define own parse method or have p field" );
		return static_cast<const parser&>(*this).p.parse(static_cast<decltype(ctx)&&>(ctx), std::move(src), result);
	}

	//NOTE: to have access to child classes we must to implement body after the classes will be defined
	constexpr auto operator()(auto act) const ;
};

} // namespace ascip_details::prs

namespace ascip_details {
} // namespace ascip_details



namespace ascip_details::prs {

struct nop_parser : base_parser<nop_parser> {
	constexpr parse_result parse(auto&&, const auto&, auto&) const {
		return 0;
	}
};

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {
template <auto sym> struct char_parser : base_parser<char_parser<sym>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const bool ok = src() == sym;
		eq(ok, result, sym);
		count_new_line(ok, ctx, sym, result);
		return -1 + 2 * ok;
	}

	constexpr bool test() const {
		char r{};
		parse(make_test_ctx(), make_source(sym), r) == 1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym + 1), r) == -1 || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(sym - 1), r) == -1 || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym), r), r) == sym || (throw __LINE__, 1);

		r = sym + 1;
		(parse(make_test_ctx(), make_source(sym - 1), r), r) == sym + 1 || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		if constexpr (sym == '\n') {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);
		}
		else {
			parse(ctx, make_source(' '), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
			parse(ctx, make_source('\n'), r);
			search_in_ctx<ascip_details::new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		}

		return true;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {
template <ascip_details::string_literal val>
struct literal_parser : base_parser<literal_parser<val>> {
  constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
    // TODO: what if the literal contains \n - it will be not counted if literal matches
    // TODO: faster? add [] operator in src for direct access (operator[](auto i){return val[ind+i];})
    auto i = -1, r = 0;
    {
      auto tsrc = src;
      tsrc += val.size() - 1;
      if (!tsrc)
        return -1;
    }
    while (++i < val.size()) r += (src() == val[i]);
    if constexpr (val.contains('\n')) {
      const bool ok = ((r + 1) * (r == val.size()) - 1) > 0;
      val.for_each([&](auto s) { count_new_line(ok, ctx, s, result); });
    }
    return ((r + 1) * (r == val.size())) - 1;
  }
};

template <ascip_details::string_literal v>
constexpr static auto lit = literal_parser<v>{};
constexpr bool test_literal_parser() {
  char r;
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd"), r) == 3);
  static_assert(literal_parser<"abc">{}.parse(make_test_ctx(), make_source("abcd_tail"), r) == 3);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("abcd"), r) == 4);
  static_assert(literal_parser<"abcdef">{}.parse(make_test_ctx(), make_source("abcdef"), r) == 6);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("bbcd"), r) == -1);
  static_assert(literal_parser<"abcd">{}.parse(make_test_ctx(), make_source("ab"), r) == -1);
  static_assert( [&] {
      auto ctx = make_test_ctx();
      literal_parser<"ab\ncd\nef">{}.parse(ctx, make_source("ab\ncd\nef"), r);
      return new_line_count(ctx);
    }() == 3, "if the literal contains new lines it must to be counted");
  return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename t> struct value_parser : base_parser<value_parser<t>> {
	t val;
	constexpr value_parser(t v) : val(v) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const bool ok = src() == val;
		eq(ok, result, val);
		count_new_line(ok, ctx, val, result);
		return -2 * !ok + 1;
	}

	constexpr bool test() const {
		char r{};
		parse(make_test_ctx(), make_source(val), r) == 1           || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val+1), r) == -1        || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source(val-1), r) == -1        || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val), r),r) == val     || (throw __LINE__, 1);

		r=val+1;
		(parse(make_test_ctx(), make_source(val-1), r),r) == val+1 || (throw __LINE__, 1);

		return true;
	}
};





template<typename t> constexpr auto parse_value(t&& val) {
	return value_parser<std::decay_t<t>>{ std::forward<decltype(val)>(val) };
}

constexpr static bool test_parser_value() {
	static_assert( value_parser{ 'a' }.test() ); static_assert( value_parser{ 'Z' }.test() );
	static_assert( value_parser{ L'!' }.test() ); static_assert( value_parser{ '\n' }.test() );
	static_assert( []{char r{}; return value_parser{'a'}.parse(make_test_ctx(), make_source("abc"), r);}() == 1 );
	static_assert( []{char r{}; return value_parser{'b'}.parse(make_test_ctx(), make_source("abc"), r);}() == -1 );
	static_assert( [] {
		auto ctx1 = make_test_ctx();
		auto ctx2 = make_test_ctx();
		char r{};
		value_parser{'\n'}.parse(ctx1, make_source("a"), r);
		value_parser{'\n'}.parse(ctx2, make_source("\n"), r);
		return (new_line_count(ctx1) == 1) + 2*(new_line_count(ctx2) == 2);
	}() == 3 );
	return true;
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

struct space_parser : base_parser<space_parser> {
	constexpr parse_result parse(auto&& ctx,auto src, auto& r) const {
		auto sym = src();
		const bool is_space = 0x07 < sym && sym < '!'; // 0x08 is a backspace
		count_new_line(is_space, ctx, sym, r);
		return -1 + 2*is_space;
	}
	constexpr bool test() const {
		constexpr char r=0x00;
		parse(make_test_ctx(), make_source(' '), r) == 1    || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\n'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('\t'), r) == 1   || (throw __LINE__, 1);
		parse(make_test_ctx(), make_source('!'), r) == -1   || (throw __LINE__, 1);
		r == 0x00                                           || (throw __LINE__, 1);

		auto ctx = make_test_ctx();
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		new_line_count(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		new_line_count(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
} ;

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

struct any_parser : base_parser<any_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = 0;
		decltype(src()) cur;
		do {
			cur = src();
			ascip_details::eq( result, cur );
			count_new_line(true, ctx, cur, result);
			++ret;
		}
		while(src && (cur & 0x80)) ;
		return ret;
	}
	constexpr bool test() const {
		char r=0x00; type_any_eq_allow rr;
		parse(make_test_ctx(), make_source(' '), r) == 1       || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('Z'), r),r) == 'Z' || (throw __LINE__, 1);
		(parse(make_test_ctx(), make_source('~'), r),r) == '~' || (throw __LINE__, 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("я"), rr)  == 2);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"я"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source(L"яz"), rr) == 1);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("❤"), rr) == 3);
		static_assert(any_parser{}.parse(make_test_ctx(), make_source("θ"), rr) == 2);

		auto ctx = make_test_ctx();
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source(' '), r);
		search_in_ctx<new_line_count_tag>(ctx) == 1 || (throw __LINE__, 1);
		parse(ctx, make_source('\n'), r);
		search_in_ctx<new_line_count_tag>(ctx) == 2 || (throw __LINE__, 1);

		return true;
	}
};

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<auto base=10> struct int_base_parser {
	constexpr static bool is_int(auto s) {
		constexpr bool is_0 = base <= 10;
		return
			   ('0' <= s && s <= '9')
			|| (!is_0 && ('a' <= s && s <= ('a'+base-10)))
			|| (!is_0 && ('A' <= s && s <= ('A'+base-10)))
			;
	}
	constexpr static int to_int(auto s) {
		const bool is_a = 'a' <= s && s <= 'z';
		const bool is_A = 'A' <= s && s <= 'Z';
		const bool is_0 = '0' <= s && s <= '9';
		return
			  is_0 * (s - '0')
			+ is_a * (s - 'a' + 10)
			+ is_A * (s - 'A' + 10)
			;
	}
	constexpr bool next(auto cur, auto& result) const {
		const bool isint = is_int(cur);
		result *= (base*isint) + !isint;
		result += to_int(cur) * isint;
		return isint;
	}
};

struct int_parser : base_parser<int_parser>, int_base_parser<> {
	constexpr parse_result parse(auto&&, auto src, auto& _result)  const {
		auto sign = src();
		if(sign != '-' && sign != '+' && !this->is_int(sign)) return -1;
		int signer = -1*(sign=='-') + this->is_int(sign) + (sign=='+');
		ascip_details::eq(_result, this->is_int(sign) * this->to_int(sign));
		auto& result = _result;
		auto ret = 1;
		while(src && this->next(src(), result)) ++ret;
		result *= signer;
		bool bad_result = ((sign=='-')+(sign=='+')+(ret==1))==2;
		return -1*bad_result + ret*!bad_result;
	}
	constexpr auto parse_without_preparation(auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret;
	}

	constexpr bool test() const {
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			auto r = answer + 100;
			int_parser p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			r == answer                                              || (throw __LINE__, 1);
			return true;
		};

		static_assert( t("-1", 2, -1) );
		static_assert( t("1", 1, 1) );
		static_assert( t("+2", 2, 2) );
		static_assert( t("+0", 2, 0) );
		static_assert( t("0", 1, 0) );
		static_assert( t("+103", 4, 103) );
		static_assert( t("103", 3, 103) );


		auto r=0;
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("!"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("a"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-"), r);}) == -1 );
		static_assert( ({ auto r=0;int_parser{}.parse(make_test_ctx(), make_source("-["), r);}) == -1 );


		return true;
	}

};

template<auto base> struct uint_parser : base_parser<uint_parser<base>>, int_base_parser<base> {
	constexpr auto parse(auto&&, auto src, auto& result) const {
		auto ret = 0;
		while(src && this->next(src(), result)) ++ret;
		return ret - (ret==0);
	}

	constexpr bool test() const {
		uint_parser p; auto r=0;

		auto t = [](auto&& src, auto sym_cnt) {
			int r=0;
			uint_parser<10> p{};
			p.parse(make_test_ctx(), make_source(src), r) == sym_cnt || (throw __LINE__, 1);
			return r;
		};

		static_assert( []{ int r=0; return uint_parser<10>{}.parse(make_test_ctx(), make_source("+"), r); }() == -1 );
		static_assert( t("1", 1) == 1 );
		static_assert( t("2", 1) == 2 );
		static_assert( t("0", 1) == 0 );
		static_assert( t("103", 3) == 103 );

		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A"), r); return r; }() == 10 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("A3"), r); return r; }() == 163 );
		static_assert( []{ int r=0; uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); return r; }() == 253 );
		static_assert( []{ int r=0; return uint_parser<16>{}.parse(make_test_ctx(), make_source("FD"), r); }() == 2 );

		return true;
	}
} ;

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

struct float_point_parser : base_parser<float_point_parser> {
	constexpr static auto pow(auto what, auto to) { const auto m = what; for(auto i=1;i<to;++i) what*=m; return what; }
	constexpr parse_result parse(auto&&, auto src, auto& result)  const {
		constexpr int_parser int_{};
		result = 0;
		auto int_pos = src;
		auto dec_pos = src;
		auto left_result = int_.parse_without_preparation(int_pos, result);
		if(left_result <= 0 && int_pos() != '.') return -1;
		dec_pos+=left_result;
		if(!dec_pos || dec_pos()!='.') return -1;
		auto right_result = int_.parse_without_preparation(dec_pos, result);
		if(right_result <= 0) return -1;
		result /= pow(10, right_result);
		return left_result + right_result + 1;
	}
	constexpr bool test() const {
		constexpr float_point_parser p{};
		auto t = [](auto&& src, auto sym_cnt, auto answer) {
			const float_point_parser p{};
			auto r = answer + 100;
			auto pr = p.parse(make_test_ctx(), make_source(src), r);
		       	pr /= (pr == sym_cnt);
			r /= (r == answer);
			return true;
		};

		static_assert( p.pow(10, 1) == 10 );
		static_assert( p.pow(10, 2) == 100 );
		static_assert( p.pow(10, 3) == 1000 );
		static_assert( t("0.5", 3, 0.5) );
		static_assert( t("1.5", 3, 1.5) );
		static_assert( t("3.075", 5, 3.075) );


		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("0"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("a"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("1."), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5+3"), r); }) == -1);
		static_assert( ({double r=100;float_point_parser{}.parse(make_test_ctx(), make_source("5-3"), r); }) == -1);


		return true;
	}
};
} // namespace ascip_details::prs

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

template<auto from, auto to> struct range_parser : base_parser<range_parser<from,to>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto sym = src();
		const bool ok = from <= sym && sym <= to;
		eq( ok, result, sym );
		if constexpr (from <= '\n' && '\n' <= to)
			count_new_line(ok, ctx, sym, result);
		return -2 * !ok + 1;
	}
};

constexpr static bool test_range_parser() {
	constexpr static auto ascii = range_parser<(char)0x01,(char)0x7F>{};

	constexpr static auto lower = range_parser<'a','z'>{};
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("a"), r);r;}) == 'a' );
	static_assert( ({char r{};lower.parse(make_test_ctx(), make_source("A"), r);}) == -1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source("A"), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+1), r);}) == 1 );
	static_assert( ({char r{};ascii.parse(make_test_ctx(), make_source('~'+2), r);}) == -1 );

	static_assert( [&] {
		char r{};
		auto ctx = make_test_ctx();
		ascii.parse(ctx, make_source("\n"), r);
		return new_line_count(ctx);
	}() == 2 );
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename parser, typename act_t> struct semact_parser : base_parser<semact_parser<parser,act_t>> {
	act_t act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool is_any_arg_pattern = !requires{ act(); };
		if constexpr(std::is_same_v<type_any_eq_allow&, decltype(result)>)
			return p.parse(std::forward<decltype(ctx)>(ctx), std::move(src), result);
		else if constexpr(requires{ act(result); requires std::is_pointer_v<decltype(act(result))>;} ) {
			auto* nr = act(result);
			return p.parse(ctx, src, *nr);
		}
		else if constexpr (requires{ act(result); requires std::is_lvalue_reference_v<decltype(act(result))>; } ) {
			auto& nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else if constexpr(requires{ act(result); } && !requires{act(0, ctx, src, result);} ) {
			auto nr = act(result);
			return p.parse(ctx, src, nr);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			if(ret >= 0) {
				if constexpr (requires{ act(ret, &ctx, src, result); }) act(ret, &ctx, src, result);
				else if constexpr (requires{ act(ret, result); }) act(ret, result);
				else act();
			}
			return ret;
		}
	}
};

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_semact() {

	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("ab"),r);a;}) == 'c', "semact: calls if parsed");
	static_assert( ({char r='z',a='y';(t<'a'>::char_ >> t<'b'>::char_([&a](){a='c';})).parse(make_test_ctx(),make_source("au"),r);a;}) == 'y', "semact: silent if fails");

	return true;
}

template<typename parser, typename act_type, typename tag> struct exec_before_parser : base_parser<exec_before_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		if constexpr(type_dc<decltype(result)> == type_c<type_any_eq_allow>) return p.parse(ctx, src, result);
		else if constexpr(requires{*act(search_in_ctx<tag>(ctx), result);}) {
			auto* new_result = act(search_in_ctx<tag>(ctx), result);
			return p.parse(ctx, src, *new_result);
		}
		else {
			act(search_in_ctx<tag>(ctx), result);
			return p.parse(ctx, src, result);
		}
	}
};

template<typename parser, typename act_type, typename tag> struct exec_after_parser : base_parser<exec_after_parser<parser, act_type, tag>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		if constexpr (type_dc<decltype(result)> != type_c<type_any_eq_allow>) act(search_in_ctx<tag>(ctx), result);
		else if constexpr(requires{act(search_in_ctx<tag>(ctx));}) act(search_in_ctx<tag>(ctx));
		return ret;
	}
};

template<typename tag, parser type> constexpr auto exec_before(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_before_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename tag, parser type> constexpr auto exec_after(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return exec_after_parser<type, act_type, tag>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool context_parsers() {
	struct tag {};
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_before<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);return &_p;}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'a'*3 );
	static_assert( []{
		char r{};
		int r_inner{};
		char val=3;
		parse(add_to_ctx<tag>(&val,
			t<'a'>::char_ >>
			exec_after<tag>([&](auto* val, auto& _p){r_inner=_p*(*val);}, t<'b'>::char_)
		), p::make_source("ab"), r);
		return r_inner;
	}() == 'b'*3 );

	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

namespace ascip_details::prs {
struct seq_stack_tag{};
struct seq_shift_stack_tag{};
struct seq_result_stack_tag{};
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {
template<parser parser>
struct use_seq_result_parser : base_parser<use_seq_result_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		auto& result = *search_in_ctx<seq_result_stack_tag>(ctx);
		return p.parse(ctx, src, result);
	}
};
}

namespace ascip_details {
template<parser type> constexpr auto use_seq_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return prs::use_seq_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) }; }
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

struct cur_shift_parser : base_parser<cur_shift_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		eq(result, *search_in_ctx<seq_shift_stack_tag>(ctx));
		return 0;
	}
};

//TODO: dose we realy need the pos parser?
struct cur_pos_parser : base_parser<cur_pos_parser> {
	constexpr parse_result parse(auto&&, auto src, auto& result) const {
		//TODO: extract the info from context or from parent's object
		//      sequence may sotre it in context
		//      sequence may have mutable field and
		//        pass it to parse method here or
		//        store it to result on it's own
		//      if use context - make it with tags
		eq(result, pos(src));
		return 0;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)







namespace ascip_details::prs {

template<auto ind>
struct seq_reqursion_parser : base_parser<seq_reqursion_parser<ind>> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		const auto* req = *search_in_ctx<seq_stack_tag, ind>(ctx);
		if constexpr( type_dc<decltype(result)> == type_c<type_any_eq_allow> )
			return src ? req->parse_mono(src) : -1;
		else return src ? req->parse_mono(src, result) : -1;
	}
};

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<auto cnt> struct _seq_rfield_val { constexpr static auto num_val = cnt; };
template<ascip_details::parser parser, typename val> struct seq_inc_rfield_val : base_parser<seq_inc_rfield_val<parser, val>> {
	constexpr seq_inc_rfield_val() =default ;
	constexpr seq_inc_rfield_val(seq_inc_rfield_val&&) noexcept =default ;
	constexpr seq_inc_rfield_val(const seq_inc_rfield_val&) noexcept =default ;
	constexpr explicit seq_inc_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};
template<ascip_details::parser parser, typename val> struct seq_num_rfield_val : base_parser<seq_num_rfield_val<parser, val>> {
	constexpr seq_num_rfield_val() =default ;
	constexpr seq_num_rfield_val(seq_num_rfield_val&&) noexcept =default ;
	constexpr seq_num_rfield_val(const seq_num_rfield_val&) noexcept =default ;
	constexpr explicit seq_num_rfield_val(parser p) : p(std::move(p)) {}
	parser p;

	constexpr static auto value = val::num_val;
};

template<typename type, typename stop_on_parser, template<typename...>class tmpl> constexpr int grab_num_val() {
	int val = 0;
	exists_in((type*)nullptr, [&val](const auto* p){
		constexpr bool is_num = ascip_details::is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
		if constexpr (is_num) val = std::decay_t<decltype(*p)>::value;
		return is_num;
	}, [](const auto* p) {
		return requires{ p->seq; } && !requires{ static_cast<const stop_on_parser*>(p); };
	});
	return val;
}

struct seq_inc_rfield : base_parser<seq_inc_rfield> {constexpr parse_result parse(auto&&,auto,auto&)const {return 0;} };
template<ascip_details::parser parser> struct seq_inc_rfield_after : base_parser<seq_inc_rfield_after<parser>> {
	constexpr seq_inc_rfield_after() =default ;
	constexpr seq_inc_rfield_after(seq_inc_rfield_after&&) noexcept =default ;
	constexpr seq_inc_rfield_after(const seq_inc_rfield_after&) noexcept =default ;
	constexpr explicit seq_inc_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_inc_rfield_before : base_parser<seq_inc_rfield_before<parser>> {
	constexpr seq_inc_rfield_before() =default ;
	constexpr seq_inc_rfield_before(seq_inc_rfield_before&&) noexcept =default ;
	constexpr seq_inc_rfield_before(const seq_inc_rfield_before&) noexcept =default ;
	constexpr explicit seq_inc_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_after : base_parser<seq_dec_rfield_after<parser>> {
	constexpr seq_dec_rfield_after() =default ;
	constexpr seq_dec_rfield_after(seq_dec_rfield_after&&) noexcept =default ;
	constexpr seq_dec_rfield_after(const seq_dec_rfield_after&) noexcept =default ;
	constexpr explicit seq_dec_rfield_after(parser p) : p(std::move(p)) {}
	parser p;
};
template<ascip_details::parser parser> struct seq_dec_rfield_before : base_parser<seq_dec_rfield_before<parser>> {
	constexpr seq_dec_rfield_before() =default ;
	constexpr seq_dec_rfield_before(seq_dec_rfield_before&&) noexcept =default ;
	constexpr seq_dec_rfield_before(const seq_dec_rfield_before&) noexcept =default ;
	constexpr explicit seq_dec_rfield_before(parser p) : p(std::move(p)) {}
	parser p;
};
template<typename p> seq_inc_rfield_after(p) -> seq_inc_rfield_after<p>;
template<typename p> seq_inc_rfield_before(p) ->  seq_inc_rfield_before<p>;
template<typename p> seq_dec_rfield_after(p) ->  seq_dec_rfield_after<p>;
template<typename p> seq_dec_rfield_before(p) -> seq_dec_rfield_before<p>;

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename value, typename type>
constexpr static auto transform_special(prs::seq_inc_rfield_val<type, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_inc_rfield_val<type, value>::value;
	return transform_apply<mutator>(finc<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, parser ptype, typename value>
constexpr static auto transform_special(prs::seq_num_rfield_val<ptype, value>&& src, auto& ctx) {
	constexpr auto num = prs::seq_num_rfield_val<ptype, value>::value;
	return transform_apply<mutator>(fnum<num>(transform<mutator>(std::move(src.p), ctx)), ctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs::seq_details {

template<typename source, typename result> struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono(source src) const =0 ;
  virtual parse_result parse_mono(source src, result& r) const =0 ;
};

template<typename parser, typename context, typename source, typename result>
struct mono_for_seq final : monomorphic<source, result> {
	using base_type = monomorphic<source, result>;
	const parser* self;
	mutable context ctx;
	constexpr mono_for_seq(const parser* self, context ctx) : self(self), ctx(std::move(ctx)) {}
	constexpr parse_result parse_mono(source src) const override {
		type_any_eq_allow r;
		return self->parse_without_prep(ctx, src, r);
	}
	constexpr parse_result parse_mono(source src, result& r) const override {
		return self->parse_without_prep(ctx, src, r);
	}
};

constexpr auto mk_mono(const auto* parser, auto ctx, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
	return mono_for_seq<std::decay_t<decltype(*parser)>, decltype(ctx), decltype(src), std::decay_t<decltype(result)>>( parser, ctx );
}

}


namespace ascip_details::prs {

template<string_literal message, parser type> struct seq_error_parser ;
template<typename... parsers> struct opt_seq_parser : base_parser<opt_seq_parser<parsers...>>, seq_tag {
	tuple<parsers...> seq;

	constexpr opt_seq_parser() =default ;
	constexpr opt_seq_parser(tuple<parsers...> t) : seq(std::move(t)) {}
	constexpr opt_seq_parser(auto&&... args) requires (sizeof...(parsers) == sizeof...(args)) : seq( static_cast<decltype(args)&&>(args)... ) {}
	constexpr opt_seq_parser(const opt_seq_parser&) =default ;

	template<template<typename...>class tmpl>
	constexpr static auto is_spec_checker = [](const auto* p) {
		return is_specialization_of<std::decay_t<decltype(*p)>, tmpl>;
	};
	template<typename type>
	constexpr static bool _exists_in(auto&& ch) {
		return exists_in((type*)nullptr, ch, [](const auto* p){
			return requires{ p->seq; } && !requires{ static_cast<const opt_seq_parser*>(p); };
		});
	}
	template<typename type> constexpr static bool is_field_separator = _exists_in<type>([](const auto* p){return requires{ static_cast<const seq_inc_rfield*>(p); };});
	template<typename type> constexpr static bool is_inc_field_val = _exists_in<type>(is_spec_checker<seq_inc_rfield_val>);
	template<typename type> constexpr static bool is_num_field_val = _exists_in<type>(is_spec_checker<seq_num_rfield_val>);
	template<typename type> constexpr static bool is_inc_field_after = _exists_in<type>(is_spec_checker<seq_inc_rfield_after>);
	template<typename type> constexpr static bool is_inc_field_before = _exists_in<type>(is_spec_checker<seq_inc_rfield_before>);
	template<typename type> constexpr static bool is_dec_field_after = _exists_in<type>(is_spec_checker<seq_dec_rfield_after>);
	template<typename type> constexpr static bool is_dec_field_before = _exists_in<type>(is_spec_checker<seq_dec_rfield_before>);
	template<typename... types> constexpr static bool is_struct_requires =
		((is_field_separator<types> + ...) + (is_inc_field_val<types> + ...) +
		 (is_inc_field_after<types> + ...) + (is_inc_field_before<types> + ...) +
		 (is_dec_field_after<types> + ...) + (is_dec_field_before<types> + ...) +
		 (is_num_field_val<types> + ...)
		) > 0;
	constexpr static bool is_struct_requires_pd = is_struct_requires<parsers...>;

	template<typename type> constexpr static int num_field_val() { return grab_num_val<type, opt_seq_parser, seq_num_rfield_val>(); }
	template<typename type> constexpr static auto inc_field_val() { return grab_num_val<type, opt_seq_parser, seq_inc_rfield_val>(); }

	template<auto find> constexpr auto call_parse(ascip_details::parser auto& p, auto&& ctx, auto src, auto& result) const {
		if constexpr (!is_struct_requires_pd) return p.parse(ctx, src, result);
		else if constexpr (is_field_separator<decltype(auto(p))>) return p.parse(ctx, src, result);
		else if constexpr ( type_dc<decltype(result)> == type_c<type_any_eq_allow> )
			return p.parse(ctx, src, result);
		else {
			return p.parse(ctx, src, ascip_reflection::get<find>(result));
		}
	}
	template<auto find, auto pind, typename cur_t, typename... tail> constexpr parse_result parse_seq(auto&& ctx, auto src, auto& result) const {
		//TODO: use -1 as last struct field, -2 as the field before last one and so on...
		constexpr auto cur_field = num_field_val<cur_t>() +
			( (find + is_inc_field_before<cur_t> + (-1*is_dec_field_before<cur_t>) + inc_field_val<cur_t>()) * !is_num_field_val<cur_t> );
		constexpr auto nxt_field = cur_field + is_inc_field_after<cur_t> + (-1*is_dec_field_after<cur_t>) + is_field_separator<cur_t>;

		auto& cur = get<pind>(seq);
		auto ret = call_parse<cur_field>(cur, ctx, src, result);
		src += ret * (0 <= ret);
		*search_in_ctx<seq_shift_stack_tag>(ctx) += ret * (0 <= ret);
		if constexpr (pind+1 == sizeof...(parsers)) return ret;
		else {
			if( ret < 0 ) return ret;
			auto req = parse_seq<nxt_field, pind+1, tail...>(ctx, src, result);
			return req*(req<0) + (ret+req)*(0<=req);
		}
	}

	template<auto find, auto pind> constexpr parse_result parse_and_store_shift(auto&& ctx, auto src, auto& result) const {
		//static_assert - exists concrete in ctx
		auto* old_shift = search_in_ctx<seq_shift_stack_tag>(ctx);
		auto cur_shift = 0;
		search_in_ctx<seq_shift_stack_tag>(ctx) = &cur_shift;
		auto ret = parse_seq<find, pind, parsers...>(ctx, src, result);
		search_in_ctx<seq_shift_stack_tag>(ctx) = old_shift;
		return ret;
	}
	constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		return parse_and_store_shift<0,0>(std::forward<decltype(ctx)>(ctx), std::move(src), result);
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		using mono_type = seq_details::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		auto shift_store = 0;
		const mono_type* mono_ptr;
		auto cur_ctx = make_ctx<seq_shift_stack_tag>(&shift_store,
		  make_ctx<seq_result_stack_tag>(&result,
		  	make_ctx<seq_stack_tag>(&mono_ptr, ctx) ) ) ;
		auto mono = seq_details::mk_mono(this, cur_ctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(src, result);
	}
};
template<typename... p> opt_seq_parser(p...) -> opt_seq_parser<std::decay_t<p>...>;

} // namespace ascip_details::prs

namespace ascip_details {

template<parser... left, typename right> constexpr auto operator>>(prs::opt_seq_parser<left...>&& l, right&& r) {
    return [&]<auto... inds>(std::index_sequence<inds...>){
        return prs::opt_seq_parser<left..., std::decay_t<right>>{ std::move(get<inds>(l.seq))..., std::move(r) };
    }(std::make_index_sequence<sizeof...(left)>{});
}
template<parser left, typename right> constexpr auto operator>>(left&& l, right&& r)
	requires (!is_specialization_of<std::decay_t<left>, prs::opt_seq_parser>) {
	return prs::opt_seq_parser<std::decay_t<left>, std::decay_t<right>>{ std::forward<left>(l), std::forward<right>(r) };
}
template<parser left> constexpr auto operator>>(left&& l, char r) {
	return std::forward<decltype(l)>(l) >> prs::value_parser{ r };
}
template<parser p> constexpr auto operator++(p&& l) {
	return prs::seq_inc_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator++(p&& l,int) {
	return prs::seq_inc_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l) {
	return prs::seq_dec_rfield_before<std::decay_t<p>>{ std::forward<p>(l) }; }
template<parser p> constexpr auto operator--(p&& l,int) {
	return prs::seq_dec_rfield_after<std::decay_t<p>>{ std::forward<p>(l) }; }

template<auto cnt, parser type> constexpr auto finc(type&& p) {
	using p_type = std::decay_t<type>;
	using inc_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_inc_rfield_val<p_type, inc_type>{ std::forward<decltype(p)>(p) }; }
template<auto cnt, parser type> constexpr auto fnum(type&& p) {
	using p_type = std::decay_t<type>;
	using num_type = prs::_seq_rfield_val<cnt>;
	return prs::seq_num_rfield_val<p_type, num_type>{ std::forward<decltype(p)>(p) }; }

template<typename mutator, template<typename...>class seq_parser, typename... list>
constexpr static auto transform(seq_parser<list...>&& src, auto& ctx) requires requires{ src.seq; } {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>(transform_apply_to_each<mutator, seq_parser, 0>(std::move(src.seq), nctx), nctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<string_literal msg, parser type> struct must_parser : base_parser<must_parser<msg, type>> {
	type p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		return call_if_error(ctx, result, ret, src);
	}

	constexpr static auto call_if_error(auto& ctx, auto& result, auto orig_ret, auto& src) {
		if (0 <= orig_ret) return orig_ret;
		constexpr bool without_result = ascip_details::type_dc<decltype(result)> == ascip_details::type_c<type_any_eq_allow>;
		auto err = search_in_ctx<err_handler_tag>(ctx);
		static_assert( !std::is_same_v<std::decay_t<decltype(err)>, ctx_not_found_type>, "for using the must parser a error handler is required" );
		if constexpr(requires{(*err)(0, msg);}) return (*err)(new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(0, src, msg);}) return (*err)(new_line_count(ctx), src, msg);
		else if constexpr(requires{(*err)(msg);}) return (*err)(msg);
		else if constexpr(requires{(*err)(result, 0, msg);}) return (*err)(result, new_line_count(ctx), msg);
		else if constexpr(requires{(*err)(result, src, 0, msg);}) return (*err)(result, src, new_line_count(ctx), msg);
		else {
			static_assert( requires{(*err)(result, msg);}, "the error handler have to request no result as parameter or it have to be a template parameter" );
			return (*err)(result, msg);
		}
	}
};

template<parser left, typename right> constexpr auto operator>(left&& l, right&& r) {
	return std::move(l) >> must<"unknown">(std::forward<decltype(r)>(r));
}

template<string_literal msg> constexpr auto must(parser auto&& p) {
	return must_parser<msg, std::decay_t<decltype(p)>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename p, template<auto>class t=p::template tmpl>
constexpr bool test_must_parser() {
	static_assert( []{
		char r='z';
		const auto err_method = [&](...){return -10;};
		return (t<'a'>::char_ >> t<'b'>::char_ >> must<"t">(t<'c'>::char_)).parse(make_test_ctx(&err_method), make_source("abe"), r);
	}() == -10, "error lambda are called");

	const auto err_method = [](
			auto&,
			auto line_number,
			auto message){
		line_number /= (line_number==2);
		return (-3 * (message=="unknown")) + (-4 * (message=="test"));
	};
	static_assert( [&]{ char r=0x00;
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ > t<'c'>::char_).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}() == -3, "on error: sources are on start sequence and on rule where the error");
	static_assert( [&]{ char r=0x00;
		return (p::any >> t<'a'>::char_ >> t<'b'>::char_ >> must<"test">(t<'c'>::char_)).parse(make_test_ctx(&err_method), make_source("\nabe"), r);
	}() == -4, "on error: sources are on start sequence and on rule where the error");

	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




       

//          Copyright Hudyaev Alexey 2023.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

constexpr static auto test_parser_char(const auto& p, auto&& s, auto pr) {
	char result='z';
	auto answer = p.parse(make_test_ctx(), make_source(s), result);
	answer /= (answer==pr);
	return result;
}

}

namespace ascip_details::prs {

template<ascip_details::parser parser> struct omit_parser : base_parser<omit_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr omit_parser() =default ;
	constexpr omit_parser(omit_parser&&) =default ;
	constexpr omit_parser(const omit_parser&) =default ;
	constexpr omit_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto&) const {
		type_any_eq_allow r;
		return p.parse(ctx, src, r);
	}
};

constexpr auto omit(auto&& p) {
	using type = std::decay_t<decltype(p)>;
	return omit_parser<type>{ std::forward<decltype(p)>(p) };
}

template<typename p>
constexpr static bool test_omit() {
	static_assert( test_parser_char(omit(p::template char_<'a'>), "a", 1) == 'z' );
	static_assert( test_parser_char(omit(p::template char_<'a'>), "b", -1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "a", 1) == 'z' );
	static_assert( test_parser_char(p::template _char<'a'>, "b", -1) == 'z' );
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<auto val, parser parser> struct tmpl_as_parser : base_parser<tmpl_as_parser<val,parser>> {
	[[no_unique_address]] parser p;
	constexpr tmpl_as_parser() =default ;
	constexpr tmpl_as_parser(tmpl_as_parser&&) =default ;
	constexpr tmpl_as_parser(const tmpl_as_parser&) =default ;
	constexpr tmpl_as_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_any_eq_allow r;
		auto shift = p.parse(ctx, static_cast<decltype(auto(src))&&>(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};

template<auto val, parser parser_t> constexpr auto as( parser_t&& p) {
	using type = std::decay_t<decltype(p)>;
	return tmpl_as_parser<val, type>{ std::forward<decltype(p)>(p) };
}

template<typename value_t, parser parser> struct as_parser : base_parser<as_parser<value_t, parser>> {
	value_t val;
	[[no_unique_address]] parser p;
	constexpr as_parser() =default ;
	constexpr as_parser(as_parser&&) =default ;
	constexpr as_parser(const as_parser&) =default ;
	constexpr as_parser(value_t val, parser p) : val(std::move(val)), p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_any_eq_allow r;
		auto shift = p.parse(ctx, std::move(src), r);
		if(shift >= 0) result = val;
		return shift;
	}
};

template<typename value_t, parser parser_t> constexpr auto as( parser_t&& p, value_t&& val ){
	using ptype = std::decay_t<decltype(p)>;
	using val_type = std::decay_t<decltype(val)>;
	return as_parser<val_type, ptype>( std::forward<decltype(val)>(val), std::forward<decltype(p)>(p) );
}

template<typename prs, template<auto>class t=prs::template tmpl>
constexpr static bool test_as() {
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "a", 1) == 'b' );
	static_assert( test_parser_char(as(t<'a'>::char_, 'b'), "b", -1) == 'z' );
	static_assert( test_parser_char(as(prs::int_, 'b'), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "123", 3) == 'b' );
	static_assert( test_parser_char(as<'b'>(prs::int_), "a", -1) == 'z' );
	static_assert( []{
		auto p = t<'a'>::char_;
		return test_parser_char(as(p, 'b'), "a", 1);
	}() == 'b' );
	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename parser, typename value_type>
constexpr static auto transform_special(prs::as_parser<value_type, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::as_parser{ src.val, std::move(np) }, nctx );
}

template<typename mutator, typename parser, auto value>
constexpr static auto transform_special(prs::tmpl_as_parser<value, parser>&& src, auto&& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform_apply<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::tmpl_as_parser<value, std::decay_t<decltype(np)>>{ std::move(np) }, nctx );
}

}





namespace ascip_details::prs {

struct variant_stack_tag {};
struct variant_stack_result_tag{};

namespace variant_details {

template<typename source, typename result>
struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono(source) =0 ;
  virtual parse_result parse_mono(source, result&) =0 ;
};

template<typename parser, typename context, typename source, typename result>
struct monomorphic_impl : monomorphic<source, result> {
  const parser* v;
  context ctx;

  constexpr monomorphic_impl(const parser* v, context ctx) : v(v), ctx(std::move(ctx)) {}
  constexpr parse_result parse_mono(source src) override {
    type_any_eq_allow fr;
    return v->template parse_ind<0>(ctx, src, fr);
  }
  constexpr parse_result parse_mono(source src, result& r) override {
    return v->template parse_ind<0>(ctx, src, r);
  }
};

template<typename parser, typename context, typename source, typename result>
constexpr auto mk_mono(const parser* p, context ctx, source src, result& r) {
  return monomorphic_impl<parser, context, source, result>( p, ctx );
}

}

template<parser parser> struct use_variant_result_parser : base_parser<use_variant_result_parser<parser>> {
	parser p;
};

template<auto val> struct variant_pos_value{ constexpr static auto pos = val; };

template<parser... parsers> struct variant_parser : base_parser<variant_parser<parsers...>> {
	using self_type = variant_parser<parsers...>;
	tuple<parsers...> seq;

	constexpr variant_parser(const variant_parser& other) : seq(other.seq) {}
	constexpr variant_parser(variant_parser&& other) : seq(std::move(other.seq)) {}
	constexpr explicit variant_parser( parsers... l ) : seq( std::forward<parsers>(l)... ) {}

	template<auto ind, auto cnt, auto cur, typename cur_parser, typename... tail>
	constexpr static auto _cur_ind() {
		//TODO: if it will be like -use_variant_result the skip variable evaluates to false (opt_parser<use_variant_result_parser<...>>)
		constexpr const bool skip = is_specialization_of<cur_parser, use_variant_result_parser>;
		if constexpr (ind == cnt) {
			if constexpr (skip) return -1;
			else return cur;
		}
		else return _cur_ind<ind,cnt+1,cur+(!skip),tail...>();
	}
	template<auto ind> consteval static auto cur_ind() { return _cur_ind<ind,0,0,parsers...>(); }
	template<auto ind> constexpr auto parse_ind(auto&& ctx, auto& src, auto& result) const {
		auto prs = [&](auto&& r){ return get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(r)); };
		if constexpr (ind+1 == sizeof...(parsers)) return prs(result);
		else {
			auto parse_result = prs(type_any_eq_allow{});
			if(parse_result >= 0) return prs(result);
			return parse_ind<ind+1>(ctx, src, result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
        using mono_type = variant_details::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
        mono_type* mono_ptr;
        auto nctx = make_ctx<variant_stack_tag>(&mono_ptr, make_ctx<variant_stack_result_tag>(&result, ctx));
        auto mono = variant_details::mk_mono(this, ctx, src, result);
        mono_ptr = &mono;
        return parse_ind<0>(nctx, src, result);
	}

	constexpr auto clang_crash_workaround(auto r) {
		return std::move(*this) | value_parser(r);
	}
};

template<parser... parsers>
variant_parser(parsers...) -> variant_parser<parsers...>;


template<typename prs, template<auto>class t=prs::template tmpl>
constexpr static bool test_variant() {
	constexpr auto run_parse = [](const auto& p, auto&& src, auto& r) {
		return p.parse(make_test_ctx(), make_source(src), r);
	};
	static_assert( ((void)static_cast<const variant_parser<char_parser<'a'>, char_parser<'b'>, char_parser<'c'>>&>(t<'a'>::char_ | t<'b'>::char_ | t<'c'>::char_),1) );

	static_assert( ({ char r{};run_parse(t<'a'>::char_|t<'b'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b', "b", r);r;}) == 'b' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);}) == 1 );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "c", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(t<'a'>::char_|'b'|'c', "d", r);}) == -1 );

	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "b", r);r;}) == 'c' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);r;}) == 'a' );
	static_assert( ({ char r{};run_parse(as(t<'b'>::char_,'c')|t<'a'>::char_, "a", r);  }) ==  1 );

	static_assert( [&] {
		char r='z';
		const auto pr = run_parse(prs::nop | t<'a'>::char_, "b", r);
		return (r=='z') + 2*(pr == 0);
	}() == 3 );

	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename... parsers>
constexpr auto operator|(prs::variant_parser<parsers...>&& left, parser auto&& right) {
	return [&]<auto... inds>(std::index_sequence<inds...>) {
		return prs::variant_parser<parsers..., std::decay_t<decltype(right)>>{ get<inds>(left.seq)..., std::forward<decltype(right)>(right) };
    }(std::make_index_sequence<sizeof...(parsers)>{});
}
constexpr auto operator|(auto&& left, parser auto&& right) {
	return prs::variant_parser( std::forward<decltype(left)>(left), std::forward<decltype(right)>(right) );
}
constexpr auto operator|(auto&& left, nonparser auto&& right) {
	using left_type = std::decay_t<decltype(left)>;
	constexpr bool is_left_variant = is_specialization_of<std::decay_t<decltype(left)>, prs::variant_parser>;
	if constexpr (is_left_variant) return std::forward<decltype(left)>(left).clang_crash_workaround(right);
	else return prs::variant_parser<left_type>(std::forward<decltype(left)>(left)).clang_crash_workaround(right);
}

template<parser type> constexpr auto use_variant_result(const type& p) {
	return prs::use_variant_result_parser<type>{ {}, p };
}

}

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<parser left, parser right> struct different_parser : base_parser<different_parser<left, right>> {
	left lp;
	right rp;
	constexpr different_parser( left l, right r ) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_any_eq_allow fake_result;
		if(rp.parse(ctx, src, fake_result) >= 0) return -1;
		return lp.parse(ctx, src, result);
	}
};
template<parser left, parser right> different_parser(left, right) -> different_parser<left, right>;

template<parser left, parser right> constexpr auto operator-(left&& l, right&& r) {
	return different_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r)); }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_different() {


	return true;
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::different_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::different_parser( std::move(lp), std::move(rp) ), nctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {
template<typename type> concept optional = requires(type& o){ o.emplace(); o.reset(); };
}

namespace ascip_details::prs {

template<ascip_details::parser parser> struct opt_parser : base_parser<opt_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr opt_parser(opt_parser&&) =default ;
	constexpr opt_parser(const opt_parser&) =default ;
	constexpr opt_parser() =default ;
	constexpr opt_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using result_type = std::decay_t<decltype(result)>;

		if(!src) return 0;
		if constexpr(optional<result_type>) {
			auto ret = p.parse(ctx, src, result.emplace());
			if(ret<0) result.reset();
			return ret * (ret >= 0);
		}
		else {
			auto ret = p.parse(ctx, src, result);
			return ret * (ret >= 0);
		}
	}
};

template<parser p> constexpr auto operator-(p&& _p) { return opt_parser<std::decay_t<p>>{ _p }; }

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_optional() {
	static_assert( test_parser_char(-t<'a'>::char_, "b", 0) == 'z' );
	static_assert( test_parser_char(-t<'a'>::char_, "a", 1) == 'a' );
	return true;
}
} // namespace ascip_details::prs






namespace ascip_details::prs {

template<parser parser> struct negate_parser : base_parser<negate_parser<parser>> {
	parser p;
	constexpr negate_parser() =default ;
	constexpr negate_parser(negate_parser&&) =default ;
	constexpr negate_parser(const negate_parser&) =default ;
	constexpr negate_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, std::move(src), result);
		return ret * (-1); //TODO: what if the ret is 0?
	}
};

constexpr auto operator!(parser auto&& p) {
	using p_type = std::decay_t<decltype(p)>;
	if constexpr (is_specialization_of<p_type, negate_parser>) return p.p;
	else return negate_parser<p_type>{ std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_negate() {

	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == -1 );
	static_assert( ({char r{};(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);}) ==  1 );
	static_assert( ({char r='z';(!t<'a'>::char_).parse(make_test_ctx(), make_source('b'), r);r;}) ==  'z' );

	static_assert( ({char r{};(!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);}) == 1 );
	static_assert( ({char r{};(!!!!t<'a'>::char_).parse(make_test_ctx(), make_source('a'), r);r;}) == 'a' );


	static_cast<const decltype(auto(t<'a'>::char_))&>(!!t<'a'>::char_);
	static_cast<const decltype(auto(t<'a'>::char_))&>(!!!!t<'a'>::char_);
	static_cast<const opt_parser<negate_parser<char_parser<'a'>>>&>(-(!t<'a'>::char_));
	static_cast<const opt_parser<char_parser<'a'>>&>(-(!!t<'a'>::char_));

	return true;
}

}

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

template<parser parser> struct unary_list_parser : base_parser<unary_list_parser<parser>> {
	[[no_unique_address]] parser p;
	constexpr unary_list_parser(unary_list_parser&&) =default ;
	constexpr unary_list_parser(const unary_list_parser&) =default ;
	constexpr unary_list_parser() =default ;
	constexpr unary_list_parser(parser p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		if(!src) return -1;
		auto ret = call_parse(ctx, src, result);
		src += ret * (0<=ret);
		auto cur_r = ret;
		while(src && 0<cur_r) {
			cur_r = call_parse(ctx, src, result);
			ret += cur_r * (0<=cur_r);
			src += cur_r * (0<=cur_r);
		}
		if(src) pop(result);
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return p.parse(ctx, src, result.o);
		else return p.parse(ctx, src, empback(result));
	}
};

template<parser p> constexpr auto operator+(p&& _p) {
	return unary_list_parser<std::decay_t<p>>{ _p };
}

template<parser p> constexpr auto operator*(p&& _p) {
	return -( +(std::forward<decltype(_p)>(_p)) );
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs {

template<parser left, parser right>
struct binary_list_parser : base_parser<binary_list_parser<left, right>> {
	left lp;
	right rp;
	constexpr binary_list_parser(left l, right r) : lp(l), rp(r) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_any_eq_allow fake_result;
		auto ret = call_parse(ctx, src, result);
		if(ret<0) pop(result);
		auto cur = ret;
		while(cur > 0) {
			src += cur;
			auto r_res = rp.parse(ctx, src, fake_result);
			if( r_res <= 0 ) break;
			src += r_res;
			cur = call_parse(ctx, src, result);
			if( cur <= 0 ) {
				pop(result);
				break;
			}
			ret += cur + r_res;
		}
		return ret;
	}
	constexpr auto call_parse(auto&& ctx, auto&& src, auto& result) const {
		constexpr bool is_fwd = is_specialization_of<std::decay_t<decltype(result)>, forwarder>;
		if constexpr (is_fwd) return lp.parse(ctx, src, result.o);
		else return lp.parse(ctx, src, empback(result));
	}
};

template<parser left, parser right> constexpr auto operator%(left&& l, const right& r) {
	return binary_list_parser( std::forward<decltype(l)>(l), std::forward<decltype(r)>(r) );
}

constexpr auto operator%(parser auto&& l, char r) {
	return binary_list_parser( std::forward<decltype(l)>(l), value_parser{r} );
}

} // namespace ascip_details::prs

namespace ascip_details {

template<typename mutator, typename left_type, typename right_type>
constexpr static auto transform(prs::binary_list_parser<left_type, right_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto lp = transform<mutator>(std::move(src.lp), nctx);
	auto rp = transform<mutator>(std::move(src.rp), nctx);
	return transform_apply<mutator>(prs::binary_list_parser( std::move(lp), std::move(rp) ), nctx);
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

template<parser parser> struct reparse_parser : base_parser<reparse_parser<parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto r = p.parse(static_cast<decltype(ctx)&&>(ctx), src, result);
		return r * (r<0);
	}
};

template<parser type> constexpr auto reparse(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return reparse_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_reparse() {
	static_assert( []{
		char r;
		auto p = (reparse(a::any) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==1) + 2*(r=='a');
	}() == 3);
	static_assert( []{
		char r='z';
		auto p = (reparse(t<'b'>::char_) >> a::any).parse(a::make_test_ctx(), a::make_source("ab"), r);
		return (p==-1) + 2*(r=='z');
	}() == 3);
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<typename good_result, parser parser>
struct result_checker_parser : base_parser<result_checker_parser<good_result, parser>> {
	parser p;
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		static_assert(
			   std::is_same_v<type_any_eq_allow, std::decay_t<decltype(result)>>
			|| std::is_same_v<good_result, std::decay_t<decltype(result)>>
			, "can only parser to required type" );
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(auto(src))&&>(src), result);
	}
};
template<typename needed, parser type> struct cast_parser : base_parser<cast_parser<needed,type>> {
	type p;
	constexpr static auto& check_result(auto& result) {
		if constexpr( std::is_same_v<std::decay_t<decltype(result)>, type_any_eq_allow> ) return result;
		else {
			static_assert(requires{ static_cast<needed&>(result); }, "the result must to be castable to needed type" );
			return static_cast<needed&>(result);
		}
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(ctx, src, check_result(result));
	}
};

template<typename result, parser type> constexpr auto check(type&& p) {
	return result_checker_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename result, parser type> constexpr auto cast(type&& p){
	return cast_parser<result, std::decay_t<type>>{ {}, std::forward<decltype(p)>(p) };
}

template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_checkers() {
  constexpr auto ca = t<'a'>::char_;
	//NOTE: for c++23 we can only test if it works and cannot test if it fails
	static_assert( requires(char& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(char& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_any_eq_allow& r) {
		check<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	static_assert( requires(type_any_eq_allow& r) {
		cast<char>(ca).parse(make_test_ctx(),a::make_source('a'),r);
		});
	return true;
}

}

namespace ascip_details {
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::cast_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(cast<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
template<typename mutator, typename type, typename parser>
constexpr static auto transform_special(prs::result_checker_parser<type,parser>&& src, auto& ctx) {
	return transform_apply<mutator>(check<type>(transform<mutator>(std::move(src.p), ctx)), ctx);
}
}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

struct rvariant_stack_tag {};
struct rvariant_cpy_result_tag {};

template<typename, parser...> struct rvariant_parser;

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs::rv_utils {

template<typename source, typename result> struct monomorphic {
  virtual ~monomorphic() =default ;
  virtual parse_result parse_mono(int ind, source src) const =0 ;
  virtual parse_result parse_mono(int ind, source src, result& r) const =0 ;
};

template<auto parsers_count, typename parser, typename context, typename source, typename result>
struct mono_for_rv final : monomorphic<source, result> {
	using base_type = monomorphic<source, result>;
	const parser* self;
	mutable context ctx;
	constexpr mono_for_rv(const parser* self, context ctx) : self(self), ctx(std::move(ctx)) {}
	template<auto ind> constexpr parse_result call(source src, auto& r) const {
		return self->template parse_without_prep<ind>(ctx, src, r);
	}
	template<auto cur> constexpr parse_result call(const int ind, source src, auto& r) const {
		if constexpr(cur==parsers_count) return -1;
		else return cur==ind ? call<cur>(std::move(src), r) : call<cur+1>(ind, std::move(src), r);
	}
	constexpr parse_result parse_mono(const int ind, source src) const override {
		type_any_eq_allow r;
		return call<0>(ind, std::move(src), r);
	}
	constexpr parse_result parse_mono(const int ind, source src, result& r) const override {
		return call<0>(ind, std::move(src), r);
	}
};

constexpr auto mk_mono(const auto* parser, auto ctx, [[maybe_unused]] auto src, [[maybe_unused]] auto& result) {
	using parser_t = std::decay_t<decltype(*parser)>;
	using result_t = std::decay_t<decltype(result)>;
	return mono_for_rv<parser_t::size(), parser_t, decltype(ctx), decltype(src), result_t>( parser, ctx );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

struct rvariant_lreq_parser : base_parser<rvariant_lreq_parser> {
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		constexpr bool need_in_result = !std::is_same_v<decltype(result), type_any_eq_allow&> ;
		if constexpr (need_in_result) {
			result = std::move(*search_in_ctx<rvariant_cpy_result_tag>(ctx));
			search_in_ctx<rvariant_cpy_result_tag>(ctx) = nullptr;
		}
		return 0;
	}
};

template<auto stop_ind>
struct rvariant_rreq_parser : base_parser<rvariant_rreq_parser<stop_ind>> {
    constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
        if(!src) return 0;
        auto* var = *search_in_ctx<rvariant_stack_tag>(ctx);
        if constexpr(type_dc<decltype(result)> == type_c<type_any_eq_allow>)
            return var->parse_mono(stop_ind+1, std::move(src));
        else return var->parse_mono(stop_ind+1, std::move(src), result);
    }
};

struct rvariant_rreq_pl_parser : base_parser<rvariant_rreq_pl_parser> {
	constexpr static parse_result parse(auto&&, auto, auto&) { return 0; }
};

template<auto ind> struct rvariant_req_parser : base_parser<rvariant_req_parser<ind>> {
    constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
        auto* var = *search_in_ctx<rvariant_stack_tag, ind>(ctx);
        if constexpr(type_dc<decltype(result)> == type_c<type_any_eq_allow>)
            return var->parse_mono(0, std::move(src));
        else return var->parse_mono(0, std::move(src), result);
    }
} ;

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)




namespace ascip_details::prs {

template<parser parser> struct rvariant_top_result_parser : base_parser<rvariant_top_result_parser<parser>> { parser p; };
template<typename p> rvariant_top_result_parser(p) -> rvariant_top_result_parser<p>;

template<parser type> constexpr auto rv_result(type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return rvariant_top_result_parser<ptype>{ {}, std::forward<decltype(p)>(p) };
}

template<template<class>class wrapper, parser parser> constexpr static auto is_top_result_parser_helper(const wrapper<parser>& p) -> decltype(p.p);
template<typename parser>
constexpr static bool is_top_result_parser() {
	if constexpr (requires{ is_top_result_parser_helper(std::declval<parser>()); })
		return
			   is_top_result_parser<decltype(is_top_result_parser_helper(std::declval<parser>()))>()
			|| is_specialization_of<parser, rvariant_top_result_parser>;
	else return is_specialization_of<parser, rvariant_top_result_parser>;
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)






namespace ascip_details::prs::rv_utils {

template<typename cur_parser_t> constexpr bool contains_reqursion() {
	auto checker = [](const auto* p){return std::is_same_v<std::decay_t<decltype(*p)>, rvariant_lreq_parser>;};
	auto stop = [](const auto* p){
		const bool is_rv = requires{ p->maker; };
		return is_rv && !is_specialization_of<cur_parser_t, rvariant_parser>;
	};
	return !exists_in((cur_parser_t*)nullptr, checker, stop);
}

}


namespace ascip_details::prs {

template<typename maker_type, parser... parsers>
struct rvariant_parser : base_parser<rvariant_parser<maker_type, parsers...>> {
	tuple<parsers...> seq;
	std::decay_t<maker_type> maker;

	constexpr rvariant_parser( maker_type m, parsers... l ) : seq( std::forward<parsers>(l)... ), maker(std::forward<maker_type>(m)) {}

	constexpr static auto size() { return sizeof...(parsers); }
	template<auto ind> constexpr static bool is_term() { return rv_utils::contains_reqursion<__type_pack_element<ind, parsers...>>(); }
	template<auto ind> consteval static auto cur_ind() {
		using cur_parser_t = std::decay_t<decltype(get<ind>(seq))>;
		if constexpr (is_top_result_parser<cur_parser_t>()) return -1;
		else {
			auto cur = 0;
			auto cnt = 0;
			(void)( ((ind==cnt)||(++cnt,cur+=!is_top_result_parser<parsers>(),false)) || ... );
			return cur;
		}
	}
	constexpr auto move_result(auto& result) const {
		if constexpr (std::is_same_v<decltype(result), type_any_eq_allow&>) return result;
		else return maker(result);
	}
	template<auto ind> constexpr parse_result parse_term(auto&& ctx, auto src, auto& result) const {
		if constexpr (ind == 0) {
			if constexpr (is_term<ind>()) return get<ind>(seq).parse(ctx, src, result);
			else return -1;
		}
		else if constexpr (!is_term<ind>()) return parse_term<ind-1>(ctx, src, result);
		else {
			auto cur = get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
			if(0 <= cur) return cur;
			if constexpr (ind==0) return cur;
			else return parse_term<ind-1>(ctx, src, result);
		}
	}
	template<auto ind, auto stop_pos> constexpr parse_result parse_nonterm(auto&& ctx, auto src, auto& result, auto shift) const {
		if(!src) return shift;
		if constexpr (ind < stop_pos) return shift;
		else if constexpr (is_term<ind>()) {
			if constexpr (ind == 0) return 0;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, shift);
		}
		else {
			type_any_eq_allow result_for_check;
			auto pr = get<ind>(seq).parse(ctx, src, result_for_check);
			decltype(pr) prev_pr = 0;
			while(0 < pr) {
				prev_pr += pr;
				auto cur = move_result(result);
				if constexpr (!std::is_same_v<decltype(result), type_any_eq_allow&>)
					search_in_ctx<rvariant_cpy_result_tag>(ctx) = &cur;
				src += get<ind>(seq).parse(ctx, src, variant_result<cur_ind<ind>()>(result));
				pr = get<ind>(seq).parse(ctx, src, result_for_check);
			}
			auto total_shift = shift + (prev_pr*(prev_pr>0));
			if constexpr (ind==0) return total_shift;
			else return parse_nonterm<ind-1, stop_pos>(ctx, src, result, total_shift);
		}
	}
	template<auto stop_pos> constexpr parse_result parse_without_prep(auto&& ctx, auto src, auto& result) const {
		auto term_r = parse_term<sizeof...(parsers)-1>(ctx, src, result);
		if(term_r < 0) return term_r;
		auto nonterm_r = parse_nonterm<sizeof...(parsers)-1, stop_pos>(ctx, src += term_r, result, 0);
		return term_r + (nonterm_r*(nonterm_r>0));
	}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		using copied_result_type = decltype(move_result(result));
		using mono_type = rv_utils::monomorphic<decltype(src), std::decay_t<decltype(result)>>;
		const mono_type* mono_ptr;
		auto rctx = make_ctx<rvariant_stack_tag>(&mono_ptr, make_ctx<rvariant_cpy_result_tag>((copied_result_type*)nullptr, ctx) );
		auto mono = rv_utils::mk_mono(this, rctx, src, result);
		mono_ptr = &mono;
		return mono.parse_mono(0, src, result);
	}
};

template<typename... t> rvariant_parser(t...) ->  rvariant_parser<t...>;

}

namespace ascip_details::prs {

template<auto ind>
struct rvariant_mutator {
	struct context{};
	constexpr static auto create_ctx() { return context{}; }
	constexpr static auto create_ctx(auto&&,auto&&) { return context{}; }
	template<typename type>
	constexpr static auto apply(rvariant_rreq_pl_parser&&,auto&&) {
		return rvariant_rreq_parser<ind>{};
	}
};

template<parser type, parser... types> constexpr auto rv(auto&& maker, type&& first, types&&... list) {
	return [&maker]<auto... inds>(std::index_sequence<inds...>, auto&&... parsers) {
		return rvariant_parser(
				std::move(maker),
				transform<rvariant_mutator<inds>>(std::move(parsers))...
				);
	}(
			std::make_index_sequence<sizeof...(list)+1>{}
		, std::forward<decltype(first)>(first)
		, std::forward<decltype(list)>(list)...
	);
}

}

       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details {

template<typename mutator, typename type, typename... parsers>
constexpr static auto transform(prs::rvariant_parser<type, parsers...>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	return transform_apply<mutator>( transform_apply_to_each<mutator,prs::rvariant_parser,0>( std::move(src.seq),nctx,std::move(src.maker)), nctx );
}

}

namespace ascip_details::prs {



template<typename p, template<auto>class t=p::template tmpl>
constexpr auto test_rvariant_simple(auto r, auto&& src, auto&&... parsers) {
	auto var = rv([](auto& r){return &r;}, parsers...);
	var.parse(make_test_ctx(), make_source(src), r);
	static_assert( var.template is_term<0>() );
	static_assert( var.template is_term<1>() );
	auto var_with_skip = inject_skipping(var, +p::space);
	static_assert( var_with_skip.template is_term<0>() );
	static_assert( var_with_skip.template is_term<1>() );
	return r;
}

template<typename p>
constexpr bool test_rvariant() {
	static_assert( test_rvariant_simple<p>(int{}, "123", p::int_, p::fp) == 123 );
	static_assert( test_rvariant_simple<p>(double{}, "1.2", p::int_, p::fp) == 1.2 );
	return true;
}

} // namespace ascip_details::prs
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)





namespace ascip_details::prs {

template<typename parser, typename tag, typename value_type> struct ctx_change_parser : base_parser<ctx_change_parser<parser, tag, value_type>> {
	value_type value;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto new_ctx = make_ctx<tag>(value, ctx);
		return p.parse(new_ctx, src, result);
	}
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_parser : base_parser<ctx_use_parser<parser, act_type, tags...>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, result);
		if(-1 < ret) act(result, search_in_ctx<tags, 0>(ctx)...);
		return ret;
	}
};

template<typename parser, typename act_type, typename... tags> struct ctx_use_as_result_parser : base_parser<ctx_use_as_result_parser<parser, act_type, tags...>> {
	act_type act;
	[[no_unique_address]] parser p;

	constexpr const parse_result parse(auto&& ctx, auto src, auto& result) const {
		auto ret = p.parse(ctx, src, search_in_ctx<tags, 0>(ctx)...);
		if(-1 < ret) act(result, search_in_ctx<tags, 0>(ctx)...);
		return ret;
	}
};

} // namespace ascip_details::prs

namespace ascip_details {

template<typename tag, typename value_type, parser type> constexpr auto add_to_ctx(value_type&& value, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	return prs::ctx_change_parser<type, tag, value_type>{ {}, std::forward<decltype(value)>(value), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto from_ctx(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return prs::ctx_use_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}
template<typename... tags, parser type> constexpr auto result_from_ctx(auto&& act, type&& p) {
	using ptype = std::decay_t<decltype(p)>;
	using act_type = std::decay_t<decltype(act)>;
	return prs::ctx_use_as_result_parser<ptype, act_type, tags...>{ {}, std::forward<decltype(act)>(act), std::forward<decltype(p)>(p) };
}

template<typename mutator, typename parser, typename tag, typename value_type>
constexpr static auto transform(prs::ctx_change_parser<parser, tag, value_type>&& src, auto& ctx) {
	auto nctx = mutator::create_ctx(src, ctx);
	auto np = transform<mutator>( std::move(src.p), nctx );
	return transform_apply<mutator>( prs::ctx_change_parser<decltype(np), tag, value_type>{ {}, std::move(src.value), std::move(np) }, nctx );
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)



namespace ascip_details::prs {

template<parser pt> struct lexeme_parser : base_parser<lexeme_parser<pt>> { [[no_unique_address]] pt p;
	constexpr lexeme_parser() =default ;
	constexpr lexeme_parser(lexeme_parser&&) =default ;
	constexpr lexeme_parser(const lexeme_parser&) =default ;
	constexpr lexeme_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};
template<parser pt> struct skip_parser : base_parser<skip_parser<pt>> { [[no_unique_address]] pt p;
	constexpr skip_parser() =default ;
	constexpr skip_parser(skip_parser&&) =default ;
	constexpr skip_parser(const skip_parser&) =default ;
	constexpr skip_parser(pt p) : p(std::move(p)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		return p.parse(static_cast<decltype(ctx)&&>(ctx), static_cast<decltype(src)&&>(src), result);
	}
};

template<parser skip, parser base> struct injected_parser : base_parser<injected_parser<skip,base>> {
	[[no_unique_address]] skip s;
	[[no_unique_address]] base b;
	constexpr injected_parser() noexcept =default ;
	constexpr injected_parser(injected_parser&&) noexcept =default ;
	constexpr injected_parser(const injected_parser&) noexcept =default ;
	constexpr injected_parser(skip s, base b) : s(std::forward<decltype(s)>(s)), b(std::forward<decltype(b)>(b)) {}
	constexpr parse_result parse(auto&& ctx, auto src, auto& result) const {
		type_any_eq_allow skip_result;
		auto sr = s.parse(ctx, src, skip_result);
		sr *= (0<=sr);
		src += sr;
		auto mr = src ? b.parse(ctx, src, result) : -1;
		return (sr * (0<=mr)) + mr; // 0<=mr ? mr+sr : mr;
	}
};

} // namespace ascip_details::prs

namespace ascip_details {

template<parser type> constexpr auto lexeme(type&& p) {
	return prs::lexeme_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

template<parser type> constexpr auto skip(type&& p) {
	return prs::skip_parser<std::decay_t<type>>{ std::forward<decltype(p)>(p) };
}

}
       

//          Copyright Hudyaev Alexey 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)









namespace ascip_details::prs {

template<typename skip_type>
struct injection_mutator {
	struct context{ };
	struct in_lexeme_context{ };
	struct in_lexeme_context_deep{ };
	template<typename ctx> constexpr static bool check_inside_lexeme() {
		using t = std::decay_t<ctx>;
		return std::is_same_v<t, in_lexeme_context> || std::is_same_v<t,  in_lexeme_context_deep>;
	}
	constexpr static auto create_ctx(){ return context{}; }
	constexpr static auto create_ctx(auto&& src, auto&& ctx) {
		if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, skip_parser>)
			return context{};
		else if constexpr (ascip_details::is_specialization_of<std::decay_t<decltype(src)>, lexeme_parser>) {
			if constexpr (check_inside_lexeme<decltype(ctx)>()) return in_lexeme_context_deep{};
			else return in_lexeme_context{ };
		}
		else return ctx;
	}

	template<typename type>
	constexpr static auto apply(auto&& p, auto& ctx) {
		constexpr const bool is_inside_lexeme = check_inside_lexeme<decltype(ctx)>();
		constexpr const bool is_inside_lexeme_deep = std::is_same_v<std::decay_t<decltype(ctx)>, in_lexeme_context_deep>;
		constexpr const bool is_parser_lexeme = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, lexeme_parser>;
		constexpr const bool is_parser_skip = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, skip_parser>;

		constexpr const bool is_parser_variant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, variant_parser>;
		constexpr const bool is_parser_rvariant = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, rvariant_parser>;
		constexpr const bool is_parser_blist = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, binary_list_parser>;
		constexpr const bool is_parser_diff = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, different_parser>;
		constexpr const bool is_opt_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, opt_seq_parser>;
		constexpr const bool is_num_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_num_rfield_val>;
		constexpr const bool is_inc_seq_parser = ascip_details::is_specialization_of<std::decay_t<decltype(p)>, seq_inc_rfield_val>;

		constexpr const bool is_parser_for_skip =
			   is_opt_seq_parser
			|| is_parser_variant
			|| is_parser_rvariant
			|| is_parser_blist
			|| is_parser_diff
			|| is_num_seq_parser
			|| is_inc_seq_parser
			;

		if constexpr (is_parser_lexeme && is_inside_lexeme_deep) return p.p;
		else if constexpr (is_parser_lexeme)
			return injected_parser<skip_type, std::decay_t<decltype(p.p)>>( skip_type{}, std::move(p.p) );
		else if constexpr (is_inside_lexeme || is_parser_for_skip) return p;
		else if constexpr (is_parser_skip) return p.p;
		else if constexpr ( requires{ p.p; } ) return p;
		else return injected_parser<skip_type, std::decay_t<decltype(p)>>( skip_type{}, std::move(p) );
	}
};

}

namespace ascip_details {

template<parser parser_type, parser skipper>
constexpr auto inject_skipping(parser_type&& to, skipper&& what) {
	using mutator = prs::injection_mutator<std::decay_t<skipper>>;
	auto ret = transform<mutator>(std::move(to));
	if constexpr(ascip_details::is_specialization_of<std::decay_t<decltype(ret)>, prs::injected_parser>)
		return ret.b;
	else return ret;
}

template<parser p1, parser p2> constexpr auto make_injected(const p1& l, const p2& r) {
	return prs::injected_parser<p1, p2>( l, r );
}

namespace prs {
template<typename a, template<auto>class t=a::template tmpl>
constexpr static bool test_injection_parser() {
	constexpr auto cs = t<' '>::char_;
	constexpr auto ca = t<'a'>::char_;

	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	}) == 2, "inejction parser can parse");
	static_assert( ({ char r='z'; make_injected(cs, ca).parse(make_test_ctx(), make_source("  b"), r);
	}) == -1, "inejction parser cannot parse invalid data");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source(" a"), r);
	r;}) == 'a', "the first parameter of inejction parser skipped");
	static_assert( ({ char r='z';
		make_injected(cs, ca).parse(make_test_ctx(), make_source("ba"), r);
	}) == -1, "inejction parser fails if cannot parse first parameter");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("a"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");
	static_assert( [&]{ char r='z';
		auto pr=make_injected(cs, ca).parse(make_test_ctx(), make_source("aa"), r);
		return (pr==1) + (2*(r=='a'));}() == 3, "inejction parser parse if only first parameter fails");

	return true;
}
template<typename a, auto p1, auto p2, template<auto>class tt=a::template tmpl>
constexpr static bool test_seq_injection() {
	//TODO: test something like skip(++lexeme(parser))([](auto& v){return &v;})
	constexpr auto cs = tt<' '>::char_;
	constexpr auto ca = tt<'a'>::char_;
	constexpr auto cb = tt<'b'>::char_;

	using p1_t = decltype(auto(p1));
	using p2_t = decltype(auto(p2));
	using inj_t = injected_parser<p2_t,p1_t>;

	(void)static_cast<const p1_t&>(inject_skipping(p1, p2));
	(void)static_cast<const opt_seq_parser<inj_t, inj_t>&>(inject_skipping(p1 >> p1, p2));

	(void)static_cast<const opt_seq_parser<p1_t, p1_t, p1_t>&>(inject_skipping(lexeme(p1 >> p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<p1_t, p1_t>>&>(inject_skipping(lexeme(p1 >> lexeme(p1 >> p1)), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(p1 >> p1))), p2));
	(void)static_cast<const opt_seq_parser<p1_t, injected_parser<p2_t, opt_seq_parser<inj_t, inj_t>>>&>(inject_skipping(lexeme(p1 >> skip(lexeme(skip(p1 >> p1)))), p2));
	(void)static_cast<const opt_seq_parser<inj_t, injected_parser<p2_t, opt_seq_parser<p1_t, p1_t>>>&>(inject_skipping(lexeme(p1) >> lexeme(p1 >> p1), p2));
	(void)static_cast<const opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>&>(
			inject_skipping(lexeme(p1 >> skip(p1 >> p1)), p2));

	constexpr auto lambda_ret_val = [](auto&v){return &v;};
	using lambda_ret_val_t = std::decay_t<decltype(lambda_ret_val)>;
	(void)static_cast<const semact_parser<injected_parser<p2_t, opt_seq_parser<p1_t, opt_seq_parser<inj_t, inj_t>>>, lambda_ret_val_t>&>(inject_skipping(lexeme(p1 >> skip(p1 >> lexeme(p1)))(lambda_ret_val), p2));

	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		cast_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|cast<char>(p1), p2 ));

	(void)static_cast<const unary_list_parser<inj_t>&>(inject_skipping( +p1, p2 ));
	(void)static_cast<const unary_list_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( +(p1>>p1), p2 ));

	(void)static_cast<const opt_parser<inj_t>&>(inject_skipping( -p1, p2 ));
	(void)static_cast<const opt_parser<opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( -(p1>>p1), p2 ));

	(void)static_cast<const different_parser<inj_t, inj_t>&>(inject_skipping( p1 - p1, p2 ));
	(void)static_cast<const opt_seq_parser<
		inj_t,
		different_parser<
			opt_seq_parser<inj_t,inj_t>,
			inj_t
		>
		>&>(inject_skipping( p1 >> ((p1>>p1) - p1), p2 ));


	/* TODO: remove support of lambda in seq?
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb >> [&t](...){++t;return 0;};
		p.parse(make_test_ctx(), make_source("ab"), r);
	t;}) == 1, "injection works win lambda in seq");
	static_assert( ({ char r='z'; int t=0;
		auto p = ca >> cb([&t](...){++t;});
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cb([&t](...){++t;}), +a::space);
		p.parse(make_test_ctx(), make_source("ab"), r);
	t; }) == 1, "injection works with semact" );
	*/
	static_assert( ({ char r='z'; int t=0;
		auto p = inject_skipping(ca >> cast<char>(cb([&t](...){++t;})), +a::space);
		p.parse(make_test_ctx(), make_source("abc"), r);
	t; }) == 1, "injection works with semact" );


	(void)static_cast<const variant_parser<inj_t,inj_t>&>(inject_skipping( p1|p1, p2 ));
	(void)static_cast<const variant_parser<inj_t,inj_t,inj_t>&>(inject_skipping( p1|p1|p1, p2 ));
	(void)static_cast<const variant_parser<
		result_checker_parser<char,inj_t>,
		result_checker_parser<char,inj_t>
		>&>(inject_skipping( check<char>(p1)|check<char>(p1), p2 ));

	(void)static_cast<const result_checker_parser<int, inj_t>&>(inject_skipping( check<int>(p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,inj_t>>&>(inject_skipping( check<int>(p1 >> p1), p2 ));
	(void)static_cast<const result_checker_parser<int, opt_seq_parser<inj_t,seq_num_rfield_val<inj_t, _seq_rfield_val<2>>>>&>(inject_skipping( check<int>(p1 >> fnum<2>(p1)), p2 ));
	{
		auto rmaker = [](auto& r){ r.reset(new (std::decay_t<decltype(*r)>){}); return r.get(); };
		auto var = rv(rmaker, p1, p1);
		(void)static_cast<rvariant_parser<decltype(rmaker), p1_t, p1_t>&>(var);
		(void)static_cast<const rvariant_parser<decltype(rmaker), inj_t, inj_t>&>(inject_skipping(var, p2));
	}


	static_assert( ({ char r{}; const auto parser = +a::alpha; const auto skipper = +a::space;
		inject_skipping(parser, skipper).parse(make_test_ctx(), make_source(" a b c "), r); }) == -1 );

	(void)static_cast<const binary_list_parser<inj_t, inj_t>&>(inject_skipping( p1 % p1, p2 ));

	return true;
}

template<typename a>
constexpr static bool test_injection() {
	return test_injection_parser<a>() && test_seq_injection<a, a::template char_<'a'>, a::space>();
}

}
}

struct ascip {
  using parse_result = ascip_details::parse_result;
  template<typename t> using base_parser = ascip_details::prs::base_parser<t>;

  static constexpr auto nop = ascip_details::prs::nop_parser{};
  template<auto s> static constexpr auto char_ = ascip_details::prs::char_parser<s>{};
  template<auto val> constexpr static auto _char = omit(char_<val>);
  template<ascip_details::string_literal s> static constexpr auto lit = ascip_details::prs::literal_parser<s>{};
  static constexpr auto space = ascip_details::prs::space_parser{};
  static constexpr auto any = ascip_details::prs::any_parser{};
  constexpr static auto nl = _char<'\n'>;

  static constexpr auto int_ = ascip_details::prs::int_parser{};
  template<auto base=10> constexpr static auto uint_ = ascip_details::prs::uint_parser<base>{};
  static constexpr auto fp = ascip_details::prs::float_point_parser{};

  // ranges
  constexpr static auto lower = ascip_details::prs::range_parser<'a','z'>{};
  constexpr static auto upper = ascip_details::prs::range_parser<'A','Z'>{};
  constexpr static auto digit = ascip_details::prs::range_parser<'0','9'>{};
  constexpr static auto d10 = ascip_details::prs::range_parser<'0', '9'>{};
  constexpr static auto ascii = ascip_details::prs::range_parser<(char)0x01,(char)0x7F>{};
  constexpr static auto alpha = lower | upper;

  // sequence
  constexpr static auto cur_pos = ascip_details::prs::cur_pos_parser{};
  constexpr static auto cur_shift = ascip_details::prs::cur_shift_parser{};
  template<auto ind> constexpr static auto req = ascip_details::prs::seq_reqursion_parser<ind>{};
  constexpr static ascip_details::prs::seq_inc_rfield sfs{} ;

  // functions
  constexpr static auto make_test_ctx(auto&&... args) { return ascip_details::make_test_ctx(std::forward<decltype(args)>(args)...); }
  constexpr static auto make_source(auto&& src) { return ascip_details::make_source(std::forward<decltype(src)>(src)); }
  constexpr static auto inject_skipping(auto&& p, auto&& s) { return ascip_details::inject_skipping(std::forward<decltype(p)>(p), std::forward<decltype(s)>(s) ); }
  constexpr static auto fwd(auto& o) { return ascip_details::fwd( o ); }
  constexpr static void write_out_error_msg(auto& os, auto fn, auto msg, auto expt, auto src, auto ln ) {
    ascip_details::write_out_error_msg(os, std::move(fn), std::move(msg), std::move(expt), std::move(src), std::move(ln));
  }

  // rvariant
  constexpr static ascip_details::prs::rvariant_lreq_parser rv_lreq{};
  constexpr static ascip_details::prs::rvariant_rreq_pl_parser rv_rreq{};
  template<auto stop_ind> constexpr static ascip_details::prs::rvariant_rreq_parser<stop_ind> _rv_rreq{};
  template<auto ind> constexpr static auto rv_req = ascip_details::prs::rvariant_req_parser<ind>{};

  constexpr static auto dquoted_string = lexeme(_char<'"'> >> *(as<'"'>(char_<'\\'> >> char_<'"'>)| (ascip::any - char_<'"'>)) >> _char<'"'>);
  constexpr static auto squoted_string = lexeme(_char<'\''> >> *(as<'\''>(char_<'\\'> >> char_<'\''>)| (ascip::any - char_<'\''>)) >> _char<'\''>);
  constexpr static auto quoted_string = lexeme(squoted_string | dquoted_string);

  // helpers
  template<auto s> struct tmpl {
    constexpr static auto& nop = ascip::nop;
    constexpr static auto& char_ = ascip::char_<s>;
    constexpr static auto& _char = ascip::_char<s>;
    constexpr static auto& space = ascip::space;
    constexpr static auto& any = ascip::any;
    constexpr static auto& nl = ascip::nl;
    constexpr static auto& req = ascip::req<s>;
    constexpr static auto& _rv_rreq = ascip::_rv_rreq<s>;
    constexpr static auto& rv_req = ascip::rv_req<s>;
    constexpr static auto& uint_ = ascip::uint_<s>;
  };
};

template<typename parser> constexpr auto ascip_details::prs::base_parser<parser>::operator()(auto act) const {
  return semact_parser<parser, decltype(auto(act))>{ {},
    static_cast<decltype(act)&&>(act),
    static_cast<const parser&>(*this)
  };
}
