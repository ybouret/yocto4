#ifndef YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_CURVILINEAR_INCLUDED 1

#include "yocto/fovea/mesh/structured.hpp"


namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class curvilinear_mesh : public Mesh<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            typedef typename array_for<LAYOUT::DIMENSIONS,T>::type   array_type;
            typedef LAYOUT                        layout_type;
            typedef curvilinear_mesh<T,LAYOUT>    mesh_type;
            typedef Mesh<LAYOUT::DIMENSIONS,T>    MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            typedef Cell<LAYOUT::DIMENSIONS,T>    CELL;
            typedef typename CELL::List           CELL_LIST;
            typedef Box<LAYOUT::DIMENSIONS,T>     BOX;
            
            // public data
            const LAYOUT & get_layout() const throw() { return *this; }
            CELL_LIST      cells;
            
            
            inline explicit curvilinear_mesh( array_db &a, const LAYOUT &L ) :
            MESH(a,
                 L.items,
                 mesh::is_curvilinear),
            LAYOUT(L),
            cells()
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
            
            
            virtual size_t num_cells() const throw()
            {
                return cells.size;
            }
            
            const ShapeBase *get_first_cell() const throw()
            {
                return cells.head;
            }
            
            //__________________________________________________________________
            //
            // mapping+compile
            //__________________________________________________________________
            inline void map_to( const BOX &box )
            {
                __map_to(box);
                this->compile();
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(curvilinear_mesh);
            
            
            //==================================================================
            //
            // construction
            //
            //==================================================================
            inline void assign( int2type<1> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                array_type &aX = X();
                size_t v = 0;
                for(unit_t i=this->lower;i<=this->upper;++i,++v)
                {
                    new (this->pvtx+v) VERTEX(v,aX[i] );
                }
                this->assigned = true;
                
                //______________________________________________________________
                //
                // allocate edges+cells
                //______________________________________________________________
                structured<mesh_type>::assign(*this);
                
            }
            
            inline void assign( int2type<2> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                array_type &aX = X();
                array_type &aY = Y();
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->pvtx+v) VERTEX(v,aX[j][i],aY[j][i] );
                    }
                }
                this->assigned = true;
                
                //______________________________________________________________
                //
                // allocate edges+cells
                //______________________________________________________________
                structured<mesh_type>::assign(*this);
            }
            
            inline void assign( int2type<3> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
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
                            new (this->pvtx+v) VERTEX(v,aX[k][j][i], aY[k][j][i], aZ[k][j][i] );
                        }
                    }
                }
                this->assigned = true;
                
                //______________________________________________________________
                //
                // allocate edges+cells
                //______________________________________________________________
                structured<mesh_type>::assign(*this);
            }
            
            //==================================================================
            //
            // mapping
            //
            //==================================================================
            inline void __map_to( const Box<1,T> &box )
            {
                const T      xmin = box.vmin;
                const T      xmax = box.vmax;
                const T      xlen = box.length;
                array_type  &xx   = X();
                const unit_t imin = this->lower;
                const unit_t imax = this->upper;
                const unit_t ilen = imax - imin;
                xx[imin] = xmin;
                xx[imax] = xmax;
                for(unit_t i=imin+1;i<imax;++i)
                {
                    xx[i] = xmin + ((i-imin)*xlen)/ilen;
                }
            }
            
            inline void __map_to( const Box<2,T> &box )
            {
                const T      xmin = box.vmin.x;
                //const T      xmax = box.vmax.x;
                const T      xlen = box.length.x;
                const T      ymin = box.vmin.y;
                //const T      ymax = box.vmax.y;
                const T      ylen = box.length.y;
                array_type  &xx   = X();
                array_type  &yy   = Y();
                const unit_t imin = this->lower.x;
                const unit_t imax = this->upper.x;
                const unit_t ilen = imax - imin;
                
                const unit_t jmin = this->lower.y;
                const unit_t jmax = this->upper.y;
                const unit_t jlen = jmax - jmin;
                
                for(unit_t j=jmin;j<=jmax;++j)
                {
                    const T yj = ymin + ( (j-jmin) * ylen )/jlen;
                    for(unit_t i=imin;i<=imax;++i)
                    {
                        const T xi = xmin + ( (i-imin) * xlen )/ilen;
                        xx[j][i] = xi;
                        yy[j][i] = yj;
                    }
                }
                
            }
            
            
        };
        
    }
    
}

#endif

