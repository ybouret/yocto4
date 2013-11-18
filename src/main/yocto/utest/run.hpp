#ifndef YOCTO_UTEST_RUN_INCLUDED
#define YOCTO_UTEST_RUN_INCLUDED 1

#include "yocto/exception.hpp"
#include <iostream>

#if defined(__ICC)
#pragma warning ( disable : 1418 869 )
#endif

#define YOCTO_UNIT_TEST_IMPL(NAME)                            \
/*	*/	int yocto_unit_test_##NAME( int argc, char **argv ) { \
/*  */  (void)argc; (void)argv;                               \
/*	*/		try 

#define YOCTO_UNIT_TEST_DONE()  \
/*	*/		catch( const yocto::exception &e ) \
/*	*/		{ std::cerr << e.when() << std::endl; std::cerr << e.what() << std::endl; return -1;}\
/*	*/		catch( const std::exception &e ) \
/*	*/		{ std::cerr << "std::exception: " << e.what() << std::endl; return -1; } \
/*	*/		catch(...) { std::cerr << "Unhandled exception!" << std::endl; return -1; }\
/*	*/	return 0; \
/*	*/	}


#endif
