// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2026 Fifengine contributors

#include <fifechan.hpp>

#include <iostream>

#include "ffdemo.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    try {
        FFDemo ffDemo;
        ffDemo.run();
    } catch (fcn::Exception const & e) {
        std::cout << e.getMessage() << '\n';
        return 1;
    } catch (std::exception const & e) {
        std::cout << "Std exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cout << "Unknown exception\n";
        return 1;
    }

    return 0;
}
