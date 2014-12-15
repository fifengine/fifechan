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
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
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

#ifndef FCN_FIFECHAN_HPP
#define FCN_FIFECHAN_HPP

#include <fifechan/actionevent.hpp>
#include <fifechan/actionlistener.hpp>
#include <fifechan/cliprectangle.hpp>
#include <fifechan/color.hpp>
#include <fifechan/containerevent.hpp>
#include <fifechan/containerlistener.hpp>
#include <fifechan/deathlistener.hpp>
#include <fifechan/event.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/focushandler.hpp>
#include <fifechan/focuslistener.hpp>
#include <fifechan/font.hpp>
#include <fifechan/genericinput.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/gui.hpp>
#include <fifechan/image.hpp>
#include <fifechan/imagefont.hpp>
#include <fifechan/imageloader.hpp>
#include <fifechan/input.hpp>
#include <fifechan/inputevent.hpp>
#include <fifechan/key.hpp>
#include <fifechan/keyevent.hpp>
#include <fifechan/keyinput.hpp>
#include <fifechan/keylistener.hpp>
#include <fifechan/listmodel.hpp>
#include <fifechan/mouseevent.hpp>
#include <fifechan/mouseinput.hpp>
#include <fifechan/mouselistener.hpp>
#include <fifechan/point.hpp>
#include <fifechan/rectangle.hpp>
#include <fifechan/selectionevent.hpp>
#include <fifechan/selectionlistener.hpp>
#include <fifechan/size.hpp>
#include <fifechan/widget.hpp>
#include <fifechan/widgetlistener.hpp>
#include <fifechan/widgets/adjustingcontainer.hpp>
#include <fifechan/widgets/bargraph.hpp>
#include <fifechan/widgets/button.hpp>
#include <fifechan/widgets/checkbox.hpp>
#include <fifechan/widgets/container.hpp>
#include <fifechan/widgets/curvegraph.hpp>
#include <fifechan/widgets/dropdown.hpp>
#include <fifechan/widgets/flowcontainer.hpp>
#include <fifechan/widgets/icon.hpp>
#include <fifechan/widgets/iconprogressbar.hpp>
#include <fifechan/widgets/imagebutton.hpp>
#include <fifechan/widgets/imageprogressbar.hpp>
#include <fifechan/widgets/label.hpp>
#include <fifechan/widgets/linegraph.hpp>
#include <fifechan/widgets/listbox.hpp>
#include <fifechan/widgets/passwordfield.hpp>
#include <fifechan/widgets/piegraph.hpp>
#include <fifechan/widgets/pointgraph.hpp>
#include <fifechan/widgets/scrollarea.hpp>
#include <fifechan/widgets/slider.hpp>
#include <fifechan/widgets/spacer.hpp>
#include <fifechan/widgets/radiobutton.hpp>
#include <fifechan/widgets/tab.hpp>
#include <fifechan/widgets/tabbedarea.hpp>
#include <fifechan/widgets/textbox.hpp>
#include <fifechan/widgets/textfield.hpp>
#include <fifechan/widgets/togglebutton.hpp>
#include <fifechan/widgets/window.hpp>

#include "fifechan/platform.hpp"
#include "fifechan/version.hpp"


class Widget;

extern "C"
{
    /**
     * Gets the the version of Fifechan. As it is a C function
     * it can be used to check for Fifechan with autotools.
     *
     * @return the version of Fifechan.
     */
    FCN_CORE_DECLSPEC extern const char* fcnFifechanVersion();
}

#endif // end FCN_FIFECHAN_HPP
