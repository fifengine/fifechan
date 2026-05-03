// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include
#include "fpsbutton.hpp"

// Standard library includes
#include <string>

int FPSButton::mInstances         = 0;
MIX_Audio* FPSButton::mHoverAudio = nullptr;
MIX_Track* FPSButton::mHoverTrack = nullptr;

/*
 * It is very important to call Buttons constructor in the constructor
 * initialization code so we do that. In the constructor execution code
 * we check instances and load the mouse hover sound if there are no
 * instances.
 */
FPSButton::FPSButton(std::string const & caption) : Button(caption), mHasMouse(false)
{
    setBorderSize(0);

    if (mInstances == 0) {
        // SDL3_mixer: Load audio via the global mixer (accessed through FPSDemo)
        // We'll set this from FPSDemo after mixer is created
        mHoverAudio = nullptr;
        mHoverTrack = nullptr;
    }

    ++mInstances;
}

/**
 * We free the sound if this is the last instance.
 */
FPSButton::~FPSButton()
{
    --mInstances;

    if (mInstances == 0) {
        if (mHoverTrack != nullptr) {
            MIX_DestroyTrack(mHoverTrack);
            mHoverTrack = nullptr;
        }
        if (mHoverAudio != nullptr) {
            MIX_DestroyAudio(mHoverAudio);
            mHoverAudio = nullptr;
        }
    }
}

/*
 * If we have the mouse we should draw the caption with the highlight
 * font, if not we should draw it with the ordinary font.
 */
void FPSButton::draw(fcn::Graphics* graphics)
{
    if (mHasMouse) {
        graphics->setFont(mHighLightFont);
        graphics->drawText(getCaption(), 0, 0);
    } else {
        graphics->setFont(getFont());
        graphics->drawText(getCaption(), 0, 0);
    }
}

void FPSButton::setHighLightFont(fcn::Font* font)
{
    mHighLightFont = font;
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseEntered is called when the
 * mouse enters the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseEntered(fcn::MouseEvent& mouseEvent)
{
    Button::mouseEntered(mouseEvent);
    if (mHoverAudio != nullptr && mHoverTrack != nullptr) {
        MIX_SetTrackAudio(mHoverTrack, mHoverAudio);
        MIX_PlayTrack(mHoverTrack, 0);
    }
    mHasMouse = true;
}

/*
 * Button is already a MouseListener. Thats why FPSButton doesn't
 * need to inherit from MouseListener, mouseExited is called when the
 * mouse exits the widget. We want to know this in order to play
 * the mouse hover sound and to save a state that we have the mouse.
 */
void FPSButton::mouseExited(fcn::MouseEvent& mouseEvent)
{
    Button::mouseExited(mouseEvent);
    mHasMouse = false;
}
