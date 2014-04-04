#ifndef YOCTO_FOVEA_POINT_MESH_INCLUDED
#define YOCTO_FOVEA_POINT_MESH_INCLUDED 1

#include "yocto/fovea/mesh.hpp"

namespace yocto
{
    
    namespace fovea
    {
        
        template <size_t DIM, typename T>
        class point_mesh : public Mesh<DIM,T>, public layout1D
        {
        public:
            typedef array1D<T>                    axis_type;
            typedef Mesh<DIM,T>                   MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            typedef Cell<DIM,T>                   CELL;
            typedef typename CELL::List           CELL_LIST;
           
            const layout1D & get_layout() const throw() { return *this; }
            CELL_LIST cells;

            virtual ~point_mesh() throw() {}
            
            explicit point_mesh(array_db       &a,
                                const layout1D &L) :
            MESH(a,L.items,mesh::is_point),
            layout1D(L)
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string    axis_n = this->axis_name(i);
                    this->adb.store( new axis_type(axis_n,*this) );
                }
                assign( int2type<DIM>() );
                this->assigned = true;
            }
            
            inline axis_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline axis_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline axis_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
            virtual size_t num_cells() const throw()
            {
                return cells.size;
            }

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(point_mesh);
            inline void assign( int2type<1> ) throw()
            {
                axis_type &aX = X();
                unit_t     i  = lower;
                for(size_t j=0;j<this->vertices;++j,++i)
                {
                    new (this->pvtx+j) VERTEX(j,aX[i]);
                }
            
            }
            
            inline void assign( int2type<2> ) throw()
            {
                axis_type &aX = X();
                axis_type &aY = Y();
                unit_t     i  = lower;
                for(size_t j=0;j<this->vertices;++j,++i)
                {
                    new (this->pvtx+j) VERTEX(j,aX[i],aY[i]);
                }

            }
            
            inline void assign( int2type<3> ) throw()
            {
                axis_type &aX = X();
                axis_type &aY = Y();
                axis_type &aZ = Z();
                unit_t     i  = lower;
                for(size_t j=0;j<this->vertices;++j,++i)
                {
                    new (this->pvtx+j) VERTEX(j,aX[i],aY[i],aZ[i]);
                }
            }
            
        };
    }
}
#endif
