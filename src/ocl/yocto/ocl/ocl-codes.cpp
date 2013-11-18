#include "yocto/ocl/codes.hpp"
#include "yocto/ocl/exception.hpp"

#include "yocto/ios/raw-file.hpp"
#include "yocto/ios/istream.hpp"

#include <cstring>

namespace yocto
{
	namespace ocl
	{
		
		
		Chunk:: Chunk( size_t len ) :
		size( len  ),
		data( NULL ),
		ref_count( 1 ),
		wksp( size )
		{
			data = memory::kind<MemoryType>::acquire_as<char>( wksp );
			assert( wksp >= size );
		}
		
		Chunk:: ~Chunk() throw()
		{
			memory::kind<MemoryType>::release_as<char>(data,wksp);
		}
		
		Chunk * Chunk:: Create( size_t len ) 
		{
			Chunk *chunk = object::acquire1<Chunk>();
			try 
			{
				new (chunk) Chunk( len );
			}
			catch(...)
			{
				object::release1<Chunk>( chunk );
				throw;
			}
			return chunk;
		}
		
		void Chunk:: Retain( Chunk &chunk ) throw()
		{
			assert( chunk.ref_count > 0 );
			++chunk.ref_count;
		}
		
		void Chunk:: Delete( Chunk *chunk ) throw()
		{
			assert( chunk != NULL );
			assert( chunk->ref_count > 0 );
			if( -- (chunk->ref_count) <= 0 )
			{
				chunk->~Chunk();
				object::release1<Chunk>( chunk );
			}
		}
		
		//======================================================================
		
		Code:: Code( size_t len ) : chunk( Chunk::Create( len ) )
		{
		}
		
		Code:: ~Code() throw() 
		{
			Chunk::Delete( chunk );
		}
		
		Code:: Code( const Code &other ) throw() :
		chunk( other.chunk )
		{
			Chunk::Retain( *chunk );
		}
		
		Chunk * Code:: operator->() throw()
		{
			return chunk;
		}
		
		
		const Chunk * Code:: operator->() const throw()
		{
			return chunk;
		}
		
		
		//======================================================================
		Codes:: Codes() throw() : CodesType(), mode_( LoadNone ) {}
		Codes:: ~Codes() throw() {}
		
		Codes:: Codes( const Codes &other ) : CodesType( other ), mode_(other.mode_) {}
		
		
		Codes & Codes:: operator<<( LoadMode m ) throw() { mode_ = m; return *this; }
		Codes & Codes:: operator<<( const string &str ) 
		{
			switch( mode_ )
			{
				case LoadNone:
					throw Exception( CL_INVALID_VALUE, "Code.LoadNone(%s)", str.c_str() );
					break;
					
				case LoadData: 
					load_data( str.ro(), str.length() );
					break;
					
				case LoadFile: load_file( str );
					break;
			}
			return *this;
		}
		
		Codes & Codes:: operator<<( const char *txt )
		{
			switch( mode_ )
			{
				case LoadNone:
					throw Exception( CL_INVALID_VALUE, "Code.LoadNone(%s)", txt );
					break;
					
				case LoadData: load_data( txt, length_of(txt) );
					break;
					
				case LoadFile: { const string fn( txt ); load_file(fn); }
					break;
			}
			
			return *this;
		}
		
		void Codes:: load_file( const string &fn )
		{
			ios::raw_file input( fn, ios::readable );
			const size_t  len = input.length();
			if( len > 0 )
			{
				Code code( len );
				input.get_all( code->data, code->size );
				push_back( code );
			}
		}
		
		void Codes:: load_data( const void *data,size_t size )
		{
			if( size > 0 )
			{
				assert( data != NULL );
				Code code( size );
				memcpy( code->data, data, size );
				push_back(code);
			}
		}
		
		Codes & Codes:: operator<<( ios::istream &input )
		{
			if( LoadData != mode_ )
				throw Exception( CL_INVALID_VALUE, "Code(istream).mode != LoadData" );
			
			string tmp;
			char   C;
			while( input.query(C) )
			{
				tmp.append( C );
			}
			load_data( tmp.ro(), tmp.length() );
			return *this;
		}
	}
}

