#include "yocto/string/base64.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/imstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/mpl/natural.hpp"

#include "yocto/exception.hpp"

#include <iostream>

using namespace yocto;

static
bool load_string( ios::istream &fp, string &s )
{
    s.clear();
    const unsigned n = fp.read<uint32_t>();
    std::cerr << "size=" << n << std::endl;
    for(unsigned i=1; i <=n; ++i )
    {
        char c = 0;
        if( ! fp.query(c) ) throw exception("missing char %u/%u", i,n);
        s += c;
    }
    return false;
}

int main(int argc, char *argv[] )
{
    const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        if( argc < 2 )
            throw exception("usage: %s id_rsa.pub", progname);
        base64::decoder dec;
        ios::icstream   input( argv[1] );
        
        char c = 0;
        //----------------------------------------------------------------------
        // skipping header
        //----------------------------------------------------------------------
        {
            static const char     hdr[] = "ssh-rsa ";
            static const unsigned len   = length_of(hdr);
            for( unsigned i=1; i <= len; ++i )
            {
                if( ! input.query(c) ) throw exception("EOF after %u chars", i);
                if( c != hdr[i-1] )    throw exception("Invalid char #%u", i);
            }
        }
        //----------------------------------------------------------------------
        // Read until space
        //----------------------------------------------------------------------
        string b64;
        while( input.query(c) && c != ' ' )
        {
            b64 += c;
        }
        const string bin = dec.to_string(b64);
        { ios::ocstream tmp( "pub.bin", false ); tmp << bin; }
        
        ios::imstream fp( bin );
        string &s = b64;
        load_string(fp,s);
        std::cerr << "hdr=" << s << std::endl;
        load_string(fp,s);
        mpn e(s); std::cerr << "e=" << e << std::endl;
        load_string(fp,s);
        mpn m(s); std::cerr << "m.bits=" << m.bits() << std::endl;

        
        return 0;
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception!" << std::endl;
    }
    return -1;
}
