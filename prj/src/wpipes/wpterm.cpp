#include "yocto/string.hpp"
#include "yocto/exceptions.hpp"
#include <iostream>
#include <windows.h>

using namespace yocto;

class Context
{

};

int main( int argc, char *argv[] )
{

	try 
    {   
		
		return 0;
    }
    catch( const exception &e )
	{
		std::cerr << e.when() << std::endl;
		std::cerr << e.what() << std::endl;
	}
	catch (...) 
	{
		std::cerr << "unhandled exception!" << std::endl;
	}
    return 1;
}