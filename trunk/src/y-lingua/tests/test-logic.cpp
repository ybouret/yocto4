#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/basic.hpp"

#include "yocto/ios/imstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace lingua;

void shared_test_motifs( p_list &motifs );

YOCTO_UNIT_TEST_IMPL(logic)
{
    p_list motifs;
    motifs.push_back( logical::EQUAL("dog" ) );
    {
        OR *p = OR::create();
        motifs.push_back(p);
        p->append( logical::EQUAL("cat") );
        p->append( logical::EQUAL("cow") );
    }
    
    {
        NOT *p = NOT::create();
        motifs.push_back(p);
        p->append( logical::EQUAL("do") );
        p->append( logical::EQUAL("re") );
    }
    
    shared_test_motifs(motifs);
    
}
YOCTO_UNIT_TEST_DONE()
