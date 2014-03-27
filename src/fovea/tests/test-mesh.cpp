#include "yocto/utest/run.hpp"
#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace fovea;

static inline void show_mesh( const mesh &msh )
{
    std::cerr << "msh dim=" << msh.dimensions << std::endl;
}

YOCTO_UNIT_TEST_IMPL(mesh)
{
    array_db a;
    
    const unit_t lox = -unit_t(1 + alea_lt(100));
    const unit_t hix =  unit_t(1 + alea_lt(100));
    const unit_t loy = -unit_t(1 + alea_lt(100));
    const unit_t hiy =  unit_t(1 + alea_lt(100));
    const unit_t loz = -unit_t(1 + alea_lt(100));
    const unit_t hiz =  unit_t(1 + alea_lt(100));
    
    const layout1D L1(lox,hix);
    
    const coord2D  lo2(lox,loy);
    const coord2D  hi2(hix,hiy);
    
    const layout2D L2(lo2,hi2);
    
    const coord3D lo3(lox,loy,loz);
    const coord3D hi3(hix,hiy,hiz);
    
    const layout3D L3(lo3,hi3);
    
    { rectilinear_mesh<double, layout1D> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { rectilinear_mesh<float, layout1D> msh(a,L1);  show_mesh(msh);}
    a.free();
    
    { rectilinear_mesh<double, layout2D> msh(a,L2); show_mesh(msh); }
    a.free();
    
    { rectilinear_mesh<float, layout2D> msh(a,L2);  show_mesh(msh);}
    a.free();
    
    { rectilinear_mesh<double, layout3D> msh(a,L3); show_mesh(msh); }
    a.free();
    
    { rectilinear_mesh<float, layout3D> msh(a,L3);  show_mesh(msh);}
    a.free();
    
    
}
YOCTO_UNIT_TEST_DONE()
