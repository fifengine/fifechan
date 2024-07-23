// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * This is an example that shows of the widgets present in
 * Fifechan. The example uses the Allegro back end.
 */

#include <fifechan.hpp>

#include <iostream>

// Here we store a global Gui object.  We make it global
// so it's easily accessable. Of course, global variables
// should normally be avioded when it comes to OOP, but
// this examples is not an example that shows how to make a
// good and clean C++ application but merely an example
// that shows how to use Fifechan.
namespace globals
{
    fcn::Gui* gui;
}

// Include code to set up an Allegro application with Fifechan.
// The allegro.hpp file is responsible for creating and deleting
// the global Gui object.
#include "allegro.hpp"
// Include code to set up a Fifechan GUI with all the widgets
// of Fifechan. The code populates the global Gui object.
#include "../widgets.hpp"

int main(int argc, char** argv)
{
    try {
        allegro::init();
        widgets::init();
        allegro::run();
        widgets::halt();
        allegro::halt();
    }
    /*
     * Catch all Fifechan exceptions
     */
    catch (fcn::Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
    /*
     * Catch all Std exceptions
     */
    catch (std::exception e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    /*
     * Catch all Unknown exceptions
     */
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
END_OF_MAIN()
