#include "yocto/lingua/pattern/posix.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;

#define __DO(NAME) do {               \
std::cerr << #NAME << std::endl;      \
auto_ptr<pattern> p( posix::NAME() ); \
p->graphviz( #NAME ".dot" );          \
ios::graphviz_render( #NAME ".dot");  \
} while(false)

YOCTO_UNIT_TEST_IMPL(pattern)
{
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
}
YOCTO_UNIT_TEST_DONE()
