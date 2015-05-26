#include "yocto/net/network.hpp"
#include "yocto/memory/buffers.hpp"
#include "yocto/memory/pooled.hpp"

#if defined(YOCTO_WIN)
#include "yocto/exceptions.hpp"
#endif

#if defined(YOCTO_BSD)
#include <csignal>
#include <unistd.h>
#endif

#include <iostream>

namespace yocto {
	
	namespace network {
		
		
#		if	defined(YOCTO_WIN)
		static WSADATA wsa;
#		endif
		
		net:: net()
		{
			YOCTO_GIANT_LOCK();
			std::cerr << "[network.startup";
#			if defined(YOCTO_WIN)
			
			memset(&wsa, 0, sizeof(WSADATA) );
			if( :: WSAStartup( MAKEWORD(2,2), &wsa ) !=  0 )
			{
				throw win32::exception( ::WSAGetLastError(), "WSAStartup" );
			}
#			endif
			
#if			defined(YOCTO_BSD)
			signal( SIGPIPE, SIG_IGN );
#endif
			std::cerr << "]" << std::endl;
		}
		
		net:: ~net() throw()
		{
#if defined(YOCTO_WIN)
			::WSACleanup();
#endif
			std::cerr << "[network.cleanup";
			std::cerr << "]" << std::endl;
		}
		
		string net:: get_host_name() const
		{
			size_t len = 256;
			
#if defined(YOCTO_BSD)
			for(;;)
			{
				memory::buffer_of<char,memory::pooled> blk( len );
				if( ::gethostname( blk(), blk.length() ) != 0)
				{
					len *= 2;
					continue;
				}
				return string( blk() );
			}
#endif
			
#if defined(YOCTO_WIN)
			YOCTO_GIANT_LOCK();
			for(;;) {
				memory::buffer_of<char,memory::pooled> blk( len );
				if( ::gethostname( blk(), blk.length() ) == SOCKET_ERROR ) {
					const DWORD err = ::WSAGetLastError();
					if( err != WSAEFAULT )
						throw   win32::exception( err, "::gethostname" );
					len *= 2;
					continue;
				}
				return string( blk() );
			}
#endif
			
		}
		
		const uint16_t net:: reserved_port   = IPPORT_RESERVED;
#       if defined(YOCTO_WIN) || defined(__FreeBSD__)
		const uint16_t net:: first_user_port = IPPORT_RESERVED;
#		else
		const uint16_t net:: first_user_port = IPPORT_USERRESERVED;
#		endif
		
		const uint16_t net:: user_port_width = (final_user_port-first_user_port)+1;
		
		const char net:: name[] = "net";
		
	}
	
}
