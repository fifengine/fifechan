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
 * This is an example that shows of the widgets present in
 * Guichan. The example uses the OpenLayer back end.
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

// Include code to set up an OpenLayer application with Guichan.
// The openlayer.hpp file is responsible for creating and deleting
// the global Gui object.
#include "openlayer.hpp"
// Include code to set up a Guichan GUI with all the widgets
// of Guichan. The code populates the global Gui object.
#include "widgets.hpp"

int main(int argc, char **argv)
{
    try
    {
        openlayer::init();
        widgets::init();
        openlayer::run();
        widgets::halt();
        openlayer::halt();
    }
    // Catch all Guichan exceptions.
    catch (fcn::Exception e)
    {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
    // Catch all Std exceptions.
    catch (std::exception e)
    {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    // Catch all unknown exceptions.
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
