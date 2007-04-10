/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */

#include "fpsdemo.hpp"
#include <cmath>
#include <sstream>

FPSDemo::FPSDemo()
		:mRotation(0), 
         mRunning(true), 
         mGlow(0),
		 mWidth(800), 
         mHeight(600), 
         mTime(-1), 
         mDeltaTime(0),
		 mInit(true), 
         mResolutionChange(false), 
         mHaveFullscreen(false),
         mMusic(NULL)
{
	// Init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	mScreen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL);	

	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_WM_SetCaption("Guichan FPS demo", NULL);
	
	// Init SDL_Mixer
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	
	// Load sounds and music
 	mChooseSound = Mix_LoadWAV("sound/sound4.wav");
 	mEscapeSound = Mix_LoadWAV("sound/sound3.wav");
 	mOptionsSound = Mix_LoadWAV("sound/sound2.wav");
 	mMusic = Mix_LoadMUS("sound/space.ogg");
	
 	// Set the mixer volume
 	Mix_Volume(-1,(int)(MIX_MAX_VOLUME*0.7));
	
	// Create some GLU quadrics	
	mQuad1 = gluNewQuadric();
	mQuad2 = gluNewQuadric();
	mMoon = gluNewQuadric();
	mMoonRed = gluNewQuadric();
	mClouds2 = gluNewQuadric();
	gluQuadricNormals(mQuad1, GLU_SMOOTH);
	gluQuadricTexture(mQuad1,GL_TRUE);
	gluQuadricNormals(mQuad2, GLU_SMOOTH);
	gluQuadricTexture(mQuad2,GL_TRUE);
	gluQuadricNormals(mMoon, GLU_SMOOTH);
	gluQuadricTexture(mMoon,GL_TRUE);
	gluQuadricNormals(mMoonRed, GLU_SMOOTH);
	gluQuadricTexture(mMoonRed,GL_TRUE);
	gluQuadricNormals(mClouds2, GLU_SMOOTH);
	gluQuadricTexture(mClouds2,GL_TRUE);

	initOpenGL();
	initGui();
	resize();
	
	mInit = false;
}

FPSDemo::~FPSDemo()
{
	cleanGui();
 	Mix_FreeChunk(mChooseSound);
 	Mix_FreeChunk(mEscapeSound);
 	Mix_FreeChunk(mOptionsSound);
 	Mix_FreeMusic(mMusic);
 	Mix_CloseAudio();

	SDL_Quit();
}

