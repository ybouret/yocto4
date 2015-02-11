#ifndef YOCTO_PROGRAM_INCLUDED
#define YOCTO_PROGRAM_INCLUDED 1

#include "yocto/fs/vfs.hpp"
#include "yocto/exception.hpp"
#include <iostream>

#define YOCTO_PROGRAM_START() \
int main(int argc, char *argv[]) \
{\
static const char *program = yocto::vfs::get_base_name(argv[0]);\
try {

#define YOCTO_PROGRAM_END() \
return 0;\
}\
catch( const yocto::exception &e )\
{\
std::cerr << "in " << program << ": " << e.what() << std::endl;\
std::cerr << e.when() << std::endl;\
}\
catch( const std::exception &e ) \
{\
std::cerr << "std::exception in " << program << std::endl;\
std::cerr << e.what()             << std::endl;\
}\
catch(...)\
{\
std::cerr << "uncaught exception in " << program << std::endl;\
}\
return 1;\
}
#endif
