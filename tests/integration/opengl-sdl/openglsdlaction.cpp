// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * This is an example that demonstrates how to use actions.
 * The example uses the OpenGL and the SDL backend.
 */

#include "openglsdlaction.hpp"

#include <fifechan/gui.hpp>

#include <fifechan.hpp>

#include <iostream>

#include "openglsdl.hpp"

int main(int argc, char** argv)
{
    try {
        openglsdl::init();
        action::init();
        openglsdl::run();
        action::halt();
        openglsdl::halt();
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    }
    // Catch all unknown exceptions.
    catch (...) {
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
