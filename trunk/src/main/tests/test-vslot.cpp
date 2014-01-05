#include "yocto/utest/run.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/core/list.hpp"

using namespace yocto;

#define __SHOW() std::cerr << "name= <" << v1.name() << ">" << std::endl
#define __COPY() do { const vslot v2(v1); std::cerr << "copy= <" << v2.name() << ">" << std::endl; } while(false)

#undef __COPY
#define __COPY() do {} while(false)

namespace
{
    class memory_node : public object, public cslot
    {
    public:
        memory_node *next, *prev;
        
        explicit memory_node() : cslot( alea_leq(100) ), next(0), prev(0)
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

YOCTO_UNIT_TEST_IMPL(vslot)
{
    std::cerr << "sizeof(cslot)=" << sizeof(cslot) << std::endl;
    
    {
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
    
    std::cerr << "sizeof(vslot)=" << sizeof(vslot) << std::endl;
    vslot v1;
    
    v1.allocate(2);    __SHOW(); __COPY();
    v1.make<float>();  __SHOW(); __COPY();
    v1.make<double>(); __SHOW(); __COPY();
    v1.release();      __SHOW();
    
    v1.make<short>(2);        std::cerr << v1.as<short>()  << std::endl; __SHOW(); __COPY();
    v1.make<float>(4);        std::cerr << v1.as<float>()  << std::endl; __SHOW(); __COPY();
    v1.make<double>(8);       std::cerr << v1.as<double>() << std::endl; __SHOW(); __COPY();
    v1.make<string>("hello"); std::cerr << v1.as<string>() << std::endl; __SHOW(); __COPY();
    
    
}
YOCTO_UNIT_TEST_DONE()
