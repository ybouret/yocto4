#include "yocto/utest/run.hpp"
#include "yocto/string/b64id.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

static int search_char( const char C, const char *tab )
{
    for( int j=0; j < 64; ++j )
    {
        if( C == tab[j] )
            return j;
    }
    return -1;
}

#define Y_B64_BYTES_FOR(INPUT_BYTES) (YOCTO_ROUND1((8*INPUT_BYTES)/6))

#define SHOW_FOR(TYPE) std::cerr << "bytes for " #TYPE << "=" << b64id<TYPE>::bytes << std::endl

YOCTO_UNIT_TEST_IMPL(base64)
{
    
    
    //--------------------------------------------------------------------------
    // generate reverse table
    //
    //--------------------------------------------------------------------------
    if( false )
    {
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
    }
    for(size_t i=1;i<=10;++i)
    {
        std::cerr << "#b64_bytes_for " << i << " = " << Y_B64_BYTES_FOR(i) << std::endl;
    }

    SHOW_FOR(char);
    SHOW_FOR(int16_t);
    SHOW_FOR(uint32_t);
    SHOW_FOR(uint64_t);

    SHOW_FOR(int);

    for(int i=0;i<10;++i)
    {
        b64id<int> b(&i),c = b;
        std::cerr << "b_int(" << i << ")=" << b.label << " / " << c.label << std::endl;
    }

    for(uint64_t i=0;i<10;++i)
    {
        b64id<uint64_t> b(&i),c = b;
        std::cerr << "b_u64(" << i << ")=" << b.label << " / " << c.label << std::endl;
    }


    base64::encoder enc;
    ios::icstream   input( ios::cstdin );
    string          line;
    base64::decoder dec;
    std::cerr << "Ready for input:" << std::endl;
    while( input.read_line(line) > 0 )
    {
        std::cerr << "-- encoding [" << line << "]" << std::endl;
        enc << line;
#if 0
        std::cerr << "unflushed: ";
        for( pack::q_codec::const_iterator i=enc.begin();i!=enc.end();++i) std::cerr << *i;
        std::cerr << std::endl;
#endif
        enc.flush();
        char C;
        while( enc.query(C) )
        {
            std::cerr << C;
            dec.write(C);
        }
        dec.flush();
        std::cerr << std::endl;

        std::cerr << "-- decoding" << std::endl;
        while( dec.query(C) )
        {
            std::cerr << C;
        }
        std::cerr << std::endl;
        
        line.clear();
    }
    
}
YOCTO_UNIT_TEST_DONE()

