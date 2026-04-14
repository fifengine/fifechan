// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

// Corresponding header include

// Standard library includes
#include <iostream>

// Platform config include
#include "fifechan/platform.hpp"

// Third-party library includes
#include <fifechan.hpp>

// Project headers (subdirs before local)
#include "fpsdemo.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    try {
        FPSDemo fpsDemo;
        fpsDemo.run();
    } catch (fcn::Exception const & e) {
        std::cout << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cout << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cout << "Unknown exception" << '\n';
        return 1;
    }

    return 0;
}
