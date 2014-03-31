#include "yocto/fovea/cell.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace fovea
    {
        static inline
        size_t check_num_vertices( size_t nv )
        {
            if(nv<2)
                throw exception("Invalid Cell #Vertices");
            return nv;
        }
        
        static inline
        size_t check_num_edges( size_t ne )
        {
            if(ne<1)
                throw exception("Invalid Cell #Edges");
            return ne;
        }

        
        ICell:: ~ICell() throw()
        {
        }
        
        ICell:: ICell( size_t nv, size_t ne) :
        vertices( check_num_vertices(nv) ),
        edges( check_num_edges(ne) )
        {
            
        }
        
        void ICell:: invalid_facet() const
        {
            throw exception("Invalid Facet, must have 3 or 4 vertices!");
        }
        
        void ICell:: invalid_facet_edge() const
        {
            throw exception("Can't find an edge for the facet!!!");
        }

    }
}
