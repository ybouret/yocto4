#include "yocto/pack/huffman.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace yocto
{
    namespace pack
    {

        const char *huffman:: item_text(const char_type ch) throw()
        {
            static char s[4] = { 0, 0, 0, 0 };
            assert(ch>=-1);
            assert(ch<int(max_items));
            switch(ch)
            {
                case NYT: s[0] = 'N'; s[1] = 'Y'; s[2] = 'T'; break;
                case END: s[0] = 'E'; s[1] = 'N'; s[2] = 'D'; break;
                case INS: s[0] = 'I'; s[1] = 'N'; s[2] = 'S'; break;

                default:
                    if(ch<32||ch>=127)
                    {
                        s[0] = 'x'; memcpy(&s[1],hexa_text[unsigned(ch)&0xff],2);
                    }
                    else
                    {
                        s[0] = '\'';
                        s[1] = char(ch);
                        s[2] = '\'';
                    }
            }
            return s;
        }

        const char *huffman:: code_text(const code_type &code)
        {
            static char s[max_bits+1];
            assert(code.size<=max_bits);
            size_t i=0;
            for(const code_bit *node=code.head;node;node=node->next)
            {
                s[i++] = (node->flag) ? '1' : '0';
            }
            while(i<=max_bits)
            {
                s[i++] = 0;
            }
            return s;
        }


        huffman:: item_type:: item_type(const char_type ch) throw() :
        data(ch),
        freq(0)
        {
        }

        void huffman::item_type:: emit( ios::bitio &IO ) const
        {
            for(const code_bit *node=code.head;node;node=node->next)
            {
                IO.push(node->flag);
            }
        }


        std::ostream & operator<<(std::ostream &os, const huffman::item_type &item )
        {

            std::cerr << huffman::item_text(item.data) << ":freq=";
            std::cerr << std::setw(6) << item.freq;
            std::cerr << ":bits=" << std::setw(2) << item.code.size << "[" << huffman::code_text(item.code) << "]";
            return os;
        }


    }
}

