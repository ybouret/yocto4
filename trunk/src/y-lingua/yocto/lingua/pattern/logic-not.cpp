#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // NOT
        //
        ////////////////////////////////////////////////////////////////////////
        NOT::  NOT() throw() : logical(tag) {}
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
        
        void NOT:: optimize() throw()
        {
            optimize_all();
        }

        void NOT:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"!=\"];\n";
            __viz(this,fp);
        }
        
    }
    
}
