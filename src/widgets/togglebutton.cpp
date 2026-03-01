// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fifechan/widgets/togglebutton.hpp"

namespace fcn
{
    ToggleButton::GroupMap ToggleButton::mGroupMap;

    ToggleButton::ToggleButton()
    {
        setSelected(false);
        adjustSize();
    }

    ToggleButton::ToggleButton(std::string const & caption, std::string const & group, bool selected)
    {
        setCaption(caption);
        setGroup(group);
        setSelected(selected);
        adjustSize();
    }

    ToggleButton::~ToggleButton()
    {
        // Remove us from the group list
        setGroup("");
    }

    bool ToggleButton::isSelected() const
    {
        return mSelected;
    }

    void ToggleButton::setSelected(bool selected)
    {
        if (selected && !mGroup.empty()) {
            // deselect all buttons in group
            GroupIterator iter, iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup); iter != iterEnd; ++iter) {
                if (iter->second->isSelected()) {
                    iter->second->setSelected(false);
                }
            }
        }

        mSelected = selected;
    }

    void ToggleButton::toggleSelected()
    {
        setSelected(!isSelected());
        distributeActionEvent();
    }

    void ToggleButton::setGroup(std::string const & group)
    {
        // Remove button from previous group
        if (!mGroup.empty()) {
            GroupIterator iter, iterEnd;
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
            mGroupMap.insert(std::pair<std::string, ToggleButton*>(group, this));
        }

        mGroup = group;
    }

    std::string const & ToggleButton::getGroup() const
    {
        return mGroup;
    }

    bool ToggleButton::isPressed() const
    {
        return isSelected();
    }

    void ToggleButton::keyReleased(KeyEvent& keyEvent)
    {
        Key const key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space) && mKeyPressed) {
            mKeyPressed = false;
            toggleSelected();
            keyEvent.consume();
        }
    }

    void ToggleButton::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left && mMousePressed && mHasMouse) {
            mMousePressed = false;
            toggleSelected();
            mouseEvent.consume();
        } else if (mouseEvent.getButton() == MouseEvent::Left) {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

} // namespace fcn
