#include "yocto/lingua/match.hpp"
#include "yocto/lingua/pattern/compiler.hpp"
#include "yocto/ios/imstream.hpp"

#include "yocto/exception.hpp"
#include "yocto/memory/buffers.hpp"

namespace yocto
{
    namespace lingua
    {
        match:: ~match() throw()
        {
        }
        
        pattern * match:: safe_clone() const
        {
            return ptr.is_valid() ? ptr->clone() : 0;
        }
        
        match:: match() throw() : ptr(), src() {}
        
        
        match:: match( const string &expr ) : ptr(), src()
        {
            load(expr,0);
        }
        
        match:: match( const char *expr ) : ptr(), src()
        {
            load(expr,0);
        }
        
        match:: match( const match &other ) : ptr( other.safe_clone() ), src()
        {
            
        }
        
        
        void match:: load( pattern *p ) throw()
        {
            ptr.reset(p);
        }
        
        void match:: load( const string &expr, p_dict *dict )
        {
            load( compile(expr,dict) );
        }
        
        void match:: load( const char *expr, p_dict *dict )
        {
            load( compile(expr,dict) );
        }
        
        
        match & match:: operator=( const string &expr )
        {
            load(expr);
            return *this;
        }
        
        match & match:: operator=( const char *expr )
        {
            load(expr);
            return *this;
        }
        
        
        match & match:: operator=( const match &other )
        {
            if( this != &other )
            {
                ptr.reset( other.safe_clone() );
            }
            return *this;
        }
        
        
        bool match:: operator()( const memory::ro_buffer &buf, mode_type flag )
        {
            switch(flag)
            {
                case exactly:
                    return __find(buf,0);
                    
                case partly:
                    for(size_t shift=0;shift<buf.length();++shift)
                    {
                        if( __find(buf,shift) ) return true;
                    }
                    return false;
                    break;
                    
            }
            throw exception("invalid lingua::match flag");
        }
        
        string match:: last_accepted() const
        {
            if(ptr.is_valid())
            {
                return ptr->to_string();
            }
            else
                return string();
        }
        
        bool match:: __find( const memory::ro_buffer &buf, size_t shift)
        {
            //__________________________________________________________________
            //
            // check ptr
            //__________________________________________________________________
            if(!ptr.is_valid())
                throw exception("no loaded lingua::match pattern");
            ptr->reset();
            
            //__________________________________________________________________
            //
            // create a temporary source
            //__________________________________________________________________
            assert(shift <= buf.length() );
            const memory::local_buffer tmpbuf( static_cast<const char *>(buf.ro())+shift, buf.length() - shift);
            {
                const input in( new ios::imstream(tmpbuf) );
                src.attach(in);
            }
            
            //__________________________________________________________________
            //
            // simple call
            //__________________________________________________________________
            return ptr->accept(src);
        }

        
    }
}
