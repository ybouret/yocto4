#include "yocto/lang/syntax/joker.hpp"

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            joker:: ~joker() throw()
            {
                
            }
            
            joker:: joker( const string &id, rule &other ) :
            rule(id),
            sub(other)
            {
            }
            
            
        }
        
    }
    
}

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            optional:: ~optional() throw()
            {
                
            }
            
            optional:: optional(const string &id, rule &other ) :
            joker(id,other)
            {
            }
            
            YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(optional)
            {
                check(Tree);
                syntax::xtree Node = 0;
                if( sub.match(Lexer, Source, Input, Node))
                {
                    // node might be NULL
                    //std::cerr << " ==> OPT '" << label << "'" << std::endl;
                    grow(Tree,Node);
                }
                return true;
            }
        }
        
    }
    
}


namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            at_least:: ~at_least() throw()
            {
            }
            
            
            at_least:: at_least(const string &id, rule &other, size_t count ) :
            joker(id,other),
            value(count)
            {
                
            }
            
            YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(at_least)
            {
                check(Tree);
                xtree SubTree = xnode::create(label,syntax::is_standard);
                x_ptr guard(SubTree);
                
                for(;;)
                {
                    xtree Node=0;
                    if( sub.match(Lexer,Source,Input,Node) )
                    {
                        if(Node)
                        {
                            SubTree->add(Node);
                        }
                        continue;
                    }
                    else
                        break;
                }
                
                guard.forget();
                size_t nch = SubTree->count();
                if(nch>=value)
                {
                    //std::cerr << " ==> >=" << value <<  "'" << label << "'" << std::endl;
                    if(!Tree)
                    {
                        Tree = SubTree;
                    }
                    else
                    {
                        assert(false==Tree->terminal);
                        while(nch-->0)
                        {
                            Tree->add( SubTree->pop() );
                        }
                        delete SubTree;
                    }
                    return true;
                }
                else
                {
                    syntax::xnode::restore(Lexer, SubTree);
                    return false;
                }
                
            }
            
        }
        
    }
    
}

