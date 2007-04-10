/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */
 
#ifndef __fpsdemo_hpp__
#define __fpsdemo_hpp__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#if defined (_WIN32)
#include <windows.h>
#if defined (DELETE)
#undef DELETE
#endif
#endif

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
