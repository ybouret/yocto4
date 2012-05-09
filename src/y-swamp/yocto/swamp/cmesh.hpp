#ifndef YOCTO_SWAMP_CURVILINEAR_MESH_INCLUDED
#define YOCTO_SWAMP_CURVILINEAR_MESH_INCLUDED 1

#include "yocto/swamp/mesh.hpp"
#include "yocto/swamp/regions.hpp"
#include "yocto/swamp/array3d.hpp"

namespace yocto 
{
    namespace swamp
    {
        //! curvilinear mesgh
        template <typename T,typename LAYOUT>
        class cmesh : public quadmesh, public LAYOUT
        {
        public:
            typedef typename __array<T,LAYOUT::DIMENSIONS>::type  array_type;
            typedef typename __region<T,LAYOUT::DIMENSIONS>::type region_type;
            
            const type_spec data_spec;
            
            explicit cmesh( array_db &db, const LAYOUT &L) :
            quadmesh( db ),
            LAYOUT(L)
            {
                const type_spec spec( typeid(array_type) );
                const type_spec held( typeid(T)          );
                
                for( size_t iAxis = 0; iAxis < LAYOUT::DIMENSIONS; ++iAxis )
                {
                    const string name( quadmesh::axis_name( iAxis ) );
                    linear_base *info = NULL;
                    void        *addr = array_type::ctor( L, &info );
                    adb(name, spec, held, addr, info, array_type::dtor);                    
                }
            }
            
            virtual ~cmesh() throw() {}
            
            array_type & operator[]( size_t iAxis ) 
            {
                return adb[ quadmesh::axis_name(iAxis) ].template as<array_type>();
            }
            
            const array_type & operator[]( size_t iAxis ) const
            {
                return adb[ quadmesh::axis_name(iAxis) ].template as<array_type>();
            }
            
            inline array_type       &X()       { return adb[ quadmesh::axis_name(0) ].template as<array_type>(); }
            inline const array_type &X() const { return adb[ quadmesh::axis_name(0) ].template as<array_type>(); }
            
            inline array_type       &Y()       { return adb[ quadmesh::axis_name(1) ].template as<array_type>(); }
            inline const array_type &Y() const { return adb[ quadmesh::axis_name(1) ].template as<array_type>(); }
            
            inline array_type       &Z()       { return adb[ quadmesh::axis_name(2) ].template as<array_type>(); }
            inline const array_type &Z() const { return adb[ quadmesh::axis_name(2) ].template as<array_type>(); }
            
            inline void regular_map_to( const region_type &r, const LAYOUT &l ) 
            {
                /*
                 //--------------------------------------------------------------
                 //-- expand region to this layout
                 //--------------------------------------------------------------
                 const region_type self_r = r.template map_to<LAYOUT>( *this, l );
                 const T *vmin = (const T*) & self_r.vmin;
                 const T *vlen = (const T*) & self_r.length;
                 const unit_t *pLower = (const unit_t *) & this->lower;
                 const unit_t *pUpper = (const unit_t *) & this->upper;
                 //--------------------------------------------------------------
                 //-- map each array
                 //--------------------------------------------------------------
                 for( size_t iAxis = 0; iAxis < LAYOUT::DIMENSIONS; ++iAxis )
                 {
                 array_type &A = (*this)[ iAxis ];
                 const unit_t lo = pLower[iAxis];
                 const unit_t up = pUpper[iAxis];
                 if( lo < up )
                 {
                 
                 }
                 }
                 */
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(cmesh);
            
        };
    }
}


#endif
