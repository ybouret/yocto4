#ifndef YOCTO_STATISTICAL_INCLUDED
#define YOCTO_STATISTICAL_INCLUDED 1


#include "yocto/math/stat/descr.hpp"
#include "yocto/math/fcn/functions.hpp"
#include "yocto/math/stat/dist.hpp"

//#include "yocto/container/pair.hpp"
//#include "yocto/memory/malloc.hpp"
//#include "yocto/sort.hpp"
#include "yocto/code/utils.hpp"
//#include "yocto/core/base/hsort.h"

namespace yocto {
	
	namespace math {
		
		//! Student t-test, equal variances
		/**
		 assume data1 and data2 have the same true variance.
		 return the significance level p (the smaller, the better)
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T test_t( const SEQ1 &data1, const SEQ2 &data2, T &t )
		{
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1);
			compute_average_and_variance<SEQ2>( ave2, var2, data2);
			const size_t n1 = data1.size();
			const size_t n2 = data2.size();
			const size_t df = (n1+n2)-2;
			const T      sd = ( (n1-1) * var1 + (n2-1) * var2 ) / df;
			t = (ave1-ave2)/std::sqrt( sd * ( T(1)/n1 + T(1)/n2) );
			return beta_i( T(0.5) * df, T(0.5), df / ( T(df) + t*t ) );
		}
		
		//! Student confidence interval, equal variances
		/**
		 assume data1 and data2 have the same true variance.
		 returns c so that delta_ave(true)=delta_ave(empiric) +/- c/2.
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T ci_test_t( const SEQ1 &data1, const SEQ2 &data2, const T confidence )
		{
			assert( confidence > 0 );
			assert( confidence < 1 );
			const T alpha = T(1) - confidence;
			const T half  = alpha/2;
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1);
			compute_average_and_variance<SEQ2>( ave2, var2, data2);
			const size_t n1 = data1.size();
			const size_t n2 = data2.size();
			const size_t df = (n1+n2)-2;
			const T      sd = ( (n1-1) * var1 + (n2-1) * var2 ) / df;
			const T      s  = std::sqrt( sd * ( T(1)/n1 + T(1)/n2) );
			const T      l  = student<T>::icdf( df, half );
			return       s * ( T(1) - (l+l) );
		}
		
		//! Student t-test, unequal variances
		/**
		 assume data1 and data2 have different true variances.
		 return the significance level p (the smaller, the better)
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T test_tu( const SEQ1 &data1, const SEQ2 &data2, T &t )
		{
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1 );
			compute_average_and_variance<SEQ2>( ave2, var2, data2 );
			const size_t n1 = data1.size();
			const size_t n2 = data2.size();
			const T      s1 = var1/n1;
			const T      s2 = var2/n2;
			const T      sa = s1+s2;
			const T      df = (sa*sa)/( (s1*s1)/(n1-1) + (s2*s2)/(n2-1) );
			t = (ave1-ave2) / std::sqrt( sa );
			return beta_i( T(0.5) * df, T(0.5), df / ( df + t*t ) );
		}
		
		
		//! Student confidence interval, unequal variances
		/**
		 assume data1 and data2 have different true variances.
		 returns c so that delta_ave(true)=delta_ave(empiric) +/- c/2.
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T ci_test_tu( const SEQ1 &data1, const SEQ2 &data2, const T confidence )
		{
			assert( confidence > 0 );
			assert( confidence < 1 );
			const T alpha = T(1) - confidence;
			const T half  = alpha/2;
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1 );
			compute_average_and_variance<SEQ2>( ave2, var2, data2 );
			const size_t n1 = data1.size();
			const size_t n2 = data2.size();
			const T      s1 = var1/n1;
			const T      s2 = var2/n2;
			const T      sa = s1+s2;
			const T      df = (sa*sa)/( (s1*s1)/(n1-1) + (s2*s2)/(n2-1) );
			const T      s  = std::sqrt( sa );
			const T      l  = student<T>::icdf( df, half );
			return       s * ( T(1) - (l+l) );
		}
		
		
		
		//! Student t-test, paired sample
		/**
		 assume data1 and data2 are pair related samples
		 return the significance level p (the smaller, the better)
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T test_tp( const SEQ1 &data1, const SEQ2 &data2, T &t )
		{
			assert( data1.size() == data2.size() );
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1 );
			compute_average_and_variance<SEQ2>( ave2, var2, data2 );
			const size_t n  = data1.size();
			const size_t df = n-1;
			T cov = 0;
			typename SEQ1::const_iterator p1 = data1.begin();
			typename SEQ2::const_iterator p2 = data2.begin();
			for( size_t i=n; i>0; --i, ++p1, ++p2 )
			{
				cov += ( *p1 - ave1 ) * ( *p2 - ave2 );
			}
			cov /= df;
			const T sd = std::sqrt( (var1+var2-(cov+cov))/n );
			t = (ave1-ave2) / sd;
			return beta_i( T(0.5) * df, T(0.5), df / ( T(df) + t*t ) );
		}
		
		//! Student t-test, paired sample
		/**
		 assume data1 and data2 are pair related samples
		 returns c so that delta_ave(true)=delta_ave(empiric) +/- c/2.
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T ci_test_tp( const SEQ1 &data1, const SEQ2 &data2, const T confidence )
		{
			assert( data1.size() == data2.size() );
			assert( confidence > 0 );
			assert( confidence < 1 );
			const T alpha = T(1) - confidence;
			const T half  = alpha/2;
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1 );
			compute_average_and_variance<SEQ2>( ave2, var2, data2 );
			const size_t n  = data1.size();
			const size_t df = n-1;
			T cov = 0;
			typename SEQ1::const_iterator p1 = data1.begin();
			typename SEQ2::const_iterator p2 = data2.begin();
			for( size_t i=n; i>0; --i, ++p1, ++p2 )
			{
				cov += ( *p1 - ave1 ) * ( *p2 - ave2 );
			}
			cov /= df;
			const T      s  = std::sqrt( (var1+var2-(cov+cov))/n );
			const T      l  = student<T>::icdf( df, half );
			return       s * ( T(1) - (l+l) );
		}
		
		
		
		//! Fisher test, test variances are different
		/**
		 small values indicates that variances are different
		 */
		template <
		class T,
		class SEQ1,
		class SEQ2
		>
		inline T test_f( const SEQ1 &data1, const SEQ2 &data2, T &f )
		{
			T ave1,var1,ave2,var2;
			compute_average_and_variance<SEQ1>( ave1, var1, data1 );
			compute_average_and_variance<SEQ2>( ave2, var2, data2 );
			const size_t n1 = data1.size();
			const size_t n2 = data2.size();
			size_t df1;
			size_t df2;
			if( var1 > var2 )
			{
				f   = var1/var2;
				df1 = n1-1;
				df2 = n2-1;
			}
			else
			{
				f   = var2/var1;
				df1 = n2-1;
				df2 = n1-1;
			}
			const T proba = T(2.0) * beta_i( T(0.5) * df2, T(0.5) * df1, ( df2 / ( T(df2) + T(df1) * f ) ) );
			return proba > T(1.0) ? T(2.0) - proba : proba;
		}
		
		
		template <class T>	T probks( T x ) throw();
		
