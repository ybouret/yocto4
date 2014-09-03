#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        namespace lexical
        {
            
            lexeme * scanner:: get(source &src, ios::istream &fp, bool &is_control)
            {
                is_control = false;
                while(true)
                {
                    //__________________________________________________________
                    //
                    // initialize search
                    //__________________________________________________________
                    if(!src.peek(fp))
                    {
                        return 0;
                    }
                    
                    //__________________________________________________________
                    //
                    // find the best pattern
                    //__________________________________________________________
                    rule    *from = 0;
                    pattern *best = 0;
                    size_t   blen = 0;
                    
                    for(rule *r=rules.head;r;r=r->next)
                    {
                        pattern *curr = r->motif; assert(curr);
                        
                        if(!curr->match(src,fp))
                        {
                            continue;
                        }
                        
                        const size_t clen = curr->size;
                        
                        if(!best)
                        {
                            //__________________________________________________
                            //
                            // initialize best
                            //__________________________________________________
                            best = curr;
                            blen = clen;
                            src.uncpy(*best);
                            from = r;
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // we have another match
                            //__________________________________________________
                            if(clen>blen)
                            {
                                best->reset();
                                best = curr;
                                blen = clen;
                                src.uncpy(*best);
                                from = r;
                            }
                            else
                            {
                                // too late
                                src.unget(*curr);
                            }
                        }
                    }
                    
                    //__________________________________________________________
                    //
                    // did we get it ?
                    //__________________________________________________________
                    if(!best)
                    {
                        const t_char    *ch = src.peek(fp); assert(ch);
                        const code_type  C  = ch->code;
                        if(C>=32 && C<127)
                        {
                            throw exception("%d: unexpected '%c", line, char(C) );
                        }
                        else
                        {
                            throw exception("%d: unexpected character 0x%02d", line, int(C));
                        }
                    }
                    
                    if(blen<=0)
                    {
                        throw exception("%d: empty lexeme, corrupted <%s>",line, name.c_str());
                    }
                    
                    //__________________________________________________________
                    //
                    // take action
                    //__________________________________________________________
                    const bool produce = from->deed(*best);
                    src.skip(blen);
                    
                    switch( from->type )
                    {
                        case rule::is_control:
                            // this is a control rule
                            if(produce)
                                throw exception("unexpected producing control rule '%s'", from->label.c_str());
                            
                            // discard content
                            best->reset();
                            
                            // set info
                            is_control = true;
                            
                            // stop looking
                            return 0;
                            
                            
                        case rule::is_regular:
                            if(produce)
                            {
                                lexeme *lex = new lexeme(from->label,line);
                                lex->swap_with(*best);
                                return lex;
                            }
                            else
                            {
                                // silently discard
                                best->reset();
                                // and go on...
                            }
                            break;
                    }
                    
                    //__________________________________________________________
                    //
                    // next search after a discard
                    //__________________________________________________________
                }
                
            }
            
        }
    }
    
}
