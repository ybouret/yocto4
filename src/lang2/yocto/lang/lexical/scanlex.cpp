#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            lexeme * scanner:: get(source &src, ios::istream &fp)
            {
            TRY_PRODUCE:
                if(cache.size)
                {
                    return cache.pop_front();
                }
                else
                {
                    if(!src.peek(fp))
                    {
                        return 0; //EOF
                    }
                    else
                    {
                        //______________________________________________________
                        //
                        // There are some chars ahead !
                        //______________________________________________________
                        rule   *best_rule = 0;
                        token   best_token;

                        for(rule *r = rules.head; r; r=r->next)
                        {
                            token tkn;
                            if(r->motif->match(tkn, src, fp))
                            {
                                best_rule  = r;
                                best_token.swap_with(tkn);
                                src.uncpy(best_token);
                                break;
                            }
                            assert(0==tkn.size);
                        }

                        if(!best_rule)
                        {
                            assert(src.peek(fp));
                            const code_t C = src.peek(fp)->code;
                            throw imported::exception("syntax error", "%d:<%s> unexpected char '%c'", iline, name.c_str(), char(C) );
                        }

                        for(rule *r = best_rule->next; r; r=r->next)
                        {
                            token tkn;
                            if(r->motif->match(tkn, src, fp))
                            {
                                if(tkn.size>best_token.size)
                                {
                                    best_rule = r;
                                    best_token.clear();
                                    best_token.swap_with(tkn);
                                    src.uncpy(best_token);
                                }
                                else
                                {
                                    // too late
                                    src.unget(tkn);
                                }
                            }
                            assert(0==tkn.size);
                        }

                        //! done
                        src.skip(best_token.size);
                        if( best_rule->apply(best_token) )
                        {
                            std::cerr << "<" << name << ">[" << best_rule->label << "]='" << best_token << "' @" << iline << std::endl;
                            lexeme *lx = new lexeme(best_rule->label,iline);
                            lx->swap_with(best_token);
                            return lx;
                        }
                        else
                        {

                        }
                        goto TRY_PRODUCE;
                    }
                }
            }
            
        }
        
    }
    
}
