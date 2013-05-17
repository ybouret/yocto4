#ifndef YOCTO_LINGUA_FIRST_CHARS_INCLUDED
#define YOCTO_LINGUA_FIRST_CHARS_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        typedef sorted_vector<uint8_t> byte_store;
        
        //! list of the first possible accepted chars
        class first_chars : public byte_store
        {
        public:
            explicit first_chars() throw();
            virtual ~first_chars() throw();
            
            bool accept_empty; //!< default true
            
            first_chars( const first_chars & );
            
            void include( const first_chars & other );
            void exclude( const first_chars &other) throw();
            
            friend std::ostream & operator<<( std::ostream &, const first_chars &);
            
        private:
            YOCTO_DISABLE_ASSIGN(first_chars);
        };
        
        
        
    }
    
}

#endif
