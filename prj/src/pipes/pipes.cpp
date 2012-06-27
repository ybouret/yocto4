
#include <unistd.h>
#include "yocto/exceptions.hpp"
#include "yocto/string.hpp"
#include "yocto/ios/raw-file.hpp"

#include <cerrno>
#include <iostream>

using namespace yocto;

#define I_READ  0
#define I_WRITE 1

void __pipe( int fd[2] )
{
	if( pipe(fd) != 0 )
		throw libc::exception( errno, "pipe" );
}

bool invoke( const string &cmd, int fd[2] )
{
	
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
	switch( fork() )
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
			//std::cerr << "-- end of child" << std::endl;
			system(cmd.c_str());
			return false;
			
		default: // in parent
			close(tube_in[I_READ]);
			close(tube_out[I_WRITE]);
			fd[0] = tube_in[I_WRITE];
			fd[1] = tube_out[I_READ];
			return true;
	}
	
	
}

int main(int argc, char *argv[] )
{
	
	void  *ptr = NULL;
	size_t len = 0;
	bool in_father = true;
	try
	{
		if( argc <= 1 ) 
			throw exception("Need a file!");
		const string cmd = "wc -c";
		const string arg = argv[1];
		int          fd[2] = { 0 };
		in_father = invoke(cmd,fd);
		if(in_father) 
		{
			{
				ios::raw_file input(arg, ios::readable);
				len = input.length();
				ptr = malloc(len);
				input.get_all(ptr, len);
			}
			
			if( len != write( fd[I_WRITE], ptr, len) )
			{
				throw libc::exception( errno, "write error");
			}
			close(fd[I_WRITE]);
			char c;
			while( 1 == read( fd[I_READ], &c, 1 ) )
				fputc(c,stdout);
			close( fd[I_READ] );
			
			wait(NULL);
		}
	}
	catch( const exception &e )
	{
		std::cerr << e.when() << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (...) 
	{
		std::cerr << "unhandled exception!" << std::endl;
		return 1;
	}
	if( in_father )
	{
		if(ptr) free(ptr);
		std::cerr  << "-- end of father" << std::endl;
	}
	return 0;
}
