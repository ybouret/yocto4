#include <unistd.h>
#include "yocto/exceptions.hpp"
#include "yocto/string.hpp"
#include "yocto/sequence/vector.hpp"

#include <cerrno>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace yocto;


void launch( const string &option, const string &command, const vector<char*> &argv )
{
    char * const * xargv = (char * const *)&argv[1];
    int idx=0;
    while( NULL != xargv[idx] ) std::cerr << xargv[idx++] << std::endl;
    
    if( option == "local" )
    {
        execv(command.c_str(), xargv);
        throw exception("Local Exec Error");
    }
    
    if( option == "global" )
    {
        execvp(command.c_str(), xargv);
        throw exception("Global Exec Error");
    }
    
    throw exception("Invalid Option");
}

int main(int argc, char *argv[] )
{
    
    try
	{
        if( argc < 3 )
        {
            throw exception("usage: %s [local|global] command args", argv[0]);
        }
        const string option  = argv[1];
        const string command = argv[2];
        vector<char *> exec_argv;
        exec_argv.push_back( (char*)command.c_str() );
        for( int i=3; i < argc; ++i )
            exec_argv.push_back( argv[i] );
        exec_argv.push_back( NULL );
        
        launch( option, command, exec_argv );
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
