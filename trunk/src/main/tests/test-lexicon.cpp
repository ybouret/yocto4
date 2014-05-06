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
    
    class AReal : public AKey
    {
    public:
        double value;
        
        explicit AReal(int c, double s) throw(): AKey(c), value(s) {}
        virtual ~AReal() throw() {}
        
        AReal( const AReal &a ) throw() : AKey(a), value(a.value) {}
        
        
        
    private:
        YOCTO_DISABLE_ASSIGN(AObj);
        
    };
    
    
}

#define SHOWSZ(TYPE) std:: cerr << "sizeof" << #TYPE << "\t=\t" << sizeof TYPE << std::endl;
#define SHOW(LX) std::cerr << "size=" << LX.size() << ", capa=" << LX.capacity() << ", #slots=" << LX.num_slots() << std::endl;

YOCTO_UNIT_TEST_IMPL(lexicon)
{
    
    SHOWSZ((lexicon<AKey,AObj>));
    SHOWSZ((lexicon<AKey,AReal>));
    
    lexicon<AKey,AObj> lx;
    const AKey k(1);
    lx.search(k);
    
    for(size_t i=1;i<=20;++i)
    {
        lx.reserve(1);
        SHOW(lx);
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
