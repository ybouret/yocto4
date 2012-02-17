#include "yocto/string.hpp"

#include "hello.h"

#include <iostream>

int main( int argc, char *argv[] )
{
    
    try {
        
        make_window();
        Hello->show(argc,argv);
        
        return Fl::run();
    } catch(...) {
        std::cerr << "Exception!" << std::endl;
    }
    return -1;
}
