#include "yocto/pack/dsf.hpp"
#include "yocto/code/utils.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace yocto
{
    namespace pack
    {

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
            }

            for(size_t i=MaxBytes;i<MaxItems;++i)
            {
                Item &item = items[i];
                item.Char  = CharType(i);
                item.Freq  = 1;
                item.Code  = 0;
                item.Bits  = 0;
            }


            count = 2;
            store[0] = items+NYT;
            store[1] = items+END;

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
                    assert(1==item[NYT].Freq);
                    item[NYT].Freq=0;
                    assert( &item[NYT] == store[0] );
                    assert(count>1);
                    memmove(store[0],store[1], --count*sizeof(Item*) );
                }

                // append to store
                Item **curr = &store[count++];
                *curr = item;
            }
            else
            {

            }

        }



    }
}
