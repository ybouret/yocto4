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
                    if(ch>=32&&ch<127)
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


        DSF:: Tree:: ~Tree() throw()
        {
            stack.reset();
            memory::kind<memory::global>::release(wksp,wlen);
        }

#define YDSF_CLEAN(NODE) do { Node *tmp = NODE; tmp->left=tmp->right=0; } while(false)

        void DSF:: Tree:: build_using( Alphabet &alphabet )
        {
            assert(alphabet.count>=2);
            FreqType freqs[MaxItems];
            
            //__________________________________________________________________
            //
            // initialize nodes a.k.a segments
            //__________________________________________________________________
            {
                Node *ini = &nodes[0];
                YDSF_CLEAN(ini);
                ini->start = alphabet.store;
                ini->count = alphabet.count;
                stack.store(ini);
            }

            size_t inode = 0;
            while( stack.size )
            {
                Node *node = stack.query();

                assert(node->count>=2);
                assert(inode<MaxNodes); Node *left   = &node[++inode]; YDSF_CLEAN(left);
                assert(inode<MaxNodes); Node *right  = &node[++inode]; YDSF_CLEAN(right);
                node->left  = left;
                node->right = right;

                assert(node->count>=2);
                std::cerr << "SPLITTING " << node->count << " nodes.." << std::endl;
                Item       **start = node->start;
                const size_t count = node->count;
                
                // build cumulative function inf freqs
                FreqType Lambda = 0;
                for(size_t i=0;i<count;++i)
                {
                    const FreqType lam = start[i]->Freq;
                    freqs[i] += (Lambda += lam);
                }
#define YSDF_ABS(A,B) ((A)<(B)) ? (B)-(A) : (A)-(B);
                
                
                exit(1);
            }

            std::cerr << "inode=" << inode << "/" << alphabet.count << std::endl;
        }


        void DSF:: Node:: viz( ios::ostream &fp ) const
        {
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
        
        
        
    }
}

