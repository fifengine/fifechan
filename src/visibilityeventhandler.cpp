/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   https://github.com/fifechan                                           *
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

#include "fifechan/visibilityeventhandler.hpp"

#include "fifechan/event.hpp"
#include "fifechan/gui.hpp"

#include <list>

namespace fcn
{
    VisibilityEventHandler::VisibilityEventHandler(Gui *gui)
    :
    mGui(gui)
    {
    }
    
    void VisibilityEventHandler::widgetHidden(const Event& e)
    {
        mGui->enqueueHiddenWidget(e.getSource());
    }
      
    void VisibilityEventHandler::widgetShown(const Event& e)
    {
        mGui->enqueueShownWidget(e.getSource());
    }    
};