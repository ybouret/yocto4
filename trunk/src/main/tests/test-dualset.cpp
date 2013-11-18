#include "yocto/utest/run.hpp"
#include "yocto/associative/dual-set.hpp"
#include "yocto/string.hpp"
#include "support.hpp"

#include "yocto/sequence/vector.hpp"

#include "yocto/ptr/nosy.hpp"
#include "yocto/counted.hpp"

using namespace yocto;

class Dummy : public object, public counted
{
public:
    explicit Dummy( int a, const string &b , float x) :
    first(a),
    second(b),
    value(x)
    {
    }
    
    virtual ~Dummy() throw()
    {
    }
    
    const int    first;
    const string second;
    float        value;
    
    Dummy( const Dummy &d ) :
    first( d.first ),
    second( d.second ),
    value(d.value)
    {
    }
    
    const int    & key() const throw() { return first; }
    const string & subkey() const throw() { return second; }
    
    typedef nosy_ptr<int,string,Dummy> Ptr;
    
private:
    YOCTO_DISABLE_ASSIGN(Dummy);
};


YOCTO_UNIT_TEST_IMPL(dualset)
{
    typedef dual_set<int, string, Dummy> ds_t;
    
    
    ds_t ds1;
    
    for(size_t i= 100 + alea_leq(1000); i>0;--i)
    {
        const int    a = gen<int>::get();
        const string b = gen<string>::get();
        const float  x = gen<float>::get();
        const Dummy  d(a,b,x);
        if( !ds1.insert(d) )
            std::cerr << "multiple dual " << a << "," <<b << std::endl;
    }
    
    std::cerr << "size     = " << ds1.size()      << std::endl;
    std::cerr << "capacity = " << ds1.capacity()  << std::endl;
    std::cerr << "slots    = " << ds1.num_slots() << std::endl;
    std::cerr << "bytes    = " << ds1.allocated_bytes() << std::endl;
    
    
    vector<int>    keys;
    vector<string> subs;
    
    for( ds_t::iterator i=ds1.begin(); i != ds1.end(); ++i )
    {
        keys.push_back( i->key() );
        subs.push_back( i->subkey() );
    }
    
    for(size_t i=1; i <= keys.size(); ++i )
    {
        if(!ds1.search(keys[i])) throw exception("missing key");
        if(!ds1.sub_search(subs[i])) throw exception("missing subkey");
    }
    
    std::cerr << std::endl << "COPY" << std::endl;
    ds_t ds2( ds1 );
    std::cerr << "size     = " << ds2.size()      << std::endl;
    std::cerr << "capacity = " << ds2.capacity()  << std::endl;
    std::cerr << "slots    = " << ds2.num_slots() << std::endl;
    std::cerr << "bytes    = " << ds2.allocated_bytes() << std::endl;

    
    for(size_t i=1; i <= keys.size(); ++i )
    {
        if( (i%2) )
        {
            if( !ds1.remove(keys[i])) throw exception("missing remove key");
        }
        else
        {
            if( !ds1.sub_remove(subs[i])) throw exception("missing remove subkey");
            
        }
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(dualset2)
{
    typedef dual_set<int, string, Dummy::Ptr> ds_t;
    
    
    ds_t ds1;
    
    for(size_t i= 100 + alea_leq(1000); i>0;--i)
    {
        const int    a = gen<int>::get();
        const string b = gen<string>::get();
        const float  x = gen<float>::get();
        const Dummy::Ptr  d( new Dummy(a,b,x) );
        if( !ds1.insert(d) )
            std::cerr << "multiple dual " << a << "," <<b << std::endl;
    }
    
    std::cerr << "size     = " << ds1.size()      << std::endl;
    std::cerr << "capacity = " << ds1.capacity()  << std::endl;
    std::cerr << "slots    = " << ds1.num_slots() << std::endl;
    std::cerr << "bytes    = " << ds1.allocated_bytes() << std::endl;
    vector<int>    keys;
    vector<string> subs;
    
    for( ds_t::iterator i=ds1.begin(); i != ds1.end(); ++i )
    {
        keys.push_back( i->key() );
        subs.push_back( i->subkey() );
    }
    
    for(size_t i=1; i <= keys.size(); ++i )
    {
        if(!ds1.search(keys[i]))     throw exception("missing key");
        if(!ds1.sub_search(subs[i])) throw exception("missing subkey");
    }
    
    std::cerr << std::endl << "COPY" << std::endl;
    ds_t ds2( ds1 );
    std::cerr << "size     = " << ds2.size()      << std::endl;
    std::cerr << "capacity = " << ds2.capacity()  << std::endl;
    std::cerr << "slots    = " << ds2.num_slots() << std::endl;
    std::cerr << "bytes    = " << ds2.allocated_bytes() << std::endl;

    
    for(size_t i=1; i <= keys.size(); ++i )
    {
        if( (i%2) )
        {
            if( !ds1.remove(keys[i])) throw exception("missing remove key");
        }
        else
        {
            if( !ds1.sub_remove(subs[i])) throw exception("missing remove subkey");
            
        }
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

