#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/associative/map.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/mpa/rsa/key.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/hashing/sha1.hpp"

#include <iostream>

using namespace yocto;
using namespace mpa;

#if 0
static inline natural get_value( const string &data )
{
    string value;
    bool   is_hex = false;
    for( size_t i=0; i < data.size(); ++i )
    {
        const char C = data[i];
        if( C >= '0' && C <= '9' )
        {
            value += C;
            continue;
        }
        
        if( (C >= 'a' && C <='f') || (C>='A' && C <= 'F') )
        {
            value += C;
            is_hex = true;
            continue;
        }
        
        if( C == ':' )
        {
            is_hex = true;
            continue;
        }
        
        if( C == '(' || C == ')' )
        {
            goto GATHER;
        }
        
    }
GATHER:
    //std::cerr << "#" << value << std::endl;
    return is_hex ? natural::hex( value ) : natural::dec( value );
}
#endif



int  main( int argc, char *argv[] )
{
    
    try
    {
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception!" << std::endl;
    }
    return 1;
}


#if 0
int  main( int argc, char *argv[] )
{
    
    try {
        ////////////////////////////////////////////////////////////////////////
        // load lines
        ////////////////////////////////////////////////////////////////////////
        vector<string> lines;
        {
            string        line;
            ios::icstream input( ios::cstdin );
            while( input.read_line( line ) > 0 )
            {
                lines.push_back( line );
                line.clear();
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        // parse lines
        ////////////////////////////////////////////////////////////////////////
        map<string,natural> db;
        size_t iLine = 1;
        while( iLine <= lines.size() )
        {
            string line = lines[iLine];
            char *org = &line[0];
            char *sep = strchr( org, ':' );
            if( !sep )
                throw exception("Invalid [KEY] on line '%s'" , line.c_str() );
            const string key(org,sep-org);
            std::cerr << "[KEY]=" << std::hex << key << std::endl;
            string data( sep+1 );
            while( data.size() > 0 && character<char>::is_space( data[0] ) ) data.skip(1);
            
            if( data.size() > 0 )
            {
                // something on the line
                ++iLine;
            }
            else
            {
                // nothing on the line : catenate following
                for(;;) {
                    if( ++iLine > lines.size() )
                        break;
                    const string &tmp = lines[ iLine ];
                    if( !character<char>::is_space( tmp[0] ) )
                        break;
                    data += tmp;
                }
            }
            const natural value = get_value( data );
            std::cerr << "value= 0x" << value << std::endl;
            if( !db.insert( key, value) )
                throw exception("multiple key '%s'", key.c_str() );
        }
        
        ////////////////////////////////////////////////////////////////////////
        // construct the key
        ////////////////////////////////////////////////////////////////////////
#define __FETCH(NAME) const natural *NAME = db.search( #NAME ); if( !NAME ) throw exception("No %s", #NAME )
        __FETCH(modulus);
        __FETCH(publicExponent);
        __FETCH(privateExponent);
        __FETCH(prime1);
        __FETCH(prime2);
        __FETCH(exponent1);
        __FETCH(exponent2);
        __FETCH(coefficient);
        
        const rsa_private_key prv( *modulus, *publicExponent, *privateExponent, *prime1, *prime2, *exponent1, *exponent2, *coefficient);
        std::cerr << "got RSA private key: bits=" << std::dec << prv.obits << std::endl;
        
        hashing::sha1 h;
        const string filename = vformat( "rsa-key-%08x-%08x.bin", unsigned(prv.obits), h.key<uint32_t>(prv.modulus) );
        {
            ios::ocstream fp( filename, false );
            prv.save_prv( fp );
        }
        
        {
            ios::icstream fp( filename );
            auto_ptr<rsa_key> reload( rsa_private_key::load_prv( fp ) );
            std::cerr << "Reload a key with maxbits=" << reload->obits << std::endl;
        }
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception!" << std::endl;
    }
    return 0;
}
#endif
