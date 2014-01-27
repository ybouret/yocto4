#include "yocto/utest/run.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/code/bzset.hpp"
#include <cstring>
#include <typeinfo>

using namespace yocto;

#define ZSET() memset(w,0,sizeof(w))
#define SHOW() do { std::cerr << std::hex; const char *p = (const char *)&w[0]; std::cerr << "w="; for(size_t i=0;i<sizeof(w);++i) std::cerr << unsigned(p[i]); std::cerr << std::endl; std::cerr << std::dec; } while(false)

template <typename T>
static void perform_endian()
{
    std::cerr << "with <" << typeid(T).name() << ">/ " << sizeof(T) << " bytes" << std::endl;
    uint16_t w[5];
    ZSET();
    
    const T x(1);
    store_be<T>(x,w);
    std::cerr << "x=" << x << std::endl;
    SHOW();
    const T y = query_be<T>(w);
    std::cerr << "y=" << y << std::endl;
}

YOCTO_UNIT_TEST_IMPL(endian)
{
    perform_endian<int>();
    perform_endian<unsigned short>();
    perform_endian<float>();
    perform_endian<int64_t>();
    perform_endian<double>();
    
}
YOCTO_UNIT_TEST_DONE()
