// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * This is an example that demonstrates how to use actions.
 * The example uses the Irrlicht backend.
 */

#include <fifechan.hpp>

#include <iostream>

#include "action.hpp"
#include "irrlicht.hpp"

int main(int argc, char** argv)
{
    try {
        irrlicht::init();
        action::init();
        irrlicht::run();
        action::halt();
        irrlicht::halt();
    } catch (fcn::Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    } catch (std::exception e) {
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
