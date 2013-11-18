#ifndef YOCTO_KR_HASHING_RMD_INCLUDED
#define YOCTO_KR_HASHING_RMD_INCLUDED 1


#include "yocto/os.hpp"

namespace yocto
{
    
    namespace hashing
    {
     
        //! input class for rmd
        class rmd
        {
        public:
            rmd() throw();
            ~rmd() throw();
            
          
            //! reset all fields
            void reset() throw();
            
            //! store in bytes, then in words
            /**
             return true when 16 * 4 bytes are ready
             */
            bool store( uint8_t b ) throw();
            
            //! return a full block
            const uint32_t *block() const throw();
            
            //! assemble the last block
            const uint8_t *flush() throw();
            
            const size_t   length;
            
            uint32_t lswlen() const throw();
            uint32_t mswlen() const throw();
            
        private:
            size_t   nx;
            size_t   nb;
            uint32_t X[16];  //!< words store
            uint8_t  B[4];   //!< bytes store
            YOCTO_DISABLE_COPY_AND_ASSIGN(rmd);
            
        };
        
    }
    
}

#endif
