#include "yocto/associative/glossary.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(glossary)
{
    glossary<double> toto;
    toto.insert("a",1);

}
YOCTO_UNIT_TEST_DONE()
