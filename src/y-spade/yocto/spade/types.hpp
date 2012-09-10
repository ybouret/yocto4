#ifndef YOCTO_SPADE_TYPES_INCLUDED
#define YOCTO_SPADE_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    
    namespace spade
    {
        //! integer logicial coordinate
        typedef ptrdiff_t unit_t;
     
        //! enumerated dimension
        enum dimension_t
        {
            on_x=0,
            on_y=1,
            on_z=2
        };
        
        //! extract coord from compound COORD
        template <typename COORD>
        inline unit_t & __coord( COORD &C, size_t dim ) throw()
        {
            assert(dim<sizeof(COORD)/sizeof(unit_t));
            return *(((unit_t *)&C)+dim);
        }
        
        //! extract coord from compound COORD (const)
        template <typename COORD>
        inline const unit_t & __coord( const COORD &C, size_t dim ) throw()
        {
            assert(dim<sizeof(COORD)/sizeof(unit_t));
            return *(((unit_t *)&C)+dim);
        }
        
        //! base class for list of offsets
        typedef sorted_vector<size_t>  sorted_offsets;
        
        
        //! sorted list of offsets
        class offsets_list : public sorted_offsets
        {
        public:
            explicit offsets_list() throw();
            explicit offsets_list(size_t num);
            virtual ~offsets_list() throw();
            offsets_list( const offsets_list & );
            
            //! permissive storage
            void store( size_t indx );
            
        private:
            YOCTO_DISABLE_ASSIGN(offsets_list);
        };

    }
    
}

#endif
