#include "yocto/program.hpp"

#include "yocto/hashing/sha1.hpp"
#include "yocto/hashing/md5.hpp"
#include "yocto/hashing/md2.hpp"
#include "yocto/hashing/md4.hpp"

#include "yocto/hashing/rmd128.hpp"
#include "yocto/hashing/rmd160.hpp"

#include "yocto/hashing/sha256.hpp"
#include "yocto/hashing/sha512.hpp"

#include "yocto/associative/map.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/hashing/digest.hpp"
#include "yocto/code/utils.hpp"

using namespace yocto;

namespace
{
    static inline
    void H_process(ios::istream      &fp,
                   hashing::function &H,
                   const string      &filename)
    {
        H.set();
        const size_t n=H.length;
        char         C=0;
        while(fp.query(C)) H.run(&C,1);
        digest md = digest::get_from(H);
        ios::ocstream op( ios::cstdout );
        for(size_t i=0;i<n;++i)
        {
            const uint8_t B=md[i];
            op << hexa_text_lower[B];
        }
        op << ' ' << H.name();
        op << '(' << filename << ')';
        op << '\n';
    }
}

YOCTO_PROGRAM_START()
{
    if(argc<=1)
        throw exception("usage: %s hash [inputs]",program);

    typedef shared_ptr<hashing::function> HPTR;
    typedef map<string,HPTR>              HDB;

#define Y_MD_REG(NAME) do { \
const HPTR   tmp( new hashing::NAME() );\
const string str( #NAME ); \
if( !hdb.insert(str,tmp) ) throw exception("multiple hashing function %s", str.c_str());\
} while(false)

    HDB hdb(16,as_capacity);
    Y_MD_REG(sha1);
    Y_MD_REG(md5);
    Y_MD_REG(md4);
    Y_MD_REG(md2);
    Y_MD_REG(rmd128);
    Y_MD_REG(rmd160);
    Y_MD_REG(sha224);
    Y_MD_REG(sha256);
    Y_MD_REG(sha384);
    Y_MD_REG(sha512);

    const string ID = argv[1];
    if("-h"==ID)
    {
        for(HDB::iterator i=hdb.begin();i!=hdb.end();++i)
        {
            std::cerr << i->key << std::endl;
        }
        return 0;
    }

    HPTR *phfn = hdb.search(ID);
    if(!phfn)
    {
        throw exception("no function '%s'", ID.c_str());
    }

    hashing::function &H = **phfn;
    if(argc>2)
    {
        for(int i=2;i<argc;++i)
        {
            const string  filename = argv[i];
            ios::icstream fp(filename);
            H_process(fp,H,filename);
        }
    }
    else
    {
        const string  filename="-";
        ios::icstream fp( ios::cstdin );
        H_process(fp,H,filename);
    }



}
YOCTO_PROGRAM_END()

