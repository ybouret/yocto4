#include "yocto/lang/types.hpp"

namespace yocto
{
    namespace lang
    {
        
        first_chars:: ~first_chars() throw()
        {
        }
        
        first_chars:: first_chars() : bytes_store(), accept_empty(false)
        {
        }
        
        first_chars:: first_chars( const first_chars &other ) :
        bytes_store(other),
        accept_empty(other.accept_empty)
        {
        }
        
        void first_chars:: add(code_type c)
        {
            (void)insert(c);
        }

        void first_chars:: add(const first_chars&fc)
        {
            for(size_t i=fc.size();i>0;--i)
            {
                add( fc[i] );
            }
        }
        
    }
}
