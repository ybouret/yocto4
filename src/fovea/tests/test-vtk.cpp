#include "yocto/utest/run.hpp"
#include "yocto/fovea/vtk.hpp"
#include "yocto/ios/ocstream.hpp"


using namespace yocto;
using namespace fovea;


template <typename T>
static inline void show_type( const char *name )
{
    std::cerr << "typeif(" << name << ")=" << typeid(T).name() << std::endl;
}

#define SHOW_TYPE(T) show_type<T>( #T )

YOCTO_UNIT_TEST_IMPL(vtk)
{
    vtk & VTK = vtk::instance();
    std::cerr << "using " << VTK.get_name() << std::endl;
    
    SHOW_TYPE(int8_t);
    SHOW_TYPE(int16_t);
    SHOW_TYPE(int32_t);
    SHOW_TYPE(int64_t);
    
    SHOW_TYPE(char);
    SHOW_TYPE(short);
    SHOW_TYPE(int);
    SHOW_TYPE(long int);
    SHOW_TYPE(unit_t);

    ios::ocstream fp( ios::cstderr );
    float  f = 1.0f;
    double d = 2.0;
    unit_t u = -879;
    math::v2d<double> v2(3,4);
    VTK.out(fp,f)  << '\n';
    VTK.out(fp,d)  << '\n';
    VTK.out(fp,u)  << '\n';
    VTK.out(fp,v2) << '\n';

    
    
}
YOCTO_UNIT_TEST_DONE()
