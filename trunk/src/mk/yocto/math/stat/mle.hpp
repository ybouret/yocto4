#ifndef _SYQUO_MLE_HPP_
#define _SYQUO_MLE_HPP_ 1

#include "statistics.hpp"
#include "functions.hpp"
#include "minimize.hpp"

namespace syquo {

	namespace math {

		//! maximum of likelihood estimator, gaussian
		/**
			provides an estimator of average in ave.b
			and of the standard deviation sig.b.
			Then the confidence parameters are estimated to
			bracket ave.a <= ave.b <= ave.c and sig.a <= sig.b <= sig.c
		*/
		template <
		class T,
		class SEQ
		>
		void mle_gaussian( triplet<T> &ave, triplet<T> &sig, const SEQ &data, const T confidence )
		{
			static const typename kernel::statistical<T>::proc_t pfn[2] = { kernel::statistical<T>::get_id, kernel::statistical<T>::get_sq  };
			assert( data.size() >= 2 );
			assert( confidence > 0 );
			assert( confidence < 1 );
			const size_t n      = data.size();
			T            ans[2] = { 0, 0 };
			T           &sum_x  = ans[0];
			T           &sum_x2 = ans[1];
			kernel::recursive_sum<T,2,typename SEQ::const_iterator,const typename kernel::statistical<T>::proc_t *>( ans, data.begin(), n, pfn );

			//------------------------------------------------------------------
			//-- Maximum of likelyhood estimator
			//------------------------------------------------------------------
			const T &mean = (ave.b = sum_x/n);
			const T  ns2  = (sum_x2 - (sum_x * sum_x)/n);
			const T  s2   = ns2/n;
			const T &sdev = (sig.b = std::sqrt( s2 ) );


			//-- compute error on average:
			//-- t= ((mean-mu_true)/sdev) * sqrt(n-1)
			//-- is a Student's law with n-1 degrees of freedom
			const size_t nu    = n-1;
			const T      alpha = clamp<T>(0,1-confidence,1);
			const T      h     = alpha * T(0.5);
			const T      omh   = T(1) - h;
			const T      t     = student_icdf<T>(nu,omh);
			const T      d_ave = (t*sdev)/std::sqrt(nu);

			ave.a = mean - d_ave;
			ave.c = mean + d_ave;

			//std::cout << "ave=" << ave << std::endl;

			//-- compute error on standard deviation
			//-- mS2/sigma_true is a chi2 law with n-1 degress of freedom
			//-- so we compute
			//-- the symetric confidence interval
			const T theta_lo = chi2_icdf<T>( nu, h   );
			const T theta_hi = chi2_icdf<T>( nu, omh );

			sig.a = std::sqrt( ns2/theta_hi );
			sig.c = std::sqrt( ns2/theta_lo );

		}

	}

}

#endif