		//! KS test
		/**
		 assume data are sorted!
		 */
		template <class T, class SEQ>
		inline T test_ks( const SEQ &data, typename numeric<T>::function &func, T &d )
		{
			assert( data.size() > 0 );
			const size_t n = data.size();
			T en = n;
			T fo = 0;
			d    = 0;
			typename SEQ::const_iterator p = data.begin();
			for( size_t j=1;j<=n;++j,++p)
			{
				const T fn = j/en;
				const T ff = func( *p );
				const T dt = max_of<T>( std::fabs( fo-ff ), std::fabs(fn-ff) );
				if( dt > d ) d = dt;
				fo = fn;
			}
			en = Sqrt(en);
			return probks( (en+T(0.12)+T(0.11)/en) * d );
		}
		
#if 0
		namespace kernel {
			template <class T>
			inline int compare_wilcoxon( const pair<T,bool> &lhs, const pair<T,bool> &rhs ) throw()
			{
				const T &L = lhs.first;
				const T &R = rhs.first;
				return L < R ? -1 : ( R < L ? 1 : 0 );
			}
		}
		
		//! Wilcoxon Mann Whithey
		/**
		 
		 */
		template <
		class T,
		class SEQX,
		class SEQY>
		inline T test_wilcoxon( const SEQX &X, const SEQY &Y, T &W )
		{
			const size_t nx  = X.size();
			const size_t ny  = Y.size();
			assert( nx > 0 );
			assert( ny > 0 );
			
			const size_t nt  = nx + ny;
			std::cout << "nx+ny=" << nt << std::endl;
			//-- build interleaved array
			typedef pair<T,bool> pair_t;
			size_t  nz = nt;
			pair_t *Z  = memory::global_acquire_as<pair_t>( nz );
			{
				pair_t *pZ = Z;
				{
					typename SEQX::const_iterator pX = X.begin();
					for( size_t i=nx;i>0;--i,++pX,++pZ)
					{
						pZ->first  = *pX;
						pZ->second = true;
					}
				}
				{
					typename SEQY::const_iterator pY = Y.begin();
					for( size_t i=ny;i>0;--i,++pY,++pZ)
					{
						pZ->first = *pY;
						pZ->second = false;
					}
				}
				assert( static_cast<size_t>(pZ-Z) == nt );
				
			}
			
			sort<pair_t>::with( YCE_HSort, kernel::compare_wilcoxon<T>,Z,nt);
			//-- compute Wilcoxon sum-rank Wx
			size_t  Wx = 0;
			--Z;
			for( size_t r=1; r <= nt; ++r )
			{
				if( Z[r].second )
					Wx += r;
			}
			memory::global_release_as<pair_t>( ++Z );
			
			//-- compute statistics
			const size_t n2 = nx * (nt+1);
			W =  (T(Wx) - T(n2)/T(2))/std::sqrt( T(n2 * ny )/T(12) );
			
			return gaussian<T>::cdf( W, 0, 1);
		}
#endif
		
		
	}
	
}

#endif

