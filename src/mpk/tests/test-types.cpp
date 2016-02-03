#include "yocto/utest/run.hpp"
#include "yocto/mpk/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace
{
    class mpblock
    {
    public:
        void  *addr;
        size_t size;

        inline mpblock() throw() : addr(0), size(0) {}

        inline mpblock(void *p,size_t n) throw() :
        addr(p),
        size(n)
        {}

        inline ~mpblock() throw()
        {
        }

        inline mpblock(const mpblock &other) throw() :
        addr(other.addr),
        size(other.size)
        {
        }



    private:
        YOCTO_DISABLE_ASSIGN(mpblock);
    };

#if 0
    template <size_t N>
    struct mpdummy
    {
        char id[N];
    };
#endif

}

YOCTO_UNIT_TEST_IMPL(types)
{

    mpk::manager &MPK = mpk::manager::instance();

    std::cerr << "name=" << MPK.name << std::endl;

    vector<mpblock> blocks(100000,as_capacity);
    for(size_t i=0;i<(3*mpk::manager::max_bytes*2);++i)
    {
        for(size_t j=1+alea_leq(3);j>0;--j)
        {
            mpblock blk;
            blk.size = alea_leq(mpk::manager::max_bytes)*2;
            blk.addr = MPK.acquire(blk.size);
            blocks.push_back(blk);
        }
    }
    std::cerr << "#blocks=" << blocks.size() << std::endl;
    c_shuffle(blocks(),blocks.size());
    while(blocks.size()>0)
    {
        mpblock &blk = blocks.back();
        MPK.release(blk.addr, blk.size);
        blocks.pop_back();
    }


}
YOCTO_UNIT_TEST_DONE()
