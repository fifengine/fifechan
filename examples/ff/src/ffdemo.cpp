// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "ffdemo.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
    constexpr int kUiWidth      = 320;
    constexpr int kUiHeight     = 240;
    constexpr int kWindowScale  = 2;
    constexpr int kWindowWidth  = kUiWidth * kWindowScale;
    constexpr int kWindowHeight = kUiHeight * kWindowScale;
} // namespace

FFDemo::FFDemo() : mRunning(true)
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if (SDL_CreateWindowAndRenderer(kWindowWidth, kWindowHeight, 0, &mWindow, &mRenderer) < 0) {
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    if (SDL_RenderSetLogicalSize(mRenderer, kUiWidth, kUiHeight) < 0) {
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }

    SDL_SetWindowTitle(mWindow, "FifeGUI demo");

    SDL_ShowCursor(SDL_DISABLE);

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        throw std::runtime_error(Mix_GetError());
    }

    mChooseSound = Mix_LoadWAV("sound/sound1.wav");
    mEscapeSound = Mix_LoadWAV("sound/sound2.wav");

    if (mChooseSound == nullptr || mEscapeSound == nullptr) {
        Mix_CloseAudio();
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        throw std::runtime_error(Mix_GetError());
    }

    mSDLImageLoader = std::make_unique<fcn::SDLImageLoader>();
    mSDLImageLoader->setRenderer(mRenderer);
    fcn::Image::setImageLoader(mSDLImageLoader.get());
    mSDLGraphics = std::make_unique<fcn::SDL2Graphics>();
    mSDLGraphics->setTarget(mRenderer, kUiWidth, kUiHeight);
    mSDLInput = std::make_unique<fcn::SDLInput>();

    mSplashImage = std::unique_ptr<fcn::Image>(fcn::Image::load("images/splash.png"));

    mTop = std::make_unique<fcn::Container>();
    mTop->setBaseColor(fcn::Color(0x000000));
    mTop->setDimension(fcn::Rectangle(0, 0, kUiWidth, kUiHeight));
    mGui = std::make_unique<fcn::Gui>();
    mGui->setTabbingEnabled(false);
    mGui->setGraphics(mSDLGraphics.get());
    mGui->setInput(mSDLInput.get());
    mGui->setTop(mTop.get());
    mFontWhite = std::make_unique<fcn::ImageFont>(
        "images/rpgfont.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mFontCyan = std::make_unique<fcn::ImageFont>(
        "images/rpgfont2.png", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    fcn::Widget::setGlobalFont(mFontWhite.get());

    initMain();

    mMenu = std::make_unique<FFContainer>();
    mMenu->setDimension(fcn::Rectangle(230, 0, 90, 130));
    mMenu->setOpaque(false);
    mTop->add(mMenu.get());

    mGoldFootsteps = std::make_unique<FFContainer>();
    mGoldFootsteps->setDimension(fcn::Rectangle(210, 170, 110, 70));
    mGoldFootsteps->setOpaque(false);
    mGoldFootsteps->setBorderSize(0);
    mTop->add(mGoldFootsteps.get());

    mTime = std::make_unique<FFContainer>();
    mTime->setDimension(fcn::Rectangle(230, 130, 90, 40));
    mTime->setOpaque(false);
    mTop->add(mTime.get());

    mGoldFootstepsInfo1 = std::make_unique<fcn::TextBox>("Steps\n\nGP");
    mGoldFootstepsInfo1->setFont(mFontCyan.get());
    mGoldFootstepsInfo1->setOpaque(false);
    mGoldFootstepsInfo1->setEditable(false);
    mGoldFootstepsInfo1->setFocusable(false);
    mGoldFootstepsInfo1->setBorderSize(0);

    mGoldFootstepsInfo2 = std::make_unique<fcn::TextBox>("\n    9119092\n\n    1009213");
    mGoldFootstepsInfo2->setOpaque(false);
    mGoldFootstepsInfo2->setEditable(false);
    mGoldFootstepsInfo2->setFocusable(false);
    mGoldFootstepsInfo2->setBorderSize(0);

    mTimeLabel1 = std::make_unique<fcn::Label>("Time");
    mTimeLabel1->setFont(mFontCyan.get());
    mTimeLabel2 = std::make_unique<fcn::Label>();
    mTime->add(mTimeLabel1.get(), 5, 5);
    mTime->add(mTimeLabel2.get(), 22, 20);

    mGoldFootsteps->add(mGoldFootstepsInfo2.get(), 5, 0);
    mGoldFootsteps->add(mGoldFootstepsInfo1.get(), 5, 5);

    mMenuListModel = std::make_unique<StringListModel>();
    mMenuListModel->add("Items");
    mMenuListModel->add("Status");
    mMenuListModel->add("Skills");
    mMenuListModel->add("Magic");
    mMenuListModel->add("About");
    mMenuListModel->add("");
    mMenuListModel->add("Quit");

    mMenuList = std::make_unique<FFListBox>();
    mMenuList->setActionEventId("menu");
    mMenuList->addActionListener(this);
    mMenuList->setListModel(mMenuListModel.get());
    mMenu->add(mMenuList.get(), 5, 5);
    mMenuList->setSelected(0);
    mMenuList->requestFocus();

    initStatus();
    initAbout();
    initItems();
    initMagicSkills();
}

