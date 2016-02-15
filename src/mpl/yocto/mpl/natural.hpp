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

#define YOCTO_CHECK_MPN(X) \
assert( (X).byte != 0 ); \
assert( (X).maxi >  0 ); \
assert( (X).size <= (X).maxi );\
assert( ! ( ((X).size>0) && (X).byte[(X).size-1] <= 0 ) )

        class natural : public memory::ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // management functions
            //
            //__________________________________________________________________
            natural();
            natural(const word_t);
            natural(const natural &);
            
            inline void xch( natural &other) throw()
            {
                cswap(maxi,other.maxi);
                cswap(size,other.size);
                cswap(byte,other.byte);
            }

            inline natural & operator=(const natural &other)
            {
                natural tmp(other); xch(tmp); return *this;
            }

            inline natural & operator=(const word_t n) throw()
            {
                size = sizeof(word_t);
                *(word_t *)byte = swap_le(n);
                update();
                YOCTO_CHECK_MPN(*this);
                return *this;
            }

            virtual ~natural() throw();

            //! try to decrease size
            inline void update() throw()
            {
                while(size>0)
                {
                    const size_t imsb = size-1;
                    if(byte[imsb]>0) break;
                    size=imsb;
                }
            }

            //! size=maxi and update
            inline void    rescan() throw() { size=maxi; update(); }

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
            
        private:
            size_t   maxi; //!< maximum #bytes
            size_t   size; //!< current #bytes
            uint8_t *byte; //!< little endian

            static  uint8_t *   build(size_t &n);
            virtual const void *get_address() const throw() { return byte; }
        };

    }

    typedef mpl::natural mpn;
}

#endif
