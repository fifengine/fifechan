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
 
#ifndef __fpsdemo_hpp__
#define __fpsdemo_hpp__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#if defined (__amigaos4__)
#include <mgl/gl.h>
#include <mgl/glu.h>
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "fpsbutton.hpp"
#include "fpscheckbox.hpp"

/*
 * This is a ListModel we will use for resultion.
 */
class ResolutionListModel : public gcn::ListModel
{
public:
    int getNumberOfElements()
    {
        return 2;
    }

    std::string getElementAt(int i)
    {
        switch(i)
        {
          case 0:
              return std::string("1024x768");
          case 1:
              return std::string("800x600");
          default:
              return std::string("");
        }
    }
};

class FPSDemo : public gcn::ActionListener
{
public:
    FPSDemo();
    ~FPSDemo();
    void run();
    void action(const gcn::ActionEvent& actionEvent);
	
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
	
    int mWidth;
    int mHeight;

    float mRotation;
    double mGlow;
    float mDeltaTime;
    float mTime;
	
    SDL_Surface* mScreen;
    SDL_Event mEvent;

    Mix_Chunk* mChooseSound;
    Mix_Chunk* mEscapeSound;
    Mix_Chunk* mOptionsSound;
    Mix_Music* mMusic;
	
    gcn::OpenGLGraphics* mOpenGLGraphics;
    gcn::SDLInput* mSDLInput;
    gcn::OpenGLSDLImageLoader* mOpenGLImageLoader;
    gcn::Gui* mGui;
    gcn::Container* mTop;
    gcn::ImageFont* mFont;
    gcn::ImageFont* mSmallBlackFont;
    gcn::ImageFont* mWhiteFont;
    gcn::ImageFont* mHighLightFont;
    gcn::Image* mTitleImage;
    gcn::Icon* mTitle;
    gcn::Image* mCloudImage;
    gcn::Image* mPlanetImage;
    gcn::Image* mStarsImage;
    gcn::Image* mMoonImage;
    gcn::Image* mMoonRedImage;

    gcn::Image* mBoxImage;
    gcn::Icon* mSingleplayBoxIcon;
    gcn::Icon* mMultiplayBoxIcon;
    gcn::Icon* mOptionsBoxIcon;
	
    gcn::Label* mVersionLabel;
    gcn::Label* mSingleplayLabel;
    gcn::Label* mMultiplayLabel;
    gcn::Label* mOptionsLabel;
    gcn::TextBox* mSingleplayText;
    gcn::TextBox* mMultiplayText;
	
    gcn::Container* mMain;
    gcn::Container* mSingleplay;
    gcn::Container* mMultiplay;
    gcn::Container* mOptions;
    gcn::TextBox* mDemoInfo;
	
    GLuint mPlanetTexture;
    GLuint mCloudTexture;
    GLuint mStarsTexture;
    GLuint mMoonTexture;
    GLuint mMoonRedTexture;
	
    GLUquadricObj *mQuad1;
    GLUquadricObj *mQuad2;
    GLUquadricObj *mClouds2;
    GLUquadricObj *mMoon;
    GLUquadricObj *mMoonRed;
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
    gcn::DropDown* mResolution;	
    ResolutionListModel* mResolutionListModel;
    gcn::ScrollArea* mResolutionScrollArea;
    gcn::ListBox* mResolutionListBox;

    gcn::Slider* mVolume;
    gcn::Label* mVolumePercent;
    gcn::Label* mVolumeLabel;
    gcn::Label* mResolutionLabel;
    gcn::Image* mSplashImage;
    gcn::Icon* mSplashIcon;
};

#endif
