#include "yocto/lang/pattern/compiler.hpp"

#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/posix.hpp"


#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

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
                void Process()
                {
                    
                    while(curr<last)
                    {
                        char C = *curr;
                        
                        switch(C)
                        {
                               
                            default:
                                
                                ++curr;
                                
                        }
                    }
                }
                
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);
			};
		}
        
        
		pattern *compile( const string &expr, const p_dict *dict)
		{
			Compiler CC(expr.c_str(),expr.c_str()+expr.size(),dict);
            CC.Process();
			return CC.stack.pop_back();
		}
        
		pattern *compile( const char   *expr, const p_dict *dict)
		{
			const string Expr(expr);
			return compile(Expr,dict);
		}
        
	}
}
