#ifndef YOCTO_RECURSIVE_SUM_INCLUDED
#define YOCTO_RECURSIVE_SUM_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/math/math.hpp"

namespace yocto
{
	
	namespace math {
		
		
		namespace kernel
		{
			
			//! recursive sum subroutine
			/**
			 T must accept T temp=0;' and 'T temp[NVAR];' and  '+='
			 */
			template <
			typename  T,        //!< sumable type, must accept 'T temp=0;' and 'T temp[NVAR];'
			size_t    NVAR,     //!< #parallel ops
			typename  ITERATOR, //!< range type
			typename  FUNCTION  //!< array of callable type
			>
			inline
			void recursive_sum(
							   typename type_traits<T>::mutable_type sum[NVAR], //!< current sums
							   ITERATOR                              i,         //!< current iterator
							   const size_t                          n,         //!< range size
							   FUNCTION                              pfn        //!< callable entities
							   )
			{
				assert( sum );
#if !defined(NDEBUG)
				for( size_t j=0;j<NVAR;++j) assert( pfn[j] != NULL );
#endif
				switch( n )
				{
					case 0:
					{
						for( size_t j=0;j<NVAR;++j)
							sum[j] = 0;
					}
						return;
						
					case 1:
					{
						const typename type_traits<T>::mutable_type &v = *i;
						for( size_t j=0;j<NVAR;++j)
							sum[j] = pfn[j]( v );
					}
						return;
						
					default:
					{
						const size_t                          half = n >> 1;                  //< bitwise half
						typename type_traits<T>::mutable_type rsum[NVAR];                     //< temporary right sum
						recursive_sum<T,NVAR,ITERATOR,FUNCTION>( sum,  i,      half,   pfn ); //< left sum
						recursive_sum<T,NVAR,ITERATOR,FUNCTION>( rsum, i+half, n-half, pfn ); //< right sum
						for( size_t j=0;j<NVAR;++j )
							sum[j] += rsum[j];
					}
						break;
				}
			}
			
			template <
			typename  SEQ,
			size_t    NVAR,
			typename  FUNCTION
			>
			inline void recursive_sum(const    SEQ &             seq,
									  typename SEQ::mutable_T    sum[NVAR],
									  FUNCTION                   pfn )
			{
				recursive_sum<
				typename SEQ::mutable_type,
				NVAR,
				typename SEQ::const_iterator,
				FUNCTION>( sum, seq.begin(), seq.size(), pfn );
			}
			
			
			
			
		} // kernel
		
	} // math
	
} 


#endif
