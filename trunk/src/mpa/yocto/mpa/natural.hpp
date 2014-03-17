#ifndef YOCTO_MPA_NATURAL_INCLUDED
#define YOCTO_MPA_NATURAL_INCLUDED 1

#include "yocto/mpa/types.hpp"
#include "yocto/memory/buffer.hpp"

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
            size_t bits() const throw();
            
            
        private:
            size_t   maxi;
            size_t   size;
            uint8_t *byte;
            
            void update() throw(); //!< start from size and adjust predicted size
            void rescan() throw(); //!< start from maxi and adjust
            virtual const void *get_address() const throw(); //!< ro_buffer API
        };
        
    }
    
    typedef mpa::natural mpn;
    
}

#endif

