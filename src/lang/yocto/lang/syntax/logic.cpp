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
			if(operands.size<=0)
			{
				throw exception("empty syntax::aggregate '%s')", label.c_str() );
			}

			syntax::xtree SubTree = syntax::xnode::create(label);

			// try to accept all of them
			for(operand *op = operands.head; op; op=op->next )
			{
				if( ! op->sub.match(Lexer, Source, Input, SubTree) )
				{
					syntax::xnode::restore(Lexer,SubTree);
					return false;
				}
			}

			// done
			if( SubTree->children().size>0)
			{
				grow(Tree,SubTree);
			}
			else
			{
				delete SubTree;
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
				if(operands.size<=0)
				{
					throw exception("empty syntax::alternate '%s')", label.c_str() );
				}

				syntax::xtree Node = 0;

				// try to accept one of them
				for(operand *op = operands.head; op; op=op->next )
				{
					if(  op->sub.match(Lexer, Source, Input, Node) )
					{
						if(Node)
						{
							grow(Tree,Node);
						}
						return true;
					}
				}

				return false;
			}

		}

	}

}
