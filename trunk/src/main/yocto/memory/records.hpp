#ifndef YOCTO_MEMORY_RECORDS_INCLUDED
#define YOCTO_MEMORY_RECORDS_INCLUDED 1

#include "yocto/type/args.hpp"

namespace yocto
{

	namespace memory
	{
		
		template <typename T>
		class records_of 
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			
			inline records_of( T *p, size_t n ) throw() :
			size(0),
			capacity(n),
			base( (mutable_type*)p )
			{
				assert( !(base==NULL&&capacity>0) );
			}
			
			inline records_of() throw() : size(0), capacity(0), base(NULL) {}
			
			inline ~records_of() throw() { free(); }
			
			
			const size_t size;     //!< # active object in records
			const size_t capacity; //!< # max active objects in records
			
			inline void free() throw()
			{
				size_t &n = (size_t&)size;
				while(n>0) destruct( &base[--n] );
			}
			
			void operator()( void (*build)(void *ptr,void *args), void *args ) const
			{
				assert( size < capacity );
				assert( build != NULL   );
				size_t &num = (size_t&)size; //-- get available #slot
				void   *ptr = base + num;    //-- get the slot
				build(ptr,args);             //-- perform action on slot
				++num;                       //-- increased upon success
			}
			
			void reset( T *p, size_t n ) throw()
			{
				assert( !(p==NULL&&n>0) );
				free();
				(size_t &) capacity = n;
				base                = (mutable_type *)p;
			}
			
			inline type       & operator[]( size_t index ) throw()       { assert( index < size ); return base[index]; }
			inline const_type & operator[]( size_t index ) const throw() { assert( index < size ); return base[index]; }
			
			inline type       * operator()( const ptrdiff_t shift = 0 ) throw()       { return base + shift; }
			inline const_type * operator()( const ptrdiff_t shift = 0 ) const throw() { return base + shift; }

		private:
			mutable_type *base;
			YOCTO_DISABLE_COPY_AND_ASSIGN(records_of);
		};
		
	}
	
}


#endif
