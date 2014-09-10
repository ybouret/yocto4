#include "yocto/utest/run.hpp"
#include "yocto/lingua/pattern/posix.hpp"

#include "yocto/ios/imstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;
using namespace lingua;

void shared_test_motifs( p_list &motifs );

YOCTO_UNIT_TEST_IMPL(posix)
{
    p_list motifs;
    motifs.push_back( posix::upper() );
    motifs.push_back( posix::lower() );
    motifs.push_back( posix::digit() );
    motifs.push_back( posix::alpha() );
    motifs.push_back( posix::alnum() );
    motifs.push_back( posix::xdigit() );
    motifs.push_back( posix::blank() );
    motifs.push_back( posix::space() );
    motifs.push_back( posix::punct() );
    
    motifs.push_back( posix::word() );
    motifs.push_back( posix::endl() );
    motifs.push_back( posix::dot()  );
    motifs.push_back( posix::cstring() );
    shared_test_motifs(motifs);
    
}
YOCTO_UNIT_TEST_DONE()
