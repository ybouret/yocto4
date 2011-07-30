#ifndef YOCTO_SPADE_WORKSPACE_INCLUDED
#define YOCTO_SPADE_WORKSPACE_INCLUDED 1

#include "yocto/spade/composition.hpp"
#include "yocto/memory/malloc.hpp"
#include "yocto/container/lw-sp.hpp"

namespace yocto
{
	
	namespace spade
	{
		
		template <typename ARRAY>
		class workspace : 
		public            composition,
		public            ARRAY::layout_type	
		{
		public:
			typedef typename ARRAY::layout_type layout_type;
			
			explicit workspace(const composition      & C,
							   const layout_type      & L ) :
			composition( C ),
			layout_type( L ),
			arr_(NULL),
			all_(NULL)
			{
				
				//--------------------------------------------------------------
				//-- allocate memory for components
				//--------------------------------------------------------------
				const size_t nc = components;
				arr_ = memory::global_allocate_as< ARRAY >( nc );
				const ARRAY *a0 = arr_;
				arr_ -= index_min;
				for( unit_t i = index_min; i <= index_max; ++i )
				{
					new ( &arr_[i] ) ARRAY( *this ); //-- according to this layout
				}
				
				//--------------------------------------------------------------
				//-- get information
				//--------------------------------------------------------------
				size_t lengths[ layout_type::dims ] = { 0 };
				a0->request( lengths );
				
				//--------------------------------------------------------------
				//-- compute offsets
				//--------------------------------------------------------------
				const size_t bytes_per_array = layout_base::len2off( lengths, layout_type::dims );
				assert( 0 == lengths[0] );
				
				//--------------------------------------------------------------
				//-- allocate all the memory
				//--------------------------------------------------------------
				try {
					all_ = memory::global_allocate( nc * bytes_per_array );
				}
				catch (...) {
					arr_ += index_min;
					memory::global_release_as< ARRAY >( arr_ );
					throw;
				}
				
				//--------------------------------------------------------------
				//-- prepare the buffers
				//-- WARNING: lengths are now OFFSETS!
				//--------------------------------------------------------------
				void * buffers[ layout_type::dims ] = { all_ };
				for( size_t i=1; i < layout_type::dims ; ++i )
					buffers[i] = static_cast<char *>( all_ ) + lengths[i];
				
				assert( buffers[0] == all_ );
				
				//--------------------------------------------------------------
				//-- link the arrays
				//--------------------------------------------------------------
				for( unit_t i = index_min; i <= index_max; ++i )
				{
					arr_[i].link_to( buffers );
					//-- forward
					for( size_t j=0; j < layout_type::dims; ++j )
						buffers[j] = static_cast<char *>( buffers[j] ) + bytes_per_array;
				}
				
			}
			
			
			virtual ~workspace() throw()
			{
				assert( all_ );
				assert( arr_ );
				arr_ += index_min;
				memory::global_release( all_ );
				memory::global_release_as<ARRAY>( arr_ );
			}
			
			inline ARRAY & operator[]( unit_t c ) throw()
			{
				assert( c >= index_min );
				assert( c <= index_max );
				assert( arr_ );
				assert( all_ );
				return arr_[ c ];
			}
			
			inline const ARRAY & operator[]( unit_t c ) const throw()
			{
				assert( c >= index_min );
				assert( c <= index_max );
				assert( arr_ );
				assert( all_ );
				return arr_[ c ];
			}
			
			inline void copy_into( workspace &sub ) const throw()
			{
				assert( index_min == sub.index_min );
				assert( index_max == sub.index_max );
				ARRAY       *dst = sub.arr_ + index_min;
				const ARRAY *src = arr_     + index_min;
				for( size_t j= components; j>0;--j,++dst,++src)
				{
					src->copy_into( *dst );
				}
			}
			
			inline void fill_with( const workspace &sub ) throw()
			{
				assert( index_min == sub.index_min );
				assert( index_max == sub.index_max );
				ARRAY       *dst = arr_     + index_min;
				const ARRAY *src = sub.arr_ + index_min;
				for( size_t j= components; j>0;--j,++dst,++src)
				{
					dst->fill_with( *src );
				}
			}
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace);
			ARRAY *arr_;
			void  *all_;
			
		};
		
	}
	
}

#endif
