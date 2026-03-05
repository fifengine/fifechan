// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __fpsdemo_hpp__
#define __fpsdemo_hpp__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#if defined(__amigaos4__)
    #include <mgl/gl.h>
    #include <mgl/glu.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <fifechan/backends/opengl/openglgraphics.hpp>
#include <fifechan/backends/opengl/openglsdlimageloader.hpp>
#include <fifechan/backends/sdl2/sdl.hpp>

#include <fifechan.hpp>

#include "fpsbutton.hpp"
#include "fpscheckbox.hpp"

/*
 * This is a ListModel we will use for resultion.
 */
class ResolutionListModel : public fcn::ListModel
{
public:
    int getNumberOfElements()
    {
        return 2;
    }

    std::string getElementAt(int i)
    {
        switch (i) {
        case 0:
            return std::string("1024x768");
        case 1:
            return std::string("800x600");
        default:
            return std::string("");
        }
    }
};

class FPSDemo : public fcn::ActionListener
{
public:
    FPSDemo();
    ~FPSDemo();
    void run();
    void action(fcn::ActionEvent const & actionEvent);

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

    SDL_Window* window      = NULL;
    SDL_GLContext glcontext = NULL;
    SDL_Event mEvent;

    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;
    Mix_Chunk* mOptionsSound;
    Mix_Music* mMusic;

    fcn::OpenGLGraphics* mOpenGLGraphics;
    fcn::SDLInput* mSDLInput;
    fcn::OpenGLSDLImageLoader* mOpenGLImageLoader;
    fcn::Gui* mGui;
    fcn::Container* mTop;
    fcn::ImageFont* mFont;
    fcn::ImageFont* mSmallBlackFont;
    fcn::ImageFont* mWhiteFont;
    fcn::ImageFont* mHighLightFont;
    fcn::Image* mTitleImage;
    fcn::Icon* mTitle;
    fcn::Image* mCloudImage;
    fcn::Image* mPlanetImage;
    fcn::Image* mStarsImage;
    fcn::Image* mMoonImage;
    fcn::Image* mMoonRedImage;

    fcn::Image* mBoxImage;
    fcn::Icon* mSingleplayBoxIcon;
    fcn::Icon* mMultiplayBoxIcon;
    fcn::Icon* mOptionsBoxIcon;

    fcn::Label* mVersionLabel;
    fcn::Label* mSingleplayLabel;
    fcn::Label* mMultiplayLabel;
    fcn::Label* mOptionsLabel;
    fcn::TextBox* mSingleplayText;
    fcn::TextBox* mMultiplayText;

    fcn::Container* mMain;
    fcn::Container* mSingleplay;
    fcn::Container* mMultiplay;
    fcn::Container* mOptions;
    fcn::TextBox* mDemoInfo;

    GLuint mPlanetTexture;
    GLuint mCloudTexture;
    GLuint mStarsTexture;
    GLuint mMoonTexture;
    GLuint mMoonRedTexture;

    GLUquadricObj* mQuad1;
    GLUquadricObj* mQuad2;
    GLUquadricObj* mClouds2;
    GLUquadricObj* mMoon;
    GLUquadricObj* mMoonRed;
    GLfloat LightAmbient[4];
    GLfloat Light2Ambient[4];
    GLfloat LightDiffuse[4];
    GLfloat Light2Diffuse[4];
    GLfloat LightPosition[4];
    GLfloat Light2Position[4];
    GLfloat LightSpotDirection[3];
    GLfloat Light2SpotDirection[3];

    FPSButton* mSingleplayButton;
    FPSButton* mMultiplayButton;
    FPSButton* mOptionsButton;
    FPSButton* mQuitButton;
    FPSButton* mSingleplayBackButton;
    FPSButton* mMultiplayBackButton;
    FPSButton* mOptionsBackButton;

    FPSCheckBox* mFullScreen;
    fcn::DropDown* mResolution;
    ResolutionListModel* mResolutionListModel;
    fcn::ScrollArea* mResolutionScrollArea;
    fcn::ListBox* mResolutionListBox;

    fcn::Slider* mVolume;
    fcn::Label* mVolumePercent;
    fcn::Label* mVolumeLabel;
    fcn::Label* mResolutionLabel;
    fcn::Image* mSplashImage;
    fcn::Icon* mSplashIcon;
};

#endif
