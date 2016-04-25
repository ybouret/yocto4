#include "yocto/associative/type-glossary.hpp"
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

    type_glossary<int> tg;
    tg.enroll( typeid(int), sizeof(int) );
    tg.enroll( typeid(int32_t), sizeof(int32_t) );
    tg.enroll( typeid(int64_t), sizeof(int64_t) );
    tg.enroll<unsigned>( sizeof(unsigned) );
    tg.enroll<uint32_t>( sizeof(uint32_t) );
    tg.enroll<uint64_t>( sizeof(uint64_t) );
}
YOCTO_UNIT_TEST_DONE()
