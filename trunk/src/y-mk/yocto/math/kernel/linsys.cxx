#include "yocto/math/kernel/linsys.hpp"
#include "yocto/math/ztype.hpp"

#include "yocto/memory/global.hpp"
#include "yocto/memory/embed.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/core/offset.hpp"


namespace yocto 
{
	namespace math
	{
		
		template <>
		linsys<z_type> :: linsys() throw() :
		size_(0),
		item_(NULL),
		scal_(NULL),
		indx_(NULL),
		buffer_(NULL),
		buflen_(0),
		dneg( false )
		{
		}
		
		template <>
		linsys<z_type>:: ~linsys() throw()
		{
			
			memory::kind<memory::global>::release( buffer_, buflen_ );
		}
		
		template <>
		linsys<z_type> :: linsys( size_t n )  : 
		size_( n ), 
		item_( NULL ),
		scal_( NULL ),
		indx_( NULL ),
		buffer_(  NULL ),
		buflen_( 0 ),
		dneg( false )
		{
			if( size_ )
			{
				memory::embed reg[] =
				{
					YOCTO_EMBED( item_, size_ ),
					YOCTO_EMBED( indx_, size_ )
				};
				const size_t num = sizeof(reg)/sizeof(reg[0]);
				buflen_ = memory::embed::prepare( reg, num );
				buffer_ = memory::kind<memory::global>::acquire( buflen_ );
				memory::embed::dispatch( reg, num, buffer_ );
				scal_ = (real_type *) &item_[0];
				--item_;
				--scal_;
				--indx_;
			}
		}
		
		template <>
		size_t linsys<z_type>:: size() const throw()
		{ return size_; }
		
		template <>
		size_t linsys<z_type>:: capacity() const throw()
		{ return size_; }
		
		
		template <>
		const char * linsys<z_type>:: name() const throw()
		{ return "linsys"; }
		
		
		
		template <>
		void linsys<z_type>:: swap_with( linsys &L ) throw()
		{
			memswap( (void *) & size_, (void *) & L.size_, sizeof( linsys ) - YOCTO_OFFSET_OF(linsys,size_) );
		}
		
		template <>
		void linsys<z_type>:: free() throw()
		{
			
		}
		
		template <>
		void linsys<z_type>:: release() throw()
		{
			linsys<z_type> L;
			swap_with(L);
		}
		
		template <>
		void linsys<z_type>:: reserve(size_t n)
		{
			if( n > 0 )
			{
				linsys<z_type> L( size_ + n );
				swap_with(L);
			}
		}
		
		
		
		template <>
		void linsys<z_type>:: ensure( size_t n )
		{
			if( size_ < n )
			{
				linsys<z_type> L( n );
				swap_with(L);
			}
		}
		
