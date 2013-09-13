#include "yocto/utest/run.hpp"
#include "yocto/associative/dual-set.hpp"
#include "yocto/string.hpp"

using namespace yocto;

class Dummy
{
public:
    explicit Dummy( int a, const string &b ) :
    first(a),
    second(b)
    {
    }
    
    virtual ~Dummy() throw()
    {
    }
    
    const int    first;
    const string second;
    
    Dummy( const Dummy &d ) :
    first( d.first ),
    second( d.second )
    {
    }
    
    const int    & key() const throw() { return first; }
    const string & subkey() const throw() { return second; }
    
    
private:
    YOCTO_DISABLE_ASSIGN(Dummy);
};


YOCTO_UNIT_TEST_IMPL(dualset)
{
    typedef dual_set<int, string, Dummy> ds_t;
    
    ds_t ds1;
    
    ds1.reserve(1);
    
    {
        const Dummy tmp(1,"toto");
        ds1.insert(tmp);
    }
}
YOCTO_UNIT_TEST_DONE()
