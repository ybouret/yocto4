#include "yocto/utest/run.hpp"
#include "yocto/ordered/heap.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(heap)
{
    heap<int>   H;
    vector<int> V;
    for( size_t i=1; i <= 10; ++i )
    {
        V.push_back( i*10 );
    }
    
    c_shuffle<int>( &V[1], V.size() );
    
    for( size_t i=1; i <= V.size(); ++i )
    {
        H.push( V[i] );
    }
}
YOCTO_UNIT_TEST_DONE()
