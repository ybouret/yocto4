#include "yocto/pack/huffman.hpp"

namespace yocto
{
    namespace pack
    {

        huffman:: item_type:: item_type(const char_type ch) throw() :
        data(ch),
        freq(0)
        {
        }
    }
}

#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace pack
    {
        huffman:: alphabet:: alphabet() :
        itnum( max_items ),
        items( memory::kind<memory::global>::acquire_as<item_type>(itnum) )
        {
        }

        huffman:: alphabet:: ~alphabet() throw()
        {
            memory::kind<memory::global>::release_as<item_type>(items,itnum);
        }


        void huffman::alphabet::initialize() throw()
        {
            size = 0;
            
            for(size_t i=0;i<max_bytes;++i)
            {
                item_type &item = items[i];

                (char_type &)(item.data) = char_type(i);
                item.freq                = 0;
            }

            for(size_t i=max_bytes;i<max_items;++i)
            {
                item_type &item = items[i];
                (char_type &)(item.data) = char_type(i);
                item.freq                = 0;
            }
            
        }


        void huffman:: alphabet:: rescale() throw()
        {
#define YHUFF_RESCALE(I) freq_type &freq = items[I].freq; if(freq>0) { (freq >>= 1) |= 1; }
            YOCTO_LOOP_FUNC_(max_items,YHUFF_RESCALE,0);
        }

        void huffman:: alphabet:: update(const char c) throw()
        {
            freq_type &freq = items[ uint8_t(c) ].freq;
            switch(freq)
            {
                case 0:
                    assert(size<max_bytes);
                    ++freq;
                    if(++size>=max_bytes)
                    {
                        items[NYT].freq = 0;
                    }
                    break;

                default:
                    ++freq;
                    break;
            }
        }

        void huffman::alphabet:: enable_nyt() throw()
        {
            items[NYT].freq = 1;
        }

        void huffman::alphabet:: enable_end() throw()
        {
            items[END].freq = 1;
        }
    }
}

namespace yocto
{
    namespace pack
    {
        huffman:: node_type:: node_type(const char_type ch, const freq_type fr) throw() :
        left(0),
        right(0),
        freq(fr),
        data(ch)
        {

        }

        
    }
}


namespace yocto
{
    namespace pack
    {

        huffman:: tree_type:: tree_type() :
        count( max_nodes ),
        nodes( memory::kind<memory::global>::acquire_as<node_type>(count) ),
        nheap( max_nodes, as_capacity )
        {
        }


        huffman::tree_type:: ~tree_type() throw()
        {
            memory::kind<memory::global>::release_as<node_type>(nodes,count);
        }

    }

}


