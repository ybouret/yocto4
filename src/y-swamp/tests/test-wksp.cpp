#include "yocto/utest/run.hpp"

#include "yocto/swamp/workspace.hpp"
#include "yocto/swamp/rectilinear-mesh.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/code/rand.hpp"


using namespace yocto;
using namespace swamp;


YOCTO_UNIT_TEST_IMPL(wksp)
{
    {
        const layout1D              L(1,11);
        ghosts_setup<coord1D>       G;
        G.local.count = 2;
        workspace<layout1D,double,rectilinear_mesh> W(L,G);
        const region1D<double>::type R(0,100);
        W.mesh.regular_map_to(R,W);
        std::cerr << "mesh.layout=" << W.mesh.__layout() << std::endl;
        std::cerr << "mesh.X=" << W.mesh.X() << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()