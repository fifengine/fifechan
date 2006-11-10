/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */

#include <iostream>
#include <guichan.hpp>
#include "fpsdemo.hpp"

int main(int argc, char** argv)
{
	try
	{
		FPSDemo fpsDemo;
		fpsDemo.run();
	}
	catch (gcn::Exception e)
	{
 		std::cout << e.getMessage() << std::endl;
		return 1;
	}	
 	catch (std::exception e)
	{
 	  std::cout << "Std exception: " << e.what() << std::endl;  
		return 1;
	}
	catch (...)
	{
		std::cout << "Unknown exception" << std::endl;
		return 1;
	}

	return 0;
}
