// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_PLATFORM_HPP
#define FCN_PLATFORM_HPP

#if defined(__MINGW32__) && defined(FIFECHAN_BUILD)
#define FCN_CORE_DECLSPEC __declspec(dllexport)

#elif defined(__MINGW32__) && defined(FIFECHAN_EXTENSION_BUILD)
#define FCN_EXTENSION_DECLSPEC __declspec(dllexport)
#define FCN_CORE_DECLSPEC      __declspec(dllimport)

#elif defined(__MINGW32__) && defined(FIFECHAN_DLL_IMPORT)
#define FCN_CORE_DECLSPEC      __declspec(dllimport)
#define FCN_EXTENSION_DECLSPEC __declspec(dllimport)

#elif defined(_MSC_VER) && defined(FIFECHAN_BUILD)
#define FCN_CORE_DECLSPEC _declspec(dllexport)

#elif defined(_MSC_VER) && defined(FIFECHAN_EXTENSION_BUILD)
#define FCN_CORE_DECLSPEC      _declspec(dllimport)
#define FCN_EXTENSION_DECLSPEC _declspec(dllexport)

#endif

#ifndef FCN_CORE_DECLSPEC
#define FCN_CORE_DECLSPEC
#endif

#ifndef FCN_EXTENSION_DECLSPEC
#define FCN_EXTENSION_DECLSPEC
#endif

#ifndef NULL
#define NULL 0
#endif

#endif // end FCN_PLATFORM_HPP
