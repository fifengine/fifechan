// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <fifechan/backends/sdl2/sdl.hpp>

#include <fifechan.hpp>

#include <memory>

#include "ffcharacterchooser.hpp"
#include "ffcontainer.hpp"
#include "fflistbox.hpp"
#include "ffscrollarea.hpp"
#include "stringlistmodel.hpp"

class FFDemo : public fcn::ActionListener, public fcn::KeyListener
{
public:
    FFDemo();
    ~FFDemo() override;
    FFDemo(FFDemo const &)            = delete;
    FFDemo& operator=(FFDemo const &) = delete;
    void run();
    void action(fcn::ActionEvent const & actionEvent) override;
    void keyPressed(fcn::KeyEvent& keyEvent) override;

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

    SDL_Window* mWindow{};
    SDL_Renderer* mRenderer{};
    SDL_Event mEvent{};
    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;

    std::unique_ptr<fcn::sdl2::Graphics> mSDLGraphics;
    std::unique_ptr<fcn::sdl2::Input> mSDLInput;
    std::unique_ptr<fcn::sdl2::ImageLoader> mSDLImageLoader;
    std::unique_ptr<fcn::Gui> mGui;

    std::unique_ptr<fcn::Container> mTop;
    std::unique_ptr<FFContainer> mMain;
    std::unique_ptr<FFContainer> mStatus;
    std::unique_ptr<FFContainer> mItems;
    std::unique_ptr<FFContainer> mMagicSkills;
    std::unique_ptr<FFContainer> mTime;
    std::unique_ptr<FFContainer> mGoldFootsteps;
    std::unique_ptr<FFContainer> mMenu;
    std::unique_ptr<FFContainer> mAbout;
    std::unique_ptr<FFContainer> mItemsInfo;

    std::unique_ptr<fcn::Icon> mPerIcon;
    std::unique_ptr<fcn::Icon> mOlofIcon;
    std::unique_ptr<fcn::Icon> mTomasIcon;
    std::unique_ptr<fcn::Image> mPerImage;
    std::unique_ptr<fcn::Image> mOlofImage;
    std::unique_ptr<fcn::Image> mTomasImage;
    std::unique_ptr<fcn::Image> mSplashImage;
    std::unique_ptr<fcn::ImageFont> mFontWhite;
    std::unique_ptr<fcn::ImageFont> mFontCyan;

    std::unique_ptr<FFListBox> mMenuList;

    std::unique_ptr<FFListBox> mMagicSkillsList;
    std::unique_ptr<FFScrollArea> mMagicSkillsScroll;

    std::unique_ptr<StringListModel> mPerSkills;
    std::unique_ptr<StringListModel> mPerMagic;
    std::unique_ptr<StringListModel> mOlofSkills;
    std::unique_ptr<StringListModel> mOlofMagic;
    std::unique_ptr<StringListModel> mTomasSkills;
    std::unique_ptr<StringListModel> mTomasMagic;

    std::unique_ptr<fcn::TextBox> mPerInfo1;
    std::unique_ptr<fcn::TextBox> mOlofInfo1;
    std::unique_ptr<fcn::TextBox> mTomasInfo1;
    std::unique_ptr<fcn::TextBox> mPerInfo2;
    std::unique_ptr<fcn::TextBox> mOlofInfo2;
    std::unique_ptr<fcn::TextBox> mTomasInfo2;
    std::unique_ptr<fcn::TextBox> mItemsInfoInfo;
    std::unique_ptr<fcn::TextBox> mOlofStatus1;
    std::unique_ptr<fcn::TextBox> mOlofStatus2;
    std::unique_ptr<fcn::TextBox> mPerStatus1;
    std::unique_ptr<fcn::TextBox> mPerStatus2;
    std::unique_ptr<fcn::TextBox> mTomasStatus1;
    std::unique_ptr<fcn::TextBox> mTomasStatus2;

    std::unique_ptr<fcn::TextBox> mGoldFootstepsInfo1;
    std::unique_ptr<fcn::TextBox> mGoldFootstepsInfo2;
    std::unique_ptr<fcn::Label> mTimeLabel1;
    std::unique_ptr<fcn::Label> mTimeLabel2;

    std::unique_ptr<fcn::Label> mNavigationLabel;

    std::unique_ptr<fcn::TextBox> mAboutInfo;
    std::unique_ptr<FFScrollArea> mAboutScrollArea;

    std::unique_ptr<FFListBox> mItemsList;
    std::unique_ptr<FFScrollArea> mItemsScrollArea;
    std::unique_ptr<StringListModel> mItemsListModel;
    std::unique_ptr<StringListModel> mItemsInfoListModel;
    std::unique_ptr<StringListModel> mMenuListModel;

    std::unique_ptr<FFCharacterChooser> mCharacterChooser;
};
