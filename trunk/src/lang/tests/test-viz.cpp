#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"
#include "yocto/utest/run.hpp"


#include "yocto/ptr/auto.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cstdlib>

using namespace yocto;
using namespace lang;


static inline
void doviz( const char *name, pattern *p )
{
    auto_ptr<pattern> q(p);
    string fn = name;
    fn += ".dot";
	string img = name;
	img += ".png";
    
    q->graphviz(fn);
    
	string cmd = "dot -Tpng -o " + img + " " + fn;
	std::cerr << "Executing '" << cmd << "'" << std::endl;
	system( cmd.c_str() );
    
    string bn = name;
    bn += ".bin";
    
    {
        ios::ocstream fp(bn,false);
        p->save(fp);
    }
}



#define YVIZ(NAME) doviz( #NAME, posix:: NAME() )

YOCTO_UNIT_TEST_IMPL(viz)
{
    YVIZ(lower);
    YVIZ(upper);
    YVIZ(digit);
	YVIZ(alpha);
	YVIZ(alnum);
	YVIZ(xdigit);
	YVIZ(space);
	YVIZ(blank);
	YVIZ(word);
	YVIZ(punct);
	YVIZ(dot);
	YVIZ(cstring);
	YVIZ(endl);
    
}
YOCTO_UNIT_TEST_DONE()
