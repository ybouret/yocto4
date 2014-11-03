#ifndef YOCTO_FAME_RECTILINEAR_MESH_INCLUDED
#define YOCTO_FAME_RECTILINEAR_MESH_INCLUDED 1

#include "yocto/fame/mesh/quad-mesh.hpp"

namespace yocto
{
    namespace fame
    {
        
        //! building a rectilinear mesh
        template <typename Layout,typename T>
        class RectilinearMesh : public QuadMesh<Layout,T>
        {
        public:
            static const size_t DIMENSIONS = Layout::DIMENSIONS;
            typedef array1D<T>  Axis;
            
            explicit RectilinearMesh(arrays       &adb,
                                     const string &id,
                                     Y_FAME_QUAD_MESH_ARGS
                                     ) :
            QuadMesh<Layout,T>(id,Y_FAME_QUAD_MESH_INIT),
            ap()
            {
                //______________________________________________________________
                //
                // build axis
                //______________________________________________________________
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    const string   axis_name   = get_axis_name(dim);
                    const layout1D axis_layout( __coord(this->outline.lower,dim), __coord(this->outline.upper,dim) );
                    ap[dim] = new Axis(axis_name,axis_layout);
                    adb.store(ap[dim]);
                }
                
                //______________________________________________________________
                //
                // build vertices
                //______________________________________________________________
                build( int2type<DIMENSIONS>() );
            }
            
            inline const Axis &X() const throw() { assert(DIMENSIONS>=1); return *ap[0]; }
            inline Axis       &X() throw()       { assert(DIMENSIONS>=1); return *ap[0]; }
            
            inline const Axis &Y() const throw() { assert(DIMENSIONS>=2); return *ap[1]; }
            inline Axis       &Y() throw()       { assert(DIMENSIONS>=2); return *ap[1]; }
            
            inline const Axis &Z() const throw() { assert(DIMENSIONS>=3); return *ap[2]; }
            inline Axis       &Z() throw()       { assert(DIMENSIONS>=3); return *ap[2]; }
            
            const Axis &get_axis(size_t dim) const throw()
            {
                assert(dim<DIMENSIONS);
                return *ap[dim];
            }
            
            
            
            virtual ~RectilinearMesh() throw()
            {
                
            }
            
        private:
            Axis *ap[DIMENSIONS];
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
            
            inline void build( int2type<1> )
            {
                Axis &XX = X();
                for(unit_t i=XX.lower;i<=XX.upper;++i)
                {
                    this->vertices.append( XX[i] );
                }
            }
            
            inline void build( int2type<2> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                for(unit_t j=YY.lower;j<=YY.upper;++j)
                {
                    for(unit_t i=XX.lower;i<=XX.upper;++i)
                    {
                        this->vertices.append( XX[i],YY[j] );
                    }
                }
                
            }
            
            inline void build( int2type<3> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                Axis &ZZ = Z();
                
                for(unit_t k=ZZ.lower;k<=ZZ.upper;++k)
                {
                    for(unit_t j=YY.lower;j<=YY.upper;++j)
                    {
                        for(unit_t i=XX.lower;i<=XX.upper;++i)
                        {
                            this->vertices.append( XX[i],YY[j],ZZ[k] );
                        }
                    }
                }
            }
            
        };
        
        
        template <size_t DIM,typename T>
        class OldRectilinearMesh : public Mesh<DIM,T>
        {
        public:
            typedef array1D<T> Axis;
            
            explicit OldRectilinearMesh( arrays &adb, const string &id) :
            Mesh<DIM, T>(id),
            ap()
            {
                initialize_vertices( adb );
            }
            
            
            virtual ~OldRectilinearMesh() throw()
            {
            }
            
            inline const Axis &X() const throw() { assert(DIM>=1); return *ap[0]; }
            inline Axis       &X() throw()       { assert(DIM>=1); return *ap[0]; }
            
            inline const Axis &Y() const throw() { assert(DIM>=2); return *ap[1]; }
            inline Axis       &Y() throw()       { assert(DIM>=2); return *ap[1]; }
            
            inline const Axis &Z() const throw() { assert(DIM>=3); return *ap[2]; }
            inline Axis       &Z() throw()       { assert(DIM>=3); return *ap[2]; }
            
            const Axis &get_axis(size_t dim) const throw()
            {
                assert(dim<DIM);
                return *ap[dim];
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(OldRectilinearMesh);
            Axis *ap[DIM];
            
            inline void initialize_vertices( arrays &adb )
            {
                for(size_t i=0;i<DIM;++i)
                {
                    ap[i] = & adb[ get_axis_name(i) ].as<Axis>();
                }
                build( int2type<DIM>() );
            }
            
            inline void build( int2type<1> )
            {
                Axis &XX = X();
                for(unit_t i=XX.lower;i<=XX.upper;++i)
                {
                    this->vertices.append( XX[i] );
                }
                
                
            }
            
            inline void build( int2type<2> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                for(unit_t i=XX.lower;i<=XX.upper;++i)
                {
                    T &xx = XX[i];
                    for(unit_t j=YY.lower;j<=YY.upper;++j)
                    {
                        this->vertices.append( xx,YY[j] );
                    }
                }
            }
            
            inline void build( int2type<3> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                Axis &ZZ = Z();
                
                for(unit_t i=XX.lower;i<=XX.upper;++i)
                {
                    T &xx = XX[i];
                    for(unit_t j=YY.lower;j<=YY.upper;++j)
                    {
                        T &yy = YY[j];
                        for(unit_t k=ZZ.lower;k<=ZZ.upper;++k)
                        {
                            this->vertices.append( xx,yy, ZZ[k] );
                        }
                    }
                }
            }
            
        };
    }
}

#endif

