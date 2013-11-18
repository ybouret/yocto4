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
        NOT::  NOT() throw() : logical(NOT::tag) {}
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
        
        void NOT:: firsts( first_chars &fch ) const
        {
            fch.free();
            fch.accept_empty = false;
			
			fch.reserve(256);
			for(size_t i=0;i<256;++i) (void) fch.insert( uint8_t(i) );

			first_chars tmp;
		
			for( const pattern *p = operands.head;p;p=p->next)
			{
				tmp.free();
				p->firsts(tmp);
				fch.exclude(tmp);
			}

        }
        
    }
    
}