		template <>
		bool linsys<z_type>::  LU( matrix<z_type> &a ) throw()
		{
			assert( a.cols   > 0   );
			assert( a.is_square()  );
			assert( a.rows <= size_ );
			dneg = false;
			const size_t n = a.rows;
			
			//------------------------------------------------------------------
			//
			// initialize implicit pivots
			//
			//------------------------------------------------------------------
			
			for( size_t i=n;i>0;--i) 
			{
				const matrix<z_type>::row & a_i = a[i];
				real_t                      piv = 0; 
				
				for( size_t j=n;j>0;--j)
				{
					const real_t tmp = Fabs( a_i[j] );
					if ( tmp > piv ) 
						piv = tmp;
				}
				if( piv <= REAL_MIN )
				{
					//std::cerr << "-- LU failure level-1" << std::endl;
					return false;
				}
				scal_[i] = real_t(1)/piv; 
			}
			
			
			//------------------------------------------------------------------
			// Crout's algorithm
			//------------------------------------------------------------------
			for(size_t j=1;j<=n;++j) 
			{ 
				for(size_t i=1;i<j;++i)
				{ 
					matrix<z_type>::row &a_i = a[i];
					z_type               sum = a_i[j];
					for(size_t k=1;k<i;++k) 
						sum -= a_i[k]*a[k][j];
					a_i[j]=sum;
				}
				
				real_t piv  = 0; 
				size_t imax = j;
				for( size_t i=j;i<=n;i++) 
				{ 
					matrix<z_type>::row &a_i = a[i];
					z_type sum=a_i[j];
					for(size_t k=1;k<j;++k)
						sum -= a_i[k]*a[k][j];
					a_i[j]=sum;
					const real_t tmp = scal_[i]*Fabs(sum);
					if( tmp >= piv ) 
					{
						piv  = tmp;
						imax = i;
					}
				}
				
				//-- TODO: check ?
				//assert( piv > 0 );
				assert( imax> 0 );
				if (j != imax) 
				{ 
					a.swap_rows( j, imax );
					dneg = !dneg;
					scal_[imax]=scal_[j];
				}
				
				indx_[j]=imax;
				
				if( Fabs(a[j][j]) <= REAL_MIN ) 
				{
					//std::cerr << "-- LU failure level-2" << std::endl;
					return false;
				}
				
				if (j != n) 
				{ 
					const z_type fac = z_type(1)/(a[j][j]);
					for(size_t i=j+1;i<=n;++i)
						a[i][j] *= fac;
				}
			}
			
#if !defined(NDEBUG)
			for( size_t i=1; i <= n; ++i )
			{
				assert( indx_[i] >  0 );
				assert( indx_[i] <= n );
			}
#endif
			
			return true;
		}
		
		
#if 0
		template <>
		void linsys<z_type>:: operator()( const matrix<z_type> &a, z_type *b ) const throw()
		{
			assert( a.cols   > 0   );
			assert( a.is_square()  );
			assert( a.rows <= size );
			assert( b != NULL );
			
			const size_t n  = a.rows;
			size_t       ii = 0;
			
			for(size_t i=1;i<=n;++i) 
			{
				const size_t ip   = indx_[i]; assert(ip>0);
				z_type        sum = b[ip];
				
				b[ip]=b[i];
				if(ii)
				{
					for( size_t j=ii; j<= i-1; ++j) 
						sum -= a[i][j]*b[j];
				}
				else
				{
					if( Fabs(sum) > 0 ) 
						ii=i;
				}
				b[i]=sum;
			}
			
			for(size_t i=n;i>0; --i) 
			{ 
				z_type sum=b[i];
				for(size_t j=i+1;j<=n;++j) 
					sum -= a[i][j]*b[j];
				b[i]=sum/a[i][i]; 
			}
		}
#endif
		
		
		template <>
		void linsys<z_type>:: operator()( const matrix<z_type> &a, array<z_type> &b ) const throw()
		{
			assert( a.cols   > 0   );
			assert( a.is_square()  );
			assert( a.rows <= size_ );
			assert( b.size() == a.rows );
			
			const size_t n  = a.rows;
			size_t       ii = 0;
			
			for(size_t i=1;i<=n;++i) 
			{
				const size_t  ip  = indx_[i]; assert(ip>0);
				z_type        sum = b[ip];
				
				b[ip]=b[i];
				if(ii)
				{
					for( size_t j=ii; j<= i-1; ++j) 
						sum -= a[i][j]*b[j];
				}
				else
				{
					if( Fabs(sum) > 0 ) 
						ii=i;
				}
				b[i]=sum;
			}
			
			for(size_t i=n;i>0; --i) 
			{ 
				z_type sum=b[i];
				for(size_t j=i+1;j<=n;++j) 
					sum -= a[i][j]*b[j];
				b[i]=sum/a[i][i]; 
			}
		}
		
#if 0
		template <>
		z_type &  linsys<z_type>:: operator[]( size_t i ) throw()
		{
			assert( i > 0 );
			assert( i <= size_ );
			assert( item_ != NULL );
			return item_[ i  ];
		}
		
		template <>
		const z_type &  linsys<z_type>:: operator[]( size_t i ) const throw()
		{
			assert( i > 0 );
			assert( i <= size_ );
			assert( item_ != NULL );
			return item_[ i  ];
		}
		
		template <>
		z_type * linsys<z_type>:: get_item() throw()
		{
			return item_;
		}
#endif

		
		template <>
		linsys<z_type>::const_type * linsys<z_type>:: get_item() const throw()
		{
			return item_;
		}
		
		template <>
		void linsys<z_type>:: operator()( const matrix<z_type> &M,  matrix<z_type> &Q ) const throw()
		{
			assert( M.cols   > 0     );
			assert( M.is_square()    );
			assert( M.rows <= size_  );
			assert( Q.rows == M.rows );
			
			const size_t n = M.rows;
			const size_t m = Q.cols;
			
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=1;i<=n;++i) 
				{
					const size_t ip           = indx_[i];
					z_type                sum = Q[ip][k];
					
					Q[ip][k]=Q[i][k];
					
					for( size_t j=1; j<i;j++) 
						sum -= M[i][j]*Q[j][k];
					
					Q[i][k]=sum;
				}
			}
			
			for( size_t k = m; k>0; --k )
			{
				for(size_t i=n;i>=1;i--) 
				{ 
					z_type                sum = Q[i][k];
					for(size_t j=i+1;j<=n;j++) 
						sum -= M[i][j]*Q[j][k];
					Q[i][k]=sum/M[i][i]; 
				}
			}
			
		}
		
		
		template <>
		void linsys<z_type>:: operator()(  const matrix<z_type> &a ) throw()
		{
			this->operator()( a, *this );
		}
		
		
	}
}