FFDemo::~FFDemo()
{
    if (mGui != nullptr) {
        mGui->setTop(nullptr);
    }
    fcn::Widget::setGlobalFont(nullptr);
    fcn::Image::setImageLoader(nullptr);

    cleanStatus();
    cleanAbout();
    cleanItems();
    cleanMagicSkills();
    cleanMain();

    Mix_FreeChunk(mChooseSound);
    Mix_FreeChunk(mEscapeSound);
    Mix_CloseAudio();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);

    SDL_Quit();
}

void FFDemo::initMain()
{
    mMain = std::make_unique<FFContainer>();
    mMain->setDimension(fcn::Rectangle(0, 0, kUiWidth, kUiHeight));
    mTop->add(mMain.get());

    mPerImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/finalman.png"));
    mOlofImage  = std::unique_ptr<fcn::Image>(fcn::Image::load("images/yakslem.png"));
    mTomasImage = std::unique_ptr<fcn::Image>(fcn::Image::load("images/peak.png"));

    mPerIcon   = std::make_unique<fcn::Icon>(mPerImage.get());
    mOlofIcon  = std::make_unique<fcn::Icon>(mOlofImage.get());
    mTomasIcon = std::make_unique<fcn::Icon>(mTomasImage.get());

    mPerInfo1 = std::make_unique<fcn::TextBox>("\n  LV\n  HP\n  MP");
    mPerInfo1->setFont(mFontCyan.get());
    mPerInfo1->setOpaque(false);
    mPerInfo1->setEditable(false);
    mPerInfo1->setFocusable(false);
    mPerInfo1->setBorderSize(0);

    mPerInfo2 = std::make_unique<fcn::TextBox>("FINALMAN\n     13\n       12/ 336\n       33/  40");
    mPerInfo2->setOpaque(false);
    mPerInfo2->setEditable(false);
    mPerInfo2->setFocusable(false);
    mPerInfo2->setBorderSize(0);

    mOlofInfo1 = std::make_unique<fcn::TextBox>("\n  LV\n  HP\n  MP");
    mOlofInfo1->setFont(mFontCyan.get());
    mOlofInfo1->setOpaque(false);
    mOlofInfo1->setEditable(false);
    mOlofInfo1->setFocusable(false);
    mOlofInfo1->setBorderSize(0);

    mOlofInfo2 = std::make_unique<fcn::TextBox>("YAKSLEM\n     41\n     1304/2932\n      298/ 300");
    mOlofInfo2->setOpaque(false);
    mOlofInfo2->setEditable(false);
    mOlofInfo2->setFocusable(false);
    mOlofInfo2->setBorderSize(0);

    mTomasInfo1 = std::make_unique<fcn::TextBox>("\n  LV\n  HP\n  MP");
    mTomasInfo1->setFont(mFontCyan.get());
    mTomasInfo1->setOpaque(false);
    mTomasInfo1->setEditable(false);
    mTomasInfo1->setFocusable(false);
    mTomasInfo1->setBorderSize(0);

    mTomasInfo2 = std::make_unique<fcn::TextBox>("PEAK\n      6\n      101/ 101\n        0/   0");
    mTomasInfo2->setOpaque(false);
    mTomasInfo2->setEditable(false);
    mTomasInfo2->setFocusable(false);
    mTomasInfo2->setBorderSize(0);

    int offset = 6;
    mMain->add(mPerIcon.get(), 10, offset);
    mMain->add(mPerInfo2.get(), 60, offset);
    mMain->add(mPerInfo1.get(), 60, offset);
    offset += 76;
    mMain->add(mOlofIcon.get(), 10, offset);
    mMain->add(mOlofInfo2.get(), 60, offset);
    mMain->add(mOlofInfo1.get(), 60, offset);
    offset += 76;
    mMain->add(mTomasIcon.get(), 10, offset);
    mMain->add(mTomasInfo2.get(), 60, offset);
    mMain->add(mTomasInfo1.get(), 60, offset);

    mCharacterChooser = std::make_unique<FFCharacterChooser>();
    mCharacterChooser->setActionEventId("character");
    mCharacterChooser->addActionListener(this);

    mMain->add(mCharacterChooser.get(), 5, 25);

    mNavigationLabel = std::make_unique<fcn::Label>("STATUS ");
    mNavigationLabel->setVisible(false);
    mMain->add(mNavigationLabel.get(), 230, 20);
}

