#include "yocto/pack/dsf.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
#include "yocto/exception.hpp"

namespace yocto
{
    namespace pack
    {

        void   DSF::Item::  emit( ios::bitio &bio ) const
        {
            bio.push(Code,Bits);
        }


#define YDSF_CHECK 1

        DSF:: Alphabet:: Alphabet() :
        size(0),
        store(0),
        count(0),
        items(0),
        wlen( MaxItems * sizeof(Item) + MaxItems*sizeof(Item*) ),
        wksp( memory::kind<memory::global>::acquire(wlen) )
        {
            items = (Item *)wksp;
            store = (Item **)( ( (char*)wksp) +  MaxItems * sizeof(Item) );
            initialize();
#if defined(YDSF_CHECK)
            __check(__LINE__);
#endif
        }

        void DSF::Alphabet:: initialize() throw()
        {
            (size_t&)size = 0;
            for(size_t i=0;i<MaxBytes;++i)
            {
                Item &item = items[i];
                item.Char  = CharType(i);
                item.Freq  = 0;
                item.Code  = CodeType(i);
                item.Bits  = 8;
                item.Slot  = 0;
            }

            for(size_t i=MaxBytes;i<MaxItems;++i)
            {
                Item &item = items[i];
                item.Char  = CharType(i);
                item.Freq  = 1;
                item.Code  = 0;
                item.Bits  = 0;
                item.Slot  = 0;
            }


            count = 2;
            store[0] = items+NYT; items[NYT].Slot = store+0;
            store[1] = items+END; items[END].Slot = store+1;

        }

        DSF:: Alphabet:: ~Alphabet() throw()
        {
            memory::kind<memory::global>::release(wksp,wlen);
        }

        static const char *Char2Text(const DSF::CharType ch) throw()
        {
            static char s[4] = {0,0,0,0};
            assert(ch>=0);
            assert(ch<DSF::MaxItems);
            switch(ch)
            {
                case DSF::NYT: s[0]='N'; s[1]='Y'; s[2]='T'; break;
                case DSF::END: s[0]='E'; s[1]='N'; s[2]='D'; break;
                default:
                    if( (ch>=32&&ch<127) && ch != '\"' )
                    {
                        s[0] = '\''; s[1]=char(ch); s[2] = '\'';
                    }
                    else
                    {
                        s[0] = 'x'; memcpy(s+1, hexa_text[uint8_t(ch)], 2);
                    }
                    break;
            }
            return s;
        }


        void DSF::Alphabet:: display() const
        {
            std::cerr << "alphabet.size  =" << size << std::endl;
            for(size_t i=0;i<count;++i)
            {
                const Item *item = store[i]; assert(item);
                std::cerr << Char2Text(item->Char) << ":";
                std::cerr << "freq=" << std::setw(6) << item->Freq << ":";
                std::cerr << "bits=" << std::setw(2) << item->Bits << ":";
                std::cerr << "[";
                for(size_t i=item->Bits;i>0;--i)
                {
                    const size_t bit = 1 << (i-1);
                    std::cerr <<  (0!=(bit&item->Code) ?'1':'0');
                }
                std::cerr << "]";
                std::cerr << std::endl;
            }
        }



        void DSF:: Alphabet:: __check(const int line)
        {
            const unsigned n  = count;
            for(unsigned i=0;i<n;++i)
            {
                Item **slot = store+i;
                if( 0 == *slot ) throw exception("null slot[%u]",i);
                Item  *item = *slot;
                if(item->Slot!=slot) throw exception("bad slot[%u] linking...",i);
                if(item->Freq<=0)    throw exception("invalid frequency at slot[%u]",i);
            }
            const unsigned nm1=n-1;
            for(unsigned i=0;i<nm1;++i)
            {
                Item **curr = store+i; assert(0!=curr[0]); assert(0!=curr[1]);
                if(curr[0]->Freq>curr[1]->Freq) throw exception("bad ranking at slot[%u]...",i);
            }

        }

