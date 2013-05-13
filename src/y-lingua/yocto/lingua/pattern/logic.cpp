#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"

namespace yocto
{
    namespace lingua
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // logical
        //
        ////////////////////////////////////////////////////////////////////////
        
        logical:: ~logical() throw() {}
        
        logical:: logical() throw() : operands() {}
        
        logical:: logical( const logical &other ) : operands(other.operands)
        {
            if(operands.size<=0) throw exception("lingua::logical(NO OPERANDS)");
        }
        
        void logical:: reset() throw()
        {
            clear();
            for( pattern *p = operands.head;p;p=p->next)
            {
                p->reset();
            }
        }
        
        void logical:: write( ios::ostream &fp ) const
        {
            fp.emit<uint32_t>(operands.size);
            for(const pattern *p = operands.head;p;p=p->next)
            {
                p->save(fp);
            }
        }
        
        void logical:: append( pattern *p ) throw()
        {
            assert(p);
            operands.push_back(p);
        }

        ////////////////////////////////////////////////////////////////////////
        //
        // AND
        //
        ////////////////////////////////////////////////////////////////////////
        AND::  AND() throw() {}
        AND:: ~AND() throw() {}
        
        AND:: AND( const AND &other ) : logical(other) {}
        
        AND * AND::create() { return new AND(); }
        
        void AND:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        pattern * AND::clone() const { return new AND( *this ); }
        
        bool AND:: accept( source &src )
        {
            assert(0==size);
            for(pattern *p=operands.head;p;p=p->next)
            {
                assert(0==p->size);
                if(! p->accept(src) )
                {
                    assert(0==p->size);
                    src.unget( *this );
                    return false;
                }
                merge_back(*p);
            }
            
            return true;
        }
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // OR
        //
        ////////////////////////////////////////////////////////////////////////
        OR::  OR() throw() {}
        OR:: ~OR() throw() {}
        
        OR:: OR( const OR &other ) : logical(other) {}
        
        OR * OR::create() { return new OR(); }
        
        void OR:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        pattern * OR::clone() const { return new OR( *this ); }

        
        bool OR:: accept( source &src )
        {
            assert(0==size);
            for(pattern *p=operands.head;p;p=p->next)
            {
                assert(0==p->size);
                if( p->accept(src) )
                {
                    p->swap_with( *this );
                    assert(0==p->size);
                    return true;
                }
            }
            return false;
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // NOT
        //
        ////////////////////////////////////////////////////////////////////////
        NOT::  NOT() throw() {}
        NOT:: ~NOT() throw() {}
        
        NOT:: NOT( const NOT &other ) : logical(other) {}
        
        NOT * NOT::create() { return new NOT(); }
        
        void NOT:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }
        
        pattern * NOT::clone() const { return new NOT( *this ); }

        
        bool NOT:: accept( source &src )
        {
            assert(0==size);
            //-- check all patterns
            for(pattern *p=operands.head;p;p=p->next)
            {
                assert(0==p->size);
                if( p->accept(src) )
                {
                    src.unget( *p );
                    return false;
                }
            }
            
            //-- check one char
            t_char *ch = src.get();
            if( ch )
            {
                push_back(ch);
                return true;
            }
            else
                return false;
        }
        
        
    }
}

#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        
        pattern *logical:: EQUAL(const string &s)
        {
            const size_t n = s.size();
            if(n<=0) throw exception("lingua::EQUAL(no char)");
            auto_ptr<logical> p( AND::create() );
            for(size_t i=0; i <n; ++i )
                p->operands.push_back( single::create( s[i] ) );
            return p.yield();
        }
        
        pattern *logical:: EQUAL(const char *s)
        {
            const string tmp(s);
            return EQUAL(tmp);
        }
        
        pattern *logical:: AMONG(const string &s)
        {
            const size_t n = s.size();
            if(n<=0) throw exception("lingua::AMONG(no char)");
            auto_ptr<logical> p( OR::create() );
            for(size_t i=0; i <n; ++i )
                p->operands.push_back( single::create( s[i] ) );
            return p.yield();
        }

        pattern *logical:: AMONG(const char *s)
        {
            const string tmp(s);
            return AMONG(tmp);
        }

    }
    
    
}

