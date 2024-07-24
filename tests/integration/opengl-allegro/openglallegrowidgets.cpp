// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * @brief This example shows the widgets present in FifeGUI using the OpenGL-Allegro backend.
 */

#include <fifechan.hpp>

#include <iostream>

#include "../widgets.hpp"
#include "openglallegro.hpp"

int main(int argc, char** argv)
{
    try {
        openglallegro::init();
        widgets::init();
        openglallegro::run();
        widgets::halt();
        openglallegro::halt();
    } catch (fcn::Exception e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    } catch (std::exception e) {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    // Catch all Unknown exceptions
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
END_OF_MAIN()
