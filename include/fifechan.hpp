// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/*

 ______   __     ______   ______     ______     __  __     ______     __   __
/\  ___\ /\ \   /\  ___\ /\  ___\   /\  ___\   /\ \_\ \   /\  __ \   /\ "-.\ \
\ \  __\ \ \ \  \ \  __\ \ \  __\   \ \ \____  \ \  __ \  \ \  __ \  \ \ \-.  \
 \ \_\    \ \_\  \ \_\    \ \_____\  \ \_____\  \ \_\ \_\  \ \_\ \_\  \ \_\\"\_\
  \/_/     \/_/   \/_/     \/_____/   \/_____/   \/_/\/_/   \/_/\/_/   \/_/ \/_/

*/

#ifndef INCLUDE_FIFECHAN_HPP_
#define INCLUDE_FIFECHAN_HPP_

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
#include <fifechan/widgets/label.hpp>
#include <fifechan/widgets/linegraph.hpp>
#include <fifechan/widgets/listbox.hpp>
#include <fifechan/widgets/passwordfield.hpp>
#include <fifechan/widgets/piegraph.hpp>
#include <fifechan/widgets/pointgraph.hpp>
#include <fifechan/widgets/radiobutton.hpp>
#include <fifechan/widgets/scrollarea.hpp>
#include <fifechan/widgets/slider.hpp>
#include <fifechan/widgets/spacer.hpp>
#include <fifechan/widgets/tab.hpp>
#include <fifechan/widgets/tabbedarea.hpp>
#include <fifechan/widgets/textbox.hpp>
#include <fifechan/widgets/textfield.hpp>
#include <fifechan/widgets/togglebutton.hpp>
#include <fifechan/widgets/window.hpp>

#include <fifechan/actionevent.hpp>
#include <fifechan/actionlistener.hpp>
#include <fifechan/cliprectangle.hpp>
#include <fifechan/color.hpp>
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
#include <fifechan/rectangle.hpp>
#include <fifechan/selectionevent.hpp>
#include <fifechan/selectionlistener.hpp>
#include <fifechan/widget.hpp>
#include <fifechan/widgetlistener.hpp>

#include "fifechan/platform.hpp"
#include "fifechan/version.hpp"

/**
 * @brief Returns the version of the library.
 *
 * @return the version.
 */
FIFEGUI_API extern char const * fcnFifechanVersion();

#endif // INCLUDE_FIFECHAN_HPP_
