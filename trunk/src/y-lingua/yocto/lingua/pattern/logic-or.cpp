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
        
        OR:: OR( const OR &other ) : logical(other) { assert(OR::tag==type); }
        
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
            assert(OR::tag==type);
            p_list tmp;
            while( operands.size )
            {
                pattern *p = pattern::collapse( operands.pop_front() );
                switch(p->type)
                {
                    case OR::tag:
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
        
        void OR:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"||\"];\n";
            __viz(this,fp);
        }
        
        void OR:: firsts( first_chars &fch ) const
        {
            fch.free();
            fch.accept_empty = true;
			bool set_accept_empty = true;

			first_chars tmp;
			for( const pattern *p = operands.head;p;p=p->next)
			{
				tmp.free();
				tmp.accept_empty = true;
				p->firsts(tmp);
				fch.include(tmp);
				if( set_accept_empty )
				{
					fch.accept_empty = tmp.accept_empty;
					set_accept_empty = false;
				}
			}

        }
        
        
    }
    
}
