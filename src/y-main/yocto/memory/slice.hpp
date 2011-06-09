#ifndef YOCTO_MEMORY_SLICE_INCLUDED
#define YOCTO_MEMORY_SLICE_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
	namespace memory
	{

		class carver;
		
		//! handle a slice of memory
		class slice
		{
		public:
			
			//! internal slice formating
			struct block_t
			{
				union 
				{
					block_t *block;
					size_t   _nil_;
				} next, prev;
				
				union
				{
					size_t value;
					void  *_nil_;
				} size;
				
				union 
				{
					slice *owner;
					size_t _nil_;
				} info;
			};
			
			static size_t blocks_for( size_t bytes ) throw();
			slice( size_t data_size, size_t mini_size );
			~slice() throw();
			
			bool is_empty() const throw();
			
			//! best fit acquire
			void *acquire( size_t &bytes ) throw();
			
			//! release
			/**
				\param p a previously slice-acquired are
				\return the owner slice
			 */
			static
			slice *release( void *p ) throw();
			
			size_t	 count() const throw();
			size_t   bytes() const throw();
			
		private:
			size_t   count_;
			block_t *entry_;
			block_t *watch_;
			size_t   ready_; //!< # ready blocks
			YOCTO_DISABLE_COPY_AND_ASSIGN(slice);
			friend class carver;
			
		public:
			slice *next;
			slice *prev;
		};

	}
	
}


#endif
