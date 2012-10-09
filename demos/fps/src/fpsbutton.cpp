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

#include "fpsbutton.hpp"

int FPSButton::mInstances = 0;
Mix_Chunk* FPSButton::mHoverSound = NULL;

/*
 * It is very important to call Buttons constructor in the constructor
 * initialization code so we do that. In the constructor execution code
 * we check instances and load the mouse hover sound if there are no
 * instances.
 */
FPSButton::FPSButton(const std::string& caption)
        :Button(caption),
         mHasMouse(false)
{
    setFrameSize(0);
  
    if (mInstances == 0)
    {		
        mHoverSound = Mix_LoadWAV("sound/sound5.wav");
        Mix_VolumeChunk(mHoverSound, 60);
    }
	
    ++mInstances;
}

/**
 * We free the sound if this is the last instance.
 */
FPSButton::~FPSButton()
{
    --mInstances;

    if (mInstances == 0)
    {
        Mix_FreeChunk(mHoverSound);
    }
}

/*
 * If we have the mouse we should draw the caption with the highlight
 * font, if not we should draw it with the ordinary font.
 */
void FPSButton::draw(fcn::Graphics* graphics)
{
    if (mHasMouse)
    {
        graphics->setFont(mHighLightFont);
        graphics->drawText(getCaption(),0,0);
    }
    else
    {
        graphics->setFont(getFont());
        graphics->drawText(getCaption(),0,0);
    }
}

void FPSButton::setHighLightFont(fcn::Font* font)
{
    mHighLightFont = font;
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseEntered is called when the
 * mouse enters the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseEntered(fcn::MouseEvent& mouseEvent)
{
    Button::mouseEntered(mouseEvent);
    Mix_PlayChannel(-1, mHoverSound, 0);
    mHasMouse = true;
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseExited is called when the
 * mouse exits the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseExited(fcn::MouseEvent& mouseEvent)
{
    Button::mouseExited(mouseEvent);
    mHasMouse = false;
}

