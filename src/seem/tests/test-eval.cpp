#include "yocto/utest/run.hpp"
#include "yocto/seem/evaluator.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/ios/graphviz.hpp"
#include <cmath>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(eval)
{
    ios::icstream  fp( ios::cstdin );

    Seem::Evaluator eval;
    eval.SetVariable("x", 0.1);
    eval.SetCFunction("cos", cos);
    eval.run(fp);

}
YOCTO_UNIT_TEST_DONE()

