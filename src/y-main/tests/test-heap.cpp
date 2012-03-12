#include "yocto/utest/run.hpp"
#include "yocto/ordered/heap.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(heap)
{
    heap<int>   H;
    vector<int> V;
    for( size_t i=1; i <= 32; ++i )
    {
        V.push_back( i );
    }
    
    c_shuffle<int>( &V[1], V.size() );
    std::cerr << "------------- insert heap -------------" << std::endl;
    for( size_t i=1; i <= V.size(); ++i )
    {
        const int &X = V[i];
        std::cerr << "push = " << X << std::endl;
        H.push( &V[i] );
        std::cerr << "\tpeek = " << H.peek() << std::endl;
    }
    
    std::cerr << "------------- remove heap -------------" << std::endl;
    while( H.size() )
    {
        std::cerr << "\tpeek = " << H.peek() << std::endl;
        H.pop();
    }
}
YOCTO_UNIT_TEST_DONE()
