#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace logical
		{
			
			////////////////////////////////////////////////////////////////////
			Operator:: ~Operator() throw()
			{
			}
			
			Operator:: Operator( uint32_t t ) throw() : pattern( t ), operands()
			{
				//data = & operands;
			}
			
			Operator:: Operator( const Operator &op ) : pattern( op.type ), operands( op.operands )
			{
				//data = & operands;
			}
			
			void Operator:: clear() throw()
			{
				this->release();
				for( pattern *p = operands.head; p; p=p->next)
				{
					p->release();
				}
			}
			
			void     Operator:: brx( ios::ostream &os ) const
			{
				os.emit(type);
				os.emit<uint16_t>( operands.size );
				for( const pattern *p = operands.head; p; p=p->next )
				{
					p->brx(os);
				}
			}
			
			////////////////////////////////////////////////////////////////////
			AND * AND::create() { return new AND(); }
			AND:: AND() throw() : Operator( id )
			{
				AND *ptr = this;
				data     = ptr;
			}
			
			AND:: AND( const AND &op ) : Operator( op )
			{
				AND *ptr = this;
				data     = ptr;
			}
			
			AND:: ~AND() throw()
			{
				
			}
			
			bool AND:: accept( source & src )
			{
				assert( 0 == size );
				for( pattern *p = operands.head; p; p=p->next )
				{
					assert( 0 == p->size );
					if( p->accept( src ) )
					{
						p->move_at_tail_of( *this );
						assert( 0 == p->size );
					}
					else 
					{
						src.unget( *this );
						assert(0==size);
						return false;
					}
					
				}
				return true;
			}
			
			void AND:: gather( first_chars &fch ) const
			{
				assert( 0 == fch.size() );
				fch.accept_empty  = true;
				for( const pattern *p  = operands.head; p; p=p->next )
				{
					first_chars sub;
					p->gather(sub);
					fch.merge( sub );
					if( ! sub.accept_empty )
					{
						fch.accept_empty = false;
						return;
					}
				}
				
			}
			
			////////////////////////////////////////////////////////////////////
			OR * OR::create() { return new OR(); }
			
			OR:: OR() throw() : Operator( id )
			{
				OR *ptr = this;
				data    = ptr;
			}
			
			OR:: OR( const OR &op ) : Operator( op )
			{
				OR *ptr = this;
				data    = ptr;
			}
			
			OR:: ~OR() throw()
			{
				
			}
			
			bool OR:: accept( source & src )
			{
				assert( 0 == size );
				for( pattern *p = operands.head; p; p=p->next )
				{
					assert( 0 == p->size );
					if( p->accept( src ) )
					{
						p->move_at_tail_of( *this );
						assert( 0 == p->size );
						return true;
					}
				}
				return false;
			}
			
			void OR:: gather( first_chars &fch ) const
			{
				assert( 0 == fch.size() );
				fch.accept_empty = true;
				for( const pattern *p  = operands.head; p; p=p->next )
				{
					first_chars sub;
					p->gather(sub);
					fch.merge( sub );
					if( ! sub.accept_empty )
						fch.accept_empty = false;
				}
			}
			
			////////////////////////////////////////////////////////////////////
			NONE * NONE::create() { return new NONE(); }
			
			NONE:: NONE() throw() : Operator( id )
			{
			}
			
			NONE:: NONE( const NONE &op ) : Operator( op )
			{
			}
			
			NONE:: ~NONE() throw()
			{
				
			}
			
			bool NONE:: accept( source & src )
			{
				assert( 0 == size );
				for( pattern *p = operands.head; p; p=p->next )
				{
					assert( 0 == p->size );
					if( p->accept( src ) )
					{
						src.unget( *p );
						assert( 0 == p->size );
						return false;
					}
				}
				assert(0==size);
				t_char *ch = src.get();
				if( ch )
				{
					push_back( ch );
					return true;
				}
				else
					return false;
			}
			
			
			void NONE:: gather( first_chars &fch ) const
			{
				assert( 0 == fch.size() );
				for( size_t i=0; i < 256; ++i )
					(void) fch.insert( i );
				for( const pattern *p = operands.head; p; p=p->next )
				{
					first_chars sub;
					p->gather(sub);
					for( symbols::const_iterator i = sub.begin(); i != sub.end(); ++i )
					{
						(void) fch.remove( *i ); 
					}
					// TODO: check sub.accept_empty == false ?
				}
				fch.accept_empty = ( fch.size() == 0 );
			}
			
		}
		
	}
}

#include "yocto/rx/pattern/basic.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace logical
		{
			
			////////////////////////////////////////////////////////////////////
			static inline void op_fill( p_list &op, const string &s )
			{
				for( size_t i=0; i < s.size(); ++i )
				{
					op.push_back( basic::single::create( s[i] ) );
				}
			}
			
			Operator *EQUAL( const string &s )
			{
				auto_ptr<Operator> p( AND::create() );
				op_fill( p->operands, s);
				return p.yield();
			}
			
			Operator *AMONG( const string &s )
			{
				auto_ptr<Operator> p( OR::create() );
				op_fill( p->operands, s);
				return p.yield();
			}
		
			Operator *EXCEPT( const string &s )
			{
				auto_ptr<Operator> p( NONE::create() );
				op_fill( p->operands, s);
				return p.yield();
			}

			
		}
		
	}
	
}
