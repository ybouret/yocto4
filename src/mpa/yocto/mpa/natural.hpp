#ifndef YOCTO_MPA_NATURAL_INCLUDED
#define YOCTO_MPA_NATURAL_INCLUDED 1

#include "yocto/mpa/types.hpp"
#include "yocto/memory/buffer.hpp"
#include <iosfwd>

namespace yocto
{
    namespace mpa
    {
        
        class natural : public memory::ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            // class operations
            //__________________________________________________________________
            natural();
            natural( const size_t n, const as_capacity_t &);
            virtual ~natural() throw();
            natural(const natural &);
            natural & operator=( const natural &);
            virtual size_t length() const throw(); //!< ro_buffer API
            
            //__________________________________________________________________
            //
            // basic API
            //__________________________________________________________________
            void   xch( natural &other ) throw();
            void   ldz() throw();   //!< size to 0
            void   clear() throw(); //!< memory clear
            size_t bits() const throw();
            
            
#define YOCTO_MPA_TO ans <<= 8; ans |= get_byte(i-1)
			template <typename T>
			inline T to() const throw()
			{
				T ans(0);
                for( size_t i=sizeof(T);i>0;--i)
				{
                    YOCTO_MPA_TO;
                }
                return ans;
            }
            
            
            //__________________________________________________________________
            //
            // setting values
            //__________________________________________________________________
            natural(uint64_t x);
            natural(const memory::ro_buffer &buf);
            
            //__________________________________________________________________
            //
            // output
            //__________________________________________________________________
            friend std::ostream & operator<<( std::ostream &, const natural &);
            
            //__________________________________________________________________
            //
            // comparison
            //__________________________________________________________________
            static int compare( const natural &, const natural &) throw();
            
            
            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            static natural add( const natural &lhs, const natural &rhs );
            friend natural operator+( const natural &lhs, const natural &rhs );
            natural & operator+=( const natural &rhs );
            void      inc(uint8_t);
            natural & operator++();     //!< prefix
            natural   operator++ (int); //!< postfix
            
            
        private:
            size_t   maxi;
            size_t   size;
            uint8_t *byte;
            
            void update() throw(); //!< start from size and adjust predicted size
            void rescan() throw(); //!< start from maxi and adjust
            virtual const void *get_address() const throw(); //!< ro_buffer API
        };
#define YOCTO_CHECK_MPN(X)    \
assert((X)->byte);            \
assert((X)->maxi>0);          \
assert((X)->size<=(X)->maxi); \
assert( ! ( (X)->size > 0 && (X)->byte[ (X)->size-1 ] == 0 ) )
        
    }
    
    typedef mpa::natural mpn;
    
}

#endif

