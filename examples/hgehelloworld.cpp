/***************************************************************************
 *   Copyright (C) 2012 by the fifechan team                               *
 *   http://fifechan.github.com/fifechan                                   *
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

/**
 * This is an example that shows a simple Hello World example
 * with Guichan. The example uses the HGE back end.
 */

#include <fifechan.hpp>
#include <iostream>

// Here we store a global Gui object.  We make it global
// so it's easily accessable. Of course, global variables
// should normally be avioded when it comes to OOP, but
// this examples it not an example that shows how to make a 
// good and clean C++ application but merely an example
// that shows how to use Guichan.
namespace globals
{
    fcn::Gui* gui;
}

// Include code to set up an HGE application with Guichan.
// The hge.hpp file is responsible for creating and deleting
// the global Gui object.
#include "hge.hpp"
// Include code to set up a Guichan GUI with a simple Hello
// World example. The code populates the global Gui object.
#include "helloworld.hpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HGE* hge = hgeCreate(HGE_VERSION);
    try
    {
        hge::init();
        helloworld::init();
        hge::run();
        helloworld::halt();
        hge::halt();
    }
    // Catch all Guichan exceptions.
    catch (fcn::Exception e)
    {
        MessageBox(NULL,  
                   e.getMessage().c_str(),
                   "Guichan exception",
                   MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
    // Catch all Std exceptions.
    catch (std::exception e)
    {
        MessageBox(NULL, 
                   e.what(),
                   "Std exception",
                   MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
    // Catch all unknown exceptions.
    catch (...)
    {
       MessageBox(NULL, 
                  hge->System_GetErrorMessage(), 
                  "Unknown exception", 
                  MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }

    return 0;
}