        void DSF:: Alphabet:: update(const char c) throw()
        {
            Item *item = &items[uint8_t(c)];

            if(item->Freq<=0)
            {
                //______________________________________________________________
                //
                // a new char
                //______________________________________________________________

                // increase count
                assert(size<MaxBytes);
                ++( (size_t&)size );


                // check if all bytes are accounted for
                if(size>=MaxBytes)
                {
                    // remove NYT from coding system...
                    assert(&items[NYT] == store[0] );
                    assert(1==items[NYT].Freq);
                    assert(count>1);

                    item[NYT].Freq=0;
                    item[NYT].Slot=0;

                    --count;
                    for(size_t i=0;i<count;++i)
                    {
                        Item **slot = store+i;
                        *slot = *(slot+1);
                        (*slot)->Slot = slot;
                    }

                }


                // append to store
                Item **curr = &store[count++];
                *curr = item;
                item->Slot = curr;
                item->Freq = 1;

                // move into position
                Item **prev = curr-1;
                while(prev>store)
                {
                    Item *curr_item = *curr;
                    Item *prev_item = *prev;
                    if(prev_item->Freq<=curr_item->Freq)
                    {
                        break;
                    }
                    *curr = prev_item;
                    *prev = curr_item;
                    curr_item->Slot = prev;
                    prev_item->Slot = curr;
                    curr = prev;
                    prev = curr-1;
                }
            }
            else
            {
                // update frequency and check position
                ++(item->Freq);
                Item **curr = item->Slot;
                Item **last = store+count;

                Item **next = curr+1;
                while(next<last)
                {
                    Item *curr_item = *curr;
                    Item *next_item = *next;
                    if(curr_item->Freq<=next_item->Freq)
                    {
                        break;
                    }
                    *curr = next_item;
                    *next = curr_item;
                    curr_item->Slot = next;
                    next_item->Slot = curr;
                    curr = next;
                    next = curr+1;
                }

            }

        }

        void DSF:: Alphabet:: rescale() throw()
        {
            std::cerr << "Rescale" << std::endl;
            for(size_t i=0;i<count;++i)
            {
                (store[i]->Freq >>= 1) |= 1;
            }
        }

