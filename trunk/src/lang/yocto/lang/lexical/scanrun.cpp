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
                        pattern *curr = r->motif;
                        
                        if(!curr->match(src,fp))
                            continue;
                        
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
                    
                    //__________________________________________________________
                    //
                    // take action
                    //__________________________________________________________
                    const action_result res = from->deed(*best);
                    src.skip(blen);
                    
                    switch(res)
                    {
                        case produce: {
                            //--------------------------------------------------
                            // return a new lexeme
                            //--------------------------------------------------
                            lexeme *lex = new lexeme(from->label,line);
                            lex->swap_with(*best);
                            return lex;
                        } break;
                            
                        case discard:
                            //--------------------------------------------------
                            // discard content and take a new loop
                            //--------------------------------------------------
                            best->reset();
                            break;
                            
                        case control:
                            //--------------------------------------------------
                            // discard content and return control flag
                            //--------------------------------------------------
                            is_control = true;
                            return 0;
                    }
                }
            }
            
        }
    }
    
}
