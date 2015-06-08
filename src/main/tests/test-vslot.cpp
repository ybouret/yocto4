#include "yocto/utest/run.hpp"
#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/core/list.hpp"

using namespace yocto;


#include "yocto/container/cslot.hpp"

namespace
{
    class memory_node : public object
    {
    public:
        memory_node *next, *prev;
        cslot        data;

        explicit memory_node() : next(0), prev(0), data( alea_leq(100) )
        {
        }

        virtual ~memory_node() throw()
        {
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(memory_node);
    };

    class mlist: public core::list_of<memory_node>
    {
    public:
        explicit mlist() throw() {}
        virtual ~mlist() throw() { while (size) { delete pop_front(); } }

        void shuffle() throw()
        {
            mlist tmp;
            while(size)
            {
                if( alea<double>() > 0.5 )
                {
                    tmp.push_front( pop_back() );
                }
                else
                {
                    tmp.push_back( pop_back() );
                }
            }
            swap_with(tmp);
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(mlist);
    };

}

YOCTO_UNIT_TEST_IMPL(cslot)
{
    std::cerr << "sizeof(cslot)=" << sizeof(cslot) << std::endl;

    mlist L;
    for(size_t i=100+alea_leq(200);i>0;--i) L.push_back( new memory_node() );
    std::cerr << "#mlist=" << L.size << std::endl;
    L.shuffle();
    std::cerr << "#mlist=" << L.size << std::endl;
    for(size_t i=100+alea_leq(200);i>0;--i) L.push_back( new memory_node() );
    std::cerr << "#mlist=" << L.size << std::endl;
    L.shuffle();
    std::cerr << "#mlist=" << L.size << std::endl;

}
YOCTO_UNIT_TEST_DONE()

#include "yocto/container/vslot.hpp"

#define __SHOW(X) std::cerr << "name= <" << X.name() << ">/bytes=" << X.size() << std::endl

namespace
{
    struct something
    {
        float x,y,z;
    };
}

YOCTO_UNIT_TEST_IMPL(vslot)
{

    std::cerr << "sizeof(vslot)=" << sizeof(vslot) << std::endl;
    vslot v1;

    v1.allocate(2);    __SHOW(v1);
    v1.make<float>();  __SHOW(v1);
    v1.make<double>(); __SHOW(v1);
    v1.release();      __SHOW(v1);

    v1.make<short>(2);        std::cerr << v1.as<short>()  << std::endl; __SHOW(v1);
    v1.make<float>(4);        std::cerr << v1.as<float>()  << std::endl; __SHOW(v1);
    v1.make<double>(8);       std::cerr << v1.as<double>() << std::endl; __SHOW(v1);
    v1.make<string>("hello"); std::cerr << v1.as<string>() << std::endl; __SHOW(v1);
    v1.make<something>(); __SHOW(v1);
    v1.free();
    vslot v2;
    v2.make<something>(); __SHOW(v2);

}
YOCTO_UNIT_TEST_DONE()

#include "yocto/container/xslot.hpp"
#define __COPY(X) do { const xslot Y(X); std::cerr << "copy= <" << Y.name() << ">" << " | hash=" << X.hash() << "/" << Y.hash() << std::endl; } while(false)

YOCTO_UNIT_TEST_IMPL(xslot)
{

    std::cerr << "sizeof(xslot)=" << sizeof(xslot) << std::endl;
    xslot x1; __SHOW(x1); __COPY(x1);

    x1.allocate(2);    __SHOW(x1); __COPY(x1);
    x1.make<float>();  __SHOW(x1); __COPY(x1);
    x1.make<double>(); __SHOW(x1); __COPY(x1);
    x1.release();      __SHOW(x1); __COPY(x1);

    x1.make<short>(2);        std::cerr << x1.as<short>()  << std::endl; __SHOW(x1); __COPY(x1);
    x1.make<float>(4);        std::cerr << x1.as<float>()  << std::endl; __SHOW(x1); __COPY(x1);
    x1.make<double>(8);       std::cerr << x1.as<double>() << std::endl; __SHOW(x1); __COPY(x1);
    x1.make<string>("hello"); std::cerr << x1.as<string>() << std::endl; __SHOW(x1); __COPY(x1);
    xslot x2(x1); std::cerr << x2.as<string>() << std::endl;

    x1.make<double>(22.1);

    x2 = x1;
    __SHOW(x2);

    }
    YOCTO_UNIT_TEST_DONE()
    
