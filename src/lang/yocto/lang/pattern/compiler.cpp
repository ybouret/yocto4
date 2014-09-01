#include "yocto/lang/pattern/compiler.hpp"

namespace yocto
{
	namespace lang
	{
        
		namespace
		{
			class Compiler
			{
			public:
                
				const char   *curr;
				const char   *last;
				const p_dict *dict;
				p_list        stack;
                int           level;
                
				inline ~Compiler() throw()
				{
                    
				}
                
                //--------------------------------------------------------------
                //
                // ctor
                //
                //--------------------------------------------------------------
				inline Compiler(const char   *usrCurr,
                                const char   *usrLast,
                                const p_dict *usrDict) throw():
				curr(usrCurr),
                last(usrLast),
                dict(usrDict),
                stack(),
                level(0)
				{
                    
				}
                
                //--------------------------------------------------------------
                //
                // Main Loop
                //
                //--------------------------------------------------------------
                inline void Process()
                {
                    
                    
                }
                
                
                
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);
			};
		}
        
        
		pattern *compile( const string &expr, const p_dict *dict)
		{
			Compiler CC(expr.c_str(),expr.c_str()+expr.size(),dict);
			return 0;
		}
        
		pattern *compile( const char   *expr, const p_dict *dict)
		{
			const string Expr(expr);
			return compile(Expr,dict);
		}
        
	}
}
