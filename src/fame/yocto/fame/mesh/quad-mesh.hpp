#ifndef YOCTO_FAME_QUADMESH_INCLUDED
#define YOCTO_FAME_QUADMESH_INCLUDED 1

#include "yocto/fame/mesh.hpp"
#include "yocto/fame/mesh/quad-grid.hpp"

namespace yocto
{
    namespace fame
    {
        
#define Y_FAME_QUAD_MESH_ARGS \
const Layout &full,           \
const bool * pbc  = NULL,     \
const int    rank = 0,        \
const int    size = 1,        \
const int    ng   = 0
        
#define Y_FAME_QUAD_MESH_INIT full,pbc,rank,size,ng
        template <typename Layout, typename T>
        class QuadMesh : public QuadGrid<Layout>,  public Mesh<Layout::DIMENSIONS,T>
        {
        public:
            static const size_t        DIMENSIONS = Layout::DIMENSIONS;
            typedef QuadGrid<Layout>   QUADGRID;
            typedef Mesh<DIMENSIONS,T> MESH;
            
            typedef typename array_for<DIMENSIONS,T>::type     ScalarField;
            typedef typename vertex_for<DIMENSIONS,T>::type    vtx;
            typedef typename array_for<DIMENSIONS,vtx>::type   VectorField;
            
            explicit QuadMesh(const string &id,
                              Y_FAME_QUAD_MESH_ARGS
                              ) :
            QUADGRID(Y_FAME_QUAD_MESH_INIT),
            MESH(id)
            {
                // then rect/curv will generates Axis...
                //std::cerr << "\tfull=" << full << std::endl;
                //std::cerr << "\tthis=" << this->__layout() << std::endl;
                //std::cerr << "\toutl=" << this->outline    << std::endl;
            }
            
            virtual ~QuadMesh() throw()
            {
            }
            
            
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(QuadMesh);
        };
        
    }
}

#endif
