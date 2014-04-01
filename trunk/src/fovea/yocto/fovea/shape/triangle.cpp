#include "yocto/fovea/shape/triangle.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        const char * TriangleInfo:: Name() throw() { return "Triangle"; }
        
        void TriangleInfo:: CheckVertices( const VertexBase &a, const VertexBase &b, const VertexBase &c )
        {
            if( &a==&b || &a==&c || &b==&c )
                throw exception("Triangle(Same Vertices)");
            
            if(a.index==b.index)
                throw exception("Unexpected Same Triangle Vertices INDEX");
        }
        
        void TriangleInfo:: NoEdgeFor( const VertexBase &a, const VertexBase &b )
        {
            throw exception("No Edge For Triangle(#%u,#%u)", unsigned(a.index), unsigned(b.index) );
        }

        void TriangleInfo:: NoTriangle1D()
        {
            throw exception("No Triangle 1D");
        }
    }
}
