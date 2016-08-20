/***************************************************************************
 *   Copyright (C) 2012 by the FCNchan team                               *
 *   http://FCNchan.github.com/FCNchan                                   *
 *   This file is part of FCNchan.                                        *
 *                                                                         *
 *   FCNchan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FCN_VERSION_HPP
#define FCN_VERSION_HPP

/** These version numbers should be checked and updated
 * as part of the release process for Fifechan.
 */
#ifndef FCN_MAJOR_VERSION
	#define FCN_MAJOR_VERSION 0
#endif
#ifndef FCN_MINOR_VERSION
	#define FCN_MINOR_VERSION 2
#endif
#ifndef FCN_PATCH_VERSION
	#define FCN_PATCH_VERSION 0
#endif

/** Types
 *  0 = none (pre-release info is not appended to the version in this case)
 *  1 = alpha
 *  2 = beta
 *  3 = rc
 */
#ifndef FCN_PRERELEASE_TYPE
	#define FCN_PRERELEASE_TYPE 0
#endif
#ifndef FCN_PRERELEASE_VERSION
	#define FCN_PRERELEASE_VERSION 0
#endif

/***************************************************************************
 * Do not update anything below this line!
 ***************************************************************************/

#define FCN_STR(s)			# s
#define FCN_XSTR(s)		FCN_STR(s)

#define FCN_DOT(a,b)		a.b
#define FCN_XDOT(a,b)		FCN_DOT(a,b)

#define FCN_PLUS(a,b)		a+b
#define FCN_XPLUS(a,b)		FCN_PLUS(a,b)

#define FCN_MINUS(a,b)		a-b
#define FCN_XMINUS(a,b)	FCN_MINUS(a,b)

#if FCN_PRERELEASE_TYPE==1
	#define FCN_PRERELEASE alpha
#elif FCN_PRERELEASE_TYPE==2
 	#define FCN_PRERELEASE beta
#elif FCN_PRERELEASE_TYPE==3
	#define FCN_PRERELEASE rc
#endif

#if FCN_PRERELEASE_VERSION>0
	#ifdef FCN_PRERELEASE
		#define FCN_PRERELEASE_STR \
			FCN_XDOT( \
				FCN_PRERELEASE, \
				FCN_PRERELEASE_VERSION \
			)
	#endif
#endif

#define FCN_VERSION \
	FCN_XDOT( \
		FCN_XDOT(FCN_MAJOR_VERSION, FCN_MINOR_VERSION), \
		FCN_PATCH_VERSION \
	)

#ifdef FCN_PRERELEASE_STR
	#define FCN_VERSION_STRING \
		FCN_XMINUS( \
			FCN_VERSION, \
			FCN_PRERELEASE_STR \
		)
#endif
#ifdef FCN_GIT_HASH
	#ifndef FCN_VERSION_STRING
		#define FCN_VERSION_STRING \
			FCN_XPLUS( \
				FCN_VERSION, \
				FCN_GIT_HASH \
			)
	#else
		#undef FCN_VERSION_STRING
		#ifdef FCN_PRERELEASE_STR
			#define FCN_VERSION_STRING \
				FCN_XMINUS( \
					FCN_VERSION, \
					FCN_XPLUS( \
						FCN_PRERELEASE_STR, \
						FCN_GIT_HASH \
					) \
				)
		#else
			#define FCN_VERSION_STRING \
				FCN_XPLUS( \
					FCN_VERSION, \
					FCN_GIT_HASH \
				)
		#endif
	#endif
#else
	#define FCN_GIT_HASH ""
#endif


// This is an actual release
#ifndef FCN_VERSION_STRING
	#define FCN_VERSION_STRING FCN_VERSION
#endif

/** All Fifechan related code is in this namespace.
 */
namespace fcn {
	inline const char* getVersion() {
		return FCN_XSTR(FCN_VERSION_STRING);
	}

	inline int getMajor() {
		return FCN_MAJOR_VERSION;
	}

	inline int getMinor() {
		return FCN_MINOR_VERSION;
	}

	inline int getPatch() {
		return FCN_PATCH_VERSION;
	}

	inline const char* getHash() {
		return FCN_XSTR(FCN_GIT_HASH);
	}
} //fcn

//cleanup
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

#endif //FCN_VERSION_HPP

