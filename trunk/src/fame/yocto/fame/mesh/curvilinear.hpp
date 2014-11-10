#ifndef YOCTO_FAME_CURVILINEAR_MESH_INCLUDED
#define YOCTO_FAME_CURVILINEAR_MESH_INCLUDED 1

#include "yocto/fame/mesh/quad-mesh.hpp"

namespace yocto
{
    namespace fame
    {
        //! building a rectilinear mesh
        template <typename Layout,typename T>
        class CurvilinearMesh : public QuadMesh<Layout,T>
        {
        public:
            static const size_t DIMENSIONS = Layout::DIMENSIONS;
            typedef array_for<DIMENSIONS,T> Axis;
            
            explicit CurvilinearMesh(arrays       &adb,
                                     const string &id,
                                     Y_FAME_QUAD_MESH_ARGS
                                     ) :
            QuadMesh<Layout,T>(id,Y_FAME_QUAD_MESH_INIT),
            ap()
            {
                std::cerr << "Creating Axis..." << std::endl;
                //______________________________________________________________
                //
                // build axis
                //______________________________________________________________
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    const string   axis_name   = get_axis_name(dim);
                    ap[dim] = new Axis(axis_name,*this);
                    adb.store(ap[dim]);
                }
                
                //______________________________________________________________
                //
                // build vertices
                //______________________________________________________________
                std::cerr << "Building " << this->items << " Vertices" << std::endl;
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
            
            
            
            virtual ~CurvilinearMesh() throw()
            {
            }
            
            
        private:
            Axis *ap[3];
            YOCTO_DISABLE_COPY_AND_ASSIGN(CurvilinearMesh);
            
            inline void build( int2type<1> )
            {
                Axis &XX = X();
                for(unit_t i=this->outline.lower;i<=this->outline.upper;++i)
                {
                    this->vertices.append( XX[i] );
                }
            }
            
            inline void build( int2type<2> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                for(unit_t j=this->outline.lower.y;j<=this->outline.upper.y;++j)
                {
                    for(unit_t i=this->outline.lower.x;i<=this->outline.upper.x;++i)
                    {
                        this->vertices.append( XX[j][i], YY[j][i] );
                    }
                }
                
            }
            
            inline void build( int2type<3> )
            {
                Axis &XX = X();
                Axis &YY = Y();
                Axis &ZZ = Z();
                for(unit_t k=this->outline.lower.z;k<=this->outline.upper.z;++k)
                {
                    for(unit_t j=this->outline.lower.y;j<=this->outline.upper.y;++j)
                    {
                        for(unit_t i=this->outline.lower.x;i<=this->outline.upper.x;++i)
                        {
                            this->vertices.append( XX[k][j][i], YY[k][j][i], ZZ[k][j][i] );
                        }
                    }
                }
            }
            
            
        };
        
    }
}

#endif
