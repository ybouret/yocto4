#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {
        
        one_char:: ~one_char() throw() {}
        
        one_char:: one_char( const uint32_t user_type ) throw() :
        pattern(user_type)
        {
        }
        
        
        bool one_char:: match(source &src,ios::istream &fp)
        {
            assert( 0 == size );
            t_char *ch = src.get(fp);
           
            if(ch)
            {
                if( is_valid(ch->code) )
                {
                    push_back(ch);
                    return true;
                }
                else
                {
                    src.unget(ch);
                    return false;
                }
            }
            else
                return false;
        }
    }
}


namespace yocto
{
    namespace lang
    {
        any1:: ~any1() throw()
        {
        }
        
        any1:: any1() throw() : one_char( tag )
        {
        }
        
        pattern * any1:: create() { return new any1(); }
        
        pattern * any1::clone() const
        {
            return new any1();
        }
        
        bool any1:: is_valid(const code_type) const throw() { return true; }
        
    }
    
}

namespace yocto
{
    namespace lang
    {
        single:: ~single() throw()
        {
        }
        
        single:: single(const int which) throw() :
        one_char(tag),
        value(which)
        {
            data = (void*)&value;
        }
        
        pattern * single:: create( const int val )
        {
            return new single(val);
        }
        
        pattern *single:: clone() const
        {
            return new single( value );
        }
        
        bool single:: is_valid(const code_type C) const throw()
        {
            return C == value;
        }
    }

}

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lang
    {
        range:: ~range() throw()
        {}
        
        range:: range( const int lo, const int up) throw() :
        one_char(tag),
        lower( min_of(lo,up) ),
        upper( max_of(lo,up) )
        {
            data = (void*)&lower;
        }
        
        bool range:: is_valid(const code_type C) const throw()
        {
            return C>=lower && C <=upper;
        }
        
        pattern * range:: clone() const
        {
            return new range(lower,upper);
        }
        
        pattern * range:: create(const code_type lo, const code_type up)
        {
            return new range(lo,up);
        }
    }
    
}


namespace yocto
{
    namespace lang
    {
        choice:: ~choice() throw()
        {
        }
        
        choice:: choice( const uint32_t id ) :
        one_char(id),
        db()
        {
            data = (void*) &db;
        }
        
        choice:: choice(const choice &other ) :
        one_char(other.type),
        db( other.db )
        {
        }
        
        
    }
}

namespace yocto
{
    namespace lang
    {
        within:: ~within() throw() {}
        
        within:: within() :
        choice(tag)
        {
        }
        
        within:: within( const within &other ) :
        choice(other)
        {
        }
        
        
        choice * within:: create()
        {
            return new within();
        }
        
        pattern * within:: clone() const
        {
            return new within(*this);
        }
        
        bool within:: is_valid(const code_type C) const throw()
        {
            return db.search(C);
        }
        
    }
    
}


namespace yocto
{
    namespace lang
    {
        none:: ~none() throw() {}
        
        none:: none() :
        choice(tag)
        {
        }
        
        none:: none( const none &other ) :
        choice(other)
        {
        }
        
        
        choice * none:: create()
        {
            return new none();
        }
        
        pattern * none:: clone() const
        {
            return new none(*this);
        }
        
        bool none:: is_valid(const code_type C) const throw()
        {
            return !db.search(C);
        }
        
    }
    
}







