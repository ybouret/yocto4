#include "yocto/utest/run.hpp"

#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/fovea/mesh/point.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/fovea/cell/line.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/fovea/cell/triangle.hpp"

using namespace yocto;
using namespace fovea;

template <typename MESH>
static inline void show_mesh( const MESH &msh )
{
    std::cerr << "msh dims     = " << msh.dims     << std::endl;
    std::cerr << "    vertices = " << msh.vertices << std::endl;
    for(size_t i=0;i<msh.dims;++i)
    {
        const char *id = mesh::axis_name(i);
        const linear_space &l = msh.adb[id];
        std::cerr << "axis " << id << " bytes: " << l.bytes << std::endl;
    }
    
    
    typedef Line<MESH::DIMS,typename MESH::TYPE>     LINE;
    typedef Triangle<MESH::DIMS,typename MESH::TYPE> TRIANGLE;

    typedef shared_ptr<LINE>
    LINE_PTR;
    typedef shared_ptr<TRIANGLE> TRIANGLE_PTR;
    vector<LINE_PTR>     lines;
    vector<TRIANGLE_PTR> tri;
    
    size_t nc = 0;
    for(size_t i=0;i<msh.vertices;++i)
    {
        for(size_t j=0;j<msh.vertices;++j)
        {
            if(i!=j)
            {
                const LINE_PTR p(new LINE(msh[i],msh[j]));
                ++nc;
                lines.push_back(p);
            }
        }
    }
    std::cerr << "#created edges=" << nc << std::endl;
    
    
    for(size_t i=0;i<100;++i)
    {
        size_t i1 = alea_lt(msh.vertices);
        size_t i2 = i1;
        while(i2==i1)
            i2 = alea_lt(msh.vertices);
        size_t i3 = i1;
        while(i3==i1||i3==i2)
            i3 = alea_lt(msh.vertices);
        const TRIANGLE_PTR p( new TRIANGLE(msh[i1],msh[i2],msh[i3]));
        tri.push_back(p);
    }
    std::cerr << "#created triangles = " << tri.size() << std::endl;
}

YOCTO_UNIT_TEST_IMPL(line)
{
    array_db a;
    
    const unit_t lox = -unit_t(1 + alea_lt(5));
    const unit_t hix =  unit_t(1 + alea_lt(5));
    const unit_t loy = -unit_t(1 + alea_lt(5));
    const unit_t hiy =  unit_t(1 + alea_lt(5));
    const unit_t loz = -unit_t(1 + alea_lt(5));
    const unit_t hiz =  unit_t(1 + alea_lt(5));
    
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
