// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#ifndef FPSBUTTON_HPP
#define FPSBUTTON_HPP

// Standard library includes
#include <string>

// Third-party library includes
#include <SDL3_mixer/SDL_mixer.h>

#include <fifechan.hpp>

// Project headers (subdirs before local)

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
    explicit FPSButton(std::string const & caption);
    ~FPSButton() override;

    FPSButton(FPSButton const &)            = delete;
    FPSButton& operator=(FPSButton const &) = delete;

    FPSButton(FPSButton&&)            = delete;
    FPSButton& operator=(FPSButton&&) = delete;

    /*
     * Just sets the font to by used on mouse hover.
     */
    void setHighLightFont(fcn::Font* font);

    /*
     * Inherited from Widget
     */
    void draw(fcn::Graphics* graphics) override;
    void mouseEntered(fcn::MouseEvent& mouseEvent) override;
    void mouseExited(fcn::MouseEvent& mouseEvent) override;

private:
    fcn::Font* mHighLightFont{};
    bool mHasMouse;

    // The sound (SDL3_mixer: MIX_Audio + MIX_Track for playback)
    // Made public so FPSDemo can initialize them after mixer creation
    static MIX_Audio* mHoverAudio;
    static MIX_Track* mHoverTrack;
    // Keep track of instances to be able to free the sound
    static int mInstances;

    friend class FPSDemo;
};

#endif // FPSBUTTON_HPP
