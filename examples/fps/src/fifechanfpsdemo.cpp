// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

#include <fifechan.hpp>

#include <iostream>

#include "fpsdemo.hpp"

int main(int argc, char** argv)
{
    try {
        FPSDemo fpsDemo;
        fpsDemo.run();
    } catch (fcn::Exception const & e) {
        std::cout << e.getMessage() << std::endl;
        return 1;
    } catch (std::exception const & e) {
        std::cout << "Std exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}
