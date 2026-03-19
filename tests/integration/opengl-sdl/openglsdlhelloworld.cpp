// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * @brief This example shows the widgets present in FifeGUI using the OpenGL backend.
 */

#include "openglsdlhelloworld.hpp"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try {
        openglsdl::init();
        helloworld::init();
        openglsdl::run();
        helloworld::halt();
        openglsdl::halt();
    } catch (fcn::Exception const & e) {
        std::cerr << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cerr << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
