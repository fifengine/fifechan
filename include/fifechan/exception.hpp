// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_EXCEPTION_HPP
#define FCN_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <utility> // For std::move

#include "fifechan/platform.hpp"

namespace fcn
{

    /**
     * An exception containing a message, a file and a line number
     * where the exception occured. Guichan will only throw exceptions
     * of this class.
     *
     * You can use this class for your own exceptions that has
     * something to do with a GUI exception. A nifty feature of the
     * excpetion class is that it can tell you from which line and
     * file it was thrown. To make things easier when throwing
     * exceptions there exists a macro for creating exceptions
     * which automatically sets the filename and line number.
     *
     * EXAMPLE: @code
     *          fcn::throwException(("my error message"), static_cast<const char*>(__FUNCTION__), __FILE__, __LINE__);
     *          @endcode
     */
    class FIFEGUI_API Exception : public std::runtime_error
    {
    public:
        /**
         * Constructor.
         *
         * @param message The error message of the exception.
         * @param function The function name where the exception occured.
         * @param filename The name of the file where the exception occured.
         * @param line The line number in the source code where the exception
         *             occured.
         */
        Exception(std::string message, std::string function, std::string filename, unsigned int line);

        /**
         * Returns a pointer to a null-terminated string with a description of the exception.
         *
         * @return const char* A pointer to a null-terminated string with a description of the exception.
         */
        char const * what() const noexcept override;

        /**
         * Gets the function name where the exception occured.
         *
         * @return The function name where the exception occured.
         */
        std::string const & getFunction() const;

        /**
         * Gets the error message of the exception.
         *
         * @return The error message of the exception.
         */
        std::string const & getMessage() const;

        /**
         * Gets the filename where the exception occured.
         *
         * @return The filename where the exception occured.
         */
        std::string const & getFilename() const;

        /**
         * Gets the line number where the exception occured.
         *
         * @return The line number where the exception occured.
         */
        unsigned int getLine() const;

    private:
        /**
         * Holds the name of the function name where the exception occured.
         */
        std::string mFunction;

        /**
         * Holds the error message of the exception.
         */
        std::string mMessage;

        /**
         * Holds the filename where the exception occured.
         */
        std::string mFilename;

        /**
         * Holds the line number where the exception occured.
         */
        unsigned int mLine;
    };

    inline void throwException(
        std::string const & message, std::string const & function, std::string const & filename, unsigned int line)
    {
        throw Exception(message, function, filename, line);
    }
} // namespace fcn

#endif // end FCN_EXCEPTION_HPP
