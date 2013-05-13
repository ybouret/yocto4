#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/lingua/pattern/joker.hpp"

#include "yocto/ios/imstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace lingua;

void shared_test_motifs( p_list &motifs );

YOCTO_UNIT_TEST_IMPL(joker)
{
    p_list motifs;
    {
        AND *p = AND::create();
        motifs.push_back(p);
        p->append( single::create('a') );
        p->append( optional::create( single::create('b') ) );
		p->append( zero_or_more( single::create('c') ) );
		p->append( one_or_more( single::create('d') ) );
    }
    shared_test_motifs( motifs );

}
YOCTO_UNIT_TEST_DONE()
