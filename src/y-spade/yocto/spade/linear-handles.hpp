#ifndef YOCTO_SPADE_LINEAR_HANDLES_INCLUDED
#define YOCTO_SPADE_LINEAR_HANDLES_INCLUDED 1

#include "yocto/spade/linear.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace spade
    {
        typedef linear *linear_ptr;
        
        class linear_handles : public vector<linear_ptr>
        {
        public:
            explicit linear_handles() throw();
            explicit linear_handles(size_t);
            virtual ~linear_handles() throw();
            
            //! size of interleaved items
            size_t interleaved() const throw();
            
            void append( linear &handle );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(linear_handles);
        };
        
    }
}

#endif
