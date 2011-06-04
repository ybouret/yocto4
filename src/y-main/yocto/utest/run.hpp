#ifndef YOCTO_UTEST_RUN_INCLUDED
#define YOCTO_UTEST_RUN_INCLUDED 1

#include "yocto/exception.hpp"
#include <iostream>

#define YOCTO_UNIT_TEST_IMPL(NAME)                            \
/*	*/	int yocto_unit_test_##NAME( int argc, char **argv ) { \
/*	*/		try 

#define YOCTO_UNIT_TEST_DONE()  \
/*	*/		catch( const yocto::exception &e ) \
/*	*/		{ std::cerr << e.when() << std::endl; std::cerr << e.what() << std::endl; }\
/*	*/		catch( const std::exception &e ) \
/*	*/		{ std::cerr << "std::exception: " << e.what() << std::endl; } \
/*	*/		catch(...) { std::cerr << "Unhandled exception!" << std::endl; return -1; }\
/*	*/	return 0; \
/*	*/	}


#endif
