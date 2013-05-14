#include "yocto/lingua/pattern/logic.hpp"

#include <iostream>

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // AND
        //
        ////////////////////////////////////////////////////////////////////////
        AND::  AND() throw() : logical(AND::tag) {}
        AND:: ~AND() throw() {}
        
        AND:: AND( const AND &other ) : logical(other) {}
        
        AND * AND::create() { return new AND(); }
        
        void AND:: save( ios::ostream &fp ) const
        {
            fp.emit(tag);
            write(fp);
        }

        void AND:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"&&\"];\n";
            __viz(this,fp);
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
        
        void AND:: optimize() throw()
        {
            //------------------------------------------------------------------
            //-- recursive opt
            //------------------------------------------------------------------
            optimize_all();

            //------------------------------------------------------------------
            //-- fusion of and
            //------------------------------------------------------------------
            p_list tmp;
            while( operands.size )
            {
                pattern *p = operands.pop_front();
                if( p->type == AND::tag )
                {
                    assert(p->data);
                    tmp.merge_back( *static_cast<p_list *>(p->data) );
                    delete p;
                }
                else
                    tmp.push_back(p);
            }

            //------------------------------------------------------------------
            //-- ok
            //------------------------------------------------------------------

            operands.swap_with(tmp);
        }
    }
    
}
