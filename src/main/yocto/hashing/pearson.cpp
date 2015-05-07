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


#if 0
        void Pearson:: build_from(int *I)
        {
            reset();
            lw_array<int>     a(I,256);
            lw_array<uint8_t> b(table,256);
            co_qsort(a,b);

            std::cerr << "table=" << std::endl;
            std::cerr.flush();
            for(size_t i=0,k=0;i<16;++i)
            {
                for(size_t j=0;j<16;++j)
                {
                    fprintf(stderr,"%3d ", int(table[k++]));
                }
                fprintf(stderr, "\n");
            }
            fflush(stderr);
        }
#endif

        static int compareWords( const Pearson::Word *lhs, const Pearson::Word *rhs, void * )
        {

            const size_t lmin = min_of(lhs->size,rhs->size);
            const Pearson::Code *L = lhs->head;
            const Pearson::Code *R = rhs->head;

            for(size_t i=0;i<lmin;++i,L=L->next,R=R->next)
            {
                if(L->data<R->data)
                {
                    return -1;
                }
                else
                {
                    if(R->data<L->data)
                    {
                        return 1;
                    }
                    // else continue
                }
            }


            if(L)
            {
                assert(NULL==R);
                // L is longer
                return 1;
            }
            else
            {
                if(R)
                {
                    return -1;
                }
                else
                {
                    return 0;
                }
            }

        }

        namespace
        {

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

#if 0
            //lexi sort
            core::merging<Word>::sort(words, compareWords, 0);

            std::cerr << "processing..." << std::endl;
            for(Word *word = words.head;word;word=word->next)
            {
                std::cerr << "'" << *word << "' => " << int(word->H) << std::endl;
            }
#endif

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
                        if(J[j]>=0) throw exception("Level-1 failure");
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
                    }
                }
                
                words.swap_with(stk);
            }
            
        }
    }
}
