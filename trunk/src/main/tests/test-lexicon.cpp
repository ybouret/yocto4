#include "yocto/utest/run.hpp"
#include "yocto/associative/lexicon.hpp"
#include "yocto/string.hpp"
#include "yocto/code/rand.hpp"

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
        
        class Hasher
        {
        public:
            Hasher() throw() {}
            ~Hasher() throw() {}
            
            size_t operator()( const AKey &key ) const throw()
            {
                return key.code;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Hasher);
        };
        
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
    
    lexicon<AKey,AObj,AKey::Hasher> lx;
    const AKey k(1);
    lx.search(k);
    
    for(size_t i=1;i<=20;++i)
    {
        lx.reserve(1);
        SHOW(lx);
    }
    
    for(size_t i=1;i<=33;++i)
    {
        const AObj obj(i,"hello");
        if( !lx.insert(obj) )
            throw exception("can't insert object into lexicon");
        SHOW(lx);
    }
    std::cerr << std::endl;
    
    lx.release();
    for(size_t i=1;i<=33;++i)
    {
        const AObj obj(i,"hello");
        if( !lx.insert(obj) )
            throw exception("can't insert object into lexicon");
        SHOW(lx);
        for( lexicon<AKey,AObj,AKey::Hasher>::iterator j=lx.begin();j!=lx.end();++j)
        {
            std::cerr << " " << (*j).code;
        }
        std::cerr << std::endl;
    }
    
    for(size_t i=1;i<=33;++i)
    {
        const AKey key(i);
        if(!lx.search(key) )
            throw exception("Can't find key #%d", key.code);
        
        if( !lx.remove(key) )
            throw exception("can't remove key #%d", key.code);
    }
    
}
YOCTO_UNIT_TEST_DONE()
