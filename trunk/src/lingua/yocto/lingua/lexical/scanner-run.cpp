#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        namespace lexical
        {
            
            
            lexeme * scanner:: get( source &src, bool &fctl)
            {
                if( rules.size <= 0 )
                    throw exception("<%s>(no rules", name.c_str());
                
                while(true)
                {
                    fctl = false;
                    if(!src.is_active())
                        return 0;
                    
                    //==========================================================
                    // find a matching rule
                    //==========================================================
                    rule   *best = 0;
                    size_t  blen = 0;
                    
                    for( rule *r = rules.head;r;r=r->next)
                    {
                        if( r->motif->accept(src) )
                        {
                            best = r;
                            blen = r->motif->size;
                            break;
                        }
                    }
                    
                    //==========================================================
                    // did we find one ?
                    //==========================================================
                    if( !best )
                    {
                        assert(src.cache_size()>0);
                        const char   c = src.peek();
                        const string C = (c>=32 && c<127) ? vformat("'%c'",c) : vformat("0x%02x", unsigned(c) );
                        throw exception("%u: <%s> no rule matching %s", unsigned(line), name.c_str(), C.c_str());
                    };
                    
                    //==========================================================
                    // try to find a better rule
                    //==========================================================
                    src.uncpy( *(best->motif) );
                    for( rule *r=best->next;r;r=r->next)
                    {
                        if(r->motif->accept(src))
                        {
                            const size_t rlen = r->motif->size;
                            if(rlen>blen)
                            {
                                //----------------------------------------------
                                // new best
                                //----------------------------------------------
                                best->motif->reset();
                                best = r;
                                blen = rlen;
                                src.uncpy( *(best->motif) );
                            }
                            else
                            {
                                //----------------------------------------------
                                // too late
                                //----------------------------------------------
                                src.unget( *(r->motif) );
                                assert(0==r->motif->size);
                            }
                        }
                    }
                    
                    //==========================================================
                    // check the resulting token
                    //==========================================================
                    if(blen<=0)
                    {
                        throw exception("%u: <%s> empty token for rule '%s'",
                                        unsigned(line),
                                        name.c_str(),
                                        best->label.c_str());
                    }
                    
                    //==========================================================
                    // forward the source
                    //==========================================================
                    assert(src.cache_size()>=blen);
                    src.skip(blen);
                    
                    //==========================================================
                    // process the token/pattern
                    //==========================================================
                    fctl = best->fctl;
                    if( best->produce() )
                    {
                        //------------------------------------------------------
                        // test consistency
                        //------------------------------------------------------
                        if(fctl)
                            throw exception("%u: <%s> control rule '%s' should not produce a lexeme",
                                            unsigned(line),
                                            name.c_str(),
                                            best->label.c_str());
                        
                        //------------------------------------------------------
                        // build the lexeme
                        //------------------------------------------------------
                        lexeme *lex = new lexeme(best->label,line);
                        lex->swap_with( *(best->motif) );
                        assert(0==best->motif->size);
                        return lex;
                    }
                    else
                    {
                        
                        //------------------------------------------------------
                        // silently remove it...
                        //------------------------------------------------------
                        best->motif->reset();
                        if( fctl )
                            return 0; // do something with this control pattern
                        
                        //------------------------------------------------------
                        // else regular pattern was discarded -> continue
                        //------------------------------------------------------
                    }
                }
                
            }
            
        }
        
    }
    
}
