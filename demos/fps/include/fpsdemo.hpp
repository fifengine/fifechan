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

#include <fifechan.hpp>
#include <fifechan/sdl.hpp>
#include <fifechan/opengl.hpp>
#include <fifechan/opengl/openglsdlimageloader.hpp>
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

class FPSDemo : public fcn::ActionListener
{
public:
    FPSDemo();
    ~FPSDemo();
    void run();
    void action(const fcn::ActionEvent& actionEvent);
	
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
