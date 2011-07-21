#include "yocto/rx/pattern/joker.hpp"
#include "yocto/rx/source.hpp"

#include "yocto/code/swap.hpp"

namespace yocto
{
	namespace regex
	{
		
		namespace 
		{
			
			class jk_pattern : public pattern
			{
			public:
				virtual ~jk_pattern() throw() { assert(ppat); delete ppat; }
				
				
				
				virtual void sendto( source &src ) throw() 
				{
					this->back_to( src.pool );
					ppat->back_to( src.pool );
				}
				
				virtual bool is_valid( size_t num ) const throw() = 0;
				
				virtual bool accept( source &src )
				{
					npat = 0;
					assert( 0 == size );
					assert( NULL != ppat );
					assert( 0 == ppat->size );
					while( ppat->accept(src) )
					{
						ppat->at_tail( *this );
						++npat;
					}
					if( is_valid( npat ) )
					{
						return true;
					}
					else
					{
						src.unget( *this );
						assert( 0 == size );
						return false;
					}
					
				}
				
			protected:
				explicit jk_pattern( uint32_t t, pattern *p ) throw() :
				pattern(t),
				ppat(p),
				npat(0)
				{
				}
				pattern *ppat;
				size_t   npat;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(jk_pattern);
			};
			
			
			class jk_at_least : public jk_pattern 
			{
			public:
				const size_t num;
				virtual ~jk_at_least() throw() {}
				
				explicit jk_at_least( pattern *p, size_t n ) throw() :
				jk_pattern( joker::_at_least, p ),
				num( n )
				{
					data = (void*)&num;
				}
				
				virtual pattern *clone() const
				{
					pattern *p = ppat->clone();
					try
					{
						return new jk_at_least(p,num);
					}
					catch(...)
					{
						delete p;
						throw;
					}
				}
				
				virtual bool is_valid( size_t n ) const throw() { return n >= num; }
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(jk_at_least);
			};
			
			
			class jk_counting : public jk_pattern 
			{
			public:
				const size_t nmin;
				const size_t nmax;
				virtual ~jk_counting() throw() {}
				
				explicit jk_counting( pattern *p, size_t amin, size_t amax ) throw() :
				jk_pattern( joker::_counting, p ),
				nmin( amin ),
				nmax( amax )
				{
					assert(nmin<=nmax);
					data = (void*)&nmin;
				}
				
				virtual pattern *clone() const
				{
					pattern *p = ppat->clone();
					try
					{
						return new jk_counting(p,nmin,nmax);
					}
					catch(...)
					{
						delete p;
						throw;
					}
				}
				
				virtual bool is_valid( size_t n ) const throw() { return n>=nmin && n <=nmax; }
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(jk_counting);
			};
			
			
			
		}
		
		
		
		pattern * joker:: at_least( pattern *p, size_t n )
		{
			try
			{
				return new jk_at_least(p,n);
			}
			catch(...)
			{
				delete p;
				throw;
			}
			
		}
		
		pattern *joker:: counting( pattern *p, size_t nmin, size_t nmax )
		{
			try
			{
				if( nmin > nmax )
				{
					cswap(nmin,nmax);
				}
				return new jk_counting(p,nmin,nmax);
			}
			catch(...)
			{
				delete p;
				throw;
			}
		}
		
		
		
	}
}