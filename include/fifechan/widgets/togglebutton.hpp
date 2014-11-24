/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
 *   This file is part of fifechan.                                        *
 *                                                                         *
 *   fifechan is free software; you can redistribute it and/or             *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA          *
 ***************************************************************************/

#ifndef FCN_TOGGLEBUTTON_HPP
#define FCN_TOGGLEBUTTON_HPP

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
    class FCN_CORE_DECLSPEC ToggleButton : public fcn::ImageButton {
    public:

        /**
         * Constructor.
         */
        ToggleButton();

        /**
         * Constructor. The toggle button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the toggle button.
         * @param group The group the toggle button should belong to.
         * @param selected True if the toggle button should be selected.
         */
        ToggleButton(const std::string &caption, const std::string &group, bool selected = false);

        /**
         * Destructor.
         */
        virtual ~ToggleButton();

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
        void setGroup(const std::string &group);

        /**
         * Gets the group the toggle button belongs to.
         *
         * @return The group the toggle button belongs to.
         * @see setGroup
         */
        const std::string &getGroup() const;


        // Inherited from KeyListener

        virtual void keyReleased(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mouseReleased(MouseEvent& mouseEvent);


    protected:

        /**
         * True if the check box is selected, false otherwise.
         */
        bool mSelected;

        /**
         * Holds the group of the toggle button.
         */
        std::string mGroup;

        /**
         * Typdef.
         */
        typedef std::multimap<std::string, ToggleButton *> GroupMap;

        /**
         * Typdef.
         */
        typedef GroupMap::iterator GroupIterator;

        /**
         * Holds all available toggle button groups.
         */
        static GroupMap mGroupMap;
    };
}

#endif // end FCN_TOGGLEBUTTON_HPP
