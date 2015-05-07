#include "yocto/hashing/pearson.hpp"
#include <cstring>

namespace yocto
{
    namespace hashing
    {

        void Pearson:: reset() throw()
        {
            for(unsigned i=0;i<256;++i)
            {
                table[i] = uint8_t(i);
            }
        }


        Pearson:: Pearson() throw() :
        table()
        {
            reset();
        }

        Pearson:: ~Pearson() throw()
        {
        }

        Pearson:: Pearson( const Pearson &other) throw() :
        table()
        {
            memcpy(table, other.table, sizeof(table) );
        }

        Pearson & Pearson:: operator= ( const Pearson &other) throw()
        {
            memmove(table, other.table, sizeof(table) );
            return *this;
        }

        uint8_t Pearson:: operator()( const void *data, const size_t size ) const throw()
        {
            assert(!(data==NULL&&size>0));
            uint8_t        h = 0;
            const uint8_t *C = (const uint8_t *)data;
            for(size_t i=0;i<size;++i)
            {
                h = table[ C[i] ^ h ];
            }
            return h;
        }

        uint8_t Pearson:: operator()( const memory::ro_buffer &buff ) const throw()
        {
            const Pearson &self = *this;
            return self( buff.ro(), buff.length() );
        }



    }

}

#include "yocto/exception.hpp"
#include "yocto/sequence/lw-array.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/sort/merge.hpp"
#include "yocto/code/utils.hpp"

#include <iostream>

namespace yocto
{
    namespace hashing
    {
        uint8_t Pearson:: index_of( const uint8_t h ) const
        {
            for(unsigned i=0;i<256;++i)
            {
                if(h==table[i]) return i;
            }
            throw exception("invalid Pearson Table, missing %u!", unsigned(h) );
        }



        static int compareWords( const Pearson::Word *lhs, const Pearson::Word *rhs, void * )
        {
            return __compare<size_t>(lhs->size,rhs->size);
        }

        void Pearson:: build( Words &words )
        {
            int J[256];
            for(size_t i=0;i<256;++i) J[i] = -1;

            const size_t nw = words.size;
            if(nw>255) throw exception("too many words...");
            std::cerr << "Building " << nw << " words" << std::endl;
            // init H
            {
                unsigned H = 0;
                for(Word *word = words.head;word;word=word->next)
                {
                    word->H = H++;
                    std::cerr << "Word #" << H << " : '" << *word << "'" << std::endl;
                }
            }

            //length
            core::merging<Word>::sort(words, compareWords, 0);

            std::cerr << "processing..." << std::endl;
            for(Word *word = words.head;word;word=word->next)
            {
                std::cerr << "'" << *word << "' => " << int(word->H) << std::endl;
            }

            unsigned top = nw;
            unsigned rnd = 0;
            while(words.size)
            {
                ++rnd;
                std::cerr << "Round #" << rnd << std::endl;
                Words stk;
                while(words.size)
                {
                    Word *w = words.pop_front(); assert(w->size>0);
                    std::cerr << "\tprocessing " << *w << std::endl;
                    const uint8_t j = w->h ^ w->head->data;
                    if(1==w->size)
                    {
                        const uint8_t H = w->H;
                        delete        w;
                        if(J[j]>=0 && J[j] != H) throw exception("Level-1 failure");
                        J[j] = H;
                    }
                    else
                    {
                        stk.push_back(w);
                        if(J[j]<0)
                        {
                            if(top>255) throw exception("Level-2 failure");
                        }
                        J[j] = top++;
                        w->h = J[j];
                        delete w->pop_front();
                    }
                }
                
                words.swap_with(stk);
            }
            
        }
    }
}
