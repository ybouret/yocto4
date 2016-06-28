#include "yocto/utest/run.hpp"
#include "yocto/seem/evaluator.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"
#include <cmath>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(eval)
{

    Seem::Evaluator SEEM;
    SEEM.SetVariable("x", 0.1);

    for(int i=1;i<argc;++i)
    {
        const char *expr = argv[i];
        Seem::vCode code = SEEM.compile(expr);
        std::cerr << expr << "=" << SEEM.eval(code) << std::endl;
    }


}
YOCTO_UNIT_TEST_DONE()

