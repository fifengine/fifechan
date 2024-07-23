// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_VERSION_HPP_
#define INCLUDE_FIFECHAN_VERSION_HPP_

/** These version numbers should be checked and updated
 * as part of the release process for Fifechan.
 */
#ifndef FCN_MAJOR_VERSION
#    define FCN_MAJOR_VERSION 0
#endif
#ifndef FCN_MINOR_VERSION
#    define FCN_MINOR_VERSION 1
#endif
#ifndef FCN_PATCH_VERSION
#    define FCN_PATCH_VERSION 5
#endif

/** Types
 *  0 = none (pre-release info is not appended to the version in this case)
 *  1 = alpha
 *  2 = beta
 *  3 = rc
 */
#ifndef FCN_PRERELEASE_TYPE
#    define FCN_PRERELEASE_TYPE 0
#endif
#ifndef FCN_PRERELEASE_VERSION
#    define FCN_PRERELEASE_VERSION 0
#endif

/***************************************************************************
 * Do not update anything below this line!
 ***************************************************************************/

#define FCN_STR(s)  #s
#define FCN_XSTR(s) FCN_STR(s)

#define FCN_DOT(a, b)  a.b
#define FCN_XDOT(a, b) FCN_DOT(a, b)

#define FCN_PLUS(a, b)  ((a) + (b))
#define FCN_XPLUS(a, b) FCN_PLUS(a, b)

#define FCN_MINUS(a, b)  ((a) - (b))
#define FCN_XMINUS(a, b) FCN_MINUS(a, b)

#if FCN_PRERELEASE_TYPE == 1
#    define FCN_PRERELEASE alpha
#elif FCN_PRERELEASE_TYPE == 2
#    define FCN_PRERELEASE beta
#elif FCN_PRERELEASE_TYPE == 3
#    define FCN_PRERELEASE rc
#endif

#if FCN_PRERELEASE_VERSION > 0
#    ifdef FCN_PRERELEASE
#        define FCN_PRERELEASE_STR FCN_XDOT(FCN_PRERELEASE, FCN_PRERELEASE_VERSION)
#    endif
#endif

#define FCN_VERSION FCN_XDOT(FCN_XDOT(FCN_MAJOR_VERSION, FCN_MINOR_VERSION), FCN_PATCH_VERSION)

#ifdef FCN_PRERELEASE_STR
#    define FCN_VERSION_STRING FCN_XMINUS(FCN_VERSION, FCN_PRERELEASE_STR)
#endif
#ifdef FCN_GIT_HASH
#    ifndef FCN_VERSION_STRING
#        define FCN_VERSION_STRING FCN_XPLUS(FCN_VERSION, FCN_GIT_HASH)
#    else
#        undef FCN_VERSION_STRING
#        ifdef FCN_PRERELEASE_STR
#            define FCN_VERSION_STRING FCN_XMINUS(FCN_VERSION, FCN_XPLUS(FCN_PRERELEASE_STR, FCN_GIT_HASH))
#        else
#            define FCN_VERSION_STRING FCN_XPLUS(FCN_VERSION, FCN_GIT_HASH)
#        endif
#    endif
#else
#    define FCN_GIT_HASH ""
#endif

// This is an actual release
#ifndef FCN_VERSION_STRING
#    define FCN_VERSION_STRING FCN_VERSION
#endif

/** All Fifechan related code is in this namespace.
 */
namespace fcn
{
    inline char const * getVersion()
    {
        return FCN_XSTR(FCN_VERSION_STRING);
    }

    inline int getMajor()
    {
        return FCN_MAJOR_VERSION;
    }

    inline int getMinor()
    {
        return FCN_MINOR_VERSION;
    }

    inline int getPatch()
    {
        return FCN_PATCH_VERSION;
    }

    inline char const * getHash()
    {
        return FCN_XSTR(FCN_GIT_HASH);
    }
} // namespace fcn

// cleanup
#undef FCN_STR
#undef FCN_XSTR
#undef FCN_DOT
#undef FCN_XDOT
#undef FCN_PLUS
#undef FCN_XPLUS
#undef FCN_MINUS
#undef FCN_XMINUS
#undef FCN_VERSION_STRING
#undef FCN_VERSION
#undef FCN_PRERELEASE
#undef FCN_PRERELEASE_STR

#endif // INCLUDE_FIFECHAN_VERSION_HPP_
