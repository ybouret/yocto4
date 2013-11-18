#include "yocto/associative/labelled.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/associative/map.hpp"
#include "yocto/associative/dictionary.hpp"
#include "yocto/associative/set.hpp"

using namespace yocto;

namespace  {
    
    class dummy
    {
    public:
        
        explicit dummy( const char *id ) :
        name(id)
        {
        }
        
        const string name;
        
        const string & key() const throw() { return name; }
        
        virtual ~dummy() throw()
        {
        }
        
        dummy( const dummy &other ) : name(other.name)
        {
            
        }
        
    private:
        YOCTO_DISABLE_ASSIGN(dummy);
    };
    
}

YOCTO_UNIT_TEST_IMPL(labelled)
{
#if 1
    labelled<map,int> M;
    M.store( "hello", 1 );
    M.query( "hello" );
    
    labelled<dictionary,string,comparator<string> > D;
    D.store("Hello","Bonjour");
    D.query("Hello");
    
    labelled<set,dummy> S;
    {
        const dummy d("hello");
        S.store( d );
    }
    S.query( "hello" );
#endif
}
YOCTO_UNIT_TEST_DONE()
