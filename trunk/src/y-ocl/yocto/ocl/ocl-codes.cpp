#include "yocto/ocl/codes.hpp"
#include "yocto/exception.hpp"

#include "yocto/ios/raw-file.hpp"
#include "yocto/ios/istream.hpp"
#include "yocto/ios/ichannel.hpp"

#include <cstdio>

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			
			
			static inline void * __chunk_allocate( size_t &n )
			{
				if( n > 0 )
				{
					try {
						return Object:: operator new( n );
					}
					catch (...) {
						n = 0;
						throw;
					}
				}
				else 
					return NULL;
				
			}
			
			Chunk:: Chunk( size_t n ) :
			size( n ),
			data( __chunk_allocate( (size_t&) size  ) )
			{
			}
			
			Chunk:: Chunk( const Chunk &other ) :
			size( other.size ),
			data( __chunk_allocate( (size_t&)size ) )
			{
				memcpy( data, other.data, size );
			}
			
			
			Chunk:: ~Chunk() throw()
			{
				if( data )
				{
					assert(size>0);
					Object:: operator delete(data,size);
					*(size_t*)&size = 0;
					data            = NULL;
				}
			}
			
		} // Code
		
		
		Codes:: Codes() :
		CodesType(1,as_capacity),
		mode_( LoadNone )
		{
			
		}
		
		Codes:: ~Codes() throw()
		{
		}
		
		Codes:: Codes( const Codes &other ) :
		CodesType( other ),
		mode_( LoadNone )
		{
		}
		
		Codes & Codes:: operator<<( LoadMode m ) throw()
		{
			mode_ = m;
			return *this;
		}
		
		static const char no_load_mode[] = "OpenCL.Codes(No LoadMode)";
		
		Codes & Codes:: operator<<( const string &str ) 
		{
			switch( mode_ )
			{
				case LoadFile: 
					_ld_file( str );
					break;
					
				case LoadData:
					_ld_data( str.ro(), str.length() );
					break;
					
				case LoadNone:
					throw exception("%s", no_load_mode);
			}
			return *this;
		}
		
		Codes & Codes:: operator<<( const char *txt )
		{
			switch( mode_ )
			{
				case LoadFile: { 
					const string str(txt);
					_ld_file( str ); }
					break;
					
				case LoadData:
					_ld_data( txt, length_of(txt) );
					break;
					
				case LoadNone:
					throw exception("%s", no_load_mode );
			}
			return *this;
			
		}
		
		Codes & Codes:: operator<<( const memory::ro_buffer &buf )
		{
			_ld_data( buf.ro(), buf.length() );
			return *this;
		}
		
		void Codes:: _ld_data( const void *buf, const size_t len )
		{
			assert( !( buf==NULL && len>0) );
			Core::Chunk *chunk = new Core::Chunk( len ); assert( len == chunk->size );
			const Code   code  = chunk;
			memcpy( chunk->data, buf, len );
			push_back( code );
		}
		
		
		void Codes:: _ld_file( const string &filename )
		{
			ios::raw_file fp( filename, ios::readable);
			Code          code = new Core::Chunk( size_t(fp.length()) );
			
			fp.get_all( code->data, code->size );
			push_back( code );
		}
		
		Codes & Codes:: operator<<(ios::istream &input )
		{
			string content(512,as_capacity);
			char c=0;
			while( input.query(c) ) content.append( c );
			_ld_data( content.ro(), content.length());
			return *this;
		}
		
		Codes & Codes:: operator<<(ios::ichannel &input )
		{
			string content(512,as_capacity);
			char   buf[ BUFSIZ ];
			for(;;)
			{
				size_t done = 0;
				input.get( buf, sizeof(buf), done );
				if( done <= 0)
					break;
				content.append( buf, done );
			}
			_ld_data( content.ro(), content.length());
			return *this;
		}
		
		
		
		
		Sources:: Sources() throw() : Codes() {}
		Sources:: ~Sources() throw() {}
		Sources:: Sources( const Sources &other ) : Codes( other )
		{
		}
		
		Binaries:: Binaries() throw() : Codes() {}
		Binaries:: ~Binaries() throw() {}
		Binaries:: Binaries( const Binaries &other ) : Codes( other )
		{
		}
		
		
	}
	
}
