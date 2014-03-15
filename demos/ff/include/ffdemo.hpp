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

#ifndef __FFDEMO_HPP
#define __FFDEMO_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <fifechan.hpp>
#include <fifechan/sdl.hpp>

#include "ffcharacterchooser.hpp"
#include "ffcontainer.hpp"
#include "fflistbox.hpp"
#include "ffscrollarea.hpp"
#include "stringlistmodel.hpp"

class FFDemo : public fcn::ActionListener, public fcn::KeyListener
{
public:
    FFDemo();
    ~FFDemo();
    void run();
    void action(const fcn::ActionEvent& actionEvent);
    void keyPressed(fcn::KeyEvent& keyEvent);

private:
    void input();
    void initMain();
    void cleanMain();
    void initStatus();
    void cleanStatus();
    void initMagicSkills();
    void cleanMagicSkills();
    void initItems();
    void cleanItems();
    void initAbout();
    void cleanAbout();

    bool mRunning;

    SDL_Surface* mScreen;
    SDL_Event mEvent;
    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;

    fcn::SDLGraphics* mSDLGraphics;
    fcn::SDLInput* mSDLInput;
    fcn::SDLImageLoader* mSDLImageLoader;
    fcn::Gui* mGui;

    fcn::Container* mTop;
    FFContainer* mMain;
    FFContainer* mStatus;
    FFContainer* mItems;
    FFContainer* mMagicSkills;
    FFContainer* mTime;
    FFContainer* mGoldFootsteps;
    FFContainer* mMenu;
    FFContainer* mAbout;
    FFContainer* mItemsInfo;

    fcn::Icon* mPerIcon;
    fcn::Icon* mOlofIcon;
    fcn::Icon* mTomasIcon;
    fcn::Image* mPerImage;
    fcn::Image* mOlofImage;
    fcn::Image* mTomasImage;
    fcn::Image* mSplashImage;
    fcn::Font* mFontWhite;
    fcn::Font* mFontCyan;

    FFListBox *mMenuList;

    FFListBox *mMagicSkillsList;
    FFScrollArea *mMagicSkillsScroll;

    StringListModel *mPerSkills;
    StringListModel *mPerMagic;
    StringListModel *mOlofSkills;
    StringListModel *mOlofMagic;
    StringListModel *mTomasSkills;
    StringListModel *mTomasMagic;

    fcn::TextBox* mPerInfo1;
    fcn::TextBox* mOlofInfo1;
    fcn::TextBox* mTomasInfo1;
    fcn::TextBox* mPerInfo2;
    fcn::TextBox* mOlofInfo2;
    fcn::TextBox* mTomasInfo2;
    fcn::TextBox* mItemsInfoInfo;
    fcn::TextBox* mOlofStatus1;
    fcn::TextBox* mOlofStatus2;
    fcn::TextBox* mPerStatus1;
    fcn::TextBox* mPerStatus2;
    fcn::TextBox* mTomasStatus1;
    fcn::TextBox* mTomasStatus2;

    fcn::TextBox* mGoldFootstepsInfo1;
    fcn::TextBox* mGoldFootstepsInfo2;
    fcn::Label* mTimeLabel1;
    fcn::Label* mTimeLabel2;

    fcn::Label* mNavigationLabel;

    fcn::TextBox* mAboutInfo;
    FFScrollArea *mAboutScrollArea;

    FFListBox *mItemsList;
    FFScrollArea *mItemsScrollArea;
    StringListModel *mItemsListModel;
    StringListModel *mItemsInfoListModel;
    StringListModel *mMenuListModel;

    FFCharacterChooser* mCharacterChooser;
};

#endif
