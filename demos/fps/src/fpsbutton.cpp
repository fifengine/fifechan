/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */

#include "fpsbutton.hpp"

int FPSButton::mInstances = 0;
Mix_Chunk* FPSButton::mHoverSound = NULL;

/*
 * It is very important to call Buttons constructor in the constructor
 * initialization code so we do that. In the constructor execution code
 * we check instances and load the mouse hover sound if there are no
 * instances.
 */
FPSButton::FPSButton(const std::string& caption)
		:Button(caption), mHasMouse(false)
{
  setBorderSize(0);
  
	if (mInstances == 0)
	{		
		mHoverSound = Mix_LoadWAV("sound/sound5.wav");
		Mix_VolumeChunk(mHoverSound, 60);
	}
	
	++mInstances;
}

/**
 * We free the sound if this is the last instance.
 */
FPSButton::~FPSButton()
{
	--mInstances;

	if (mInstances == 0)
	{
		Mix_FreeChunk(mHoverSound);
	}
}

/*
 * If we have the mouse we should draw the caption with the highlight
 * font, if not we should draw it with the ordinary font.
 */
void FPSButton::draw(gcn::Graphics* graphics)
{
    if (mHasMouse)
	{
		graphics->setFont(mHighLightFont);
		graphics->drawText(getCaption(),0,0);
	}
	else
	{
		graphics->setFont(getFont());
		graphics->drawText(getCaption(),0,0);
	}
}

void FPSButton::setHighLightFont(gcn::Font* font)
{
	mHighLightFont = font;
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseEntered is called when the
 * mouse enters the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseEntered(gcn::MouseEvent& mouseEvent)
{
	Mix_PlayChannel(-1, mHoverSound, 0);
	mHasMouse = true;
	mouseEvent.consume();
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseExited is called when the
 * mouse exits the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseExited(gcn::MouseEvent& mouseEvent)
{
	mHasMouse = false;
	mouseEvent.consume();
}

