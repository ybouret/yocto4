#include "yocto/lang/syntax/logic.hpp"


namespace yocto
{
	namespace lang
	{
        
		namespace syntax
		{
            
			logical:: ~logical() throw()
			{
                
			}
            
			logical:: logical( const string &id ) :
			rule(id),
            operands()
			{
			}
            
			void logical:: append( rule &sub )
			{
				operands.push_back( new operand(sub) );
			}
            
			logical:: operand:: operand( rule &r ) throw() :
			sub(r),
            next(0),
            prev(0)
			{
			}
            
			logical:: operand:: ~operand() throw() {}
            
			logical & logical:: operator<<(rule&sub)
			{
				append(sub);
				return *this;
			}
            
		}
        
	}
    
}

#include "yocto/exception.hpp"

namespace yocto
{
	namespace lang
	{
        
		namespace syntax
		{
			aggregate:: aggregate(const string &id) :
            logical(id)
            {
            }
            
            aggregate:: ~aggregate() throw()
            {
                
            }
            
            
            aggregate & aggregate:: operator+=( rule &r )
            {
                append(r);
                return *this;
            }
            
            YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(aggregate)
            {
                check(Tree);
                if(operands.size<=0)
                {
                    throw exception("empty syntax::aggregate '%s')", label.c_str() );
                }
                
                syntax::xtree SubTree = syntax::xnode::create(label,is_regular);
                syntax::x_ptr guard(SubTree);
                
                //______________________________________________________________
                //
                // try to accept all of them
                //______________________________________________________________
                for(operand *op = operands.head; op; op=op->next )
                {
                    syntax::xtree Node = 0;
                    if( ! op->sub.match(Lexer, Source, Input, Node) )
                    {
                        assert(0==Node);
                        guard.forget();
                        syntax::xnode::restore(Lexer,SubTree);
                        return false;
                    }
                }
                
                //______________________________________________________________
                //
                // all were accepted
                //______________________________________________________________
                guard.forget();
                grow(Tree,SubTree);
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
            
			alternate:: ~alternate() throw() {}
            
			alternate:: alternate(const string &id) :
			logical(id)
			{
			}
            
			alternate & alternate:: operator|=( rule &r )
			{
				append(r);
				return *this;
			}
            
			YOCTO_LANG_SYNTAX_RULE_MATCH_IMPL(alternate)
			{
                check(Tree);
				if(operands.size<=0)
				{
					throw exception("empty syntax::alternate '%s')", label.c_str() );
				}
                
                //______________________________________________________________
                //
				// try to accept one of them
                //______________________________________________________________
				for(operand *op = operands.head; op; op=op->next )
				{
                    syntax::xtree Node = 0;
					if(  op->sub.match(Lexer, Source, Input, Node) )
					{
                        grow(Tree,Node);
						return true;
					}
                    assert(0==Node);
				}
                
                //______________________________________________________________
                //
                // none were accepted
                //______________________________________________________________
				return false;
			}
            
		}
        
	}
    
}
