#ifndef YOCTO_IOS_ISTREAM_INCLUDED
#define YOCTO_IOS_ISTREAM_INCLUDED 1

#include "yocto/ios/stream.hpp"
#include "yocto/ios/ichannel.hpp"
#include "yocto/string.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
	
	namespace ios
	{
		class istream : public stream, public ichannel
		{
		public:
			virtual ~istream() throw();
			
			virtual bool query( char &C ) = 0;
			virtual void store( char  C ) = 0;
			char         read_char(const char *field=0);
            uint8_t      read_byte(const char *field=0);
            
			//! get at most buflen bytes
			virtual void get( void *data, size_t size, size_t &done );
			
			//! must get exactly buflen bytes
			void load( void *buffer, size_t buflen );
			
			//! read a BigEndian integral type
			template <typename T>
			inline T read() { T x(0); load(&x, sizeof(T)); return swap_be_as<T>(x);}
			
			//! use the read_line API
			int read_line( string &s );
			
			
		protected:
			explicit istream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(istream);
		};
		
		struct read_line
		{
			
			static const int failure      = -2; //!< no end of line detected, and no read chars
			static const int partial      = -1; //!< no end of line detected, but some chars are read
			
			static const int hangs_on_cr  =  0; //!< a CR is detected but currently out of input => KEEP CR and leave the choice to the user
			
			static const int found_lf     =  1; //!< a LF is detected => remove LF and line is ready
			static const int found_crlf   =  2; //!< a CRLF is detected => remove CRLF and line is ready
			static const int found_cr     =  3; //!< a CR + char is detected => remove CR, unread char and line is ready
			
			static int scan( istream &in, string &line );
			
		};
		
	}
	
}
#endif