void FFDemo::cleanMain()
{
    mNavigationLabel.reset();
    mCharacterChooser.reset();
    mPerInfo1.reset();
    mOlofInfo1.reset();
    mTomasInfo1.reset();
    mPerInfo2.reset();
    mOlofInfo2.reset();
    mTomasInfo2.reset();
    mPerIcon.reset();
    mOlofIcon.reset();
    mTomasIcon.reset();
    mPerImage.reset();
    mOlofImage.reset();
    mTomasImage.reset();
    mMain.reset();
}

void FFDemo::initStatus()
{
    mStatus = std::make_unique<FFContainer>();
    mStatus->setDimension(fcn::Rectangle(0, 80, 320, 160));
    mStatus->setVisible(false);
    mTop->add(mStatus.get());

    mPerStatus1 = std::make_unique<fcn::TextBox>(
        "  STR           EXP\n"
        "  INT           NEXT\n"
        "  DEF\n"
        "  MAGDEF\n");
    mPerStatus1->setFont(mFontCyan.get());
    mPerStatus1->setOpaque(false);
    mPerStatus1->setEditable(false);
    mPerStatus1->setFocusable(false);
    mPerStatus1->setVisible(false);
    mPerStatus1->setBorderSize(0);

    mPerStatus2 = std::make_unique<fcn::TextBox>(
        "         32          12382\n"
        "         56          13872\n"
        "         12\n"
        "         11\n\n"
        " FINALMAN is immune against\n"
        " poisinous attacks, thanks to his\n"
        " face mask.");
    mPerStatus2->setOpaque(false);
    mPerStatus2->setEditable(false);
    mPerStatus2->setFocusable(false);
    mPerStatus2->setVisible(false);
    mPerStatus2->setBorderSize(0);

    mOlofStatus1 = std::make_unique<fcn::TextBox>(
        "  STR           EXP\n"
        "  INT           NEXT\n"
        "  DEF\n"
        "  MAGDEF\n");
    mOlofStatus1->setFont(mFontCyan.get());
    mOlofStatus1->setOpaque(false);
    mOlofStatus1->setEditable(false);
    mOlofStatus1->setFocusable(false);
    mOlofStatus1->setVisible(false);
    mOlofStatus1->setBorderSize(0);

    mOlofStatus2 = std::make_unique<fcn::TextBox>(
        "          2          412382\n"
        "         72          513872\n"
        "          4\n"
        "         34\n\n"
        " YAKSLEM has one passion in life,\n"
        " to annoy other people...\n"
        " especially FINALMAN.");
    mOlofStatus2->setOpaque(false);
    mOlofStatus2->setEditable(false);
    mOlofStatus2->setFocusable(false);
    mOlofStatus2->setVisible(false);
    mOlofStatus2->setBorderSize(0);

    mTomasStatus1 = std::make_unique<fcn::TextBox>(
        "  STR           EXP\n"
        "  INT           NEXT\n"
        "  DEF\n"
        "  MAGDEF\n");
    mTomasStatus1->setFont(mFontCyan.get());
    mTomasStatus1->setOpaque(false);
    mTomasStatus1->setEditable(false);
    mTomasStatus1->setFocusable(false);
    mTomasStatus1->setVisible(false);
    mTomasStatus1->setBorderSize(0);

    mTomasStatus2 = std::make_unique<fcn::TextBox>(
        "          1          412382\n"
        "          3          513872\n"
        "          9\n"
        "         24\n\n"
        " PEAK is very weak but so cute!\n"
        " He has a tendency of answering\n"
        " any question with \"KUPO!\"");
    mTomasStatus2->setOpaque(false);
    mTomasStatus2->setEditable(false);
    mTomasStatus2->setFocusable(false);
    mTomasStatus2->setVisible(false);
    mTomasStatus2->setBorderSize(0);

    mStatus->add(mPerStatus2.get(), 5, 10);
    mStatus->add(mPerStatus1.get(), 5, 10);
    mStatus->add(mOlofStatus2.get(), 5, 10);
    mStatus->add(mOlofStatus1.get(), 5, 10);
    mStatus->add(mTomasStatus2.get(), 5, 10);
    mStatus->add(mTomasStatus1.get(), 5, 10);
}

