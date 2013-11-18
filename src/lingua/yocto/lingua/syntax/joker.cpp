#include "yocto/lingua/syntax/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            ////////////////////////////////////////////////////////////////////
            //
            //
            //
            ////////////////////////////////////////////////////////////////////
            joker:: ~joker() throw() {}
            
            joker:: joker( const string &id, rule &r) :
            rule(id),
            handle( &r )
            {
            }
            
            
            ////////////////////////////////////////////////////////////////////
            //
            // OPTIONAL
            //
            ////////////////////////////////////////////////////////////////////
            optional:: optional( const string &id, rule &r) : joker(id,r)
            {
            }
            
            optional:: ~optional() throw()
            {
            }
            
            Y_SYNTAX_MATCH_TYPE optional:: match( Y_SYNTAX_MATCH_ARGS)
            {
                check(Tree);
                assert(handle);
                //std::cerr << "?OPT@" << label << ": " << handle->label << std::endl;
                xnode *subTree = 0;
                if( handle->match(Lexer, Source, subTree) && subTree != 0)
                {
                    grow(Tree, subTree);
                }
                return true;
            }
            
            
            ////////////////////////////////////////////////////////////////////
            //
            // REPEATING
            //
            ////////////////////////////////////////////////////////////////////
            repeating:: repeating( const string &id, rule &r,size_t n) :
            joker(id,r),
            at_least(n)
            {
            }
            
            repeating:: ~repeating() throw() {}
            
            Y_SYNTAX_MATCH_TYPE repeating:: match( Y_SYNTAX_MATCH_ARGS)
            {
                check(Tree);
                assert(handle);
                //std::cerr << "?REP@" << label << ">=" << at_least << ": " << handle->label << std::endl;
        
                xnode *           rep = xnode::create(label, 0, syntax::is_merging_all );
                auto_ptr<xnode> p(rep);
                size_t  count = 0;
                
                while( handle->match(Lexer, Source, rep) )
                {
                    ++count;
                }
                assert(rep->children().size == count);
                
                if(count>=at_least)
                {
                    //std::cerr << "+REP@" << label << " #" << count << " " << handle->label << std::endl;
                    p.forget();
                    grow(Tree,rep);
                    return true;
                }
                else
                {
                    //std::cerr << "-REP@" << label << " #" << count << " " << handle->label << std::endl;
                    xnode::restore(Lexer, p.yield());
                    return false;
                }
            }
            
        }
    }
}

