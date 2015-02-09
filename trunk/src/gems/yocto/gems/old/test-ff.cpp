#include "yocto/gems/forces/field.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(ff)
{
    library<double> lib;
    lib.append("H", 0, 1);
    
    size_t N = 100;
    
    atoms<double> particles(N);
    for(size_t i=1;i<=N;++i)
    {
        particles.append(i, lib["H"] );
    }
    
    force_field<double> ff;
    ff.append( new gravity_force<double>( v3d<double>(0,0,-9.81) ) );
    ff.append( new friction_force<double>( 1.0 ) );

    ff.reset(particles);
    ff.collect(particles);
    
}
YOCTO_UNIT_TEST_DONE()
