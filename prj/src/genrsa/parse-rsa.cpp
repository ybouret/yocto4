#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/associative/map.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/mpl/rsa/keys.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/hashing/sha1.hpp"

#include "yocto/string/tokenizer.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>

using namespace yocto;
using namespace mpl;

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


static inline bool is_col(char C) throw()
{
    return C == ':';
}

static inline bool is_space(char C) throw()
{
    return C == ' ' || C == '\t';
}

static mpn toHex( const string &value )
{
    string data;
    for(size_t i=0;i<value.size();++i)
    {
        const char C = value[i];
        if(isxdigit(C))
            data += C;
    }
    return mpn::hex2mpn(data);
}

static inline void parse_for( associative<string,mpn> &db, const string &line )
{
    tokenizer tkn(line);
    if( !tkn.get_next(is_col) )
    {
        throw exception("No Key!");
    }
    const string key = tkn.to_string();
    if( !tkn.get_next(is_space) )
    {
        throw exception("No Value for %s!", key.c_str());
    }
    const string value = tkn.to_string();
    if(value.size()>0 && hex2dec(value[0])>=0 )
    {
        const mpn x = strchr(value.c_str(), ':') ? toHex(value) : mpn::dec2mpn(value);
        //std::cerr << key << " = " << x << std::endl;
        if( !db.insert(key,x) )
            throw exception("Multiple '%s' in DB", key.c_str());
    }
    
}


static inline mpn query_mpn( const associative<string, mpn> &db, const char *id )
{
    const string name(id);
    const mpn *p = db.search(name);
    if( !p )
    {
        throw exception("No Key '%s'", name.c_str() );
    }
    return *p;
}

int  main( int argc, char *argv[] )
{
    
    try
    {
        //______________________________________________________________________
        //
        // Load Lines
        //______________________________________________________________________
        vector<string> lines;
        {
            string line;
            ios::icstream input( ios::cstdin );
            while( line.clear(), input.read_line( line ) > 0 )
            {
                if(line.size()<=0)
                    throw exception("Empty Line!");
                if(is_space(line[0]))
                {
                    if(lines.size()<=0)
                        throw exception("No Previous Key!");
                    line.clean(is_space);
                    lines.back() += line;
                }
                else
                {
                    lines.push_back(line);
                }
            }

        }
        map<string,mpn> db;
        for(size_t i=1;i<=lines.size();++i)
        {
            parse_for(db, lines[i]);
        }
        
#define __FETCH(NAME) const mpn NAME = query_mpn(db, #NAME )
        
        __FETCH(modulus);
        __FETCH(publicExponent);
        __FETCH(privateExponent);
        __FETCH(prime1);
        __FETCH(prime2);
        __FETCH(exponent1);
        __FETCH(exponent2);
        __FETCH(coefficient);
        const RSA::PrivateKey prv(modulus,publicExponent,privateExponent,prime1,prime2,exponent1,exponent2,coefficient);
        
        hashing::sha1 h;
        const string filename = vformat( "rsa-key-%08x-%08x.bin", unsigned(prv.obits), h.key<uint32_t>(prv.modulus) );
        {
            ios::ocstream fp( filename, false );
            prv.save_prv( fp );
        }
 
        std::cerr << std::hex;
        {
            ios::icstream fp( filename );
            RSA::PublicKey pub = RSA::PublicKey::load_pub(fp);
            std::cerr << "pub=(" << pub.modulus << "," << pub.publicExponent << ")" << std::endl;
        }
        
        {
            ios::icstream   fp( filename );
            RSA::PrivateKey prv = RSA::PrivateKey::load_prv(fp);
            std::cerr << "prv=(" << prv.modulus << "," << prv.privateExponent << ")" << std::endl;
        }
        
        

        
        
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
