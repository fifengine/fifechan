/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */

#ifndef __fpsbutton_hpp__
#define __fpsbutton_hpp__

#include <guichan.hpp>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

/*
 * The ordinary default button is quite ugly so we have made ourselves
 * our own button be overloading Button. To make it simple we just
 * want our new button to show the caption, nothing more. If we have
 * a mouse hover we want the font to change which gives us a simple hover
 * effect.
 * Oh yes, we also want our button to play a sound on mouse hover.
 */
class FPSButton : public gcn::Button
{
public:
	FPSButton(const std::string& caption);
	~FPSButton();

	/* 
	 * Just sets the font to by used on mouse hover.
	 */
	void setHighLightFont(gcn::Font* font);
	
	/*
	 * Inherited from Widget
	 */
	void draw(gcn::Graphics* graphics);
	void mouseIn();
 
private:
	gcn::Font* mHighLightFont;
	// The sound (see SDL_mixer docs)
	static Mix_Chunk* mHoverSound;
	// Keep track of instances to be able to free the sound
	static int mInstances;
};

#endif
