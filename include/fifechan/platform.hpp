// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_PLATFORM_HPP
#define FCN_PLATFORM_HPP

#if defined(__MINGW32__) && defined(FIFECHAN_BUILD)
#define FIFEGUI_API __declspec(dllexport)

#elif defined(__MINGW32__) && defined(FIFECHAN_EXTENSION_BUILD)
#define FIFEGUI_EXT_API __declspec(dllexport)
#define FIFEGUI_API __declspec(dllimport)

#elif defined(__MINGW32__) && defined(FIFECHAN_DLL_IMPORT)
#define FIFEGUI_API __declspec(dllimport)
#define FIFEGUI_EXT_API __declspec(dllimport)

#elif defined(_MSC_VER) && defined(FIFECHAN_BUILD)
#define FIFEGUI_API _declspec(dllexport)

#elif defined(_MSC_VER) && defined(FIFECHAN_EXTENSION_BUILD)
#define FIFEGUI_API _declspec(dllimport)
#define FIFEGUI_EXT_API _declspec(dllexport)

#endif

#ifndef FIFEGUI_API
#define FIFEGUI_API
#endif

#ifndef FIFEGUI_EXT_API
#define FIFEGUI_EXT_API
#endif

#ifndef NULL
#define NULL 0
#endif

#endif // end FCN_PLATFORM_HPP
