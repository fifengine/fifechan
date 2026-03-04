// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_EXCEPTION_HPP_
#define INCLUDE_FIFECHAN_EXCEPTION_HPP_

#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

#include "fifechan/platform.hpp"

namespace fcn
{
    /**
     * An exception class containing a message, a file, and a line number
     * where the exception occurred. This class is used to throw exceptions
     * in the GUI code with detailed information about the source location.
     * The exception will contain the filename, line number, and function name
     * where the exception was thrown. This makes it easier to identify the source
     * of the exception.
     *
     * The exception is thrown using the `fcn::throwException` function, which
     * automatically captures the source location (filename, line number, and function name).
     *
     * EXAMPLE:
     * @code
     * throwException("my error message");
     * @endcode
     *
     * This is a header only class and so its not exported via FIFEGUI_API.
     */
    class /*FIFEGUI_API*/ Exception : public std::runtime_error
    {
    public:
        /**
         * Constructor that takes a message and source location.
         *
         * @param message The error message of the exception.
         * @param location The source location of where the exception occurred.
         */
        explicit Exception(std::string message, std::source_location location = std::source_location::current()) :
            std::runtime_error(message),  // Pass by reference, do not move.
            mMessage(std::move(message)), // Move only once into mMessage.
            mFunction(location.function_name()),
            mFilename(location.file_name()),
            mLine(location.line())
        {
        }

        /**
         * Returns a pointer to a null-terminated string with a description of the exception.
         *
         * @return const char* A pointer to a null-terminated string with a description of the exception.
         */
        char const * what() const noexcept override
        {
            return mMessage.c_str();
        }

        /**
         * Gets the function name where the exception occurred.
         *
         * @return The function name where the exception occurred.
         */
        std::string const & getFunction() const
        {
            return mFunction;
        };

        /**
         * Gets the message of the exception.
         *
         * @return The message of the exception.
         */
        std::string const & getMessage() const
        {
            return mMessage;
        };

        /**
         * Gets the filename where the exception occurred.
         *
         * @return The filename where the exception occurred.
         */
        std::string const & getFilename() const
        {
            return mFilename;
        };

        /**
         * Gets the line number where the exception occurred.
         *
         * @return The line number where the exception occurred.
         */
        unsigned int getLine() const
        {
            return mLine;
        };

    private:
        /**
         * Holds the error message of the exception.
         */
        std::string mMessage = "Message?";

        /**
         * Holds the name of the function name where the exception occurred.
         */
        std::string mFunction = "Function?";

        /**
         * Holds the filename where the exception occurred.
         */
        std::string mFilename = "Filename?";

        /**
         * Holds the line number where the exception occurred.
         */
        unsigned int mLine = 0;
    };

    inline void throwException(
        std::string const & message, std::source_location location = std::source_location::current())
    {
        throw Exception(message, location);
    }

} // namespace fcn

#endif // INCLUDE_FIFECHAN_EXCEPTION_HPP_
