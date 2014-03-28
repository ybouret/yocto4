#include "yocto/utest/run.hpp"
#include "yocto/fovea/cell/line.hpp"

#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/fovea/mesh/point.hpp"
#include "yocto/code/rand.hpp"


using namespace yocto;
using namespace fovea;

static inline void show_mesh( const mesh &msh )
{
    std::cerr << "msh dims     = " << msh.dims        << std::endl;
    std::cerr << "   #vertices = " << msh.vertices    << std::endl;
    std::cerr << "   #edges    = " << msh.num_edges() << std::endl;
    for(size_t i=0;i<msh.dims;++i)
    {
        const char *id = mesh::axis_name(i);
        const linear_space &l = msh.adb[id];
        std::cerr << "axis " << id << " bytes: " << l.bytes << std::endl;
    }
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
    
    
    { curvilinear_mesh<double, layout1D> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { curvilinear_mesh<float, layout1D> msh(a,L1);  show_mesh(msh);}
    a.free();
    
    { curvilinear_mesh<double, layout2D> msh(a,L2); show_mesh(msh); }
    a.free();
    
    { curvilinear_mesh<float, layout2D> msh(a,L2);  show_mesh(msh);}
    a.free();
    
    { curvilinear_mesh<double, layout3D> msh(a,L3); show_mesh(msh); }
    a.free();
    
    { curvilinear_mesh<float, layout3D> msh(a,L3);  show_mesh(msh);}
    a.free();
    
    { point_mesh<1,float> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { point_mesh<2,double> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { point_mesh<3,float> msh(a,L1); show_mesh(msh); }
    a.free();
    
    
}
YOCTO_UNIT_TEST_DONE()
