#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // OR
        //
        ////////////////////////////////////////////////////////////////////////
        OR::  OR() throw() : logical(OR::tag) {}
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

        void OR::optimize() throw()
        {
            optimize_all();
            
            //------------------------------------------------------------------
            //-- fusion of OR
            //------------------------------------------------------------------
            p_list tmp;
            while( operands.size )
            {
                pattern *p = operands.pop_front();
                if( p->type == OR::tag )
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
        
        void OR:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"||\"];\n";
            __viz(this,fp);
        }
        
    }
    
}
