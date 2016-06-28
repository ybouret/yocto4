#include "yocto/utest/run.hpp"
#include "yocto/seem/evaluator.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace math;

YOCTO_UNIT_TEST_IMPL(xfunc)
{
    Seem::Evaluator::Pointer SEEM( new Seem::Evaluator );

    if(argc>1)
    {
        const string       expr  = argv[1];
        const Seem::vCode  vcode = SEEM->compile(expr);
        const Seem::Evaluator::Proxy   vproxy("x",SEEM,vcode);
        numeric<double>::function      F( vproxy );
        const string   fname = "xfunc.dat";
        {
            std::cerr << "saving to " << fname << std::endl;
            ios::wcstream  fp(fname);
            for(double x=-1;x<=1;x+=0.01)
            {
                fp("%g %g\n", x, F(x) );
            }
        }

    }

}
YOCTO_UNIT_TEST_DONE()
