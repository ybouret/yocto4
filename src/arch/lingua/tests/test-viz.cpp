#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/posix.hpp"


#include "yocto/ptr/auto.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lingua;

static void __save_viz( const char *name, pattern *p)
{
    assert(name);
    auto_ptr<pattern> q(p);
    
    string fn = name;
    fn += ".dot";
    
    p->graphviz(fn);
    
    const string cmd = vformat("dot -Tpng -o%s.png %s.dot", name,name);
    int res=system( cmd.c_str() );(void)res;
    
}

#define __VIZ(ID) __save_viz( #ID, posix::ID() )

YOCTO_UNIT_TEST_IMPL(viz)
{
    __VIZ(upper);
    __VIZ(lower);
    __VIZ(digit);
    __VIZ(alpha);
    __VIZ(alnum);
    __VIZ(xdigit);
    __VIZ(blank);
    
    __VIZ(space);
    __VIZ(punct);
    
    __VIZ(word);
    __VIZ(endl);
    __VIZ(dot);
    __VIZ(cstring);
    
}
YOCTO_UNIT_TEST_DONE()
