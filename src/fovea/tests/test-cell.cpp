#include "yocto/utest/run.hpp"
#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/fovea/mesh/point.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/fovea/cell/line.hpp"
#include "yocto/fovea/cell/triangle.hpp"

using namespace yocto;
using namespace fovea;

template <typename MESH>
static inline void show_mesh( const MESH &msh )
{
    typedef typename MESH::EDGE EDGE;
    typedef Line<MESH::DIMS,typename MESH::TYPE> LINE;
    typedef Triangle<MESH::DIMS,typename MESH::TYPE> TRIANGLE;
    
    std::cerr << "mesh is " << msh.form_name() << std::endl;
    std::cerr << "#VERTEX=" << msh.vertices    << std::endl;
    std::cerr << "#EDGE  =" << msh.edb.size()  << std::endl;
    if(msh.edb.size()>2)
    {
        const EDGE &e1 = msh.edb.front();
        LINE  l(e1.v1,e1.v2);
        l.load_edges(msh);
        {
            const size_t i1 = alea_lt(msh.vertices);
            size_t i2 = i1;
            while(i2==i1)
                i2 = alea_lt(msh.vertices);
            const LINE l2(msh[i1],msh[i2]);
            size_t i3=i1;
            while(i3==i1||i3==i2)
                i3 = alea_lt(msh.vertices);
            const TRIANGLE t(msh[i1],msh[i2],msh[i3]);
        }
        
    }
}

YOCTO_UNIT_TEST_IMPL(cell)
{
    array_db a;
    
    const unit_t lox = -unit_t(1 + alea_lt(20));
    const unit_t hix =  unit_t(1 + alea_lt(20));
    const unit_t loy = -unit_t(1 + alea_lt(20));
    const unit_t hiy =  unit_t(1 + alea_lt(20));
    const unit_t loz = -unit_t(1 + alea_lt(10));
    const unit_t hiz =  unit_t(1 + alea_lt(10));
    
    const layout1D L1(lox,hix);
    
    const coord2D  lo2(lox,loy);
    const coord2D  hi2(hix,hiy);
    
    const layout2D L2(lo2,hi2);
    
    const coord3D lo3(lox,loy,loz);
    const coord3D hi3(hix,hiy,hiz);
    
    const layout3D L3(lo3,hi3);
    
    std::cerr << "RECTILINEAR" << std::endl;
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
    
    
    std::cerr << "CURVILINEAR" << std::endl;
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
    
    
    std::cerr << "POINTMESH" << std::endl;
    { point_mesh<1,float> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { point_mesh<2,double> msh(a,L1); show_mesh(msh); }
    a.free();
    
    { point_mesh<3,float> msh(a,L1); show_mesh(msh); }
    a.free();
    
    
}
YOCTO_UNIT_TEST_DONE()

