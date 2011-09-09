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
				template <typename HOST >
				inline void make( pattern *p, HOST &host, void (HOST:: *method)(const token & ) )
				{
					assert( p != NULL );
					assert( method != NULL );
					try 
					{
						const action todo( &host, method );
						rule        *r = make::create( todo );
						on( p, r );
					} catch(...) { delete p; throw; }
				}
				
				template <typename HOST >
				inline void make( const string &expr, HOST &host, void (HOST:: *method)(const token & ), const pattern_db *dict=NULL)
				{
					pattern *p = compile( expr, dict );
					this->make( p, host, method );
				}
				
				template <typename HOST >
				inline void make( const char *expr, HOST &host, void (HOST:: *method)(const token & ), const pattern_db *dict=NULL)
				{
					pattern *p = compile( expr, dict );
					this->make( p, host, method );
				}
							
				//==============================================================
				//-- stop
				//==============================================================
				template <typename HOST >
				inline void stop( pattern *p, HOST &host, void (HOST:: *method)(const token & ) )
				{
					assert( p != NULL );
					assert( method != NULL );
					try 
					{
						const action todo( &host, method );
						rule        *r = stop::create(*this, todo );
						on( p, r );
					} catch(...) { delete p; throw; }
				}
				
				template <typename HOST >
				inline void stop( const string &expr, HOST &host, void (HOST:: *method)(const token & ), const pattern_db *dict=NULL)
				{
					pattern *p = compile( expr, dict );
					this->stop( p, host, method );
				}
				
				template <typename HOST >
				inline void stop( const char *expr, HOST &host, void (HOST:: *method)(const token & ), const pattern_db *dict=NULL)
				{
					pattern *p = compile( expr, dict );
					this->stop( p, host, method );
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
