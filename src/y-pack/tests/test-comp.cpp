#include "yocto/utest/run.hpp"
#include "yocto/pack/huffman.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/raw-file.hpp"
#include "yocto/code/bwt.hpp"
#include "yocto/code/mtf.hpp"

#include <cstdlib>

using namespace yocto;
using namespace packing;


static inline void emit( ios::ostream &fp, ios::bitio &bio )
{
    while( bio.size() >= 8 )
        fp.write( bio.pop_full<char>() );
}

static inline void emit( ios::ostream &fp, ios::bitio &bio, Huffman::Codec &huff, const size_t size, const size_t pidx, const char *coded )
{
    std::cerr << "Emit " << size << "..."; std::cerr.flush();
    bio.push_full<uint32_t>( pidx );
    emit(fp,bio);
    for( size_t i=0; i < size; ++i )
    {
        huff.write(bio, coded[i] );
        emit(fp,bio);
    }
    huff.flush(bio);
    emit(fp,bio);
    assert(bio.size()==0);
    std::cerr << "done" << std::endl;
}


YOCTO_UNIT_TEST_IMPL(comp)
{
    const size_t        block_size = 32;
    char                block[ block_size ];
    char                coded[ block_size ];
    size_t              indices[ block_size ];
    size_t              count      = 0;
    core::move_to_front mtf;
    Huffman::Codec      huff;
    ios::bitio          bio;
    
    ios::icstream input( ios::cstdin );
    ios::ocstream output( ios::cstdout );
    
    char C = 0;
    while( input.query(C) )
    {
        block[ count++ ] = C;
        if( count >= block_size )
        {
            const size_t pidx = core::bwt::encode(coded, block, count, indices, &mtf);
            emit( output, bio, huff, count, pidx, coded );
            count = 0;
        }
    }
    
    if( count > 0 )
    {
        const size_t pidx = core::bwt::encode(coded, block, count, indices, &mtf);
        emit( output, bio, huff, count, pidx, coded );
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
