// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * This is an example that demonstrates how to use actions.
 * The example uses the HGE backend.
 */

#include <fifechan.hpp>

#include <iostream>

#include "action.hpp"
#include "hge.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HGE* hge = hgeCreate(HGE_VERSION);
    try {
        hge::init();
        action::init();
        hge::run();
        action::halt();
        hge::halt();
    } catch (fcn::Exception e) {
        MessageBox(NULL, e.getMessage().c_str(), "FifeGUI exception", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    } catch (std::exception e) {
        MessageBox(NULL, e.what(), "Std exception", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
    // Catch all unknown exceptions.
    catch (...) {
        MessageBox(NULL, hge->System_GetErrorMessage(), "Unknown exception", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }

    return 0;
}