void FFDemo::initMagicSkills()
{
    mMagicSkills = std::make_unique<FFContainer>();
    mMagicSkills->setDimension(fcn::Rectangle(0, 80, 320, 160));
    mMagicSkills->setVisible(false);

    mMagicSkillsScroll = std::make_unique<FFScrollArea>();
    mMagicSkillsScroll->setDimension(fcn::Rectangle(5, 5, 310, 150));

    mMagicSkillsList = std::make_unique<FFListBox>();
    mMagicSkillsList->setWidth(300);
    mMagicSkillsScroll->setContent(mMagicSkillsList.get());
    mMagicSkills->add(mMagicSkillsScroll.get());
    mTop->add(mMagicSkills.get());

    mPerSkills   = std::make_unique<StringListModel>();
    mPerMagic    = std::make_unique<StringListModel>();
    mOlofSkills  = std::make_unique<StringListModel>();
    mOlofMagic   = std::make_unique<StringListModel>();
    mTomasSkills = std::make_unique<StringListModel>();
    mTomasMagic  = std::make_unique<StringListModel>();

    mPerSkills->add("Use");
    mPerSkills->add("Steal");
    mPerSkills->add("Disassemble");
    mPerSkills->add("Tech-Talk");
    mPerSkills->add("Double Compile");

    mPerMagic->add("Fire");
    mPerMagic->add("Fire 2");
    mPerMagic->add("Bio");
    mPerMagic->add("Magic Missile");

    mOlofSkills->add("Annoy");
    mOlofSkills->add("Juggle");
    mOlofSkills->add("Somersault");
    mOlofSkills->add("Evil Laughter");
    mOlofSkills->add("Meta-circular Evaluation");
    mOlofSkills->add("Lisp");
    mOlofSkills->add("Cursing PHP");
    mOlofSkills->add("Paint");
    mOlofSkills->add("Compose obscure music");

    mOlofMagic->add("Ultima");
    mOlofMagic->add("Sonic Blast");

    mTomasSkills->add("Precision Throw");
    mTomasSkills->add("Jump");
    mTomasSkills->add("Dance");
    mTomasSkills->add("Much talk and little factory");
    mTomasSkills->add("Cheat");
    mTomasSkills->add("Wear hotpants");
    mTomasSkills->add("Programming Pong games");
    mTomasSkills->add("Eat meat pie");

    mTomasMagic->add("Slow");
    mTomasMagic->add("Sleep");
    mTomasMagic->add("Doom");
}

void FFDemo::cleanMagicSkills()
{
    mMagicSkillsList.reset();
    mMagicSkillsScroll.reset();
    mPerSkills.reset();
    mPerMagic.reset();
    mOlofSkills.reset();
    mOlofMagic.reset();
    mTomasSkills.reset();
    mTomasMagic.reset();
    mMagicSkills.reset();
}

