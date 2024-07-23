// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * This is an example that shows a simple Hello World example
 * with Fifechan. The example uses the OpenGL back end and the
 * SDL back end.
 */

#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <iostream>

// Include code to set up an OpenGL and SDL application with Fifechan.
// The openglsdl.hpp file is responsible for creating and deleting
// the global Gui object.
#include "openglsdl.hpp"
// Include code to set up a Fifechan GUI with a simple Hello
// World example. The code populates the global Gui object.
#include "../helloworld.hpp"

int main(int argc, char** argv)
{
    try {
        openglsdl::init();
        helloworld::init();
        openglsdl::run();
        helloworld::halt();
        openglsdl::halt();
    }
    // Catch all Fifechan exceptions.
    catch (fcn::Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
    // Catch all Std exceptions.
    catch (std::exception e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    // Catch all unknown exceptions.
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
