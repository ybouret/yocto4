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
#include "yocto/ordered/sorted-vector.hpp"

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
                    word->h = 0;
                    std::cerr << "Word #" << H << " : '" << *word << "'" << std::endl;
                }
            }

#if 1
            //length
            core::merging<Word>::sort(words, compareWords, 0);
#endif
            std::cerr << "processing..." << std::endl;
            for(Word *word = words.head;word;word=word->next)
            {
                std::cerr << "'" << *word << "' => " << int(word->H) << std::endl;
            }

            sorted_vector<uint8_t> codes;

            unsigned top = nw;
            unsigned rnd = 0;
            while(words.size)
            {
                ++rnd;
                Words stk;

                std::cerr << "Round #" << rnd << std::endl;
                codes.free();

                while(words.size)
                {
                    Word *w = words.pop_front(); assert(w->size>0);
                    stk.push_back(w);
                    const size_t j = w->head->data ^ w->h;
                    if(1==w->size)
                    {
                        std::cerr << "Assigning Final H=" << int(w->H) << std::endl;
                        if(J[j]<0) J[j] = w->H;
                        if(J[j]>=0&&J[j]!=w->H)
                        {
                            throw exception("Level 1 FAILURE");
                        }
                        delete stk.pop_back();
                    }
                    else
                    {
                        w->h = j;
                        delete w->pop_front();
                    }
                }

                //std::cerr << "codes=" << codes << std::endl;
                //break;
                words.swap_with(stk);
            }
            
        }
    }
}