void FFDemo::cleanStatus()
{
    mPerStatus1.reset();
    mPerStatus2.reset();
    mOlofStatus1.reset();
    mOlofStatus2.reset();
    mTomasStatus1.reset();
    mTomasStatus2.reset();
    mStatus.reset();
}

void FFDemo::initItems()
{
    mItems = std::make_unique<FFContainer>();

    mItemsListModel     = std::make_unique<StringListModel>();
    mItemsInfoListModel = std::make_unique<StringListModel>();
    mItemsListModel->add("23 x Potion");
    mItemsInfoListModel->add("Restores 100 HP");
    mItemsListModel->add("12 x Ether");
    mItemsInfoListModel->add("Restores 50 MP");
    mItemsListModel->add(" 8 x Elixir");
    mItemsInfoListModel->add("Restores all HP/MP");
    mItemsListModel->add("16 x Fenix Up");
    mItemsInfoListModel->add("Kills a party member");
    mItemsListModel->add(" 1 x Brass Key");
    mItemsInfoListModel->add("No idea...");
    mItemsListModel->add(" 1 x Atma Weapon");
    mItemsInfoListModel->add("Grows with it's user");
    mItemsListModel->add(" 1 x Converse Allstars");
    mItemsInfoListModel->add("Yakslems red shoes");
    mItemsListModel->add(" 1 x Oil Canister");
    mItemsInfoListModel->add("Get greasy!");
    mItemsListModel->add(" 1 x Geeky t-shirt");
    mItemsInfoListModel->add("Belongs to finalman");
    mItemsListModel->add(" 1 x Synthesizer");
    mItemsInfoListModel->add("Yakslems mega cool Ensoniq EPS 16+");
    mItemsListModel->add(" 1 x Graphic Pen");
    mItemsInfoListModel->add("Someone left it here. Maybe\nNodajo?");
    mItemsListModel->add(" 1 x Floppy Disk");
    mItemsInfoListModel->add("Stores your important data");
    mItemsListModel->add(" 1 x FifeGui Plush Doll");
    mItemsInfoListModel->add("Soooo cute and soooo plushy!!!");
    mItemsListModel->add(" 1 x Fenix Blade");
    mItemsInfoListModel->add("We are waiting for Demo3");
    mItemsListModel->add(" 2 x Joy Division LP");
    mItemsInfoListModel->add("Unknown Pleasures and Closer");

    mItemsInfo = std::make_unique<FFContainer>();
    mItemsInfo->setDimension(fcn::Rectangle(0, 0, 320, 50));
    mItemsInfo->setVisible(false);

    mItemsInfoInfo = std::make_unique<fcn::TextBox>();
    mItemsInfoInfo->setOpaque(false);
    mItemsInfoInfo->setEditable(false);
    mItemsInfoInfo->setFocusable(false);
    mItemsInfoInfo->setDimension(fcn::Rectangle(5, 5, 310, 40));
    mItemsInfoInfo->setBorderSize(0);
    mItemsInfo->add(mItemsInfoInfo.get());

    mItemsList = std::make_unique<FFListBox>();
    mItemsList->setActionEventId("items");
    mItemsList->addKeyListener(this);
    mItemsList->setWidth(300);
    mItemsList->setListModel(mItemsListModel.get());
    mItemsScrollArea = std::make_unique<FFScrollArea>();
    mItemsScrollArea->setContent(mItemsList.get());
    mItemsScrollArea->setDimension(fcn::Rectangle(5, 5, 310, 180));
    mItems = std::make_unique<FFContainer>();
    mItems->setDimension(fcn::Rectangle(0, 50, 320, 190));
    mItems->setVisible(false);
    mItems->add(mItemsScrollArea.get());
    mTop->add(mItems.get());
    mTop->add(mItemsInfo.get());
}

void FFDemo::cleanItems()
{
    mItemsInfoInfo.reset();
    mItemsList.reset();
    mItemsScrollArea.reset();
    mItemsListModel.reset();
    mItemsInfoListModel.reset();
    mItems.reset();
    mItemsInfo.reset();
}

