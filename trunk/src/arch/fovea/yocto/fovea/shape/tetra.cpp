#include "yocto/fovea/shape/tetra.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        const char * TetraInfo:: Name() throw() { return "Triangle"; }
        
        void TetraInfo:: CheckVertices( const VertexBase &a, const VertexBase &b, const VertexBase &c, const VertexBase &d )
        {
            if(&a==&b || &a==&c || &a==&d ||
               &b==&c || &b==&d ||
               &c == &d )
                throw exception("Tetra(Same Vertices)");
            
            if(a.index==b.index || a.index == c.index || a.index == d.index ||
               b.index==c.index || b.index == d.index ||
               c.index==d.index)
                throw exception("Unexpected Same Tetra Vertices INDEX");
        }
        
        void TetraInfo:: NoEdgeFor( const VertexBase &a, const VertexBase &b )
        {
            throw exception("No Edge For Tetra (#%u,#%u)", unsigned(a.index), unsigned(b.index) );
        }
        
        
        
        void TetraInfo:: MissingEdge( const edge_key &ek )
        {
            throw exception("No Tetra Edge #%u <-> #%u", unsigned(ek.i1), unsigned(ek.i2) );
        }
        
        void TetraInfo:: ZeroLength( const VertexBase &a, const VertexBase &b )
        {
            throw exception("Zero Length for Tetra Edge #%u <-> #%u", unsigned(a.index), unsigned(b.index) );
        }
        
    }
}