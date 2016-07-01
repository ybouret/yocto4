#include "yocto/pack/huffman.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>

namespace yocto
{
    namespace pack
    {

        const char *huffman:: item_text(const char_type ch) throw()
        {
            static char s[4] = { 0, 0, 0, 0 };
            assert(ch>=-1);
            assert(ch<max_items);
            switch(ch)
            {
                case NYT: s[0] = 'N'; s[1] = 'Y'; s[2] = 'T'; break;
                case END: s[0] = 'E'; s[1] = 'N'; s[2] = 'D';break;
                case INS: s[0] = 'I'; s[1] = 'N'; s[2] = 'S';break;

                default:
                    if(ch<32||ch>=127)
                    {
                        s[0] = 'x'; memcpy(s+1, hexa_text[unsigned(ch)&0xff],2);
                    }
                    else
                    {
                        s[0] = ' ';
                        s[1] = char(ch);
                        s[2] = ' ';
                    }
            }
            return s;
        }

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
                item.code                = char_type(i);
                item.bits                = 8;
            }

            for(size_t i=max_bytes;i<max_items;++i)
            {
                item_type &item = items[i];
                (char_type &)(item.data) = char_type(i);
                item.freq                = 0;
                item.code                = 0;
                item.bits                = 0;
            }
            enable_nyt();
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
            items[NYT].freq = 1;
        }


        void huffman::alphabet:: enable_end() throw()
        {
            items[END].freq = 1;
        }

        huffman::item_type & huffman::alphabet:: operator[](const size_t i) throw()
        {
            assert(i<max_items);
            return items[ i ];
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

        void huffman:: node_type:: viz(ios::ostream &fp) const
        {
            fp.viz(this);
            fp(" label=\"%s#%u\"\n", item_text(data), unsigned(freq) );
            if(left)
            {
                left->viz(fp);
                fp.viz(this); fp << "->"; fp.viz(left);
            }
            if(right)
            {
                right->viz(fp);
                fp.viz(this); fp << "->"; fp.viz(right);
            }
        }

    }
}


#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace pack
    {

        huffman:: tree_type:: tree_type() :
        root(0),
        count( max_nodes ),
        nodes( memory::kind<memory::global>::acquire_as<node_type>(count) ),
        nheap( max_nodes, as_capacity )
        {
        }


        huffman::tree_type:: ~tree_type() throw()
        {
            memory::kind<memory::global>::release_as<node_type>(nodes,count);
        }

        void huffman::tree_type::build_for(alphabet &alpha)
        {
            root = NULL;
            {
                //! transfert all freq>0
                nheap.free();
                for(size_t i=0;i<max_bytes;++i)
                {
                    item_type &item = alpha[i];
                    if(item.freq>0)
                    {
                        nheap.push(new (&nodes[i]) node_type(item.data,item.freq));
                    }
                }

                assert(nheap.size()>=1);

                //! head/tree build
                size_t i = max_bytes;
                while(nheap.size()>1)
                {
                    assert(i<max_nodes);
                    node_type *left   = nheap.pop();
                    node_type *right  = nheap.pop();
                    node_type *parent = new (&nodes[i++]) node_type(INS,left->freq+right->freq);
                    nheap.push(parent);
                }

                assert(1==nheap.size());
                root = nheap.pop();
            }
        }

        void huffman::tree_type:: graphviz(const string &filename) const
        {
            assert(root);
            ios::wcstream fp(filename);
            fp << "digraph G {\n";
            root->viz(fp);
            fp << "}\n";
        }



    }

}


