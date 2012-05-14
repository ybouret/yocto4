#ifndef YOCTO_SWAMP_DATA_BLOCK_INCLUDED
#define YOCTO_SWAMP_DATA_BLOCK_INCLUDED 1

#include "yocto/swamp/linear.hpp"

namespace yocto 
{
	namespace swamp 
	{
		//! memory for an array
		class data_block : public object
		{
		public:
			explicit data_block( linear_base &linear_object );
			virtual ~data_block() throw();

		private:
			size_t size;
			void  *base;
			YOCTO_DISABLE_COPY_AND_ASSIGN(data_block);
		};

#if defined(_MSC_VER)
		// this in ctor
#pragma warning (push)
#pragma warning ( disable : 4355 )
#endif
		template <typename ARRAY>
		class standalone : public ARRAY
		{
		public:
			explicit standalone( const typename ARRAY::layout_type &L ) :
			ARRAY( L ),
				blk( *this ) {}

			virtual ~standalone() throw() {}

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(standalone);
			data_block blk;

		};
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
	}

}

#endif
