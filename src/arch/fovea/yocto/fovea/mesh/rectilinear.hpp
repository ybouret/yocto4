#ifndef YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED
#define YOCTO_FOVEA_MESH_RECTILINEAR_INCLUDED 1

#include "yocto/fovea/mesh/structured.hpp"


namespace yocto
{
    
    namespace fovea
    {
        
        template <typename T,typename LAYOUT>
        class rectilinear_mesh : public Mesh<LAYOUT::DIMENSIONS,T>, public LAYOUT
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef LAYOUT                        layout_type;
            typedef rectilinear_mesh<T,LAYOUT>    mesh_type;
            typedef array1D<T>                    axis_type;
            typedef Mesh<LAYOUT::DIMENSIONS,T>    MESH;
            typedef typename MESH::VERTEX         VERTEX;
            typedef typename MESH::EDGE           EDGE;
            typedef Cell<LAYOUT::DIMENSIONS,T>    CELL;
            typedef typename CELL::List           CELL_LIST;
            typedef Box<LAYOUT::DIMENSIONS,T>     BOX;
            
            // public data
            const LAYOUT & get_layout() const throw() { return *this; }
            CELL_LIST cells;
            
            //__________________________________________________________________
            //
            // initialization
            //__________________________________________________________________
            inline explicit rectilinear_mesh(array_db     &a,
                                             const LAYOUT &L ) :
            MESH(a,L.items,mesh::is_rectilinear),
            LAYOUT(L),
            cells()
            {
                for(size_t i=0;i<this->dims;++i)
                {
                    const string    axis_n = this->axis_name(i);
                    const layout1D  axis_l( __coord(this->lower,i), __coord(this->upper,i));
                    this->adb.store( new axis_type(axis_n,axis_l) );
                }
                assign( int2type<LAYOUT::DIMENSIONS>() );
            }
            
            inline virtual ~rectilinear_mesh() throw() {}
            
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
            // get axis
            //__________________________________________________________________
            inline axis_type       &X()       { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline const axis_type &X() const { return this->adb[ mesh::axis_name(0) ].template as<axis_type>(); }
            inline axis_type       &Y()       { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline const axis_type &Y() const { return this->adb[ mesh::axis_name(1) ].template as<axis_type>(); }
            inline axis_type       &Z()       { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            inline const axis_type &Z() const { return this->adb[ mesh::axis_name(2) ].template as<axis_type>(); }
            
            inline axis_type & get_axis(size_t dim)
            {return this->adb[ mesh::axis_name(dim) ].template as<axis_type>();}
            
            inline const axis_type & get_axis(size_t dim) const
            {return this->adb[ mesh::axis_name(dim) ].template as<axis_type>();}

            //__________________________________________________________________
            //
            // mapping+compile
            //__________________________________________________________________
            inline void map_to( const BOX &box )
            {
                const T *pmin = (T*)&(box.vmin);
                const T *pmax = (T*)&(box.vmax);
                const T *plen = (T*)&(box.length);

                for(size_t d=0;d<MESH::DIMS;++d)
                {
                    axis_type &A = get_axis(d);
                    const T qmin = pmin[d];
                    const T qmax = pmax[d];
                    A[ A.lower ] = qmin;
                    A[ A.upper ] = qmax;
                    const T      qlen = plen[d];
                    const size_t na   = A.upper - A.lower;
                    for(unit_t i=A.lower+1;i<A.upper;++i)
                    {
                        A[ i ] = qmin + ( (i-A.lower) * qlen ) / na;
                    }
                }
                this->compile();
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rectilinear_mesh);
            inline void assign( int2type<1> )
            {
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                axis_type &aX = X();
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
                axis_type &aX = X();
                axis_type &aY = Y();
                
                //______________________________________________________________
                //
                // allocate vertices
                //______________________________________________________________
                size_t v = 0;
                for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                {
                    T &y = aY[j];
                    for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                    {
                        new (this->pvtx+v) VERTEX(v,aX[i], y );
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
                axis_type &aX = X();
                axis_type &aY = Y();
                axis_type &aZ = Z();
                size_t v = 0;
                for(unit_t k=this->lower.z;k<=this->upper.z;++k)
                {
                    T &z = aZ[k];
                    for(unit_t j=this->lower.y;j<=this->upper.y;++j)
                    {
                        T &y = aY[j];
                        for(unit_t i=this->lower.x;i<=this->upper.x;++i,++v)
                        {
                            new (this->pvtx+v) VERTEX(v,aX[i], y,z );
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
            
        };
        
        
    }
    
}

#endif

