// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fifechan/widgets/radiobutton.hpp"

// Standard library includes
#include <string>

namespace fcn
{
    RadioButton::GroupMap RadioButton::mGroupMap;

    RadioButton::RadioButton()
    {
        addMouseListener(this);
        setMarkerStyle(MarkerStyle::Rhombus);
        setSelected(false);
        adjustSize();
    }

    RadioButton::RadioButton(std::string const & caption, std::string const & group, bool selected) : mGroup(group)
    {
        addMouseListener(this);
        setMarkerStyle(MarkerStyle::Rhombus);
        setCaption(caption);
        setGroup(group);
        setSelected(selected);
        adjustSize();
    }

    RadioButton::~RadioButton()
    {
        // Remove us from the group list
        setGroup("");
    }

    void RadioButton::setSelected(bool selected)
    {
        if (selected && !mGroup.empty()) {
            // deselect all buttons in group
            GroupIterator iter;
            GroupIterator iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup); iter != iterEnd; ++iter) {
                if (iter->second->isSelected()) {
                    iter->second->setSelected(false);
                }
            }
        }

        mSelected = selected;
    }

    void RadioButton::toggleSelected()
    {
        setSelected(!isSelected());
    }

    void RadioButton::setGroup(std::string const & group)
    {
        // Remove button from previous group
        if (!mGroup.empty()) {
            GroupIterator iter;
            GroupIterator iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup); iter != iterEnd; ++iter) {
                if (iter->second == this) {
                    mGroupMap.erase(iter);
                    break;
                }
            }
        }
        // Add button to new group
        if (!group.empty()) {
            mGroupMap.insert(std::pair<std::string, RadioButton*>(group, this));
        }

        mGroup = group;
    }

    std::string const & RadioButton::getGroup() const
    {
        return mGroup;
    }

} // namespace fcn
