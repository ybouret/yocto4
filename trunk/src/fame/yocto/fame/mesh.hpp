#ifndef YOCTO_FAME_MESH_INCLUDED
#define YOCTO_FAME_MESH_INCLUDED 1

#include "yocto/fame/vertices.hpp"
#include "yocto/fame/arrays.hpp"


namespace yocto
{
    
    namespace fame
    {
        template <size_t DIM,typename T>
        class Mesh
        {
        public:
            static const size_t DIMENSIONS=DIM;
            
            typedef Vertex<DIM,T>      VTX;
            typedef typename VTX::List Vertices;
            typedef array1D<T>         Axis;
            
            Vertices vertices;
            
            inline   Mesh( arrays &adb ) throw() : vertices() {}
            virtual ~Mesh() throw() {}
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Mesh);
        };
    }
    
}

#endif
