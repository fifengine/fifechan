// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#ifndef __fpsbutton_hpp__
#define __fpsbutton_hpp__

#include <SDL2/SDL_mixer.h>

#include <fifechan.hpp>

#include <string>

/*
 * The ordinary default button is quite ugly so we have made ourselves
 * our own button be overloading Button. To make it simple we just
 * want our new button to show the caption, nothing more. If we have
 * a mouse hover we want the font to change which gives us a simple hover
 * effect.
 * Oh yes, we also want our button to play a sound on mouse hover.
 */
class FPSButton : public fcn::Button
{
public:
    FPSButton(std::string const & caption);
    ~FPSButton();

    /*
     * Just sets the font to by used on mouse hover.
     */
    void setHighLightFont(fcn::Font* font);

    /*
     * Inherited from Widget
     */
    void draw(fcn::Graphics* graphics);
    void mouseEntered(fcn::MouseEvent& mouseEvent);
    void mouseExited(fcn::MouseEvent& mouseEvent);

private:
    fcn::Font* mHighLightFont;
    bool mHasMouse;
    // The sound (see SDL_mixer docs)
    static Mix_Chunk* mHoverSound;
    // Keep track of instances to be able to free the sound
    static int mInstances;
};

#endif
