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
        class rectilinear_mesh : public mesh, public LAYOUT
        {
        public:
            typedef array1D<T> axis_type;
            typedef layout1D   axis_layout;
            const   type_spec  axis_spec;
            
            explicit rectilinear_mesh( array_db &db, const LAYOUT &L ) :
            mesh( db ),
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
                        const string      name( mesh::axis_name(iAxis) );       //!< find its name
                        linear_base *     info = NULL;                          //!< prepare memory info
                        void *            addr = axis_type::ctor( alay, &info); //!< create the axis
                        adb(name, axis_spec, addr, info, axis_type::dtor);      //!< insert it in the database
                    }
                    /*
                     if( up > lo )
                     {
                     const axis_layout alay( lo, up-1 );                     //!< create the delta axis layout
                     const string      name( mesh::delta_name(iAxis) );      //!< find its name
                     linear_base *     info = NULL;                          //!< prepare memory info
                     void *            addr = axis_type::ctor( alay, &info); //!< create the axis
                     adb(name, axis_spec, addr, info, axis_type::dtor);      //!< insert it in the database
                     }
                     */
                }
            }
            
            virtual ~rectilinear_mesh() throw() {}
            
            
            inline axis_type & operator[]( size_t iAxis )
            {
                return adb[ mesh::axis_name(iAxis) ].template as<axis_type>();
            }
            
            inline const axis_type & operator[]( size_t iAxis ) const
            {
                return adb[ mesh::axis_name(iAxis) ].template as<axis_type>();
            }
            
            inline axis_type       &X()       { return adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            
            inline axis_type       &Y()       { return adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            
            
            inline axis_type       &Z()       { return adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
        };
    }
}

#endif
