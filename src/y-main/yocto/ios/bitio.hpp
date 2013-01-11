#ifndef YOCTO_BITIO_INCLUDED
#define YOCTO_BITIO_INCLUDED 1

#include "yocto/sequence/list.hpp"
#include <iosfwd>

namespace yocto
{
	namespace ios
	{
		//! a list of bits + FIFO structure
		class bitio : public list<bool>
		{
		public:
			typedef list<bool>::node_type bit_t;
			
			explicit bitio() throw();
			explicit bitio( size_t n, const as_capacity_t & );
			virtual ~bitio() throw();
			
			void     fill_to_byte_with( bool b ); //!< atomic filling to byte boundary
			
			//! FIFO push
			void push(bool);
			
			template <typename T>
			inline void push( T X, size_t nbits)
			{
				assert(nbits<=sizeof(T)*8);
				while( nbits > 0 )
				{
					push_back( 0 != ( X & 1 ) );
					X >>= 1;
					--nbits;
				}
			}
			
			template <typename T>
			inline void push_full( T X )
			{
				this->push<T>( X, sizeof(T)*8 );
			}
			
			//! FIFO pop/peek
			bool peek() const throw();
			bool pop()  throw();
			void skip() throw();
			void skip( size_t nbits ) throw();
			
			template <typename T>
			inline T peek( size_t nbits ) const throw()
			{
				assert(nbits<=sizeof(T)*8);
				assert(nbits<=list_.size);
				T            X(0);
				T            B(1);
				const bit_t *b = list_.head;
				while( nbits > 0 )
				{
					if(b->data) X |= B;
					--nbits;
					b = b->next;
					B <<= 1;
				}
				return X;
			}
			
			template <typename T>
			inline T peek_full() const throw()
			{
				return this->peek<T>( sizeof(T) * 8 ); 
			}
			
			template <typename T>
			inline T pop( size_t nbits ) throw()
			{
#if defined(__ICC)
#pragma warning(push)
#pragma warning(disable:2259)
#endif
				assert(nbits<=sizeof(T)*8);
				assert(nbits<=list_.size);
				T            X(0);
				T            B(1);
				while( nbits > 0 )
				{
					bit_t   *b = list_.pop_front();
					if( b->data )
						X |= B;
					pool_.store(b);
					--nbits;
					B <<= 1;
				}
				return X;
#if defined(__ICC)
#pragma warning(pop)
#endif
			}
			
			template <typename T>
			inline T pop_full() throw()
			{
				return this->pop<T>( sizeof(T) * 8 );
			}
			
            void output( std::ostream &os, size_t nbits) const;
            
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(bitio);
			
		};
	}
}

#endif
