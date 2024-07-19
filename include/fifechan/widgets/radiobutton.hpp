// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef FCN_RADIOBUTTON_HPP
#define FCN_RADIOBUTTON_HPP

#include <map>
#include <string>

#include "fifechan/platform.hpp"
#include "fifechan/widgets/checkbox.hpp"

namespace fcn
{
    /**
     * An implementation of a radio button where a user can select or deselect
     * the radio button and where the status of the radio button is displayed to the user.
     * A radio button can belong to a group and when a radio button belongs to a
     * group only one radio button can be selected in the group. A radio button is
     * capable of displaying a caption.
     *
     * If a radio button's state changes an action event will be sent to all action
     * listeners of the radio button.
     */
    class FIFEGUI_API RadioButton : public fcn::CheckBox
    {
    public:
        /**
         * Constructor.
         */
        RadioButton();

        /**
         * Constructor. The radio button will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the radio button.
         * @param group The group the radio button should belong to.
         * @param selected True if the radio button should be selected.
         */
        RadioButton(std::string const & caption, std::string const & group, bool selected = false);

        /**
         * Destructor.
         */
        virtual ~RadioButton();

        /**
         * Sets the group the radio button should belong to. Note that
         * a radio button group is unique per application, not per Gui object
         * as the group is stored in a static map.
         *
         * @param group The name of the group.
         * @see getGroup
         */
        void setGroup(std::string const & group);

        /**
         * Gets the group the radio button belongs to.
         *
         * @return The group the radio button belongs to.
         * @see setGroup
         */
        std::string const & getGroup() const;

        // Inherited from CheckBox

        virtual void setSelected(bool selected);
        virtual void toggleSelected();

    protected:
        /**
         * Holds the group of the radio button.
         */
        std::string mGroup;

        /**
         * Typdef.
         */
        typedef std::multimap<std::string, RadioButton*> GroupMap;

        /**
         * Typdef.
         */
        typedef GroupMap::iterator GroupIterator;

        /**
         * Holds all available radio button groups.
         */
        static GroupMap mGroupMap;
    };
} // namespace fcn

#endif // end FCN_RADIOBUTTON_HPP
