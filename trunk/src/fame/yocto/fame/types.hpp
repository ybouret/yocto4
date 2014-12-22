#ifndef YOCTO_FAME_TYPES_INCLUDED
#define YOCTO_FAME_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/math/v3d.hpp"

namespace yocto
{
    //! Fields And Meshes Explorer
    namespace fame
    {
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef memory::global         memory_kind;
        typedef memory_kind::allocator memory_allocator;
        
        const char *get_axis_name(size_t dim) throw(); //!< dim in [0..2]
        
        
        //______________________________________________________________________
        //
        // vertex for dimension
        //______________________________________________________________________
        template <size_t,typename>
        struct vertex_for; //! root template
        
        template <typename T> struct vertex_for<1,T> { typedef T            type; typedef T           args; };
        template <typename T> struct vertex_for<2,T> { typedef math::v2d<T> type; typedef const type &args; };
        template <typename T> struct vertex_for<3,T> { typedef math::v3d<T> type; typedef const type &args; };
        
        
        //______________________________________________________________________
        //
        // logical coordinates
        //______________________________________________________________________
        typedef unit_t            coord1D;
        typedef math::v2d<unit_t> coord2D;
        typedef math::v3d<unit_t> coord3D;
        
        template <size_t>
        struct coord_for;
        template <> struct coord_for<1> { typedef coord1D type; };
        template <> struct coord_for<2> { typedef coord2D type; };
        template <> struct coord_for<3> { typedef coord3D type; };
        
        
        
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

