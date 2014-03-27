#ifndef YOCTO_FOVEA_TYPES_INCLUDED
#define YOCTO_FOVEA_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace fovea
    {
        typedef ptrdiff_t              unit_t;
        typedef memory::global         memory_kind;
        typedef memory_kind::allocator memory_allocator;
        
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
        
        //! compare coordinates (integral types)
        template <typename COORD>
        inline bool are_same_coord( const COORD &A, const COORD &B ) throw()
        {
            const unit_t * a = & __coord(A,0);
            const unit_t * b = & __coord(B,0);
            for(size_t i=0;i<sizeof(COORD)/sizeof(unit_t);++i)
            {
                if(a[i]!=b[i])
                    return false;
            }
            return true;
        }
        
        typedef sorted_vector<
        unit_t,
        comparator<unit_t>,
        memory_allocator>  sorted_offsets;
        
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

