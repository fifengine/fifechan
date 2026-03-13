// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include "fpsdemo.hpp"

#include <cmath>
#include <memory>
#include <sstream>
#include <string>

namespace
{
    std::unique_ptr<fcn::Image> loadSplashImageWithWhiteAsTransparent(std::string const & filename)
    {
        auto image = std::unique_ptr<fcn::Image>(fcn::Image::load(filename, false));

        bool hasTransparency = false;

        for (int y = 0; y < image->getHeight() && !hasTransparency; ++y) {
            for (int x = 0; x < image->getWidth(); ++x) {
                if (image->getPixel(x, y).a != 255) {
                    hasTransparency = true;
                    break;
                }
            }
        }

        if (!hasTransparency) {
            for (int y = 0; y < image->getHeight(); ++y) {
                for (int x = 0; x < image->getWidth(); ++x) {
                    fcn::Color const color = image->getPixel(x, y);
                    if (color.r == 255 && color.g == 255 && color.b == 255) {
                        image->putPixel(x, y, fcn::Color(255, 255, 255, 0));
                    }
                }
            }
        }

        image->convertToDisplayFormat();
        return image;
    }
} // namespace

FPSDemo::FPSDemo() :
    mRotation(0),
    mRunning(true),
    mGlow(0),
    mWidth(800),
    mHeight(600),
    mTime(-1),
    mDeltaTime(0),
    mInit(true),
    mResolutionChange(false),
    mHaveFullscreen(false),
    mAudioAvailable(false),
    glcontext(SDL_GL_CreateContext(window)),
    window(SDL_CreateWindow("FifeGUI FPS demo", 0, 0, 800, 600, SDL_WINDOW_OPENGL)),
    mChooseSound(nullptr),
    mEscapeSound(nullptr),
    mOptionsSound(nullptr),
    mMusic(nullptr)
{
    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Init SDL_Mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == 0) {
        mAudioAvailable = true;

        // Load sounds and music
        mChooseSound  = Mix_LoadWAV("sound/sound4.wav");
        mEscapeSound  = Mix_LoadWAV("sound/sound3.wav");
        mOptionsSound = Mix_LoadWAV("sound/sound2.wav");
        mMusic        = Mix_LoadMUS("sound/space.ogg");

        // Set the mixer volume
        Mix_Volume(-1, static_cast<int>(MIX_MAX_VOLUME * 0.7));
    }

    // Create some GLU quadrics
    mQuad1   = gluNewQuadric();
    mQuad2   = gluNewQuadric();
    mMoon    = gluNewQuadric();
    mMoonRed = gluNewQuadric();
    mClouds2 = gluNewQuadric();
    gluQuadricNormals(mQuad1, GLU_SMOOTH);
    gluQuadricTexture(mQuad1, GL_TRUE);
    gluQuadricNormals(mQuad2, GLU_SMOOTH);
    gluQuadricTexture(mQuad2, GL_TRUE);
    gluQuadricNormals(mMoon, GLU_SMOOTH);
    gluQuadricTexture(mMoon, GL_TRUE);
    gluQuadricNormals(mMoonRed, GLU_SMOOTH);
    gluQuadricTexture(mMoonRed, GL_TRUE);
    gluQuadricNormals(mClouds2, GLU_SMOOTH);
    gluQuadricTexture(mClouds2, GL_TRUE);

    initOpenGL();
    initGui();
    resize();

    mInit = false;
}

