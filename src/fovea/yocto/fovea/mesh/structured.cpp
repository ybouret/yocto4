#include "yocto/fovea/mesh/structured.hpp"

namespace yocto
{
    namespace fovea
    {
        const quad_edge_index quad_edge_indices[quad_edge_index::count] =
        {
            {0,2}, {2,6}, {6,4}, {4,0}, // face (x4)
            {1,3}, {3,7}, {7,5}, {5,1}, // face (x4)
            {0,1}, {4,5}, {2,3}, {6,7}, // side (x4)
            {0,6}, {1,7},  // on xy faces
            {0,5}, {2,7},  // on xz faces
            {0,3}, {4,7},  // on yz faces
            {0,7}          // big diag
        };
        
        const tetra_edge_index tetra_edge_indices[ tetra_edge_index::count ] =
        {
            { 0,7,4,6 }
        };
    }
}
