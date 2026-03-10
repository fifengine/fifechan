// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef INCLUDE_FIFECHAN_WIDGETS_TOGGLEBUTTON_HPP_
#define INCLUDE_FIFECHAN_WIDGETS_TOGGLEBUTTON_HPP_

#include <map>
#include <string>

#include "fifechan/platform.hpp"
#include "fifechan/widgets/imagebutton.hpp"

namespace fcn
{
    /**
     * An implementation of a toggleable button.
     *
     * If the button is in a group, all other buttons in that group will be untoggled
     * when a button gets toggled. If the button is already toggled, you can untoggle
     * it by clicking on it.
     *
     * If a toggle button's state changes an action event will be sent to all action
     * listeners of the toggle button.
     */
    class FIFEGUI_API ToggleButton : public fcn::ImageButton
    {
    public:
        ToggleButton();

        /**
         * Constructor. The toggle button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the toggle button.
         * @param group The group the toggle button should belong to.
         * @param selected True if the toggle button should be selected.
         */
        ToggleButton(std::string const & caption, std::string const & group, bool selected = false);

        ~ToggleButton() override;

        /**
         * Checks if the check box is selected.
         *
         * @return True if the check box is selected, false otherwise.
         * @see setSelected
         */
        virtual bool isSelected() const;

        /**
         * Sets the check box to be selected or not.
         *
         * @param selected True if the check box should be set as selected.
         * @see isSelected
         */
        virtual void setSelected(bool selected);

        /**
         * Toggles the check box between being selected and
         * not being selected. It distribute a ActionEvent.
         */
        virtual void toggleSelected();

        /**
         * Sets the group the toggle button should belong to. Note that
         * a toggle button group is unique per application, not per Gui object
         * as the group is stored in a static map.
         *
         * @param group The name of the group.
         * @see getGroup
         */
        void setGroup(std::string const & group);

        /**
         * Gets the group the toggle button belongs to.
         *
         * @return The group the toggle button belongs to.
         * @see setGroup
         */
        std::string const & getGroup() const;

        // Inherited from KeyListener

        void keyReleased(KeyEvent& keyEvent) override;

        // Inherited from MouseListener

        void mouseReleased(MouseEvent& mouseEvent) override;

    protected:
        // Inherited from Button

        bool isPressed() const override;

        /**
         * True if the check box is selected, false otherwise.
         */
        bool mSelected{false};

        /**
         * Holds the group of the toggle button.
         */
        std::string mGroup;

        /**
         * Typdef.
         */
        using GroupMap = std::multimap<std::string, ToggleButton*>;

        /**
         * Typdef.
         */
        using GroupIterator = GroupMap::iterator;

        /**
         * Holds all available toggle button groups.
         */
        static GroupMap mGroupMap;
    };
} // namespace fcn

#endif // INCLUDE_FIFECHAN_WIDGETS_TOGGLEBUTTON_HPP_