void FFDemo::initAbout()
{
    mAbout = std::make_unique<FFContainer>();
    mAbout->setDimension(fcn::Rectangle(0, 0, 320, 240));
    mAbout->setVisible(false);
    mTop->add(mAbout.get());

    mAboutInfo = std::make_unique<fcn::TextBox>();
    mAboutInfo->setOpaque(false);
    mAboutInfo->setEditable(false);
    mAboutInfo->setFocusable(false);
    mAboutInfo->setText(
        "Welcome to FifeGUI FF Demo!\n\n"
        "What is this, you wonder?\n"
        "Well, this is a little proof of\n"
        "concept (proof of l33tness) demo\n"
        "for the FifeGUI library.\n"
        "It demonstrates the\n"
        "flexibility of the library,\n"
        "how to overload widgets to get a\n"
        "custom look and feel.\n\n"
        "FifeGUI is a GUI library\n"
        "especially made with games in\n"
        "mind. It has a modular, object\n"
        "oriented API. The back-end is\n"
        "replaceable, so it can work\n"
        "on any platform. It is bundled\n"
        "with graphics back-ends for\n"
        "SDL and OpenGL and\n"
        "input-backends for SDL.\n"
        "And don't be fooled\n"
        "by this demo, it does support\n"
        "mouse input!\n\n");

    mAboutScrollArea = std::make_unique<FFScrollArea>();
    mAboutScrollArea->setContent(mAboutInfo.get());
    mAboutScrollArea->setFocusable(true);
    mAboutScrollArea->setDimension(fcn::Rectangle(5, 5, 310, 230));
    mAboutScrollArea->setBorderSize(0);
    mAbout->add(mAboutScrollArea.get());
    mAbout->setBorderSize(0);
}

void FFDemo::cleanAbout()
{
    mAboutInfo.reset();
    mAboutScrollArea.reset();
    mAbout.reset();
}

void FFDemo::run()
{
    while (mRunning) {
        input();

        int sec       = SDL_GetTicks() / 1000;
        int const min = sec / 60;
        sec           = sec % 60;
        std::string const str;
        std::ostringstream os(str);

        if (min < 10) {
            os << " " << min << ":";
        } else {
            os << min << ":";
        }

        if (sec < 10) {
            os << "0" << sec;
        } else {
            os << sec;
        }

        mTimeLabel2->setCaption(os.str());
        mTimeLabel2->adjustSize();

        if (SDL_GetTicks() < 3000) {
            SDL_Rect src;
            SDL_Rect dst;
            src.x = src.y = 0;
            src.w = dst.w = mSplashImage->getWidth();
            src.h = dst.h = mSplashImage->getHeight();
            dst.x         = 10;
            dst.y         = 50;

            auto* image = dynamic_cast<fcn::SDLImage*>(mSplashImage.get());
            SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mRenderer);
            SDL_RenderCopy(mRenderer, image->getTexture(), &src, &dst);
        } else {
            mGui->logic();
            mGui->draw();
        }

        SDL_RenderPresent(mRenderer);
        SDL_Delay(10);
    }
}

