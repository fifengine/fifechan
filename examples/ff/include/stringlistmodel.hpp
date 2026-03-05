// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __STRINGLISTMODEL_HPP
#define __STRINGLISTMODEL_HPP

#include <fifechan.hpp>

#include <vector>

class StringListModel : public fcn::ListModel
{
public:
    StringListModel() { }

    int getNumberOfElements()
    {
        return mStrings.size();
    }

    std::string getElementAt(int i)
    {
        return mStrings.at(i);
    }

    void add(std::string str)
    {
        mStrings.push_back(str);
    }

private:
    std::vector<std::string> mStrings;
};

#endif
