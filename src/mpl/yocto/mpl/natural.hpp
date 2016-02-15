#ifndef YOCTO_MPL_NATURAL_INCLUDED
#define YOCTO_MPL_NATURAL_INCLUDED 1

#include "yocto/mpl/types.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/code/bswap.hpp"
#include <iosfwd>

namespace yocto
{
    namespace mpl
    {

#define YOCTO_CHECK_MPN(X)     \
assert( (X).byte != 0 );       \
assert( (X).maxi >  0 );       \
assert( (X).size <= (X).maxi );\
assert( ! ( ((X).size>0) && (X).byte[(X).size-1] <= 0 ) )

#define YOCTO_MPN_DECL(OP,CALL) \
inline friend natural OP (const natural &lhs, const natural &rhs) \
{ return CALL(lhs.byte,lhs.size,rhs.byte,rhs.size); } \
inline friend natural OP (const natural &lhs, word_t rhs ) \
{ const size_t n = prepare(rhs); return CALL(lhs.byte,lhs.size,&rhs,n); } \
inline friend natural OP (word_t lhs, const natural &rhs) \
{ const size_t n = prepare(lhs); return CALL(&lhs,n,rhs.byte,rhs.size); }

        class natural : public memory::ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // management functions
            //
            //__________________________________________________________________
            natural();                   //!< this is zero
            natural(const word_t);       //!< any word
            natural(const natural &);    //!< copy constructor

            //! no throw swaps
            inline void xch( natural &other) throw()
            {
                cswap(maxi,other.maxi);
                cswap(size,other.size);
                cswap(byte,other.byte);
            }

            //! assign operator
            inline natural & operator=(const natural &other)
            {
                natural tmp(other); xch(tmp); return *this;
            }


            //! assign a word, optimized, no throw
            inline natural & operator=(const word_t n) throw()
            {
                assert(maxi>=sizeof(word_t));
                size = sizeof(word_t);
                *(word_t *)byte = swap_le(n);
                update();
                YOCTO_CHECK_MPN(*this);
                return *this;
            }

            //! destructor
            virtual ~natural() throw();


            static inline void update(const uint8_t *b, size_t &n)
            {
                assert(b!=0);
                while(n>0)
                {
                    const size_t imsb = n-1;
                    if(b[imsb]>0) break;
                    n=imsb;
                }
            }

            static inline size_t prepare(word_t &w)
            {
                size_t n = sizeof(word_t);
                w        = swap_le(w);
                update( (uint8_t *)&w, n );
                return n;
            }

            //! try to decrease size
            inline void update() throw() {  update(byte,size);  }

            //! size=maxi and update
            inline void rescan() throw() {  update(byte,size=maxi); }

            //! ro_buffer interface
            virtual size_t length() const throw() { return size; }

            //__________________________________________________________________
            //
            //
            // output
            //
            //__________________________________________________________________
            void display( std::ostream & ) const;
            inline friend std::ostream & operator<<( std::ostream &os, const natural &n )
            {
                n.display(os);
                return os;
            }

            //! least significant word
            inline word_t lsw() const throw()
            {
                word_t         ans = 0;
                const uint8_t *b = byte;
                for(size_t i=min_of<size_t>(sizeof(word_t),size);i>0;--i)
                {
                    (ans <<= 8) |= unsigned( *(b++) );
                }
                return ans;
            }

            //__________________________________________________________________
            //
            //
            // comparison
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //
            // addition
            //
            //__________________________________________________________________
            inline void    ldz() throw() { size=0; }

            static natural add(const void *lhs, size_t nl,
                               const void *rhs, size_t nr);

            natural  operator+() { return natural(*this); }
            YOCTO_MPN_DECL(operator+,add)


        private:
            size_t   maxi; //!< maximum #bytes
            size_t   size; //!< current #bytes
            uint8_t *byte; //!< little endian

            static  uint8_t *   build(size_t &n);
            virtual const void *get_address() const throw() { return byte; }
            natural(size_t n, const as_capacity_t &); //!< default n bytes, must be updated
        };

    }

    typedef mpl::natural mpn;
}

#endif
