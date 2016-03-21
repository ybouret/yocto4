#include "yocto/parallel/field.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/string.hpp"
#include "support.hpp"
#include "yocto/math/point2d.hpp"
#include "yocto/parallel/vtk.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(field)
{
    parallel::field1D<float> F1( parallel::patch1D(-10,12) );
    for(unit_t x=F1.lower;x<=F1.upper;++x)
    {
        F1[x] = alea<float>();
    }

    {
        ios::wcstream fp("f1f.vtk");
        parallel::vtk::save_patch(fp, "F1f", F1);
        parallel::vtk::scalars(fp, "float_values", F1);
    }

    const parallel::coord2D   c2min(-5,-8), c2max(15,10);
    const parallel::patch2D   p2(c2min,c2max);
    parallel::field2D<double> F2d(p2);
    for(unit_t y=F2d.lower.y;y<=F2d.upper.y;++y)
    {
        for(unit_t x=F2d.lower.x;x<=F2d.upper.x;++x)
        {
            F2d[y][x] = alea<double>();
        }
    }

    {
        ios::wcstream fp("f2d.vtk");
        parallel::vtk::save_patch(fp, "f2d", F2d);
        parallel::vtk::scalars(fp, "double_values", F2d);
    }


    parallel::field2D<string> F2s(p2);
    for(unit_t y=F2s.lower.y;y<=F2s.upper.y;++y)
    {
        for(unit_t x=F2s.lower.x;x<=F2s.upper.x;++x)
        {
            F2s[y][x] = gen<string>::get();
        }
    }


    const parallel::coord3D   c3min(-5,-8,-1), c3max(15,10,20);
    const parallel::patch3D   p3(c3min,c3max);
    parallel::field3D<int>    F3i(p3);

    int sumA = 0;
    for(unit_t z=p3.lower.z;z<=p3.upper.z;++z)
    {
        for(unit_t y=p3.lower.y;y<=p3.upper.y;++y)
        {
            for(unit_t x=p3.lower.x;x<=p3.upper.x;++x)
            {
                const int tmp = gen<int>::get();
                F3i[z][y][x] = tmp;
                sumA += tmp;
            }
        }

    }

    int sumB = 0;
    for(unit_t z=p3.lower.z;z<=p3.upper.z;++z)
    {
        for(unit_t y=p3.lower.y;y<=p3.upper.y;++y)
        {
            for(unit_t x=p3.lower.x;x<=p3.upper.x;++x)
            {
                sumB += F3i[z][y][x];
            }
        }
    }
    std::cerr << "sum: " << sumA << ", " << sumB << std::endl;
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
