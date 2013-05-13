#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // one_char
        //
        ////////////////////////////////////////////////////////////////////////
        
        one_char:: one_char() throw() {}
        
        one_char:: ~one_char() throw() {}
        
        bool one_char:: accept(source &src)
        {
            assert(size==0);
            t_char *ch = src.get();
            if(!ch)
                return false;
            if( is_valid(ch->data) )
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
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // any1
        //
        ////////////////////////////////////////////////////////////////////////
        any1:: any1() throw() {}
        
        any1:: ~any1() throw() {}
        
        bool any1:: is_valid(char) const throw() { return true; }
        
        pattern * any1:: clone() const { return any1::create(); }
        
        any1 *any1::create() { return new any1(); }

        void any1:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // single
        //
        ////////////////////////////////////////////////////////////////////////
        single:: single(char c) throw() : value(c) {}
        single:: ~single() throw() {}
        
        single * single::create( char c) { return new single(c); }
        
        bool single:: is_valid(char c) const throw() { return value == c; }

        pattern * single:: clone() const { return new single(value); }
        
        
        void single:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            fp.write(value);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // range
        //
        ////////////////////////////////////////////////////////////////////////
        range::~range() throw() {}
        range:: range(int a, int b) throw() : lower( min_of(a,b) ), upper( max_of(a,b) )
        {
        }
        
        range * range:: create(int a, int b) { return new range(a,b); }
        
        range:: range( const range &other ) throw() :
        lower( other.lower ),
        upper( other.upper )
        {
        }
        
        pattern * range:: clone() const { return new range(*this); }
        
        bool range:: is_valid(char c) const throw()
        {
            const int C = c;
            return C >= lower && C <= upper;
        }
        
        void range:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            fp.emit<int32_t>(lower);
            fp.emit<int32_t>(upper);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // choice
        //
        ////////////////////////////////////////////////////////////////////////
        choice:: ~choice() throw() {}
        choice::  choice() :
        chars( default_capacity, as_capacity)
        {
        }
        
        void choice:: append(char c)
        {
            const uint8_t C(c);
            (void)chars.insert(C);
        }
        
        void choice:: append(int lo,int hi)
        {
            if(hi<lo) cswap(lo,hi);
            for(int i=lo;i<=hi;++i)
                (void)chars.insert( uint8_t(i) );
        }
        
               
        choice:: choice( const choice &other ) :
        chars( other.chars )
        {
            if( chars.size() <= 0 )
                throw exception("lingua::choice(NO CHARS)");
        }
        
        void choice:: write( ios::ostream &fp ) const
        {
            fp.emit<uint32_t>( chars.size() );
            for(size_t i=1;i<=chars.size();++i)
            {
                fp.write( chars[i] );
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // within
        //
        ////////////////////////////////////////////////////////////////////////
        within:: ~within() throw() {}
        
        within:: within() : choice() {}
        
        within:: within( const within &other ) : choice(other) {}
        
        pattern *within:: clone() const { return new within(*this); }
        
        within *within:: create()
        {
            return new within();
        }
        
        void within:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        bool within:: is_valid(char c) const throw()
        {
            const uint8_t C(c);
            return chars.search(C);
        }

        
        ////////////////////////////////////////////////////////////////////////
        //
        // none
        //
        ////////////////////////////////////////////////////////////////////////
        none:: ~none() throw() {}
        
        none:: none() : choice() {}
        
        none:: none( const none &other ) : choice(other) {}
        
        pattern *none:: clone() const { return new none(*this); }
        
        none *none:: create()
        {
            return new none();
        }

        void none:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        bool none:: is_valid(char c) const throw()
        {
            const uint8_t C(c);
            return !chars.search(C);
        }
    }
}