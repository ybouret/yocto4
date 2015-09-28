#include "yocto/container/tuple.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;

YOCTO_PAIR_DECL(Coord,int,x,double,y);

YOCTO_DISABLE_ASSIGN(Coord);
YOCTO_PAIR_END();

YOCTO_TRIPLE_DECL(Coord3,int,x,const double,y,long,z);

YOCTO_DISABLE_ASSIGN(Coord3);
YOCTO_TRIPLE_END();

YOCTO_QUAD_DECL(Qat,int,x,const double,y,long,z,float,w);

inline Qat() throw() : x(0), y(0), z(0), w(0) {}

YOCTO_DISABLE_ASSIGN(Qat);
YOCTO_QUAD_END();


YOCTO_PENTUPLE_DECL(Pentuple,int,a,int, b, int, c, int,d,int,e);
YOCTO_PENTUPLE_END();

YOCTO_SEXTUPLE_DECL(Sextuple,int,a,int, b, int, c, int,d,int,e,int,f);
YOCTO_SEXTUPLE_END();

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

    Qat Q1;
    Qat Q2(Q1);
}
YOCTO_UNIT_TEST_DONE()
