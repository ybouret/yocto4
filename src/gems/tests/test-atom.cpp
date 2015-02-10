#include "yocto/gems/library.hpp"
#include "yocto/gems/atoms.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace gems;

YOCTO_UNIT_TEST_IMPL(atom)
{
    library lib;
    lib.insert("H", 1.0);
    lib.insert("He",2.0);


    std::cerr << std::endl << "initial lib" << std::endl;
    lib.display();

    atoms<double> aa;

    std::cerr << "insertion" << std::endl;

    for(size_t i=1;i<=10;++i)
    {
        aa.insert( lib.create<double>(i,"H") );
    }

    atoms<double> tmp;
    for(size_t i=1;i<=5;++i)
    {
        aa.transfer_to(tmp,i);
    }

    binary_atoms<double> iodata;
    tmp.store_into(iodata);

    std::cerr << std::endl << "current lib" << std::endl;
    lib.display();

    tmp.release();

    std::cerr << std::endl << "cleanup lib" << std::endl;
    lib.display();

    lib.GC();

    std::cerr << std::endl << "updated lib" << std::endl;
    lib.display();

    std::cerr << "sizeof(atom<double>)=" << sizeof(atom<double>) << std::endl;
    std::cerr << "sizeof(atom<float> )=" << sizeof(atom<float>)  << std::endl;
    std::cerr << "atom<double>::io_bytes=" << atom<double>::io_bytes << std::endl;
    std::cerr << "atom<float>: :io_bytes=" << atom<float>::io_bytes << std::endl;

}
YOCTO_UNIT_TEST_DONE()

