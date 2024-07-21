// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_PLATFORM_HPP
#define FCN_PLATFORM_HPP

/**
 * @brief Symbol Visibility
 *
 * This header defines macros for symbol visibility in the library.
 * The macros are used to define whether a symbol should be exported or imported.
 * There are three cases to consider:
 *    1. Building the core library
 *    2. Building an extension to the core library
 *    3. Using the core library or an extension in a client application
 *
 * @section core_library Building the core library
 * When we are building the core library, we define `FIFECHAN_BUILD`.
 * This makes sure the core library's symbols are exported (FIFEGUI_API as `__declspec(dllexport)`).
 * Extension symbols would not be defined in this context.
 *
 * @section extension_library Building an extension to the core library
 * If you are building an extension to the core library, you would define `FIFECHAN_EXTENSION_BUILD` in your project.
 * This makes sure the core library's symbols are imported (FIFEGUI_API as `__declspec(dllimport)`),
 * and the extension's symbols are exported (FIFEGUI_EXT_API as `__declspec(dllexport)`).
 * You can do this by using set_target_properties in CMake:
 * @code
 * set_target_properties(${PROJECT_NAME}_yourextension PROPERTIES COMPILE_DEFINITIONS FIFECHAN_EXTENSION_BUILD)
 *
 * @section client_application Using the core library or an extension in a client application
 * If you are using the core library or an extension in a client application,
 * you would not define `FIFECHAN_BUILD` or `FIFECHAN_EXTENSION_BUILD`.
 * For MinGW, you would define `FIFECHAN_DLL_IMPORT`.
 * This makes sure the core library's symbols and the extension's symbols are imported
 * (FIFEGUI_API and FIFEGUI_EXT_API as `__declspec(dllimport)`).
 */

#if defined(__MINGW32__) && defined(FIFECHAN_BUILD)
#define FIFEGUI_API __declspec(dllexport)

#elif defined(__MINGW32__) && defined(FIFECHAN_EXTENSION_BUILD)
#define FIFEGUI_API __declspec(dllimport)
#define FIFEGUI_EXT_API __declspec(dllexport)

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
