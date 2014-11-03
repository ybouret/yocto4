#ifndef YOCTO_FAME_QUADMESH_INCLUDED
#define YOCTO_FAME_QUADMESH_INCLUDED 1

#include "yocto/fame/mesh.hpp"
#include "yocto/fame/mesh/quad-grid.hpp"

namespace yocto
{
    namespace fame
    {
        
        template <typename Layout, typename T>
        class QuadMesh : public QuadGrid<Layout>,  public Mesh<Layout::DIMENSIONS,T>
        {
        public:
            typedef Mesh<Layout::DIMENSIONS,T> MESH;
            static const size_t DIMENSIONS = Layout::DIMENSIONS;
            typedef typename array_for<DIMENSIONS,T>::type Field;
            
            explicit QuadMesh(const string &id,
                              arrays       &adb);
            
                              
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(QuadMesh);
        };
        
    }
}

#endif
