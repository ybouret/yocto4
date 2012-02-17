#include "yocto/utest/run.hpp"
#include "yocto/string/base64.hpp"
#include "yocto/ios/icstream.hpp"


using namespace yocto;

static inline int search_char( const char C, const char *tab )
{
    for( int j=0; j < 64; ++j )
    {
        if( C == tab[j] )
            return j;
    }
    return -1;
}

YOCTO_UNIT_TEST_IMPL(base64)
{
    
    
    //--------------------------------------------------------------------------
    // generate reverse table
    //
    //--------------------------------------------------------------------------
    for( size_t i=0; i < 256; ++i )
    {
        const char    C   = char(i);
        if( C == '=' )
        {
            fprintf( stderr, "0x40" );
        }
        else
        {
            int           idx = search_char( C, base64::encoder::table_iso );
            if( idx < 0 ) idx = search_char( C, base64::encoder::table_url );
            if( idx < 0 )
                fprintf( stderr, "0xff");
            else
                fprintf( stderr, "%4d", idx );
        }
        if( i < 255 )
            fprintf( stderr, ",");
        if( 0 == ( (i+1) % 16 ) )
            fprintf( stderr, "\n");
        else
            fprintf( stderr, " ");
    }
    
    
    base64::encoder enc;
    ios::icstream   input( ios::cstdin );
    string line;
    while( input.read_line(line) > 0 )
    {
        enc.append( line );
        enc.flush();
        char C;
        while( enc.query(C) )
            std::cerr << C;
        std::cerr << std::endl;
        
        line.clear();
    }
    
}
YOCTO_UNIT_TEST_DONE()