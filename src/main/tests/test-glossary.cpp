#include "yocto/associative/glossary.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(glossary)
{
    glossary<double> toto;
    toto.insert("a",1);
    toto.insert("b",3);
    for(size_t i=0;i<toto.size();++i)
    {
        std::cerr << toto(i) << std::endl;
    }

}
YOCTO_UNIT_TEST_DONE()
