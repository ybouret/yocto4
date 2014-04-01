#include "yocto/fovea/cell/line.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        const char * LineInfo:: Name() throw() { return "Line"; }
        
        void LineInfo:: CheckVertices( const VertexBase &a, const VertexBase &b )
        {
            if( &a==&b )
                throw exception("Line(Same Vertices)");
            if(a.index==b.index)
                throw exception("Unexpected Same Vertices INDEX");
        }
        
        void LineInfo:: NoEdgeFor( const VertexBase &a, const VertexBase &b )
        {
            throw exception("No Edge For Line(#%u,#%u)", unsigned(a.index), unsigned(b.index) );
        }
        
    }
}
