#ifndef YOCTO_RX_LEX_ENGINE
#define YOCTO_RX_LEX_ENGINE 1

#include "yocto/rx/lexical/rule.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace lexical
		{
			
			class stop;
			
			class engine : public object
			{
			public:
				
				explicit engine();
				virtual ~engine() throw();
				
				void on( pattern *p, rule *r ) throw(); //!< register pattern => rule
				bool run( source &src );
				void reset( source &src ) throw();
				
				
				//==============================================================
				//-- make
				//==============================================================
				inline void make( pattern *p, const action &a )
				{
					try { on(p, make::create(a) ); }
					catch (...) { delete p; throw; }
				}
				
				inline void make( const string &expr, const action &a, const pattern_db *dict= NULL )
				{
					this->make( compile( expr, dict ), a );
				}
				void make( const char   *expr, const action &a, const pattern_db *dict = NULL )
				{
					this->make( compile( expr, dict ), a );
				}
				
				//==============================================================
				//-- stop
				//==============================================================
				inline void stop( pattern *p, const action &a )
				{
					try { on(p, stop::create(*this,a) ); }
					catch (...) { delete p; throw; }
				}
				
				inline void stop( const string &expr, const action &a, const pattern_db *dict= NULL )
				{
					this->stop( compile( expr, dict ), a );
				}
				void stop( const char   *expr, const action &a, const pattern_db *dict = NULL )
				{
					this->stop( compile( expr, dict ), a );
				}
				
				
				
			private:
				p_list   accepters_;  //!< acceptable pattern
				r_list   callbacks_;  //!< callback to perform
				source  *source_;     //!< active source
				bool     stop_;       //!< manual stop by token => success
				friend class stop;
				YOCTO_DISABLE_COPY_AND_ASSIGN(engine);
				
			};
			
		}
	}
	
}


#endif
