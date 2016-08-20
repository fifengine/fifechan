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

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess�n and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess�n a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    class FCN_CORE_DECLSPEC RadioButton : public fcn::CheckBox {
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
        RadioButton(const std::string &caption,
                    const std::string &group,
                    bool selected = false);

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
        void setGroup(const std::string &group);

        /**
         * Gets the group the radio button belongs to.
         *
         * @return The group the radio button belongs to.
         * @see setGroup
         */
        const std::string &getGroup() const;


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
        typedef std::multimap<std::string, RadioButton *> GroupMap;

        /**
         * Typdef.
         */
        typedef GroupMap::iterator GroupIterator;

        /**
         * Holds all available radio button groups.
         */
        static GroupMap mGroupMap;
    };
}

#endif // end FCN_RADIOBUTTON_HPP
