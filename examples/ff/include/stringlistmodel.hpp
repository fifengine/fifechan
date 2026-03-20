// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef STRINGLISTMODEL_HPP
#define STRINGLISTMODEL_HPP

#include <fifechan.hpp>

#include <string>
#include <vector>

class StringListModel : public fcn::ListModel
{
public:
    StringListModel() = default;

    int getNumberOfElements() override
    {
        return static_cast<int>(mStrings.size());
    }

    std::string getElementAt(int i) override
    {
        return mStrings.at(i);
    }

    void add(std::string const & str)
    {
        mStrings.push_back(str);
    }

private:
    std::vector<std::string> mStrings;
};

#endif
