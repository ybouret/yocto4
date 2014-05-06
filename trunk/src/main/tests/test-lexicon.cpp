#include "yocto/utest/run.hpp"
#include "yocto/associative/lexicon.hpp"
#include "yocto/string.hpp"

using namespace yocto;

namespace
{
    
    class AKey
    {
    public:
        const int code;
        
        inline AKey(int c) throw() : code(c) {}
        virtual ~AKey() throw() {}
        AKey( const AKey &a ) throw() : code(a.code) {}
        friend inline bool operator==( const AKey &l, const AKey &r ) throw()
        {
            return l.code == r.code;
        }
        
    private:
        YOCTO_DISABLE_ASSIGN(AKey);
    };

    class AObj : public AKey
    {
    public:
        string value;
        
        explicit AObj(int c, const char *s) : AKey(c), value(s) {}
        virtual ~AObj() throw() {}
        
        AObj( const AObj &a ) : AKey(a), value(a.value) {}
        
        
        
    private:
        YOCTO_DISABLE_ASSIGN(AObj);
    };
    
    
}

YOCTO_UNIT_TEST_IMPL(lexicon)
{
    lexicon<AKey,AObj> lx;
    const AKey k(1);
    lx.search(k);
    
    
}
YOCTO_UNIT_TEST_DONE()
