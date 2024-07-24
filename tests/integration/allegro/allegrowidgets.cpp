// SPDX-License-Identifier: LGPL-2.1-or-later OR BSD-3-Clause
// SPDX-FileCopyrightText: 2004 - 2008 Olof Naessén and Per Larsson
// SPDX-FileCopyrightText: 2013 - 2024 Fifengine contributors

/**
 * @brief Example that shows of the widgets present in FifeGUI.
 *
 * This is an example that shows of the widgets present in
 * FifeGUI. The example uses the Allegro backend.
 */

#include <fifechan.hpp>

#include <iostream>

#include "../widgets.hpp"
#include "allegro.hpp"

int main(int argc, char** argv)
{
    try {
        allegro::init();
        widgets::init();
        allegro::run();
        widgets::halt();
        allegro::halt();
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
