#ifndef YOCTO_SOCKET_ADDRESS_CTOR_INCLUDED
#define YOCTO_SOCKET_ADDRESS_CTOR_INCLUDED 1

		#define YOCTO_SOCKET_ADDRESS_ADDR(X) ( (void*)(static_cast<uint8_t *>( (void *)&addr_[0] ) + (X)) )
		#define YOCTO_SOCKET_ADDRESS_HOST YOCTO_SOCKET_ADDRESS_ADDR( fmt.host_offset )
		#define YOCTO_SOCKET_ADDRESS_PORT *(net16_t *)YOCTO_SOCKET_ADDRESS_ADDR( fmt.port_offset )
		
		#define YOCTO_SOCKET_ADDRESS_CTOR()      \
			addr_(),                             \
			host_( YOCTO_SOCKET_ADDRESS_HOST ),  \
			port(  YOCTO_SOCKET_ADDRESS_PORT )   

#if 0
		namespace
		{
			template <size_t N>
			inline void qw_zero( uint64_t *target ) throw()
			{
				target[0] = 0;
				qw_zero<N-1>( target+1 );
			}
			
			template <>
			inline void qw_zero<1>( uint64_t *target ) throw()
			{
				target[0] = 0;
			}
			
			template <size_t N>
			inline void qw_copy( uint64_t *target, const uint64_t *source) throw()
			{
				target[0] = source[0];
				qw_copy<N-1>( target+1, source+1 );
			}
			
			template <>
			inline void qw_copy<1>( uint64_t *target, const uint64_t *source ) throw()
			{
				target[0] = source[0];
			}

		}
#endif

#endif
