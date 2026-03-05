// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __FFDEMO_HPP
#define __FFDEMO_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <fifechan/backends/sdl2/sdl.hpp>

#include <fifechan.hpp>

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
    void action(fcn::ActionEvent const & actionEvent);
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

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    SDL_Event mEvent;
    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;

    fcn::SDL2Graphics* mSDLGraphics;
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

    FFListBox* mMenuList;

    FFListBox* mMagicSkillsList;
    FFScrollArea* mMagicSkillsScroll;

    StringListModel* mPerSkills;
    StringListModel* mPerMagic;
    StringListModel* mOlofSkills;
    StringListModel* mOlofMagic;
    StringListModel* mTomasSkills;
    StringListModel* mTomasMagic;

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
    FFScrollArea* mAboutScrollArea;

    FFListBox* mItemsList;
    FFScrollArea* mItemsScrollArea;
    StringListModel* mItemsListModel;
    StringListModel* mItemsInfoListModel;
    StringListModel* mMenuListModel;

    FFCharacterChooser* mCharacterChooser;
};

#endif
