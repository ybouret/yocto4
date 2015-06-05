#ifndef YOCTO_GEMS_BINARY_INCLUDED
#define YOCTO_GEMS_BINARY_INCLUDED 1

#include "yocto/gems/atom.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename T>
        class binary_atoms : public memory::rw_buffer
        {
        public:
            explicit binary_atoms() throw() :
            size(0), maxi(0), curr(0), wlen(0), wksp(0) {}

            inline void release() throw()
            {
                memory::kind<memory::global>::release(wksp, wlen);
                (size_t&)maxi=0;
                (size_t&)size=0;
                curr=0;
            }

            virtual ~binary_atoms() throw()
            {
                release();
            }

            inline void build(size_t nmin)
            {
                (size_t&)size=0;
                if(nmin>maxi)
                {
                    nmin = YOCTO_ROUND256(nmin);
                    memory::kind<memory::global>::release(wksp, wlen);
                    (size_t&)maxi=0;

                    wlen = nmin * atom<T>::io_bytes;
                    wksp = memory::kind<memory::global>::acquire(wlen);
                    (size_t&)maxi = wlen/atom<T>::io_bytes;
                }
                curr = (uint8_t *)wksp;
            }

            inline void reset() throw() { size=0; curr = (uint8_t *)wksp; }

            inline void store( const atom<T> &a ) throw()
            {
                assert(size<maxi);
                a.append_to(curr);
                ++(size_t&)size;
            }

            virtual size_t length() const throw() { return size *atom<T>::io_bytes; }

            inline void assume( size_t n ) throw()
            {
                assert(n<=maxi);
                (size_t&)size=n;
            }

            const uint8_t *query(size_t indx) const throw()
            {
                assert(indx<size);
                return static_cast<uint8_t*>(wksp)+(indx*atom<T>::io_bytes);
            }

            const size_t size;
            const size_t maxi;


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(binary_atoms);
            uint8_t *curr;
            size_t   wlen;
            void    *wksp;
            const void *get_address() const throw() { return wksp; }
        };
        
    }
}

#endif
