// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/exception.hpp"

namespace fcn
{
    Exception::Exception(std::string message, std::string function, std::string filename, unsigned int line) :
        std::runtime_error(message),
        mFunction(std::move(function)),
        mMessage(std::move(message)),
        mFilename(std::move(filename)),
        mLine(line)
    {
    }

    char const * Exception::what() const noexcept
    {
        return mMessage.c_str();
    }

    std::string const & Exception::getFunction() const
    {
        return mFunction;
    }

    std::string const & Exception::getMessage() const
    {
        return mMessage;
    }

    std::string const & Exception::getFilename() const
    {
        return mFilename;
    }

    unsigned int Exception::getLine() const
    {
        return mLine;
    }
} // namespace fcn