        const DSF::Item & DSF::Alphabet:: operator[](const CharType C) const throw()
        {
            assert(C>=0);
            assert(C<MaxItems);
            return items[ C ];
        }

    }
}

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace pack
    {
        DSF:: Tree:: Tree() :
        nodes(0),
        wlen( MaxNodes * sizeof(Node) ),
        wksp( memory::kind<memory::global>::acquire(wlen) )
        {
            nodes = (Node *)wksp;
        }


        static inline
        DSF::FreqType __delta_abs(const DSF::FreqType lhs,
                                  const DSF::FreqType rhs) throw()
        {
            return (lhs<rhs) ? (rhs-lhs) : (lhs-rhs);
        }

        DSF:: Tree:: ~Tree() throw()
        {
            stack.reset();
            memory::kind<memory::global>::release(wksp,wlen);
        }


        static inline bool updateCodes(DSF::Item         **it,
                                       const size_t        ni,
                                       const DSF::CodeType bit) throw()
        {
            for(size_t i=0;i<ni;++i)
            {
                DSF::Item &item = *it[i];
                if( ++item.Bits > DSF::MaxBits )
                {
                    return false;
                }
                (item.Code<<=1)|=bit;
            }
            return true;
        }

        void DSF:: Tree:: initialize() throw()
        {
            memset(wksp,0,wlen);
        }


        void DSF:: Tree:: build_using( Alphabet &alphabet )
        {
            assert(alphabet.count>=2);
            FreqType freqs[MaxItems];

        BUILD_TREE:
#define YDSF_CLEAN(NODE) do { Node *tmp = (NODE); tmp->next=tmp->left=tmp->right=0; } while(false)
            //__________________________________________________________________
            //
            // initialize nodes a.k.a segments
            //__________________________________________________________________
            {
                Item        **it = alphabet.store;
                const size_t  ni = alphabet.count;
                for(size_t i=0;i<ni;++i)
                {
                    Item &item = *it[i];
                    item.Bits = 0;
                    item.Code = 0;
                }
                Node *ini = &nodes[0];
                YDSF_CLEAN(ini);
                ini->start = it;
                ini->count = ni;
                stack.reset();
                stack.store(ini);
            }

            size_t inode = 0;
            while(stack.size>0)
            {
                Node *node = stack.query();

                assert(node->count>=2);
                ++inode; assert(inode<MaxNodes); Node *left   = &nodes[inode]; YDSF_CLEAN(left);
                ++inode; assert(inode<MaxNodes); Node *right  = &nodes[inode]; YDSF_CLEAN(right);
                node->left  = left;
                node->right = right;

                assert(node->count>=2);
                Item       **start = node->start;
                const size_t count = node->count;

                //______________________________________________________________
                //
                // Split...
                //______________________________________________________________
                if(2==count)
                {
                    left->start = start;    left->count = 1;
                    right->start = start+1; right->count = 1;
                }
                else
                {
                    // build cumulative function and freqs
                    FreqType Lambda = 0;
                    for(size_t i=0;i<count;++i)
                    {
                        const FreqType lam = start[i]->Freq;
                        freqs[i] = (Lambda += lam);
                    }

                    // find the Shannon-Fano criteria
                    const size_t I_max = count-1;
                    size_t       I_cut = 0;
                    size_t       delta = __delta_abs(Lambda, freqs[I_cut] << 1);
                    for(size_t i=1;i<I_max;++i)
                    {
                        const size_t dtemp = __delta_abs(Lambda,freqs[i]<<1);
                        if(dtemp<=delta)
                        {
                            I_cut = i;
                            delta = dtemp;
                        }
                    }

                    //cut according to criteria
                    const size_t  nleft = I_cut+1;
                    left->start = start;
                    left->count = nleft;
                    if(left->count>1)
                    {
                        stack.store(left);
                    }

                    right->start = start+nleft;
                    right->count = count-nleft;
                    if(right->count>1)
                    {
                        stack.store(right);
                    }

                }

                //______________________________________________________________
                //
                // update codes
                //______________________________________________________________
                if(!updateCodes(left->start,left->count,1))
                {
                    alphabet.rescale();
                    goto BUILD_TREE;
                }
                
                if(!updateCodes(right->start,right->count,0))
                {
                    alphabet.rescale();
                    goto BUILD_TREE;
                }


            }

            //std::cerr << "inode=" << inode << "/" << alphabet.count << std::endl;
        }


        void DSF:: Node:: viz( ios::ostream &fp ) const
        {
            assert(start);
            assert(count>0);

            fp.viz(this);
            if(1==count)
            {
                fp(" [label=\"%s,freq=%u\"];\n", Char2Text(start[0]->Char), unsigned(start[0]->Freq) );
            }
            else
            {
                unsigned sum = 0;
                for(size_t i=0;i<count;++i)
                {
                    sum += start[i]->Freq;
                }
                fp(" [label=\"freq=%u\"];\n",sum);
            }
            
            if(left)
            {
                left->viz(fp);
                fp.viz(this); fp( "->" ); fp.viz(left); fp(" [label=1];\n");
            }
            
            if(right)
            {
                right->viz(fp);
                fp.viz(this); fp( "->" ); fp.viz(right); fp(" [label=0];\n");
            }
        }
        
        
        void DSF::Tree:: graphviz( const string &filename ) const
        {
            ios::wcstream fp(filename);
            fp << "digraph G{\n"; nodes->viz(fp); fp << "}\n";
        }
        
        const DSF::Node * DSF::Tree:: root() const throw()
        {
            return &nodes[0];
        }


    }
}


namespace yocto
{
    namespace pack
    {
        DSF:: Codec:: ~Codec() throw()
        {
        }

