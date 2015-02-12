//! \file

#ifndef YOCTO_STAT_DESCR_INCLUDED
#define YOCTO_STAT_DESCR_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto {
	
	namespace math {
		
		namespace kernel {
			
			
			template <
			typename T,
			typename ITERATOR
			>
			inline void compute_average( T &ave, ITERATOR begin, const ITERATOR end ) throw()
			{
				size_t n = 0;
				ave = 0;
				
				for( ITERATOR i = begin; i != end; ++i, ++n )
				{
					ave += (*i);
				}
				if(n>0)
					ave /= n;
			}
			
			template <
			class T,
			class ITERATOR
			>
			inline void compute_average_and_variance( T &ave, T &var, ITERATOR begin, const ITERATOR end ) throw()
			{
				size_t n = 0;
				ave = 0;
				var = 0;
				
				T sum_x  = 0;
				T sum_x2 = 0;
				for( ITERATOR i = begin; i != end; ++i, ++n )
				{
					const T x = *i;
					sum_x  += x;
					sum_x2 += x * x;
				}
				switch( n )
				{
					case 0:
						return;
						
					case 1:
						ave = sum_x/n;
						return;
						
					default:
						ave = sum_x/n;
						var = (sum_x2 - (sum_x * sum_x)/n) / (n-1);
				}
			}
			
			template <
			class T,
			class ITERATOR
			>
			inline void compute_average_and_stddev( T &ave, T &sig, ITERATOR begin, const ITERATOR end) throw()
			{
				compute_average_and_variance<T,ITERATOR>(ave,sig,begin,end);
				sig = Sqrt( sig );
			}
			
          
			
		} // kernel
		
		template <
		class SEQ
		>
		inline void compute_average( typename SEQ::mutable_type &ave, const SEQ &seq ) throw()
		{
			kernel::compute_average<typename SEQ::mutable_type, typename SEQ::const_iterator>( ave, seq.begin(), seq.end() );
		}
		
		template <
		class SEQ
		>
		inline void compute_average_and_variance( typename SEQ::mutable_type &ave, typename SEQ::mutable_type &var, const SEQ &seq ) throw()
		{
			kernel::compute_average_and_variance<typename SEQ::mutable_type, typename SEQ::const_iterator>( ave, var, seq.begin(), seq.end() );
		}
		
		template <
		class SEQ
		>
		inline void compute_average_and_stddev( typename SEQ::mutable_type &ave, typename SEQ::mutable_type &sig, const SEQ &seq ) throw()
		{
			kernel::compute_average_and_stddev<typename SEQ::mutable_type, typename SEQ::const_iterator>( ave, sig, seq.begin(), seq.end() );
		}
		
        template <
		class SEQ
		>
		inline void compute_median_and_absdev( typename SEQ::mutable_type &median, typename SEQ::mutable_type &absdev, const SEQ &seq ) throw()
		{
			const size_t n = seq.size();
            if( n > 0 )
            {
                typename SEQ::const_type __zero(0);
                typename SEQ::const_type __half(0.5);
                vector<typename SEQ::mutable_type> data(n,__zero);
                {
                    typename  SEQ::const_iterator j = seq.begin();
                    for( size_t i=1; i <= n; ++i, ++j ) data[i] = *j;
                }
                quicksort( data );
                const size_t im = n >> 1;
                if( 0 != (im & 1) )
                    median = data[im+1];
                else 
                    median = __half * ( data[im] + data[im+1] );
                absdev = 0;
                for( size_t i=n; i >0; --i ) 
                    absdev += Fabs( data[i] - median );
                absdev /= n;
            }
		}

		
		namespace kernel  {
			
			template <
			class T,
			class ITER_X,
			class ITER_Y
			>
			inline
			T compute_correlation( ITER_X pX, ITER_Y pY, const size_t n ) throw()
			{
				assert( n>0 );
				T sum_sq_x      = 0;
				T sum_sq_y      = 0;
				T sum_coproduct = 0;
				T mean_x        = *pX;
				T mean_y        = *pY;
				for( size_t i=2; i <= n; ++i ) {
					++pX;
					++pY;
					const T sweep   = T(i - 1) / i;
					const T delta_x = *pX - mean_x;
					const T delta_y = *pY - mean_y;
					sum_sq_x       += delta_x * delta_x * sweep;
					sum_sq_y       += delta_y * delta_y * sweep;
					sum_coproduct  += delta_x * delta_y * sweep;
					mean_x         += delta_x / i;
					mean_y         += delta_y / i;
				}
				const T pop_sd_x = Sqrt( sum_sq_x / n );
				const T pop_sd_y = Sqrt( sum_sq_y / n );
				const T cov_x_y  = sum_coproduct / n;
				return  cov_x_y / (pop_sd_x * pop_sd_y);
			}
			
		}
		
		template
		<
		class SEQ_X,
		class SEQ_Y
		>
		inline
		typename SEQ_X::mutable_type
		compute_correlation( const SEQ_X &seqx, const SEQ_Y &seqy ) throw()
		{
			assert( seqx.size() > 0 );
			assert( seqy.size() > 0 );
			assert( seqx.size() == seqy.size() );
			return kernel::compute_correlation<
			typename SEQ_X::mutable_type,
			typename SEQ_X::const_iterator,
			typename SEQ_Y::const_iterator
			>( seqx.begin(), seqy.begin(), seqx.size() );
			
		}
		
		
		//! append val[1..n] to histogram hst, depending on bin
		/**
		 assume that bins are increasingly sorted
		 */
		template
		<
		class SEQ_HST,
		class SEQ_BIN,
		class SEQ_VAL
		>
		inline
		void histogram( SEQ_HST &hst, const SEQ_BIN &bin, const SEQ_VAL &val )
		{
			assert( hst.size() > 0 );
			assert( bin.size() > 0 );
			assert( hst.size() == bin.size() );
			for( typename SEQ_VAL::const_iterator i = val.begin(); i != val.end(); ++i )
			{
				const typename SEQ_VAL::mutable_type   x = *i;
				typename       SEQ_HST::iterator       h = hst.begin();
				for( typename  SEQ_BIN::const_iterator j = bin.begin(); j != bin.end(); ++j, ++h )
				{
					if( x <= *j )
					{
						++(*h);
						break;
					}
				}
			}
		}
		
		//! build the cumulative distribution function
		/**
		 \brief assume z is increasingly sorted
		 */
		template
		<
		class SEQ_X,
		class SEQ_Y,
		class SEQ_Z
		>
		inline
		void build_cdf( SEQ_X &x, SEQ_Y &y, const SEQ_Z &z )
		{
			assert( z.size() > 0 );
			x.free();
			y.free();
			size_t                                i    = 0;         //!< counter
			typename SEQ_Z::const_iterator        curr = z.begin(); //!< iterator
			const typename SEQ_Z::const_iterator  done = z.end();   //!< when to stop
			x.push_back( *curr  );
			y.push_back(   ++i  );
			for( ++curr; curr != done; ++curr )
			{
				++i;
				const typename SEQ_Z::mutable_type &t = *curr;
				if( t > x.back() )
				{
					//-- new class
					x.push_back( t );
					y.push_back( i );
				}
				else
				{
					//-- increase class count
					y.back() = i;
				}
			}
			assert( i == z.size() );
		}
		
		
	}
	
}

#endif

