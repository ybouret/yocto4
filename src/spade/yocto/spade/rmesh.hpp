#ifndef YOCTO_SPADE_RMESH_INCLUDED
#define YOCTO_SPADE_RMESH_INCLUDED 1

#include "yocto/spade/quadmesh.hpp"
#include "yocto/spade/array1d.hpp"
#include "yocto/spade/region.hpp"

namespace yocto
{
    namespace spade
    {
        //! rectilinear mesh
        template <typename LAYOUT, typename T>
        class rmesh : public quadmesh, public LAYOUT
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef spade::array1D<T> axis_type;
            typedef layout1D          axis_layout;
            
            explicit rmesh( const LAYOUT &outline, array_db &adb_ref ) :
            quadmesh( adb_ref ),
            LAYOUT( outline   )
            {
                //-- create the axis with their name
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
                {
                    const unit_t      lower1 = __coord(outline.lower,dim);
                    const unit_t      upper1 = __coord(outline.upper,dim);
                    const axis_layout layout1(lower1,upper1);
                    const string      name( quadmesh::get_axis_name(dimension_t(dim)));
                    const type_spec   spec( typeid(axis_type) );
                    const type_spec   held( typeid(T) );
                    linear           *info = 0;
                    void             *addr = axis_type::ctor( layout1, &info );
                    try
                    {
                        const varray::ptr ap( new varray(name, spec, held, addr, info, axis_type::dtor));
                        addr = 0;
                        adb.insert(ap);
                    }
                    catch(...)
                    {
                        if( addr ) axis_type::dtor(addr);
                        throw;
                    }
                }
                
            }
            
            virtual ~rmesh() throw() {}
            
            inline axis_type & get_axis( dimension_t dim ) throw()
            {
                return adb[ quadmesh::get_axis_name(dim) ].template as<axis_type>();
            }
            
            inline const axis_type & get_axis( dimension_t dim ) const throw()
            {
                return adb[ quadmesh::get_axis_name(dim) ].template as<axis_type>();
            }

            inline axis_type       &X() throw()        { return get_axis( on_x ); }
            inline axis_type       &Y() throw()        { return get_axis( on_y ); }
            inline axis_type       &Z() throw()        { return get_axis( on_z ); }
            inline const axis_type & X() const throw() { return get_axis( on_x ); }
            inline const axis_type & Y() const throw() { return get_axis( on_y ); }
            inline const axis_type & Z() const throw() { return get_axis( on_z ); }

            //! extrapolate full_region/full_layout to this layout
            template <typename REGION>
            void regular_map_to( const REGION &full_region, const LAYOUT &full_layout)
            {
                assert( REGION::DIMENSIONS == LAYOUT::DIMENSIONS );
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim)
                {
                    axis_type   &A   = get_axis( dimension_t(dim) );
                    const_type   v0  = __component<typename REGION::type>(full_region.vmin,   dim);
                    const_type   dv  = __component<typename REGION::type>(full_region.length, dim);
                    const unit_t org = __coord(full_layout.lower,dim);
                    const unit_t den = __coord(full_layout.upper,dim) - org;
                    for( unit_t i = A.lower; i <= A.upper; ++i )
                    {
                        A[i] = v0 + ( (i-org) * dv ) / den;
                    }
                }
            }
            
            inline bool is_valid() const throw()
            {
                for( size_t dim=0; dim < LAYOUT::DIMENSIONS; ++dim)
                {
                    const axis_type &A = get_axis( dimension_t(dim) );
                    for(unit_t i=A.lower;i<A.upper;++i)
                    {
                        if( A[i] >= A[i+1]) return false;
                    }
                }
                return true;
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rmesh);
            
        };
    }
}


#endif