        DSF:: Codec:: Codec() :
        alphabet(),
        tree(),
        bio(2*MaxBits,as_capacity)
        {
            tree.build_using(alphabet);
        }

        void DSF:: Codec:: cleanup() throw()
        {
            alphabet.initialize();
            tree.initialize();
            bio.free();
            tree.build_using(alphabet);
            clear();
        }

    }
}

namespace yocto
{
    namespace pack
    {
        DSF:: Encoder:: ~Encoder() throw()
        {
        }

        DSF:: Encoder:: Encoder() : Codec()
        {
        }

        void DSF:: Encoder:: reset() throw()
        {
            cleanup();
        }

        void DSF:: Encoder:: write(char C)
        {
            const uint8_t B(C);
            const Item &item = alphabet[B];

            // check if we need to emit NYT
            if(item.Freq<=0)
            {
                assert(alphabet.size<MaxBytes);
                if(alphabet.size>0)
                {
                    alphabet[NYT].emit(bio);
                    std::cerr << "<NYT>";
                    assert(alphabet[B].Bits==8);
                }
            }

            // emit current encoding
            alphabet[B].emit(bio);
            std::cerr << int(B) << "/";

            // update model
            alphabet.update(C);
            tree.build_using(alphabet);

            // transfer to queue
            while(bio.size()>=8)
            {
                Q.push_back( bio.pop_full<uint8_t>() );
            }
        }

        void DSF:: Encoder:: flush()
        {
            alphabet[END].emit(bio);
            bio.fill_to_byte_with(false);
            while(bio.size()>=8)
            {
                Q.push_back( bio.pop_full<uint8_t>() );
            }
            assert(0==bio.size());
        }

    }

}


namespace yocto
{
    namespace pack
    {
        DSF:: Decoder:: ~Decoder() throw()
        {

        }

        DSF:: Decoder:: Decoder() :
        status(wait_for8),
        walker(0)
        {

        }

        void DSF:: Decoder:: reset() throw()
        {
            cleanup();
            status = wait_for8;
            walker = 0;
        }

        void DSF:: Decoder:: flush()
        {
        }


        void DSF:: Decoder:: on_new(const char C)
        {
            std::cerr << int(uint8_t(C)) << "/";
            Q.push_back(C);
            alphabet.update(C);
            tree.build_using(alphabet);
            walker = tree.root();
            assert(walker->count>1);
            status = wait_for1;
        }


        void DSF:: Decoder:: write(char C)
        {
            bio.push_full<uint8_t>(C);
            while( bio.size() )
            {
                switch(status)
                {
                        //______________________________________________________
                        //
                        // we need a byte
                        //______________________________________________________
                    case wait_for8:
                        assert(0==walker);
                        if(bio.size()<8)
                        {
                            return;
                        }
                        else
                        {
                            on_new(bio.pop_full<uint8_t>());
                        }
                        break;

                        //______________________________________________________
                        //
                        // we walk down the tree
                        //______________________________________________________
                    case wait_for1: {
                        assert(walker);
                        assert(walker->count>1);
                        assert(walker->left);
                        assert(walker->right);

                        // walk down the tree
                        const bool flag = bio.pop();
                        if(flag)
                        {
                            walker = walker->left;
                        }
                        else
                        {
                            walker = walker->right;
                        }

                        // is it the end ?
                        if(1==walker->count)
                        {
                            assert(0==walker->left);
                            assert(0==walker->right);
                            const CharType ch = walker->start[0]->Char;
                            switch(ch)
                            {
                                case END: std::cerr << "<END>" << std::endl;
                                    bio.free(); walker=0; status=wait_for8;
                                    break;

                                case NYT:
                                    std::cerr << "<NYT>";
                                    walker=0;
                                    status=wait_for8;
                                    break;

                                default:
                                    assert(ch>=0);
                                    assert(ch<256);
                                    on_new(char(ch));
                                    break;
                            }
                        }
                    } break;
                        //______________________________________________________


                }
            }
            // end decoding
        }



    }

}



