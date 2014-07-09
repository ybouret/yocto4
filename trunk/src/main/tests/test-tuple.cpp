#include "yocto/container/tuple.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_PAIR_DECL(Coord,int,x,double,y);

YOCTO_DISABLE_ASSIGN(Coord);
YOCTO_PAIR_END();

YOCTO_TRIPLE_DECL(Coord3,int,x,const double,y,long,z);

YOCTO_DISABLE_ASSIGN(Coord3);
YOCTO_TRIPLE_END();



YOCTO_UNIT_TEST_IMPL(tuple)
{
    Coord c1(1,2);
    Coord c2(c1);
    
    
    std::cerr << "c1=" << c1 << std::endl;
    std::cerr << "c2=" << c2 << std::endl;
    
    Coord3 X1(1,2,3);
    Coord3 X2(X1);
    
    std::cerr << "X1=" << X1 << std::endl;
    std::cerr << "X2=" << X2 << std::endl;

}
YOCTO_UNIT_TEST_DONE()
