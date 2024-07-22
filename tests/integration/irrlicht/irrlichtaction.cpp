
/**
 * This is an example that demonstrates how to use actions
 * in Fifechan. The example uses the Irrlicht back end.
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

// Include code to set up an Irrlicht application with Fifechan.
// The irrlicht.hpp file is responsible for creating and deleting
// the global Gui object.
#include "irrlicht.hpp"
// Include code to set up a Fifechan GUI that demonstrates how
// to use actions in Fifechan. The code populates the global Gui object.
#include "action.hpp"

int main(int argc, char** argv)
{
    try {
        irrlicht::init();
        action::init();
        irrlicht::run();
        action::halt();
        irrlicht::halt();
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
