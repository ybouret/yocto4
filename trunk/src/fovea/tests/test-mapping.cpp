#include "yocto/utest/run.hpp"
#include "yocto/fovea/mesh/rectilinear.hpp"
#include "yocto/fovea/mesh/curvilinear.hpp"
#include "yocto/fovea/mesh/point.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/fovea/vtk.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace fovea;
using namespace math;

#define gen_coord()  (alea<double>()+1.0)
#define gen_coordf() (alea<float>()+1.0f)

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
    
    const size_t nxmax = 5;
    const size_t nymax = 5;
    const size_t nzmax = 5;
    
    const unit_t lox = -unit_t(1 + alea_lt(nxmax));
    const unit_t hix =  unit_t(1 + alea_lt(nxmax));
    const unit_t loy = -unit_t(1 + alea_lt(nymax));
    const unit_t hiy =  unit_t(1 + alea_lt(nymax));
    const unit_t loz = -unit_t(1 + alea_lt(nzmax));
    const unit_t hiz =  unit_t(1 + alea_lt(nzmax));
    
    const layout1D L1(lox,hix);
    
    const coord2D  lo2(lox,loy);
    const coord2D  hi2(hix,hiy);
    
    const layout2D L2(lo2,hi2);
    
    const coord3D lo3(lox,loy,loz);
    const coord3D hi3(hix,hiy,hiz);
    
    const layout3D L3(lo3,hi3);
    
    const Box<1,float>   B1f( -gen_coordf(), gen_coordf() );
    const Box<1,double>  B1d( -gen_coord(),  gen_coord()  );
    
    const Box<2,float>   B2f( v2d<float>(-gen_coordf(), -gen_coordf()),  v2d<float>( gen_coordf(), gen_coordf()));
    const Box<2,double>  B2d( v2d<double>(-gen_coord(), -gen_coord()),   v2d<double>(gen_coord(),  gen_coord()));
    
    const Box<3,float>   B3f(v3d<float>(-gen_coordf(),-gen_coordf(),-gen_coordf()),
                             v3d<float>(gen_coordf(),gen_coordf(),gen_coordf()));
    
    const Box<3,double>   B3d(v3d<double>(-gen_coord(),-gen_coord(),-gen_coord()),
                              v3d<double>(gen_coord(),gen_coord(),gen_coord()));
    
    VTK & vtk = VTK::instance();
    
    // RECTILINEAR 1D
    {
        rectilinear_mesh<float,layout1D> msh(a,L1);
        msh.map_to(B1f);
        show_mesh(msh);
        ios::ocstream fp("r1df.vtk",false);
        vtk.write_mesh(fp,msh,"r1df");
    }
    a.free();
    
    {
        rectilinear_mesh<double,layout1D> msh(a,L1);
        msh.map_to(B1d);
        show_mesh(msh);
        ios::ocstream fp("r1dd.vtk",false);
        vtk.write_mesh(fp,msh,"r1dd");
    }
    a.free();
    
    // RECTILINEAR 2D
    {
        rectilinear_mesh<float,layout2D> msh(a,L2);
        msh.map_to(B2f);
        show_mesh(msh);
        ios::ocstream fp("r2df.vtk",false);
        vtk.write_mesh(fp,msh,"r2df");
    }
    a.free();
    
    
    {
        rectilinear_mesh<double,layout2D> msh(a,L2);
        msh.map_to(B2d);
        show_mesh(msh);
        ios::ocstream fp("r2dd.vtk",false);
        vtk.write_mesh(fp,msh,"r2dd");
    }
    a.free();
    
    
    // RECTILINEAR 3D
    {
        rectilinear_mesh<float,layout3D> msh(a,L3);
        msh.map_to(B3f);
        show_mesh(msh);
        ios::ocstream fp("r3df.vtk",false);
        vtk.write_mesh(fp,msh,"r3df");
    }
    a.free();
    
    {
        rectilinear_mesh<double,layout3D> msh(a,L3);
        msh.map_to(B3d);
        show_mesh(msh);
        ios::ocstream fp("r3dd.vtk",false);
        vtk.write_mesh(fp,msh,"r3dd");
    }
    a.free();
    
    
    
    
    // CURVILINEAR 1D
    {
        curvilinear_mesh<float,layout1D> msh(a,L1);
        msh.map_to(B1f);
        show_mesh(msh);
        ios::ocstream fp("c1df.vtk",false);
        vtk.write_mesh(fp,msh,"c1df");
    }
    a.free();
    
    {
        curvilinear_mesh<double,layout1D> msh(a,L1);
        msh.map_to(B1d);
        show_mesh(msh);
        ios::ocstream fp("c1dd.vtk",false);
        vtk.write_mesh(fp,msh,"c1dd");
        
    }
    a.free();
    
    // CURVILINEAR 2D
    {
        curvilinear_mesh<float,layout2D> msh(a,L2);
        msh.map_to(B2f);
        show_mesh(msh);
        ios::ocstream fp("c2df.vtk",false);
        vtk.write_mesh(fp,msh,"c2df");
    }
    a.free();
    
    {
        curvilinear_mesh<double,layout2D> msh(a,L2);
        msh.map_to(B2d);
        show_mesh(msh);
        ios::ocstream fp("c2dd.vtk",false);
        vtk.write_mesh(fp,msh,"c1dd");
    }
    a.free();
    
    // CURVILINEAR 3D
    {
        curvilinear_mesh<float,layout3D> msh(a,L3);
        msh.map_to(B3f);
        show_mesh(msh);
        ios::ocstream fp("c3df.vtk",false);
        vtk.write_mesh(fp,msh,"c3df");
    }
    a.free();
    
    {
        curvilinear_mesh<double,layout3D> msh(a,L3);
        msh.map_to(B3d);
        show_mesh(msh);
        ios::ocstream fp("c3dd.vtk",false);
        vtk.write_mesh(fp,msh,"c3dd");
    }
    a.free();

    
    
}
YOCTO_UNIT_TEST_DONE()
