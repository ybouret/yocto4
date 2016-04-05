#include "yocto/lingua/pattern/posix.hpp"
#include "yocto/lingua/pattern/dict.hpp"
#include "yocto/utest/run.hpp"
//#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;

#define __DO(NAME) do {               \
std::cerr << #NAME << std::endl;      \
auto_ptr<pattern> p( posix::NAME() ); \
p->graphviz( #NAME ".dot" );          \
ios::graphviz_render( #NAME ".dot");  \
dict.define( #NAME, p.yield() );      \
} while(false)

YOCTO_UNIT_TEST_IMPL(pattern)
{
    p_dict dict;

    __DO(upper);
    __DO(lower);
    __DO(digit);
    __DO(alpha);
    __DO(alnum);
    __DO(xdigit);
    __DO(blank);
    __DO(space);
    __DO(punct);
    __DO(word);
    __DO(endl);
    __DO(dot);
    __DO(cstring);

    std::cerr << std::endl;
    for(pattern_db::iterator i=dict.begin();i!=dict.end();++i)
    {
        std::cerr << i->key << std::endl;
        const pattern &p = **i;
        {
            ios::wcstream  fp( i->key + ".bin" );
            p.save(fp);
        }
        std::cerr << "\t" << p.b64() << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()
