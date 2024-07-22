// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_PLATFORM_HPP
#define FCN_PLATFORM_HPP
// clang-format off

// Defines for the opterating system
#ifndef FIFEGUI_OS_WINDOWS
    #define FIFEGUI_OS_WINDOWS      defined(_WIN32) || defined(_WIN64)
#endif
#ifndef FIFEGUI_OS_LINUX
    #define FIFEGUI_OS_LINUX        defined(__linux__)
#endif
#ifndef FIFEGUI_OS_MACOS
    #define FIFEGUI_OS_MACOS        defined(__APPLE__)
#endif

// Defines for the compiler
#ifndef FIFEGUI_COMPILER_MSVC
    #define FIFEGUI_COMPILER_MSVC   defined(_MSC_VER)
#endif
#ifndef FIFEGUI_COMPILER_GNU
    #define FIFEGUI_COMPILER_GNU    defined(__GNUC__)
#endif
#ifndef FIFEGUI_COMPILER_CLANG
    #define FIFEGUI_COMPILER_CLANG  defined(__clang__)
#endif
#ifndef FIFEGUI_COMPILER_MINGW
    #define FIFEGUI_COMPILER_MINGW  defined(__MINGW32__)
#endif

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

// FIFECHAN_BUILD and FIIFECHAN_EXTENSION_BUILD are defined in CMakeLists.txt.
// FIFECHAN_BUILD is our manual definition for building the core library.
// FIFECHAN_EXTENSION_BUILD is our manual definition for building an extension to the core library.
// Additionally CMake automatically defines target_EXPORTS when building a shared library, e.g
// fifechan_EXPORTS when building the fifechan library.

#if defined(FIFEGUI_OS_LINUX)

#elif defined(FIFEGUI_OS_WINDOWS)
    #if defined(FIFEGUI_COMPILER_MSVC) || defined(FIFEGUI_COMPILER_MINGW)
        #if defined(FIFECHAN_BUILD) || defined(fifechan_EXPORTS)
            // Building the library
            #define FIFEGUI_API __declspec(dllexport)
        #elif defined(FIFECHAN_EXTENSION_BUILD)
            // Building an extension to the library
            #define FIFEGUI_API     __declspec(dllimport)
            #define FIFEGUI_EXT_API __declspec(dllexport)
        #else
            // Using the library
            #define FIFEGUI_API     __declspec(dllimport)
            #define FIFEGUI_EXT_API __declspec(dllimport)
        #endif
    #elif defined(FIFEGUI_COMPILER_CLANG)
        #if defined(FIFECHAN_BUILD) || defined(fifechan_EXPORTS)
            // Building the library
            #define FIFEGUI_API __declspec(dllexport) __attribute__ ((visibility("default")))
        #elif defined(FIFECHAN_EXTENSION_BUILD)
            // Building an extension to the library
            #define FIFEGUI_API     __declspec(dllimport) __attribute__ ((visibility("default")))
            #define FIFEGUI_EXT_API __declspec(dllexport) __attribute__ ((visibility("default")))
        #else
            // Using the library
            #define FIFEGUI_API     __declspec(dllimport) __attribute__ ((visibility("default")))
            #define FIFEGUI_EXT_API __declspec(dllimport) __attribute__ ((visibility("default")))
        #endif
    #endif
#endif

// For other compilers/platforms, default visibility is assumed.
#ifndef FIFEGUI_API
#define FIFEGUI_API __attribute__ ((visibility("default")))
#endif

#ifndef FIFEGUI_EXT_API
#define FIFEGUI_EXT_API __attribute__ ((visibility("default")))
#endif

#ifndef NULL
#define NULL 0
#endif

// clang-format on
#endif // end FCN_PLATFORM_HPP
