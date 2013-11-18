#ifndef YOCTO_MEMORY_BUFFER_INCLUDED
#define YOCTO_MEMORY_BUFFER_INCLUDED 1


#include "yocto/object.hpp"

namespace yocto
{
	
	namespace memory
	{
		
		class ro_buffer : public object
		{
		public:
			virtual ~ro_buffer() throw();
			
			const void * ro() const throw();
			virtual size_t length() const throw() = 0; //!< in bytes
			
			uint8_t get_byte(size_t index) const throw(); //!< pad with 0
			
		protected:
			explicit ro_buffer() throw();
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ro_buffer);
			virtual const void *get_address() const throw() = 0;
		};
		
		
		class rw_buffer : public ro_buffer
		{
		public:
			virtual ~rw_buffer() throw();
			void *rw() throw();
			
		protected:
			explicit rw_buffer() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rw_buffer);
		};

#if defined(_MSC_VER)
#pragma warning (disable : 4351 )
#endif
        template <size_t N>
        class fixed_length_buffer : public rw_buffer
        {
        public:

            inline fixed_length_buffer() throw() :
            addr()
            {
                clear();
            }

			inline fixed_length_buffer( const fixed_length_buffer &other ) throw() :
			addr()
			{
				for(size_t i=0;i<N;++i) addr[i] = other.addr[i];
			}

			inline fixed_length_buffer & operator=( const fixed_length_buffer &other ) throw()
			{
				for(size_t i=0;i<N;++i) addr[i] = other.addr[i];
				return *this;
			}
            virtual ~fixed_length_buffer() throw() { clear(); }
            
            virtual size_t length() const throw() { return N; }
            
            inline friend bool operator==( const fixed_length_buffer &lhs, const fixed_length_buffer &rhs ) throw()
            {
                for(size_t i=0;i<N;++i) if( lhs.addr[i] != rhs.addr[i] ) return false;
                return true;
            }
            
            inline friend bool operator!=( const fixed_length_buffer &lhs, const fixed_length_buffer &rhs ) throw()
            {
                for(size_t i=0;i<N;++i) if( lhs.addr[i] != rhs.addr[i] ) return true;
                return false;
            }
            
			

        private:
            uint8_t addr[N];
            virtual const void *get_address() const throw() { return addr; }
            inline void clear() throw() { for(size_t i=0;i<N;++i) addr[i] = 0; }
        };
	}
	
}

#endif
