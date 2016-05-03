#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    namespace lang
    {

        namespace lexical
        {
            lexeme * scanner:: get(source &src, ios::istream &fp, bool &ctrl)
            {
            TRY_PRODUCE:

                if(!src.peek(fp))
                {
                    return 0; //EOF
                }
                else
                {
                    //______________________________________________________
                    //
                    // There are some chars ahead: find a first matching
                    // rule
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
                        throw imported::exception("syntax error", "%d:<%s> unexpected char '%c'", line, name.c_str(), char(C) );
                    }

                    //______________________________________________________
                    //
                    // Check other rules
                    //______________________________________________________
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

                    //______________________________________________________
                    //
                    // Done
                    //______________________________________________________
                    assert(src.size>=best_token.size);

                    src.skip(best_token.size); // forward
                    ctrl = best_rule->ctrl;    // set if control rule
                    if( best_rule->apply(best_token) )
                    {
                        if(ctrl) throw exception("control rule '%s' MUST return FALSE", best_rule->label.c_str());
                        //std::cerr << "<" << name << ">[" << best_rule->label << "]='" << best_token << "' @" << line << std::endl;
                        lexeme *lx = new lexeme(best_rule->label,line);
                        lx->swap_with(best_token);
                        return lx;
                    }
                    else
                    {
                        if(ctrl)
                        {
                            // something has changed, return to lexer...
                            return 0;
                        }
                        else
                        {
                            // it was discarded
                        }
                    }
                    goto TRY_PRODUCE;
                }
            }

        }

    }

}
