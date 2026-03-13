// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#if defined(_WIN32)
    #include <windows.h>

#endif

#if defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <fifechan/backends/opengl/graphics.hpp>
#include <fifechan/backends/opengl/imageloader.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>

#include <fifechan.hpp>

#include <array>
#include <memory>
#include <string>

#include "fpsbutton.hpp"
#include "fpscheckbox.hpp"

/*
 * This is a ListModel we will use for resultion.
 */
class ResolutionListModel : public fcn::ListModel
{
public:
    int getNumberOfElements() override
    {
        return 2;
    }

    std::string getElementAt(int i) override
    {
        switch (i) {
        case 0:
            return {"1024x768"};
        case 1:
            return {"800x600"};
        default:
            return {""};
        }
    }
};

class FPSDemo : public fcn::ActionListener
{
public:
    FPSDemo();
    ~FPSDemo() override;

    FPSDemo(FPSDemo const &)            = delete;
    FPSDemo& operator=(FPSDemo const &) = delete;

    FPSDemo(FPSDemo&&)            = delete;
    FPSDemo& operator=(FPSDemo&&) = delete;

    void run();
    void action(fcn::ActionEvent const & actionEvent) override;

private:
    void runIntro();
    void runMain();
    void resize();
    void initOpenGL();
    void initVideo();
    void initGui();
    void cleanGui();
    void initMain();
    void cleanMain();
    void initSingleplay();
    void cleanSingleplay();
    void initMultiplay();
    void cleanMultiplay();
    void initOptions();
    void cleanOptions();

    void loadTextures();
    void cleanTextures();

    void input();
    void drawSpace();
    void drawBackground();

    bool mRunning;
    bool mInit;
    bool mResolutionChange;
    bool mHaveFullscreen;
    bool mAudioAvailable;

    int mWidth;
    int mHeight;

    float mRotation;
    double mGlow;
    float mDeltaTime;
    float mTime;

    SDL_Window* window      = nullptr;
    SDL_GLContext glcontext = nullptr;
    SDL_Event mEvent{};

    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;
    Mix_Chunk* mOptionsSound;
    Mix_Music* mMusic;

    std::unique_ptr<fcn::opengl::Graphics> mOpenGLGraphics;
    std::unique_ptr<fcn::sdl2::Input> mSDLInput;
    std::unique_ptr<fcn::opengl::ImageLoader> mOpenGLImageLoader;
    std::unique_ptr<fcn::Gui> mGui;
    std::unique_ptr<fcn::Container> mTop;
    std::unique_ptr<fcn::ImageFont> mFont;
    std::unique_ptr<fcn::ImageFont> mSmallBlackFont;
    std::unique_ptr<fcn::ImageFont> mWhiteFont;
    std::unique_ptr<fcn::ImageFont> mHighLightFont;
    std::unique_ptr<fcn::Image> mTitleImage;
    std::unique_ptr<fcn::Icon> mTitle;
    std::unique_ptr<fcn::Image> mCloudImage;
    std::unique_ptr<fcn::Image> mPlanetImage;
    std::unique_ptr<fcn::Image> mStarsImage;
    std::unique_ptr<fcn::Image> mMoonImage;
    std::unique_ptr<fcn::Image> mMoonRedImage;

    std::unique_ptr<fcn::Image> mBoxImage;
    std::unique_ptr<fcn::Icon> mSingleplayBoxIcon;
    std::unique_ptr<fcn::Icon> mMultiplayBoxIcon;
    std::unique_ptr<fcn::Icon> mOptionsBoxIcon;

    std::unique_ptr<fcn::Label> mVersionLabel;
    std::unique_ptr<fcn::Label> mSingleplayLabel;
    std::unique_ptr<fcn::Label> mMultiplayLabel;
    std::unique_ptr<fcn::Label> mOptionsLabel;
    std::unique_ptr<fcn::TextBox> mSingleplayText;
    std::unique_ptr<fcn::TextBox> mMultiplayText;

    std::unique_ptr<fcn::Container> mMain;
    std::unique_ptr<fcn::Container> mSingleplay;
    std::unique_ptr<fcn::Container> mMultiplay;
    std::unique_ptr<fcn::Container> mOptions;
    std::unique_ptr<fcn::TextBox> mDemoInfo;

    GLuint mPlanetTexture{};
    GLuint mCloudTexture{};
    GLuint mStarsTexture{};
    GLuint mMoonTexture{};
    GLuint mMoonRedTexture{};

    GLUquadricObj* mQuad1;
    GLUquadricObj* mQuad2;
    GLUquadricObj* mClouds2;
    GLUquadricObj* mMoon;
    GLUquadricObj* mMoonRed;
    std::array<GLfloat, 4> LightAmbient{};
    std::array<GLfloat, 4> Light2Ambient{};
    std::array<GLfloat, 4> LightDiffuse{};
    std::array<GLfloat, 4> Light2Diffuse{};
    std::array<GLfloat, 4> LightPosition{};
    std::array<GLfloat, 4> Light2Position{};
    std::array<GLfloat, 3> LightSpotDirection{};
    std::array<GLfloat, 3> Light2SpotDirection{};

    std::unique_ptr<FPSButton> mSingleplayButton;
    std::unique_ptr<FPSButton> mMultiplayButton;
    std::unique_ptr<FPSButton> mOptionsButton;
    std::unique_ptr<FPSButton> mQuitButton;
    std::unique_ptr<FPSButton> mSingleplayBackButton;
    std::unique_ptr<FPSButton> mMultiplayBackButton;
    std::unique_ptr<FPSButton> mOptionsBackButton;

    std::unique_ptr<FPSCheckBox> mFullScreen;
    std::unique_ptr<fcn::DropDown> mResolution;
    std::unique_ptr<ResolutionListModel> mResolutionListModel;
    std::unique_ptr<fcn::ScrollArea> mResolutionScrollArea;
    std::unique_ptr<fcn::ListBox> mResolutionListBox;

    std::unique_ptr<fcn::Slider> mVolume;
    std::unique_ptr<fcn::Label> mVolumePercent;
    std::unique_ptr<fcn::Label> mVolumeLabel;
    std::unique_ptr<fcn::Label> mResolutionLabel;
    std::unique_ptr<fcn::Image> mSplashImage;
    std::unique_ptr<fcn::Icon> mSplashIcon;
};
