#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/utest/run.hpp"


#include "yocto/ptr/auto.hpp"

using namespace yocto;
using namespace lang;


static inline
void doviz( const char *name, pattern *p )
{
    auto_ptr<pattern> q(p);
    string fn = name;
    fn += ".dot";
    q->graphviz(fn);
}

#define YVIZ(NAME) doviz( #NAME, posix:: NAME() )

YOCTO_UNIT_TEST_IMPL(viz)
{
    YVIZ(lower);
    YVIZ(upper);
    
}
YOCTO_UNIT_TEST_DONE()
