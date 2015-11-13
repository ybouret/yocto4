#include "yocto/spade/format/stl.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/container/matrix.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace spade;
using namespace math;

typedef point3d<double> vtx;

YOCTO_UNIT_TEST_IMPL(stl)
{
    const size_t nr  = 20;
    const size_t nz  = 10;

    const double R   = 6;
    const double Lz  = 7;

    const vtx inside(0,0,Lz/2);

    // create shapes
    matrix<vtx> shape(nz,nr);

    for(size_t i=1;i<=nz;++i)
    {
        const double z      = (i-1)*Lz/(nz-1);
        const double radius = R+z/Lz+alea<double>();
        for(size_t j=1;j<=nr;++j)
        {
            const double angle =  (j-1)*360.0/nr;
            const double aa    =  Deg2Rad(angle);
            const double ca    =  Cos(aa);
            const double sa    =  Sin(aa);
            const vtx    v(radius*ca,radius*sa,z);
            shape[i][j] = v;
        }
    }

    // create STL
    vector< stl::facet<double> > facets;

    // close one side
    const vtx top(0,0,0);
    stl::close_contour(facets,shape[1],top,inside);


    // close the other side
    const vtx bot(0,0,Lz);
    stl::close_contour(facets, shape[nz],bot,inside);

    std::cerr << "Got " << facets.size() << " facets" << std::endl;

    {
        ios::wcstream fp("shape.stl");
        stl::save(fp,facets);
    }

}
YOCTO_UNIT_TEST_DONE()
