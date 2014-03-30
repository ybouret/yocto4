#ifndef YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED 1

#include "yocto/fovea/mesh.hpp"
#include "yocto/fovea/array3d.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class curvilinear_mesh : public Mesh<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            typedef typename array_for<LAYOUT::DIMENSIONS,T>::type   array_type;
            typedef Mesh<LAYOUT::DIMENSIONS,T>    MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            
            const LAYOUT & get_layout() const throw() { return *this; }
            
            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            MESH(a,
                 L.items,
                 mesh::is_curvilinear),
            LAYOUT(L)
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string array_n = this->axis_name(i);
                    this->adb.store( new array_type(array_n, *this) );
                }
                assign( int2type<LAYOUT::DIMENSIONS>() );
                this->assigned = true;
            }
            
            inline virtual ~curvilinear_mesh() throw()
            {
            }
            
            inline array_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline const array_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<array_type>(); }
            inline array_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline const array_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<array_type>(); }
            inline array_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<array_type>(); }
            inline const array_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<array_type>(); }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(curvilinear_mesh);
            
            inline void assign( int2type<1> ) throw()
            {
                array_type &aX = X();
                size_t v = 0;
                for(unit_t i=this->lower;i<=this->upper;++i,++v)
                {
                    new (this->vtx+v) VERTEX(v,aX[i] );
                }
                
            }
            
            inline void assign( int2type<2> ) throw()
            {
                array_type &aX = X();
                array_type &aY = Y();
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->vtx+v) VERTEX(v,aX[j][i],aY[j][i] );
                    }
                }
            }
            
            inline void assign( int2type<3> ) throw()
            {
                array_type &aX = X();
                array_type &aY = Y();
                array_type &aZ = Z();
                size_t v = 0;
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                        {
                            new (this->vtx+v) VERTEX(v,aX[k][j][i], aY[k][j][i], aZ[k][j][i] );
                        }
                    }
                }
                
            }
        };
        
    }
    
}

#endif

