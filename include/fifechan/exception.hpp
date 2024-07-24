// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_EXCEPTION_HPP_
#define INCLUDE_FIFECHAN_EXCEPTION_HPP_

#include <stdexcept>
#include <string>
#include <utility> // For std::move

#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * An exception containing a message, a file and a line number
     * where the exception occurred. This class is used to throw
     * exceptions in the GUI code. The exception will contain the
     * filename and line number where the exception was thrown.
     * This makes it easier to find the source of the exception.
     * The exception will also contain the function name where the
     * exception was thrown.
     * The exception is thrown using the fcn::throwException function.
     *
     * EXAMPLE:
     * @code
     * fcn::throwException("my error message", static_cast<const char*>(__FUNCTION__), __FILE__, __LINE__);
     * @endcode
     */
    class FIFEGUI_API Exception : public std::runtime_error
    {
    public:
        /**
         * Constructor.
         *
         * @param message The error message of the exception.
         * @param function The function name where the exception occurred.
         * @param filename The name of the file where the exception occurred.
         * @param line The line number in the source code where the exception
         *             occurred.
         */
        Exception(std::string message, std::string function, std::string filename, unsigned int line);

        /**
         * Returns a pointer to a null-terminated string with a description of the exception.
         *
         * @return const char* A pointer to a null-terminated string with a description of the exception.
         */
        char const * what() const noexcept override;

        /**
         * Gets the function name where the exception occurred.
         *
         * @return The function name where the exception occurred.
         */
        std::string const & getFunction() const;

        /**
         * Gets the error message of the exception.
         *
         * @return The error message of the exception.
         */
        std::string const & getMessage() const;

        /**
         * Gets the filename where the exception occurred.
         *
         * @return The filename where the exception occurred.
         */
        std::string const & getFilename() const;

        /**
         * Gets the line number where the exception occurred.
         *
         * @return The line number where the exception occurred.
         */
        unsigned int getLine() const;

    private:
        /**
         * Holds the name of the function name where the exception occurred.
         */
        std::string mFunction;

        /**
         * Holds the error message of the exception.
         */
        std::string mMessage;

        /**
         * Holds the filename where the exception occurred.
         */
        std::string mFilename;

        /**
         * Holds the line number where the exception occurred.
         */
        unsigned int mLine;
    };

    inline void throwException(
        std::string const & message, std::string const & function, std::string const & filename, unsigned int line)
    {
        throw Exception(message, function, filename, line);
    }
} // namespace fcn

#endif // INCLUDE_FIFECHAN_EXCEPTION_HPP_
