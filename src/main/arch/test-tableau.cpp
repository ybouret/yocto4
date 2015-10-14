#include "yocto/sequence/tableau.hpp"
#include "yocto/utest/run.hpp"
#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(tableau)
{

    tableau_of<int>    it;
    tableau_of<float>  ft(100);
    tableau_of<double> dt(5,1.0);
    std::cerr << "dt=" << dt << std::endl;
    tableau_of<double> dt2(dt);

    tableau_of<string> st(4);
    st[1] = "Hello";
    std::cerr << "st=" << st << std::endl;

}
YOCTO_UNIT_TEST_DONE()
