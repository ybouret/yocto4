#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

#include <iostream>
#include <cstring>

using namespace yocto;

#define SYMBOLS_PER_LINE 16


class Writer
{
public:
    Writer( ios::ostream &output ) :
    fp( output ),
    count(0),
    total(0),
    store()
    {
        
    }
    
    
    ~Writer() throw(){}
    
    ios::ostream &fp;
    size_t        count;
    size_t        total;
    char          store[SYMBOLS_PER_LINE];
    
    void operator()(unsigned C)
    {
        assert(count<SYMBOLS_PER_LINE);
        //-- store char
        if( (C >= 32 && C < 127) && C != '\'' && C != '\\' )
        {
            store[count] = C;
        }
        else
        {
            store[count] = '.';
        }
        //-- emit char
        if(total>0)
            fp.write(',');
        else
            fp.write(' ');
        
        fp( " 0x%02x", C );
        ++count;
        ++total;
        if(  count >= SYMBOLS_PER_LINE )
        {
            emit();
            count = 0;
        }
    }
    
    void flush()
    {
        if(count>0)
        {
            for(size_t i=count;i<SYMBOLS_PER_LINE;++i)
            {
                fp("      ");
            }
        }
        emit();
    }
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Writer);
    inline void emit()
    {
        assert(count<=SYMBOLS_PER_LINE);
        if(count > 0 )
        {
            fp(" /*|");
            for( size_t i=0; i < count; ++i )
            {
                fp.write(store[i]);
            }
            for( size_t i=count;i<SYMBOLS_PER_LINE;++i)
                fp.write(' ');
            fp("|*/\n");
        }
    }
    
};


int main( int argc, char *argv[] )
{
	try
	{
        
		
		//-- default: save has binary
		bool binary = true;
		
		//-- do we save as string ? ( add a zero )
		if( argc > 1 && !strcmp( "-s", argv[1]) )
		{
			binary = false;
			--argc;
			++argv;
		}
		
		//== input
		ios::icstream *input_ = NULL;
		if( argc > 1 )
		{
			const string filename = argv[1];
			input_ = new ios::icstream( filename );
		}
		else
		{
			input_ = new ios::icstream( ios::cstdin );
		}
		auto_ptr< ios::icstream > input( input_ );
		
		//== output
		ios::ocstream *output_ = NULL;
		if( argc > 2 )
		{
			const string filename = argv[2];
			output_ = new ios::ocstream( filename, false);
		}
		else
		{
			output_ = new ios::ocstream( ios::cstdout );
		}
		auto_ptr< ios::ocstream > output( output_ );
		
		char   c = 0;
        Writer W( *output );
        
		while( input->query(c) )
		{
            const uint8_t C = c;
			W(C);
		}
		if( !binary )
		{
			W(0);
		}
		W.flush();
		
		
		return 0;
	}
	catch( const exception &e )
	{
		std::cerr << e.when() << std::endl;
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "unhandled exception!" << std::endl;
		return -1;
	}
	
}
