#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // AND
        //
        ////////////////////////////////////////////////////////////////////////
        AND::  AND() throw() :
        logical(AND::tag)
        {
            assert(AND::tag==type);
        }
        
        AND:: ~AND() throw()
        {}
        
        AND:: AND( const AND &other ) :
        logical(other)
        {
            assert(AND::tag==type);
        }
        
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
                pattern *p = pattern::collapse(operands.pop_front());
                switch(p->type)
                {
                    case AND::tag:
                        assert(p->data);
                        tmp.merge_back( *static_cast<p_list *>(p->data) );
                        delete p;
                        break;
                        
                    default:
                        tmp.push_back(p);
                        break;
                }
                
            }
            
            //------------------------------------------------------------------
            //-- ok
            //------------------------------------------------------------------
            operands.swap_with(tmp);
        }
        
        void AND:: firsts( first_chars &fch ) const
        {
            fch.free();
            fch.accept_empty = true;
            
            first_chars tmp;
            for(const pattern *p = operands.head;p;p=p->next)
            {
                tmp.free();
                tmp.accept_empty = true;
                p->firsts(tmp);
                fch.include(tmp);
                if( !tmp.accept_empty )
                {
                    fch.accept_empty = false;
                    return;
                }
            }
        }
        
    }
    
}