void FPSDemo::initGui()
{
	mOpenGLImageLoader = new gcn::OpenGLSDLImageLoader();
	gcn::Image::setImageLoader(mOpenGLImageLoader); 
	mOpenGLGraphics = new gcn::OpenGLGraphics();
	mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
	mSDLInput = new gcn::SDLInput();

	mTop = new gcn::Container();
	mTop->setOpaque(false);
	mGui = new gcn::Gui();
	mGui->setTabbingEnabled(false);
	mGui->setGraphics(mOpenGLGraphics);
	mGui->setInput(mSDLInput);
	
 	mFont = new gcn::ImageFont(          "images/techyfontbig2.png",        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");	
 	mHighLightFont = new gcn::ImageFont( "images/techyfontbighighlight.png"," abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
 	mSmallBlackFont = new gcn::ImageFont("images/techyfontblack.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
 	mWhiteFont = new gcn::ImageFont(     "images/techyfontwhite.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
 	gcn::Widget::setGlobalFont(mWhiteFont);
	
	mTitleImage = gcn::Image::load("images/title2.png");
	mTitle = new gcn::Icon(mTitleImage);
	mTop->add(mTitle);
	
	mDemoInfo = new gcn::TextBox("     Copyright 2004, 2005, 2006 (c) Darkbits. This is a Demo demonstrating Guichan with SDL and OpenGL.\n"
								 "Guichan is licensed under BSD. For more information about Guichan and visit http://guichan.sourceforge.net\n"
							     "            Code Yakslem (Olof Nassen). Art Finalman (Per Larsson). Darkbits logo Haiko (Henrik Vahlgren).");
	mDemoInfo->setFont(mSmallBlackFont);
	mDemoInfo->setOpaque(false);
	mDemoInfo->setEditable(false);
	mDemoInfo->setFocusable(false);
    mDemoInfo->setBorderSize(0);
	mTop->add(mDemoInfo);

	mVersionLabel = new gcn::Label("Version 1.00");
	mVersionLabel->setFont(mSmallBlackFont);
	mTop->add(mVersionLabel);
	
	mBoxImage = gcn::Image::load("images/box.png");
	
	mSplashImage = gcn::Image::load("images/splash.png");
	mSplashIcon = new gcn::Icon(mSplashImage);

	if (mInit)
	{
		mGui->setTop(mSplashIcon);		
		mSplashIcon->setPosition(mWidth / 2 -mSplashImage->getWidth() /2,
                                 mHeight / 2 -mSplashImage->getHeight() /2);	
	}
	else
	{
		mGui->setTop(mTop);
	}
	
	loadTextures();
	initMain();
	initSingleplay();
	initMultiplay();
	initOptions();
}

void FPSDemo::cleanGui()
{
	cleanTextures();
	cleanMain();	
	cleanSingleplay();
	cleanMultiplay();
	cleanOptions();
	delete mBoxImage;
	delete mFont;
	delete mHighLightFont;
	delete mSmallBlackFont;
	delete mWhiteFont;
 	delete mTop;
 	delete mGui;		
	delete mSplashIcon;
	delete mSplashImage;
 	delete mSDLInput;
 	delete mOpenGLGraphics;
 	delete mOpenGLImageLoader;
}

/*
 * Init the main container
 */
void FPSDemo::initMain()
{
	mMain = new gcn::Container();
	mMain->setOpaque(false);
	mTop->add(mMain);
	
	mSingleplayButton = new FPSButton("Singleplayer");
	mSingleplayButton->setHighLightFont(mHighLightFont);
	mSingleplayButton->adjustSize();
	mSingleplayButton->setActionEventId("singleplay");
	mSingleplayButton->addActionListener(this);
	mSingleplayButton->setFont(mFont);
	mSingleplayButton->adjustSize();
	mMain->add(mSingleplayButton);
	
	mMultiplayButton = new FPSButton("Multiplayer");
	mMultiplayButton->setHighLightFont(mHighLightFont);
	mMultiplayButton->adjustSize();
	mMultiplayButton->setActionEventId("multiplay");
	mMultiplayButton->addActionListener(this);
	mMultiplayButton->setFont(mFont);
	mMultiplayButton->adjustSize();
	mMain->add(mMultiplayButton);

	mOptionsButton = new FPSButton("Options");
	mOptionsButton->setHighLightFont(mHighLightFont);
	mOptionsButton->adjustSize();
	mOptionsButton->setActionEventId("options");
	mOptionsButton->addActionListener(this);
	mOptionsButton->setFont(mFont);
	mOptionsButton->adjustSize();
	mMain->add(mOptionsButton);

	mQuitButton = new FPSButton("Quit");
	mQuitButton->setHighLightFont(mHighLightFont);
	mQuitButton->adjustSize();
	mQuitButton->setActionEventId("quit");
	mQuitButton->addActionListener(this);
	mQuitButton->setFont(mFont);
	mQuitButton->adjustSize();
	mMain->add(mQuitButton);
}

void FPSDemo::cleanMain()
{
	delete mSingleplayButton;
	delete mMultiplayButton;
	delete mOptionsButton;
	delete mQuitButton;
 	delete mVersionLabel;
	delete mTitle;
 	delete mTitleImage;
 	delete mDemoInfo;
 	delete mMain;
}

/*
 * Init the singleplay
 */
void FPSDemo::initSingleplay()
{
	mSingleplay = new gcn::Container();
	mSingleplay->setVisible(false);
	mSingleplay->setOpaque(false);		
	mTop->add(mSingleplay);

	mSingleplayBoxIcon = new gcn::Icon(mBoxImage);
	mSingleplay->add(mSingleplayBoxIcon);
	
	mSingleplayLabel = new gcn::Label("Singleplayer");
	mSingleplayLabel->setFont(mWhiteFont);
	mSingleplayLabel->adjustSize();
	mSingleplay->add(mSingleplayLabel);

	mSingleplayText = new gcn::TextBox("I'm verry sorry but this is not an actual game.\n"
									   "It's a demonstration of the GUI library Guichan.\n"
                                       "But who knows...\n"
                                       "Maybe it will be a game here someday.\n");
	mSingleplayText->setFont(mWhiteFont);
	mSingleplayText->setOpaque(false);
    mSingleplayText->setEditable(false);
    mSingleplayText->setBorderSize(0);
	mSingleplay->add(mSingleplayText);

	mSingleplayBackButton = new FPSButton("Back");
	mSingleplayBackButton->setHighLightFont(mHighLightFont);
	mSingleplayBackButton->adjustSize();
	mSingleplayBackButton->setActionEventId("back");
	mSingleplayBackButton->addActionListener(this);
	mSingleplayBackButton->setFont(mFont);
	mSingleplayBackButton->adjustSize();
	
	mSingleplay->add(mSingleplayBackButton);
}

void FPSDemo::cleanSingleplay()
{
	delete mSingleplayBoxIcon;
	delete mSingleplayText;
	delete mSingleplayLabel;
	delete mSingleplay;
}

/*
 * Init the multiplay container
 */
void FPSDemo::initMultiplay()
{
	mMultiplay = new gcn::Container();
	mMultiplay->setVisible(false);
	mMultiplay->setOpaque(false);
	mTop->add(mMultiplay);

	mMultiplayBoxIcon = new gcn::Icon(mBoxImage);
	mMultiplay->add(mMultiplayBoxIcon);
 
	mMultiplayLabel = new gcn::Label("Multiplayer");
	mMultiplayLabel->setFont(mWhiteFont);
	mMultiplayLabel->adjustSize();  
	mMultiplay->add(mMultiplayLabel);
	
	mMultiplayText = new gcn::TextBox("I'm verry sorry but this is not an actuall game.\n"
	                                  "It's a demonstration of the GUI library Guichan.\n"
                                      "But who knows...\n"
                                      "Maybe it will be a game here someday.\n");
	mMultiplayText->setFont(mWhiteFont);
	mMultiplayText->setOpaque(false);
	mMultiplayText->setEditable(false);
    mMultiplayText->setBorderSize(0);
	mMultiplay->add(mMultiplayText);

	mMultiplayBackButton = new FPSButton("Back");
	mMultiplayBackButton->setHighLightFont(mHighLightFont);
	mMultiplayBackButton->adjustSize();
	mMultiplayBackButton->setActionEventId("back");
	mMultiplayBackButton->addActionListener(this);
	mMultiplayBackButton->setFont(mFont);
	
	mMultiplayBackButton->adjustSize();
	
	mMultiplay->add(mMultiplayBackButton);
}

void FPSDemo::cleanMultiplay()
{
	delete mMultiplayBoxIcon;
	delete mMultiplayBackButton;
	delete mMultiplayText;
	delete mMultiplayLabel;
	delete mMultiplay;
}

/*
 * Init the options container
 */ 
void FPSDemo::initOptions()
{
	mOptions = new gcn::Container();
	mOptions->setVisible(false);
	mOptions->setOpaque(false);
	mTop->add(mOptions);
	
	mOptionsBoxIcon = new gcn::Icon(mBoxImage);
	mOptions->add(mOptionsBoxIcon);
	
	mOptionsLabel = new gcn::Label("Options");
	mOptionsLabel->setFont(mWhiteFont);
	mOptionsLabel->adjustSize();
	mOptions->add(mOptionsLabel);

	mFullScreen = new FPSCheckBox("Fullscreen");
	mFullScreen->setFont(mWhiteFont);
	mFullScreen->adjustSize();
	mFullScreen->setBackgroundColor(0x331010);
	mFullScreen->setForegroundColor(0xffffff);
	mFullScreen->setBaseColor(0x771010);
	mFullScreen->setActionEventId("fullscreen");
	mFullScreen->addActionListener(this);
	mFullScreen->setMarked(mHaveFullscreen);
	mOptions->add(mFullScreen);

	mResolutionScrollArea = new gcn::ScrollArea();
	mResolutionScrollArea->setBackgroundColor(0x331010);
	mResolutionScrollArea->setForegroundColor(0x331010);
	mResolutionScrollArea->setBaseColor(0x771010);

	mResolutionListBox = new gcn::ListBox();
	mResolutionListBox->setBackgroundColor(0x331010);
	mResolutionListBox->setForegroundColor(0x331010);
	mResolutionListBox->setBaseColor(0x771010);
	mResolutionListBox->setSelectionColor(0x552020);
	
	mResolutionListModel = new ResolutionListModel();
	mResolution = new gcn::DropDown(mResolutionListModel,
									mResolutionScrollArea,
									mResolutionListBox);
	mResolution->setWidth(200);
	mResolution->setBackgroundColor(0x331010);
	mResolution->setForegroundColor(0x331010);
	mResolution->setBaseColor(0x771010);
	mResolution->setSelectionColor(0x552020);

	if (mWidth == 800)
	{
		mResolution->setSelected(1);
	}
	else
	{
		mResolution->setSelected(0);
	}	
	mResolution->setActionEventId("resolution");
	mResolution->addActionListener(this);
	mOptions->add(mResolution);

	mVolume = new gcn::Slider(0.0, 1.0);
	mVolume->setWidth(200);
	mVolume->setHeight(20);
	mVolume->setValue(0.7);
	mVolume->setBackgroundColor(0x331010);
	mVolume->setForegroundColor(0x331010);
	mVolume->setBaseColor(0x771010);
	mVolume->setActionEventId("volume");
	mVolume->addActionListener(this);
	mVolume->setMarkerLength(20);
	mOptions->add(mVolume);

	mVolumePercent = new gcn::Label("70%");
	mOptions->add(mVolumePercent);

	mVolumeLabel = new gcn::Label("Volume");
	mOptions->add(mVolumeLabel);

	mResolutionLabel = new gcn::Label("Resolution");
	mOptions->add(mResolutionLabel);

	mOptionsBackButton = new FPSButton("Back");
	mOptionsBackButton->setHighLightFont(mHighLightFont);
	mOptionsBackButton->adjustSize();
	mOptionsBackButton->setActionEventId("back");
	mOptionsBackButton->addActionListener(this);
	mOptionsBackButton->setFont(mFont);
	mOptionsBackButton->adjustSize();
	mOptions->add(mOptionsBackButton);
}

void FPSDemo::cleanOptions()
{
	delete mOptionsBoxIcon;
	delete mOptionsBackButton;
	delete mResolutionLabel;
	delete mVolumeLabel;
	delete mVolumePercent;
	delete mVolume;
	delete mResolutionListBox;
	delete mResolutionScrollArea;
	delete mResolutionListModel;
	delete mResolution;
	delete mFullScreen;
	delete mOptionsLabel;
	delete mOptions;
}

void FPSDemo::loadTextures()
{
    gcn::OpenGLImage* image;
	// Load textures with the OpenGLImageLoader from Guichan
	mCloudImage = gcn::Image::load("images/cloudsblackwhite.png");
    image = (gcn::OpenGLImage*)mCloudImage;
	mCloudTexture = image->getTextureHandle();

	mPlanetImage = gcn::Image::load("images/planet.png");
    image = (gcn::OpenGLImage*)mPlanetImage;
	mPlanetTexture = image->getTextureHandle();

	mStarsImage = gcn::Image::load("images/background.png");
    image = (gcn::OpenGLImage*)mStarsImage;
	mStarsTexture = image->getTextureHandle();

	mMoonImage = gcn::Image::load("images/moon.png");
    image = (gcn::OpenGLImage*)mMoonImage;
	mMoonTexture = image->getTextureHandle();
	
	mMoonRedImage = gcn::Image::load("images/moonred.png");
	image = (gcn::OpenGLImage*)mMoonRedImage;
	mMoonRedTexture = image->getTextureHandle();
}


void FPSDemo::cleanTextures()
{
 	delete mPlanetImage;
 	delete mCloudImage;
 	delete mStarsImage;
 	delete mMoonImage;
 	delete mMoonRedImage;
}
	
/*
 * Draw the planet and the two moons
 */
void FPSDemo::drawBackground()
{
	glEnable(GL_LIGHTING);
	mRotation+= mDeltaTime / 2000;
	
	glPushMatrix();
	glTranslatef(0.0,0.0,1.0);
	glRotatef(80,0.0, 0.0, 1.0);
	glRotatef(mRotation*7,0.0, -1.0, 0.0); 
	glTranslatef(0,0.0,2.1);
	glRotatef(mRotation*50,1.0, 0.0, 0.0); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mMoonTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluSphere(mMoon, .07, 10, 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.0,1.0);
	glRotatef(110,0.0, 1.0, 0.0);
 	glRotatef(mRotation*5,0.0, 0.0, 1.0); 
 	glTranslatef(2.2,0.0,0.0);
 	glRotatef(mRotation*30,1.0, 0.0, 0.0); 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mMoonRedTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluSphere(mMoonRed, 0.15, 10, 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
 	glTranslatef(0.0,0.0,1.0);
 	glRotatef(mRotation*3,1.0, 0.0, 0.0); 
	glRotatef(77,0.0, 1.0, 0.0); 
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
	glBlendFunc(GL_ONE,GL_ONE);
	glTranslatef(0.0,0.0,1.0);
	glRotatef(mRotation*6,1.0, 0.0, 0.0);
	glRotatef(90,0.0, 1.0, 0.0);		
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
	glBlendFunc(GL_ONE,GL_ONE);
	glTranslatef(0.0,0.0,1.0);
	glRotatef(mRotation*5,1.0, 0.0, 0.0);
	glRotatef(90,0.0, 1.0, 0.0);		
	glRotatef(90,0.0, 0.0, 1.0);
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
	int y = -200;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 600, 0.0, 1, 0.0);

	glEnable(GL_TEXTURE_2D);
 	glBindTexture(GL_TEXTURE_2D, mStarsTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glScalef(2,1.5,1);
	glBegin(GL_QUADS);    
	glTexCoord2f(0.0f, 0.0f);
	glVertex3i(0,y,0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3i(mStarsImage->getWidth(),y,0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3i(mStarsImage->getWidth(),mStarsImage->getHeight()+y,0); 
	glTexCoord2f(0.0f, 1.0f);
	glVertex3i(0,mStarsImage->getHeight()+y,0);
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
	while(SDL_GetTicks() < 3000)
	{		
		if (mTime < 0)
		{
			mTime = SDL_GetTicks();
		}
		mDeltaTime = SDL_GetTicks() - mTime;
		mTime = SDL_GetTicks();	

		input();
		
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mGui->draw();
		SDL_GL_SwapBuffers();
		SDL_Delay(10);		
	}
	
    mGui->setTop(mTop);
	
    if (mMusic != NULL)
	{
        Mix_FadeInMusic(mMusic, -1, 2000);		
	}
}

/*
 * The main run loop
 */ 
void FPSDemo::runMain()
{
	while(mRunning)
	{		
		if (mResolutionChange)
		{
			// Clear the screen before remaking the Gui
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			SDL_GL_SwapBuffers();
			
			cleanGui();
			initGui();		
			resize();
			mResolutionChange = false;
		}

		if (mTime < 0)
		{
			mTime = SDL_GetTicks();
		}
		
		mDeltaTime = SDL_GetTicks() - mTime;
		mTime = SDL_GetTicks();	
		
		input();
		mGui->logic();
		
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glLoadIdentity( );
		
		drawSpace();
		gluLookAt(0.0,-1.8,-2.9,
							0.0,-1.2,1.0,
							0.0,-1.0,0.0);		
		
		drawBackground();
		glPushMatrix();
		mGui->draw();
		glPopMatrix();
		
		SDL_GL_SwapBuffers();
		SDL_Delay(10);		
	}
}

void FPSDemo::input()
{
	while(SDL_PollEvent(&mEvent))
	{
		if (mEvent.type == SDL_KEYDOWN)
		{
			if (mEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				mMain->setVisible(true);
 				mSingleplay->setVisible(false);
 				mMultiplay->setVisible(false);
 				mOptions->setVisible(false);
			}
		}
		else if (mEvent.type == SDL_QUIT)
		{
			mRunning = false;
		}
		// We ignore keyboard input and just sends mouse input to Guichan
		else if (mEvent.type == SDL_MOUSEMOTION
						 || mEvent.type == SDL_MOUSEBUTTONDOWN
						 || mEvent.type == SDL_MOUSEBUTTONUP)
		{
			mSDLInput->pushInput(mEvent);
		}
	}
}

/**
 * The action funcion from ActionListener
 */
void FPSDemo::action(const gcn::ActionEvent& actionEvent)
{
	if (actionEvent.getId() == "quit")
	{
		Mix_PlayChannel(-1, mEscapeSound, 0);
		mRunning = false;
	}
 	else if (actionEvent.getId() == "singleplay")
 	{
		Mix_PlayChannel(-1, mChooseSound, 0);
 		mMain->setVisible(false);
 		mSingleplay->setVisible(true);
 	}
 	else if (actionEvent.getId() == "multiplay")
 	{
 		Mix_PlayChannel(-1, mChooseSound, 0);
 		mMain->setVisible(false);
 		mMultiplay->setVisible(true);
 	}
 	else if (actionEvent.getId() == "options")
 	{
		Mix_PlayChannel(-1, mChooseSound, 0);
 		mMain->setVisible(false);
 		mOptions->setVisible(true);
 	}
 	else if (actionEvent.getId() == "back")
 	{
 		Mix_PlayChannel(-1, mEscapeSound, 0);
 		mMain->setVisible(true);
 		mSingleplay->setVisible(false);
 		mMultiplay->setVisible(false);
 		mOptions->setVisible(false);
 	}
	else if (actionEvent.getId() == "fullscreen")
	{
		Mix_PlayChannel(-1, mOptionsSound, 0);
		initVideo();
	}
	else if (actionEvent.getId() == "resolution")
	{
		Mix_PlayChannel(-1, mOptionsSound, 0);
		initVideo();
	}
	else if (actionEvent.getId() == "volume")
	{
		std::string str;
		std::ostringstream os(str);
		os << (int)(mVolume->getValue() * 100) << "%";
		mVolumePercent->setCaption(os.str());
		mVolumePercent->adjustSize();
 		double m = MIX_MAX_VOLUME;
 		double p = mVolume->getValue();
 		Mix_Volume(-1,(int)(m*p));
	}
}

void FPSDemo::initVideo()
{		
	if (mResolution->getSelected() == 0)
 	{
 		mWidth = 1024;
 		mHeight = 768;
		mResolutionChange = true;
 	}
 	else if (mResolution->getSelected() == 1)
 	{
 		mWidth = 800;
 		mHeight = 600;
		mResolutionChange = true;
 	}
 	if (mFullScreen->isMarked())
 	{
 		mScreen = SDL_SetVideoMode(mWidth, mHeight, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL | SDL_FULLSCREEN);
		mHaveFullscreen = true;
 	}
 	else
 	{
		mHaveFullscreen = false;
 		mScreen = SDL_SetVideoMode(mWidth, mHeight, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL);
 	}
 	mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
	initOpenGL();
}

void FPSDemo::initOpenGL()
{
	// Init OpenGL
	glViewport(0, 0, mWidth, mHeight);		
 	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();	
 	gluPerspective(50.0, mWidth/mHeight, 1.0, 10.0); 

 	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Init Light
	Light2Ambient[0] =  0.5f;
	Light2Ambient[1]	= 0.4f;
	Light2Ambient[2]	= 0.7f;
	Light2Ambient[3]	= 1.0f;

	Light2Diffuse[0] = 1.0f;
	Light2Diffuse[1] = 1.0f;
	Light2Diffuse[2] = 1.0f;
	Light2Diffuse[3] =	1.0f;

	Light2Position[0] =  10.0f;
	Light2Position[1] =	 .5f;
	Light2Position[2] =  0.0f;
	Light2Position[3] =  1.0f;

	Light2SpotDirection[0] = -1.0;
	Light2SpotDirection[1] = 0.0;
	Light2SpotDirection[2] = 0.0;

	glLightfv(GL_LIGHT2, GL_POSITION, Light2Position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, Light2Ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse); 
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Light2SpotDirection);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	
	glEnable(GL_LIGHT2);
}

void FPSDemo::resize()
{
 	mTitle->setPosition(mWidth/2 - 330, mHeight/2 - 290);
 	mDemoInfo->setPosition(mWidth/2 - 390, mHeight - 50);
 	mVersionLabel->setPosition(mWidth - 100, mHeight - 80);

 	mTop->setDimension(gcn::Rectangle(0, 0, mWidth, mHeight));
 	mMain->setDimension(gcn::Rectangle(0, 0, mWidth, mHeight));
 	mSingleplay->setDimension(gcn::Rectangle(0, 0, mWidth, mHeight));
	mMultiplay->setDimension(gcn::Rectangle(0, 0, mWidth, mHeight));
	mOptions->setDimension(gcn::Rectangle(0, 0, mWidth, mHeight));	

 	mSingleplayButton->setPosition(mWidth/2 - 100, mHeight/2 - 100);
 	mMultiplayButton->setPosition(mWidth/2 - 100, mHeight/2 - 60);
 	mOptionsButton->setPosition(mWidth/2 - 100, mHeight/2 - 20);
 	mQuitButton->setPosition(mWidth/2 - 100, mHeight/2 +60);

 	mSingleplayBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);
	mMultiplayBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);
 	mOptionsBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);
		
 	mSingleplayBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
	mMultiplayBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
 	mOptionsBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
	
 	mSingleplayText->setPosition(mWidth/2 - 285, mHeight/2 - 120);
 	mSingleplayLabel->setPosition(mWidth/2 + 150, mHeight/2 - 145);

 	mMultiplayText->setPosition(mWidth/2 - 285, mHeight/2 - 120);
 	mMultiplayLabel->setPosition(mWidth/2 + 150, mHeight/2 - 145);

 	mOptionsLabel->setPosition(mWidth/2 + 150, mHeight/2 - 145);
 	mFullScreen->setPosition(mWidth/2 - 200, mHeight/2 -100);
 	mResolution->setPosition(mWidth/2 - 90, mHeight/2 -70);
 	mResolutionLabel->setPosition(mWidth/2 -200, mHeight/2-70);
 	mVolume->setPosition(mWidth/2 - 90, mHeight/2 - 40);
 	mVolumePercent->setPosition(mWidth/2 +120, mHeight/2-40);
 	mVolumeLabel->setPosition(mWidth/2 -200, mHeight/2-40);
 }