FPSDemo::~FPSDemo()
{
    cleanGui();
    if (mAudioAvailable) {
        Mix_FreeChunk(mChooseSound);
        Mix_FreeChunk(mEscapeSound);
        Mix_FreeChunk(mOptionsSound);
        Mix_FreeMusic(mMusic);
        Mix_CloseAudio();
    }
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void FPSDemo::initGui()
{
    mOpenGLImageLoader = std::make_unique<fcn::opengl::ImageLoader>();
    fcn::Image::setImageLoader(mOpenGLImageLoader.get());
    mOpenGLGraphics = std::make_unique<fcn::opengl::Graphics>();
    mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
    mSDLInput = std::make_unique<fcn::sdl2::Input>();

    mTop = std::make_unique<fcn::Container>();
    mTop->setOpaque(false);
    mGui = std::make_unique<fcn::Gui>();
    mGui->setTabbingEnabled(false);
    mGui->setGraphics(mOpenGLGraphics.get());
    mGui->setInput(mSDLInput.get());

    mFont = std::make_unique<fcn::ImageFont>(
        "images/techyfontbig2.png",
        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mHighLightFont = std::make_unique<fcn::ImageFont>(
        "images/techyfontbighighlight.png",
        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mSmallBlackFont = std::make_unique<fcn::ImageFont>(
        "images/techyfontblack.png",
        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    mWhiteFont = std::make_unique<fcn::ImageFont>(
        "images/techyfontwhite.png",
        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    fcn::Widget::setGlobalFont(mWhiteFont.get());

    mTitleImage = std::unique_ptr<fcn::Image>(fcn::Image::load("images/title2.png"));
    mTitle      = std::make_unique<fcn::Icon>(mTitleImage.get());
    mTitle->setOpaque(false);
    mTop->add(mTitle.get());

    mDemoInfo = std::make_unique<fcn::TextBox>(
        "This is a Demo demonstrating FifeGUI with SDL and OpenGL.\n"
        "FifeGUI is dual-licensed under LGPL v2.1 and BSD.\n"
        "For more information visit http://fifengine.github.io/fifechan/\n");
    mDemoInfo->setFont(mSmallBlackFont.get());
    mDemoInfo->setOpaque(false);
    mDemoInfo->setEditable(false);
    mDemoInfo->setFocusable(false);
    mDemoInfo->setBorderSize(0);
    mTop->add(mDemoInfo.get());

    mVersionLabel = std::make_unique<fcn::Label>("Version 1.01");
    mVersionLabel->setFont(mSmallBlackFont.get());
    mTop->add(mVersionLabel.get());

    mBoxImage = std::unique_ptr<fcn::Image>(fcn::Image::load("images/box.png"));

    mSplashImage = loadSplashImageWithWhiteAsTransparent("images/splash.png");
    mSplashIcon  = std::make_unique<fcn::Icon>(mSplashImage.get());
    mSplashIcon->setOpaque(false);

    if (mInit) {
        mGui->setTop(mSplashIcon.get());
        mSplashIcon->setPosition(
            (mWidth / 2) - (mSplashImage->getWidth() / 2), (mHeight / 2) - (mSplashImage->getHeight() / 2));
    } else {
        mGui->setTop(mTop.get());
    }

    loadTextures();
    initMain();
    initSingleplay();
    initMultiplay();
    initOptions();
}

void FPSDemo::cleanGui()
{
    if (mGui != nullptr) {
        mGui->setTop(nullptr);
    }
    fcn::Widget::setGlobalFont(nullptr);
    fcn::Image::setImageLoader(nullptr);

    cleanTextures();
    cleanMain();
    cleanSingleplay();
    cleanMultiplay();
    cleanOptions();

    mBoxImage.reset();
    mFont.reset();
    mHighLightFont.reset();
    mSmallBlackFont.reset();
    mWhiteFont.reset();
    mTop.reset();
    mGui.reset();
    mSplashIcon.reset();
    mSplashImage.reset();
    mSDLInput.reset();
    mOpenGLGraphics.reset();
    mOpenGLImageLoader.reset();
}

/*
 * Init the main container
 */
void FPSDemo::initMain()
{
    mMain = std::make_unique<fcn::Container>();
    mMain->setOpaque(false);
    mTop->add(mMain.get());

    mSingleplayButton = std::make_unique<FPSButton>("Singleplayer");
    mSingleplayButton->setHighLightFont(mHighLightFont.get());
    mSingleplayButton->adjustSize();
    mSingleplayButton->setActionEventId("singleplay");
    mSingleplayButton->addActionListener(this);
    mSingleplayButton->setFont(mFont.get());
    mSingleplayButton->adjustSize();
    mMain->add(mSingleplayButton.get());

    mMultiplayButton = std::make_unique<FPSButton>("Multiplayer");
    mMultiplayButton->setHighLightFont(mHighLightFont.get());
    mMultiplayButton->adjustSize();
    mMultiplayButton->setActionEventId("multiplay");
    mMultiplayButton->addActionListener(this);
    mMultiplayButton->setFont(mFont.get());
    mMultiplayButton->adjustSize();
    mMain->add(mMultiplayButton.get());

    mOptionsButton = std::make_unique<FPSButton>("Options");
    mOptionsButton->setHighLightFont(mHighLightFont.get());
    mOptionsButton->adjustSize();
    mOptionsButton->setActionEventId("options");
    mOptionsButton->addActionListener(this);
    mOptionsButton->setFont(mFont.get());
    mOptionsButton->adjustSize();
    mMain->add(mOptionsButton.get());

    mQuitButton = std::make_unique<FPSButton>("Quit");
    mQuitButton->setHighLightFont(mHighLightFont.get());
    mQuitButton->adjustSize();
    mQuitButton->setActionEventId("quit");
    mQuitButton->addActionListener(this);
    mQuitButton->setFont(mFont.get());
    mQuitButton->adjustSize();
    mMain->add(mQuitButton.get());
}

void FPSDemo::cleanMain()
{
    mSingleplayButton.reset();
    mMultiplayButton.reset();
    mOptionsButton.reset();
    mQuitButton.reset();
    mVersionLabel.reset();
    mTitle.reset();
    mTitleImage.reset();
    mDemoInfo.reset();
    mMain.reset();
}

/*
 * Init the singleplay
 */
void FPSDemo::initSingleplay()
{
    mSingleplay = std::make_unique<fcn::Container>();
    mSingleplay->setVisible(false);
    mSingleplay->setOpaque(false);
    mTop->add(mSingleplay.get());

    mSingleplayBoxIcon = std::make_unique<fcn::Icon>(mBoxImage.get());
    mSingleplay->add(mSingleplayBoxIcon.get());

    mSingleplayLabel = std::make_unique<fcn::Label>("Singleplayer");
    mSingleplayLabel->setFont(mWhiteFont.get());
    mSingleplayLabel->adjustSize();
    mSingleplay->add(mSingleplayLabel.get());

    mSingleplayText = std::make_unique<fcn::TextBox>(
        "I'm verry sorry but this is not an actual game.\n"
        "It's a demonstration of the GUI library Fifechan.\n"
        "But who knows...\n"
        "Maybe it will be a game here someday.\n");
    mSingleplayText->setFont(mWhiteFont.get());
    mSingleplayText->setOpaque(false);
    mSingleplayText->setEditable(false);
    mSingleplayText->setBorderSize(0);
    mSingleplay->add(mSingleplayText.get());

    mSingleplayBackButton = std::make_unique<FPSButton>("Back");
    mSingleplayBackButton->setHighLightFont(mHighLightFont.get());
    mSingleplayBackButton->adjustSize();
    mSingleplayBackButton->setActionEventId("back");
    mSingleplayBackButton->addActionListener(this);
    mSingleplayBackButton->setFont(mFont.get());
    mSingleplayBackButton->adjustSize();

    mSingleplay->add(mSingleplayBackButton.get());
}

void FPSDemo::cleanSingleplay()
{
    mSingleplayBoxIcon.reset();
    mSingleplayBackButton.reset();
    mSingleplayText.reset();
    mSingleplayLabel.reset();
    mSingleplay.reset();
}

/*
 * Init the multiplay container
 */
void FPSDemo::initMultiplay()
{
    mMultiplay = std::make_unique<fcn::Container>();
    mMultiplay->setVisible(false);
    mMultiplay->setOpaque(false);
    mTop->add(mMultiplay.get());

    mMultiplayBoxIcon = std::make_unique<fcn::Icon>(mBoxImage.get());
    mMultiplay->add(mMultiplayBoxIcon.get());

    mMultiplayLabel = std::make_unique<fcn::Label>("Multiplayer");
    mMultiplayLabel->setFont(mWhiteFont.get());
    mMultiplayLabel->adjustSize();
    mMultiplay->add(mMultiplayLabel.get());

    mMultiplayText = std::make_unique<fcn::TextBox>(
        "I'm verry sorry but this is not an actuall game.\n"
        "It's a demonstration of the GUI library Fifechan.\n"
        "But who knows...\n"
        "Maybe it will be a game here someday.\n");
    mMultiplayText->setFont(mWhiteFont.get());
    mMultiplayText->setOpaque(false);
    mMultiplayText->setEditable(false);
    mMultiplayText->setBorderSize(0);
    mMultiplay->add(mMultiplayText.get());

    mMultiplayBackButton = std::make_unique<FPSButton>("Back");
    mMultiplayBackButton->setHighLightFont(mHighLightFont.get());
    mMultiplayBackButton->adjustSize();
    mMultiplayBackButton->setActionEventId("back");
    mMultiplayBackButton->addActionListener(this);
    mMultiplayBackButton->setFont(mFont.get());

    mMultiplayBackButton->adjustSize();

    mMultiplay->add(mMultiplayBackButton.get());
}

void FPSDemo::cleanMultiplay()
{
    mMultiplayBoxIcon.reset();
    mMultiplayBackButton.reset();
    mMultiplayText.reset();
    mMultiplayLabel.reset();
    mMultiplay.reset();
}

/*
 * Init the options container
 */
void FPSDemo::initOptions()
{
    mOptions = std::make_unique<fcn::Container>();
    mOptions->setVisible(false);
    mOptions->setOpaque(false);
    mTop->add(mOptions.get());

    mOptionsBoxIcon = std::make_unique<fcn::Icon>(mBoxImage.get());
    mOptions->add(mOptionsBoxIcon.get());

    mOptionsLabel = std::make_unique<fcn::Label>("Options");
    mOptionsLabel->setFont(mWhiteFont.get());
    mOptionsLabel->adjustSize();
    mOptions->add(mOptionsLabel.get());

    mFullScreen = std::make_unique<FPSCheckBox>("Fullscreen");
    mFullScreen->setFont(mWhiteFont.get());
    mFullScreen->adjustSize();
    mFullScreen->setBackgroundColor(fcn::Color(0x331010));
    mFullScreen->setForegroundColor(fcn::Color(0xffffff));
    mFullScreen->setBaseColor(fcn::Color(0x771010));
    mFullScreen->setActionEventId("fullscreen");
    mFullScreen->addActionListener(this);
    mFullScreen->setSelected(mHaveFullscreen);
    mOptions->add(mFullScreen.get());

    mResolutionScrollArea = std::make_unique<fcn::ScrollArea>();
    mResolutionScrollArea->setBackgroundColor(fcn::Color(0x331010));
    mResolutionScrollArea->setForegroundColor(fcn::Color(0x331010));
    mResolutionScrollArea->setBaseColor(fcn::Color(0x771010));

    mResolutionListBox = std::make_unique<fcn::ListBox>();
    mResolutionListBox->setBackgroundColor(fcn::Color(0x331010));
    mResolutionListBox->setForegroundColor(fcn::Color(0x331010));
    mResolutionListBox->setBaseColor(fcn::Color(0x771010));
    mResolutionListBox->setSelectionColor(fcn::Color(0x552020));

    mResolutionListModel = std::make_unique<ResolutionListModel>();
    mResolution          = std::make_unique<fcn::DropDown>(
        mResolutionListModel.get(), mResolutionScrollArea.get(), mResolutionListBox.get());
    mResolution->setWidth(200);
    mResolution->setBackgroundColor(fcn::Color(0x331010));
    mResolution->setForegroundColor(fcn::Color(0x331010));
    mResolution->setBaseColor(fcn::Color(0x771010));
    mResolution->setSelectionColor(fcn::Color(0x552020));

    if (mWidth == 800) {
        mResolution->setSelected(1);
    } else {
        mResolution->setSelected(0);
    }
    mResolution->setActionEventId("resolution");
    mResolution->addActionListener(this);
    mOptions->add(mResolution.get());

    mVolume = std::make_unique<fcn::Slider>(0.0, 1.0);
    mVolume->setWidth(200);
    mVolume->setHeight(20);
    mVolume->setValue(0.7);
    mVolume->setBackgroundColor(fcn::Color(0x331010));
    mVolume->setForegroundColor(fcn::Color(0x331010));
    mVolume->setBaseColor(fcn::Color(0x771010));
    mVolume->setActionEventId("volume");
    mVolume->addActionListener(this);
    mVolume->setMarkerLength(20);
    mOptions->add(mVolume.get());

    mVolumePercent = std::make_unique<fcn::Label>("70%");
    mOptions->add(mVolumePercent.get());

    mVolumeLabel = std::make_unique<fcn::Label>("Volume");
    mOptions->add(mVolumeLabel.get());

    mResolutionLabel = std::make_unique<fcn::Label>("Resolution");
    mOptions->add(mResolutionLabel.get());

    mOptionsBackButton = std::make_unique<FPSButton>("Back");
    mOptionsBackButton->setHighLightFont(mHighLightFont.get());
    mOptionsBackButton->adjustSize();
    mOptionsBackButton->setActionEventId("back");
    mOptionsBackButton->addActionListener(this);
    mOptionsBackButton->setFont(mFont.get());
    mOptionsBackButton->adjustSize();
    mOptions->add(mOptionsBackButton.get());
}

void FPSDemo::cleanOptions()
{
    mOptionsBoxIcon.reset();
    mOptionsBackButton.reset();
    mResolutionLabel.reset();
    mVolumeLabel.reset();
    mVolumePercent.reset();
    mVolume.reset();
    mResolutionListBox.reset();
    mResolutionScrollArea.reset();
    mResolutionListModel.reset();
    mResolution.reset();
    mFullScreen.reset();
    mOptionsLabel.reset();
    mOptions.reset();
}

void FPSDemo::loadTextures()
{
    fcn::OpenGLImage* image = nullptr;
    // Load textures with the OpenGLImageLoader from Guichan
    mCloudImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/cloudsblackwhite.png"));
    image         = dynamic_cast<fcn::OpenGLImage*>(mCloudImage.get());
    mCloudTexture = image->getTextureHandle();

    mPlanetImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/planet.png"));
    image          = dynamic_cast<fcn::OpenGLImage*>(mPlanetImage.get());
    mPlanetTexture = image->getTextureHandle();

    mStarsImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/background.png"));
    image         = dynamic_cast<fcn::OpenGLImage*>(mStarsImage.get());
    mStarsTexture = image->getTextureHandle();

    mMoonImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/moon.png"));
    image        = dynamic_cast<fcn::OpenGLImage*>(mMoonImage.get());
    mMoonTexture = image->getTextureHandle();

    mMoonRedImage   = std::unique_ptr<fcn::Image>(fcn::Image::load("images/moonred.png"));
    image           = dynamic_cast<fcn::OpenGLImage*>(mMoonRedImage.get());
    mMoonRedTexture = image->getTextureHandle();
}

void FPSDemo::cleanTextures()
{
    mPlanetImage.reset();
    mCloudImage.reset();
    mStarsImage.reset();
    mMoonImage.reset();
    mMoonRedImage.reset();
}

/*
 * Draw the planet and the two moons
 */
void FPSDemo::drawBackground()
{
    glEnable(GL_LIGHTING);
    mRotation += mDeltaTime / 2000;

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(80, 0.0, 0.0, 1.0);
    glRotatef(mRotation * 7, 0.0, -1.0, 0.0);
    glTranslatef(0, 0.0, 2.1);
    glRotatef(mRotation * 50, 1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mMoonTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mMoon, .07, 10, 10);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(110, 0.0, 1.0, 0.0);
    glRotatef(mRotation * 5, 0.0, 0.0, 1.0);
    glTranslatef(2.2, 0.0, 0.0);
    glRotatef(mRotation * 30, 1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mMoonRedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mMoonRed, 0.15, 10, 10);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(mRotation * 3, 1.0, 0.0, 0.0);
    glRotatef(77, 0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mPlanetTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad1, 1.93, 60, 60);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(mRotation * 6, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mCloudTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad2, 2.0, 60, 60);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(mRotation * 5, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mCloudTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad2, 1.98, 60, 60);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glDisable(GL_LIGHTING);
}

/**
 * Draw the space background
 */
void FPSDemo::drawSpace()
{
    int const y = -200;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 800, 600, 0.0, 1, 0.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mStarsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glScalef(2, 1.5, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3i(0, y, 0);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3i(mStarsImage->getWidth(), y, 0);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3i(mStarsImage->getWidth(), mStarsImage->getHeight() + y, 0);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3i(0, mStarsImage->getHeight() + y, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void FPSDemo::run()
{
    runIntro();
    runMain();
}

/*
 * Just an intro showing our cool Darkbits logo
 */
void FPSDemo::runIntro()
{
    while (SDL_GetTicks() < 3000) {
        if (mTime < 0) {
            mTime = SDL_GetTicks();
        }
        mDeltaTime = SDL_GetTicks() - mTime;
        mTime      = SDL_GetTicks();

        input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mGui->draw();
        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    mGui->setTop(mTop.get());

    if (mAudioAvailable && mMusic != nullptr) {
        Mix_FadeInMusic(mMusic, -1, 2000);
    }
}

/*
 * The main run loop
 */
void FPSDemo::runMain()
{
    while (mRunning) {
        if (mResolutionChange) {
            // Clear the screen before remaking the Gui
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SDL_GL_SwapWindow(window);
            cleanGui();
            initGui();
            resize();
            mResolutionChange = false;
        }

        if (mTime < 0) {
            mTime = SDL_GetTicks();
        }

        mDeltaTime = SDL_GetTicks() - mTime;
        mTime      = SDL_GetTicks();

        input();
        mGui->logic();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        drawSpace();
        gluLookAt(0.0, -1.8, -2.9, 0.0, -1.2, 1.0, 0.0, -1.0, 0.0);

        drawBackground();
        glPushMatrix();
        mGui->draw();
        glPopMatrix();

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }
}

void FPSDemo::input()
{
    while (SDL_PollEvent(&mEvent) != 0) {
        // We ignore keyboard input and just sends mouse input to Fifechan
        if (mEvent.type == SDL_KEYDOWN) {
            if (mEvent.key.keysym.sym == SDLK_ESCAPE) {
                mMain->setVisible(true);
                mSingleplay->setVisible(false);
                mMultiplay->setVisible(false);
                mOptions->setVisible(false);
            }
        } else if (mEvent.type == SDL_QUIT) {
            mRunning = false;
        } else if (
            mEvent.type == SDL_MOUSEMOTION || mEvent.type == SDL_MOUSEBUTTONDOWN || mEvent.type == SDL_MOUSEBUTTONUP) {
            mSDLInput->pushInput(mEvent);
        }
    }
}

/**
 * The action function from ActionListener
 */
void FPSDemo::action(fcn::ActionEvent const & actionEvent)
{
    std::string const & id = actionEvent.getId();

    if (id == "quit") {
        if (mAudioAvailable && mEscapeSound != nullptr) {
            Mix_PlayChannel(-1, mEscapeSound, 0);
        }
        mRunning = false;
    } else if (id == "singleplay") {
        if (mAudioAvailable && mChooseSound != nullptr) {
            Mix_PlayChannel(-1, mChooseSound, 0);
        }
        mMain->setVisible(false);
        mSingleplay->setVisible(true);
    } else if (id == "multiplay") {
        if (mAudioAvailable && mChooseSound != nullptr) {
            Mix_PlayChannel(-1, mChooseSound, 0);
        }
        mMain->setVisible(false);
        mMultiplay->setVisible(true);
    } else if (id == "options") {
        if (mAudioAvailable && mChooseSound != nullptr) {
            Mix_PlayChannel(-1, mChooseSound, 0);
        }
        mMain->setVisible(false);
        mOptions->setVisible(true);
    } else if (id == "back") {
        if (mAudioAvailable && mEscapeSound != nullptr) {
            Mix_PlayChannel(-1, mEscapeSound, 0);
        }
        mMain->setVisible(true);
        mSingleplay->setVisible(false);
        mMultiplay->setVisible(false);
        mOptions->setVisible(false);
    } else if (id == "fullscreen" || id == "resolution") {
        if (mAudioAvailable && mOptionsSound != nullptr) {
            Mix_PlayChannel(-1, mOptionsSound, 0);
        }
        initVideo();
    } else if (id == "volume") {
        std::ostringstream os;
        os << static_cast<int>(mVolume->getValue() * 100) << "%";
        mVolumePercent->setCaption(os.str());
        mVolumePercent->adjustSize();
        if (mAudioAvailable) {
            double const m = MIX_MAX_VOLUME;
            double const p = mVolume->getValue();
            Mix_Volume(-1, static_cast<int>(m * p));
        }
    }
}

void FPSDemo::initVideo()
{
    if (mResolution->getSelected() == 0) {
        mWidth            = 1024;
        mHeight           = 768;
        mResolutionChange = true;
    } else if (mResolution->getSelected() == 1) {
        mWidth            = 800;
        mHeight           = 600;
        mResolutionChange = true;
    }
    if (mFullScreen->isSelected()) {
        SDL_SetWindowSize(window, mWidth, mHeight);
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        mHaveFullscreen = true;
    } else {
        mHaveFullscreen = false;
        SDL_SetWindowFullscreen(window, 0);
        SDL_SetWindowSize(window, mWidth, mHeight);
    }
    mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
    initOpenGL();
}

void FPSDemo::initOpenGL()
{
    // Init OpenGL
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0F);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, static_cast<double>(mWidth) / static_cast<double>(mHeight), 1.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Init Light
    Light2Ambient[0] = 0.5F;
    Light2Ambient[1] = 0.4F;
    Light2Ambient[2] = 0.7F;
    Light2Ambient[3] = 1.0F;

    Light2Diffuse[0] = 1.0F;
    Light2Diffuse[1] = 1.0F;
    Light2Diffuse[2] = 1.0F;
    Light2Diffuse[3] = 1.0F;

    Light2Position[0] = 10.0F;
    Light2Position[1] = .5F;
    Light2Position[2] = 0.0F;
    Light2Position[3] = 1.0F;

    Light2SpotDirection[0] = -1.0;
    Light2SpotDirection[1] = 0.0;
    Light2SpotDirection[2] = 0.0;

    glLightfv(GL_LIGHT2, GL_POSITION, Light2Position.data());
    glLightfv(GL_LIGHT2, GL_AMBIENT, Light2Ambient.data());
    glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse.data());
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Light2SpotDirection.data());
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);

    glEnable(GL_LIGHT2);
}

void FPSDemo::resize()
{
    int const mHalfWidth  = mWidth / 2;
    int const mHalfHeight = mHeight / 2;

    mTitle->setPosition(mHalfWidth - 330, mHalfHeight - 290);
    mDemoInfo->setPosition(mHalfWidth - 390, mHeight - 50);
    mVersionLabel->setPosition(mWidth - 100, mHeight - 80);

    mTop->setDimension(fcn::Rectangle(0, 0, mWidth, mHeight));
    mMain->setDimension(fcn::Rectangle(0, 0, mWidth, mHeight));
    mSingleplay->setDimension(fcn::Rectangle(0, 0, mWidth, mHeight));
    mMultiplay->setDimension(fcn::Rectangle(0, 0, mWidth, mHeight));
    mOptions->setDimension(fcn::Rectangle(0, 0, mWidth, mHeight));

    mSingleplayButton->setPosition(mHalfWidth - 100, mHalfHeight - 100);
    mMultiplayButton->setPosition(mHalfWidth - 100, mHalfHeight - 60);
    mOptionsButton->setPosition(mHalfWidth - 100, mHalfHeight - 20);
    mQuitButton->setPosition(mHalfWidth - 100, mHalfHeight + 60);

    mSingleplayBackButton->setPosition(mHalfWidth - 290, mHalfHeight + 180);
    mMultiplayBackButton->setPosition(mHalfWidth - 290, mHalfHeight + 180);
    mOptionsBackButton->setPosition(mHalfWidth - 290, mHalfHeight + 180);

    mSingleplayBoxIcon->setPosition(mHalfWidth - 300, mHalfHeight - 150);
    mMultiplayBoxIcon->setPosition(mHalfWidth - 300, mHalfHeight - 150);
    mOptionsBoxIcon->setPosition(mHalfWidth - 300, mHalfHeight - 150);

    mSingleplayText->setPosition(mHalfWidth - 285, mHalfHeight - 120);
    mSingleplayLabel->setPosition(mHalfWidth + 150, mHalfHeight - 145);

    mMultiplayText->setPosition(mHalfWidth - 285, mHalfHeight - 120);
    mMultiplayLabel->setPosition(mHalfWidth + 150, mHalfHeight - 145);

    mOptionsLabel->setPosition(mHalfWidth + 150, mHalfHeight - 145);
    mFullScreen->setPosition(mHalfWidth - 200, mHalfHeight - 100);
    mResolution->setPosition(mHalfWidth - 90, mHalfHeight - 70);
    mResolutionLabel->setPosition(mHalfWidth - 200, mHalfHeight - 70);
    mVolume->setPosition(mHalfWidth - 90, mHalfHeight - 40);
    mVolumePercent->setPosition(mHalfWidth + 120, mHalfHeight - 40);
    mVolumeLabel->setPosition(mHalfWidth - 200, mHalfHeight - 40);
}
