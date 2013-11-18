#ifndef YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED
#define YOCTO_SPADE_CONTOUR_LEVELS_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        //! float or double levels
        template <typename T>
        class levels : public sorted_vector<T,comparator<T> >
        {
        public:
            typedef sorted_vector<T,comparator<T> > ordered_type;
            explicit levels() throw();
            virtual ~levels() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(levels);
        };
        
        
        
    }
    
}


#endif
