#include "yocto/utest/run.hpp"
#include "yocto/associative/lexicon.hpp"
#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

namespace
{
    class AObj
    {
    public:
        const int code;
        string    value;
        
        inline AObj(int c, const char *s) :
        code(c),
        value(s)
        {
        }
        
        inline ~AObj() throw() {}
        
        inline const int & key() const throw() { return code; }
        
        inline AObj( const AObj &other ) : code(other.code), value(other.value) {}
        
        YOCTO_DISABLE_ASSIGN(AObj);
        
    };
    
}

#define SHOWLX(LX) std::cerr << "size=" << LX.size() << ", maxi=" << LX.capacity() << ", #lists=" << LX.num_lists() << ", allocated=" << LX.bytes() << std::endl

YOCTO_UNIT_TEST_IMPL(lexicon)
{
    { lexicon<int,AObj> lx0; }
    
    lexicon<int,AObj> lx(10);
    
    SHOWLX(lx);
    for(int i=50;i>0;--i)
    {
        const AObj a(i,"Hello");
        if(!lx.insert(a) )
            throw exception("can't insert #%d", i);
        
        SHOWLX(lx);
        for( lexicon<int, AObj>::iterator j=lx.begin();j!=lx.end();++j)
        {
            std::cerr << " " << (*j).code;
        }
        std::cerr << std::endl;
    }
    
    for(int i=1;i<=10;i+=2)
    {
        if( ! lx.search(i) )
            throw exception("Missing #%d",i);
        if( !lx.remove(i) )
            throw exception("can't remove #%d",i);
        SHOWLX(lx);
        for( lexicon<int, AObj>::iterator j=lx.begin();j!=lx.end();++j)
        {
            std::cerr << " " << (*j).code;
        }
        std::cerr << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