#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace pack
    {
        huffman:: alphabet:: alphabet( code_pool &cache ) :
        size(0),
        itnum( max_items ),
        items( memory::kind<memory::global>::acquire_as<item_type>(itnum) )
        {
            initialize(cache);
        }

        huffman:: alphabet:: ~alphabet() throw()
        {
            for(size_t i=0;i<max_items;++i)
            {
                items[i].code.clear();
            }
            memory::kind<memory::global>::release_as<item_type>(items,itnum);
        }


        void huffman::alphabet::initialize( code_pool &cache ) throw()
        {
            (size_t&)size = 0;
            discard_in(cache);

            // build code from byte
            for(size_t i=0;i<max_bytes;++i)
            {
                item_type &item = items[i];

                (char_type &)(item.data) = char_type(i);
                item.freq                = 0;
                for(int j=7;j>=0;--j)
                {
                    const size_t mask = (1<<j);
                    code_bit    *node = cache.size ? cache.query() : new code_bit();
                    node->flag        = 0 != (mask&i);
                    item.code.push_back(node);
                }
                assert(8==item.code.size);
            }

            // no bits for controls so far
            for(size_t i=max_bytes;i<max_items;++i)
            {
                item_type &item = items[i];

                (char_type &)(item.data) = char_type(i);
                item.freq                = 1;
            }

        }

        void huffman::alphabet:: display( std::ostream &os ) const
        {
            os << "#Alphabet=" << size << std::endl;
            for(size_t i=0;i<max_items;++i)
            {
                if(items[i].freq>0)
                {
                    std::cerr << items[i] << std::endl;
                }
            }
        }


        void huffman:: alphabet:: rescale() throw()
        {
            //std::cerr << "RESCALE!" << std::endl;
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
                    if(++( (size_t&)size) >=max_bytes)
                    {
                        items[NYT].freq = 0;
                    }
                    break;

                default:
                    ++freq;
                    break;
            }
        }


        huffman::item_type & huffman::alphabet:: operator[](const size_t i) throw()
        {
            assert(i<max_items);
            return items[ i ];
        }

        void huffman::alphabet:: discard_in( code_pool &cache ) throw()
        {
            for(size_t i=0;i<max_items;++i)
            {
                code_type &code = items[i].code;
                while(code.size)
                {
                    cache.store( code.pop_back() );
                }
            }
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
        parent(0),
        freq(fr),
        data(ch),
        bits(0)
        {

        }

        void huffman:: node_type:: viz(ios::ostream &fp) const
        {
            fp.viz(this);
            if(data>=0)
            {
                fp(" [label=\"%s freq=%u bits%u\"];\n", item_text(data), unsigned(freq), unsigned(bits) );
            }
            else
            {
                fp(" [label=\"freq=%u bits=%u\"];\n", unsigned(freq), unsigned(bits) );
            }
            if(left)
            {
                left->viz(fp);
                fp.viz(this); fp << "->"; fp.viz(left); fp << " [label=\"0\"];\n";
            }
            if(right)
            {
                right->viz(fp);
                fp.viz(this); fp << "->"; fp.viz(right); fp << " [label=\"1\"];\n";
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
        count( max_nodes ),
        nodes( memory::kind<memory::global>::acquire_as<node_type>(count) ),
        nheap( max_nodes, as_capacity ),
        root(NULL),
        cache()
        {
        }


        huffman::tree_type:: ~tree_type() throw()
        {
            memory::kind<memory::global>::release_as<node_type>(nodes,count);
        }

        huffman::node_type *huffman::tree_type::build_for(alphabet &alpha)
        {
            root = NULL;
        TRY_BUILD:
            {
                //______________________________________________________________
                //
                // transfert all freq>0
                //______________________________________________________________
                nheap.free();
                size_t i=0;
                for(;i<max_items;++i)
                {
                    item_type &item = alpha[i];
                    assert(i==item.data);
                    if(item.freq>0)
                    {
                        node_type *node = new (&nodes[i]) node_type(item.data,item.freq);
                        node->bits      = 1;
                        nheap.push(node);
                        code_type &code = item.code;
                        while(code.size) cache.store(code.pop_back());
                    }

                }

                assert(nheap.size()>=1);

                //! head/tree build
                while(nheap.size()>1)
                {
                    assert(i<max_nodes);
                    node_type   *left   = nheap.pop();
                    node_type   *right  = nheap.pop();
                    const size_t pbits  = 1+max_of(left->bits,right->bits);
                    if(pbits>max_bits)
                    {
                        alpha.rescale();
                        goto TRY_BUILD;
                    }
                    node_type *parent = new (&nodes[i++]) node_type(INS,left->freq+right->freq);
                    parent->left  = left;
                    parent->right = right;
                    parent->bits  = pbits;
                    left->parent  = right->parent = parent;
                    left->flag    = false; // will code 0
                    right->flag   = true;  // will code 1
                    nheap.push(parent);
                }

                assert(1==nheap.size());
                root = nheap.pop();
            }
            assert(root);


            //__________________________________________________________________
            //
            // encoding
            //__________________________________________________________________
            for(size_t i=0;i<max_items;++i)
            {
                item_type &item = alpha[i];
                if(item.freq>0)
                {
                    code_type       &code = item.code;
                    const node_type *curr = &nodes[i];
                    while(curr->parent)
                    {
                        code_bit  *node = (cache.size>0) ? cache.query() : new code_bit();
                        node->flag      = curr->flag;
                        code.push_front(node);
                        curr  = curr->parent;
                    }
                }
            }

            return root;
        }

        void huffman::tree_type:: graphviz(const string &filename) const
        {
            assert(root);
            ios::wcstream fp(filename);
            fp << "digraph G {\n";
            fp << "rankdir=\"TB\";\n";
            root->viz(fp);
            fp << "}\n";
        }



    }

}


namespace yocto
{
    namespace pack
    {

        huffman::codec_type:: codec_type() :
        tree(),
        alpha(tree.cache),
        bio(max_bits,as_capacity)
        {
        }

        huffman:: codec_type:: ~codec_type() throw()
        {
        }
        
        void huffman::codec_type:: cleanup() throw()
        {
            alpha.initialize(tree.cache);
            bio.free();
            Q.free();
        }
        
    }

}

namespace yocto
{
    namespace pack
    {

        huffman:: encoder:: encoder() : codec_type()
        {
        }

        huffman:: encoder:: ~encoder() throw()
        {
        }

        void huffman:: encoder:: reset() throw()
        {
            cleanup();
        }

        void huffman:: encoder:: write(char C)
        {
            //__________________________________________________________________
            //
            // administrativia
            //__________________________________________________________________
            const item_type &item = alpha[ uint8_t(C) ];
            if(item.freq<=0)
            {
                if(alpha.size>0)
                {
                    //std::cerr << item_text(NYT) << "/" << alpha[NYT].code.size << std::endl;
                    alpha[NYT].emit(bio);
                }
            }

            //__________________________________________________________________
            //
            // use current model
            //__________________________________________________________________
            //std::cerr << item_text(C) << "/" << item.code.size << std::endl;
            item.emit(bio);

            //__________________________________________________________________
            //
            // bits to bytes
            //__________________________________________________________________
            while(bio.size()>=8)
            {
                Q.push_back(bio.pop_full<uint8_t>());
            }

            //__________________________________________________________________
            //
            // update model
            //__________________________________________________________________
            alpha.update(C);
            (void) tree.build_for(alpha);
        }

        void huffman:: encoder:: flush()
        {
            //__________________________________________________________________
            //
            // send END
            //__________________________________________________________________
            alpha[END].emit(bio);
            bio.fill_to_byte_with(false);
            while(bio.size()>=8)
            {
                Q.push_back(bio.pop_full<uint8_t>());
            }
            assert(0==bio.size());
        }
        
    }

}





