#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/raw-file.hpp"
#include "yocto/pack/bwt.hpp"
#include "yocto/pack/mtf.hpp"

#include <cstdlib>

using namespace yocto;
using namespace packing;


static inline size_t emit( ios::ostream &fp, ios::bitio &bio )
{
    size_t num = 0;
    while( bio.size() >= 8 )
    {
        fp.write( bio.pop_full<char>() );
        ++num;
    }
    return num;
}

static inline size_t emit( ios::ostream &fp, ios::bitio &bio, Huffman::Codec &huff, const size_t size, const size_t pidx, const char *coded )
{
    size_t ans = 0;
    bio.push_full<uint32_t>( pidx );
    emit(fp,bio);
    for( size_t i=0; i < size; ++i )
    {
        huff.write(bio, coded[i] );
        ans += emit(fp,bio);
    }
    huff.flush(bio);
    ans += emit(fp,bio);
    assert(bio.size()==0);
    return ans;
}


YOCTO_UNIT_TEST_IMPL(comp)
{
    const size_t        block_size = 1024;
    char                block[ block_size ];
    char                coded[ block_size ];
    size_t              indices[ block_size ];
    size_t              count      = 0;
    pack::move_to_front mtf;
    Huffman::Codec      huff;
    ios::bitio          bio;
    
    ios::icstream input( ios::cstdin );
    ios::ocstream output( ios::cstdout );
    
    
    char C = 0;
    size_t nIn = 0;
    size_t nOut = 0;
    while( input.query(C) )
    {
        ++nIn;
        block[ count++ ] = C;
        if( count >= block_size )
        {
            const size_t pidx = pack::bwt::encode(coded, block, count, indices);
            mtf.encode(coded,coded,count);
            nOut += emit( output, bio, huff, count, pidx, coded );
            count = 0;
        }
    }
    
    if( count > 0 )
    {
        const size_t pidx = pack::bwt::encode(coded, block, count, indices);
        mtf.encode(coded,coded,count);
        nOut += emit( output, bio, huff, count, pidx, coded );
    }
    
    std::cerr << "nIn=" << nIn << " => nOut=" << nOut << std::endl;
}
YOCTO_UNIT_TEST_DONE()
