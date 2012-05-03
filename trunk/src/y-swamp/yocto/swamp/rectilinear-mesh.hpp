#ifndef YOCTO_SWAMP_RECTILINEAR_MESH_INCLUDED
#define YOCTO_SWAMP_RECTILINEAR_MESH_INCLUDED 1

#include "yocto/swamp/mesh.hpp"
#include "yocto/swamp/regions.hpp"
#include "yocto/swamp/array1d.hpp"

namespace yocto 
{
    namespace swamp 
    {
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public quadmesh, public LAYOUT
        {
        public:
            typedef array1D<T> axis_type;   //!< one axis
            typedef layout1D   axis_layout; //!< axis layout
            const   type_spec  axis_spec;   //!< for array_db
            
            //! associated region type
            typedef typename __region<T,LAYOUT::DIMENSIONS>::type   region_type;
            typedef typename __region<T,LAYOUT::DIMENSIONS>::vertex vertex;
            
            explicit rectilinear_mesh( array_db &db, const LAYOUT &L ) :
            quadmesh( db ),
            LAYOUT( L ),
            axis_spec( typeid(axis_type) )
            {
                const unit_t *pLower = (const unit_t *) & (this->lower);
                const unit_t *pUpper = (const unit_t *) & (this->upper);
                for( size_t iAxis = 0; iAxis < LAYOUT::DIMENSIONS; ++iAxis )
                {
                    const unit_t      lo   = pLower[iAxis];
                    const unit_t      up   = pUpper[iAxis]; assert(up>=lo);
                    {
                        const axis_layout alay( lo, up );                       //!< create the axis layout
                        const string      name( quadmesh::axis_name(iAxis) );       //!< find its name
                        linear_base *     info = NULL;                          //!< prepare memory info
                        void *            addr = axis_type::ctor( alay, &info); //!< create the axis
                        adb(name, axis_spec, addr, info, axis_type::dtor);      //!< insert it in the database
                    }
                    if( up > lo )
                    {
                        const axis_layout alay( lo, up-1 );                     //!< create the delta axis layout
                        const string      name( quadmesh::delta_name(iAxis) );      //!< find its name
                        linear_base *     info = NULL;                          //!< prepare memory info
                        void *            addr = axis_type::ctor( alay, &info); //!< create the axis
                        adb(name, axis_spec, addr, info, axis_type::dtor);      //!< insert it in the database
                    }
                }
            }
            
            virtual ~rectilinear_mesh() throw() {}
            
           
            
            //! get main axis
            inline axis_type & operator[]( size_t iAxis )
            {
                return adb[ quadmesh::axis_name(iAxis) ].template as<axis_type>();
            }
            
            //! get main const axis
            inline const axis_type & operator[]( size_t iAxis ) const
            {
                return adb[ quadmesh::axis_name(iAxis) ].template as<axis_type>();
            }
            
            //! get delta axis
            inline axis_type &delta( size_t iAxis ) 
            {
                return adb[ quadmesh::delta_name(iAxis) ].template as<axis_type>();
            }
            
            //! get delta const axis
            inline const axis_type & delta( size_t iAxis ) const
            {
                return adb[ quadmesh::delta_name(iAxis) ].template as<axis_type>();
            }
            
            
            inline axis_type       &X()       { return adb[ quadmesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return adb[ quadmesh::axis_name(0) ].template as<axis_type>(); }
            
            inline axis_type       &Y()       { return adb[ quadmesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return adb[ quadmesh::axis_name(1) ].template as<axis_type>(); }
            
            
            inline axis_type       &Z()       { return adb[ quadmesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return adb[ quadmesh::axis_name(2) ].template as<axis_type>(); }
            
            inline axis_type       &dX()       { return adb[ quadmesh::delta_name(0) ].template as<axis_type>(); }
            inline const axis_type &dX() const { return adb[ quadmesh::delta_name(0) ].template as<axis_type>(); }
            
            inline axis_type       &dY()       { return adb[ quadmesh::delta_name(1) ].template as<axis_type>(); }
            inline const axis_type &dY() const { return adb[ quadmesh::delta_name(1) ].template as<axis_type>(); }
            
            
            inline axis_type       &dZ()       { return adb[ quadmesh::delta_name(2) ].template as<axis_type>(); }
            inline const axis_type &dZ() const { return adb[ quadmesh::delta_name(2) ].template as<axis_type>(); }
            

            void compute_deltas()
            {
                for( size_t iAxis = 0; iAxis < LAYOUT::DIMENSIONS; ++iAxis )
                {
                    const axis_type &A = (*this)[ iAxis ];
                    if( A.lower < A.upper )
                    {
                        axis_type &D = delta( iAxis );
                        assert( A.lower == D.lower ); assert( A.upper-1 == D.upper );
                        for( size_t i=A.lower; i < A.upper; ++i )
                        {
                            D[i] = A[i+1] - A[i];
                        }
                    }
                }
            }
            
            inline void regular_map_to( const region_type &r, const LAYOUT &l ) 
            {
                //--------------------------------------------------------------
                //-- expand region to this layout
                //--------------------------------------------------------------
                const region_type self_r = r.template map_to<LAYOUT>( *this, l );
                const T *vmin = (const T*) & self_r.vmin;
                const T *vlen = (const T*) & self_r.length;
                
                //--------------------------------------------------------------
                //-- map each axis
                //--------------------------------------------------------------
                for( size_t iAxis = 0; iAxis < LAYOUT::DIMENSIONS; ++iAxis )
                {
                    axis_type &A = (*this)[iAxis];
                    if( A.lower < A.upper )
                    {
                        const T      amin = vmin[iAxis];
                        const T      alen = vlen[iAxis];
                        const unit_t del  = A.upper - A.lower;
                        for( unit_t i= A.lower; i <= A.upper; ++i )
                        {
                            A[i] = amin + ( alen * (i-A.lower) ) / del;
                        }
                        
                    }
                    else
                    {
                        A[A.lower] = vmin[iAxis]; // set by map_to as the average.
                    }
                    
                }
                
                compute_deltas();
            }
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
        };
    }
}

#endif