void FFDemo::action(fcn::ActionEvent const & actionEvent)
{
    if (actionEvent.getId() == "menu") {
        switch (mMenuList->getSelected()) {
        case 0:
            mItems->setVisible(true);
            mItemsList->setSelected(0);
            mItemsList->requestFocus();
            mItemsInfo->setVisible(true);
            mItemsInfoInfo->setText(mItemsInfoListModel->getElementAt(mItemsList->getSelected()));
            break;

        case 1:
        case 2:
        case 3:
            mCharacterChooser->setSelected(0);
            mCharacterChooser->requestFocus();
            break;
        case 4:
            mAbout->setVisible(true);
            mAboutScrollArea->setVerticalScrollAmount(0);
            mAboutScrollArea->requestFocus();
            break;

        case 6:
            mRunning = false;
            break;

        default:
            break;
        }
    }

    if (actionEvent.getId() == "escape") {
        mAbout->setVisible(false);
        mItems->setVisible(false);
        mItemsInfo->setVisible(false);
        mMenu->setVisible(true);
        mTime->setVisible(true);
        mGoldFootsteps->setVisible(true);
        mMenuList->requestFocus();
        mMain->slideContentTo(0);
        mStatus->setVisible(false);
        mPerStatus1->setVisible(false);
        mPerStatus2->setVisible(false);
        mOlofStatus1->setVisible(false);
        mOlofStatus2->setVisible(false);
        mTomasStatus1->setVisible(false);
        mTomasStatus2->setVisible(false);
        mMagicSkills->setVisible(false);
        mNavigationLabel->setVisible(false);
    }

    if (actionEvent.getId() == "character") {
        mMain->slideContentTo(-76 * mCharacterChooser->getSelected());
        mMenu->setVisible(false);
        mTime->setVisible(false);
        mGoldFootsteps->setVisible(false);

        mGui->focusNone();

        mNavigationLabel->setVisible(true);
        mNavigationLabel->setY((mCharacterChooser->getSelected() * 76) + 30);

        switch (mMenuList->getSelected()) {
        case 1:
            mNavigationLabel->setCaption("STATUS");

            if (mCharacterChooser->getSelected() == 0) {
                mPerStatus1->setVisible(true);
                mPerStatus2->setVisible(true);
            } else if (mCharacterChooser->getSelected() == 1) {
                mOlofStatus1->setVisible(true);
                mOlofStatus2->setVisible(true);
            } else if (mCharacterChooser->getSelected() == 2) {
                mTomasStatus1->setVisible(true);
                mTomasStatus2->setVisible(true);
            }
            mStatus->setVisible(true);
            break;

        case 2:
            mNavigationLabel->setCaption("SKILLS");

            if (mCharacterChooser->getSelected() == 0) {
                mMagicSkillsList->setListModel(mPerSkills.get());
            } else if (mCharacterChooser->getSelected() == 1) {
                mMagicSkillsList->setListModel(mOlofSkills.get());
            } else if (mCharacterChooser->getSelected() == 2) {
                mMagicSkillsList->setListModel(mTomasSkills.get());
            }
            mMagicSkillsList->setSelected(0);
            mMagicSkills->setVisible(true);
            mMagicSkillsList->requestFocus();
            break;

        case 3:
            mNavigationLabel->setCaption("MAGIC");

            if (mCharacterChooser->getSelected() == 0) {
                mMagicSkillsList->setListModel(mPerMagic.get());
            } else if (mCharacterChooser->getSelected() == 1) {
                mMagicSkillsList->setListModel(mOlofMagic.get());
            } else if (mCharacterChooser->getSelected() == 2) {
                mMagicSkillsList->setListModel(mTomasMagic.get());
            }
            mMagicSkillsList->setSelected(0);
            mMagicSkills->setVisible(true);
            mMagicSkillsList->requestFocus();
            break;
        }
    }
}

void FFDemo::input()
{
    while (SDL_PollEvent(&mEvent) != 0) {
        if (mEvent.type == SDL_KEYDOWN) {
            if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
                Mix_PlayChannel(-1, mEscapeSound, 0);

                action(fcn::ActionEvent(nullptr, "escape"));
            } else if (
                mEvent.key.keysym.sym == SDLK_RETURN || mEvent.key.keysym.sym == SDLK_UP ||
                mEvent.key.keysym.sym == SDLK_DOWN) {
                Mix_PlayChannel(-1, mChooseSound, 0);
            } else if (mEvent.key.keysym.sym == SDLK_f) {
                bool const isFullscreen = (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
                SDL_SetWindowFullscreen(mWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                SDL_RenderSetLogicalSize(mRenderer, kUiWidth, kUiHeight);
                mSDLGraphics->setTarget(mRenderer, kUiWidth, kUiHeight);
            }
            mSDLInput->pushInput(mEvent);
        } else if (mEvent.type == SDL_KEYUP) {
            mSDLInput->pushInput(mEvent);
        } else if (mEvent.type == SDL_QUIT) {
            mRunning = false;
        }
    }
}

void FFDemo::keyPressed(fcn::KeyEvent& /*keyEvent*/)
{
    mItemsInfoInfo->setText(mItemsInfoListModel->getElementAt(mItemsList->getSelected()));
}
