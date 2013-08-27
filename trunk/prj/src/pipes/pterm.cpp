

#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#include "yocto/exceptions.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/icstream.hpp"

#include "yocto/sys/wtime.hpp"

using namespace yocto;

#include <iostream>

#define I_READ  0
#define I_WRITE 1

void __pipe( int fd[2] )
{
	if( pipe(fd) != 0 )
		throw libc::exception( errno, "pipe" );
}


static
pid_t invoke( char *argv[], int fd[2] )
{
    
    //-- create the pipes
    int tube_in[2];
	int tube_out[2];
	__pipe(tube_in);
	try
	{
		__pipe(tube_out);
	}
	catch(...)
	{
		close(tube_in[0]);
		close(tube_in[1]);
	}
    
    const pid_t p  = fork();
    switch( p )
	{
		case -1:
			close( tube_in[0]  );
			close( tube_in[1]  );
			close( tube_out[0] );
			close( tube_out[1] );
			throw libc::exception( errno, "fork" );
			
		case 0: // in child
			close(tube_in[I_WRITE]);
			close(tube_out[I_READ]);
			dup2(tube_in[I_READ],STDIN_FILENO);
			dup2(tube_out[I_WRITE],STDOUT_FILENO);
            execvp(argv[0], (char * const *)argv);
            throw libc::exception( errno, "Cannot exec %s", argv[0] );
			
		default: // in parent
			close(tube_in[I_READ]);
			close(tube_out[I_WRITE]);
			fd[I_WRITE] = tube_in[I_WRITE];
			fd[I_READ]  = tube_out[I_READ];
	}
    return p;
    
    
}

int main(int argc, char *argv[] )
{
    
    try 
    {
        if( argc > 1 )
        {
            ++argv;
            std::cerr << "Executing " << argv[0];
            int idx = 1;
            while( argv[idx] != NULL) std::cerr << " " << argv[idx++];
            std::cerr << std::endl;
            
            int fd[2] = { -1, -1 };
            const pid_t child = invoke( argv, fd);
            
#if 0
            string line;
            ios::icstream fp( ios::cstdin );
            while( line.clear(), fp.read_line(line) > 0)
            {
                if( line.size() != write( fd[I_WRITE], line.c_str(), line.size()) )
                {
                    throw libc::exception( errno, "write");
                }
            }
#endif
            close( fd[I_WRITE] );
            
            char         buffer[8];
            const size_t buflen = sizeof(buffer);
            wtime chrono;
            chrono.start();
            while(true)
            {
                ssize_t len = read(fd[I_READ], buffer, buflen);
                if( len < 0 )
                {
                    throw libc::exception( errno, "read" );
                }
                if( 0 == len )
                    break;
                fwrite(buffer, len, 1, stdout ); fflush(stdout);
                if( chrono.query() > 10 )
                    break;
            }
            close( fd[I_READ] );
            std::cerr << "---- done: waiting... " << std::endl;
            int status = -1;
            waitpid(child, &status, 0);
            std::cerr << "--- status=" << status << std::endl;
            
            
            
        }
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
