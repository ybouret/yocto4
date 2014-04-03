#include "yocto/utest/run.hpp"
#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/fovea/mesh/point.hpp"
#include "yocto/code/rand.hpp"


using namespace yocto;
using namespace fovea;
using namespace math;

#define gen_coord() (alea<double>()-0.5)
#define gen_coordf() (alea<float>()-0.5f)

template <typename MESH>
static inline void show_mesh( const MESH &msh )
{
    std::cerr << "mesh is " << msh.form_name()   << " " << msh.DIMS << "D" << std::endl;
    std::cerr << "#VERTEX=" << msh.vertices      << std::endl;
    std::cerr << "#EDGE  =" << msh.edges.size()  << std::endl;
    std::cerr << "#CELLS =" << msh.cells.size    << std::endl;
    
    std::cerr << std::endl;
}

YOCTO_UNIT_TEST_IMPL(mapping)
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

    const Box<1,float>  B1f( gen_coordf(), gen_coordf() );
    const Box<1,double> B1d( gen_coord(),  gen_coord()  );

    const Box<2,float>   B2f( v2d<float>(gen_coordf(), gen_coordf()),  v2d<float>(gen_coordf(), gen_coordf()));
    const Box<2,double>  B2d( v2d<double>(gen_coord(), gen_coord()),  v2d<double>(gen_coord(), gen_coord()));

    // RECTILINEAR 1D
    {
        rectilinear_mesh<float,layout1D> msh(a,L1);
        msh.map_to(B1f);
        show_mesh(msh);
    }
    a.free();
    
    {
        rectilinear_mesh<double,layout1D> msh(a,L1);
        msh.map_to(B1d);
        show_mesh(msh);
    }
    a.free();
    
    // RECTILINEAR 2D
    {
        rectilinear_mesh<float,layout2D> msh(a,L2);
        msh.map_to(B2f);
        show_mesh(msh);
    }
    a.free();
    
    
    {
        rectilinear_mesh<double,layout2D> msh(a,L2);
        msh.map_to(B2d);
        show_mesh(msh);
    }
    a.free();

    // CURVILINEAR 1D
    {
        curvilinear_mesh<float,layout1D> msh(a,L1);
        msh.map_to(B1f);
        show_mesh(msh);
    }
    a.free();
    
    {
        curvilinear_mesh<double,layout1D> msh(a,L1);
        msh.map_to(B1d);
        show_mesh(msh);
    }
    a.free();

    // CURVILINEAR 2D
    {
        curvilinear_mesh<float,layout2D> msh(a,L2);
        msh.map_to(B2f);
        show_mesh(msh);
    }
    a.free();
    
    {
        curvilinear_mesh<double,layout2D> msh(a,L2);
        msh.map_to(B2d);
        show_mesh(msh);
    }
    a.free();
    
    
    
}
YOCTO_UNIT_TEST_DONE()
