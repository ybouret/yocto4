#include "yocto/gems/library.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(atom)
{
    library lib;
    lib.insert("H", 1.0);
    lib.insert("He",2.0);

    atom<float> *a1( lib.create<float>(0,"H")   );
    atom<double>::pointer a2( lib.create<double>(1,"He") );
    std::cerr << "a1->uuid=" << a1->uuid << std::endl;
    std::cerr << "a2->uuid=" << a2->uuid << std::endl;

}
YOCTO_UNIT_TEST_DONE()

