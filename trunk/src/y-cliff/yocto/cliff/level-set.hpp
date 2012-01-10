#ifndef YOCTO_CLIFF_LEVEL_SET_INCLUDED
#define YOCTO_CLIFF_LEVEL_SET_INCLUDED 1


#include "yocto/cliff/types.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
	namespace cliff
	{

				
		//! database of ordered levels
		template <typename T>
		class level_set 
		{
		public:
			typedef int KEY; //!< key type to identify level
			YOCTO_ASSOCIATIVE_KEY_T;
			
			class level
			{
			public:
				level( param_key id, param_type v );				
				~level() throw();
				level( const level &other );		
				
				const_key   key;    //!< unique identifier
				const_type  value;  //!< unique value
				
				//! wrapper for comparator
				friend inline bool operator<( const level &lhs, const level &rhs ) throw()
				{
					return  lhs.value < rhs.value;
				}
				
			private:
				YOCTO_DISABLE_ASSIGN(level);
			};
			
			
			explicit level_set() throw();
			explicit level_set( size_t n );
			virtual ~level_set() throw();
			 
			void add( param_key key, param_type value );			
			
			inline const level & operator[]( size_t i ) const throw() { return levels_[i]; }
			inline size_t size() const throw() { return levels_.size(); }
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(level_set);
			sorted_vector<level> levels_;
		};
		
	}
	
	
}

#endif
