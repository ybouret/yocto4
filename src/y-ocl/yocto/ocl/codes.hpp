#ifndef YOCTO_OPENCL_CODES_INCLUDED
#define YOCTO_OPENCL_CODES_INCLUDED 1

#include "yocto/ocl/types.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
	namespace ios
	{
		class istream;
		class ichannel;
	}
	
	namespace ocl
	{
		
		namespace Core 
		{
			
			class Chunk : public Object
			{
			public:
				explicit Chunk( size_t );
				virtual ~Chunk() throw();
				Chunk( const Chunk & );
				
				typedef Proxy<Chunk> Pointer;
				const size_t size;
				void        *data;
				
			private:
				YOCTO_DISABLE_ASSIGN(Chunk);
			};
		}
		
		typedef Core::Chunk::Pointer Code;
		
		enum LoadMode
		{
			LoadNone,
			LoadFile,
			LoadData
		};
		
		typedef vector<Code,Core::Allocator> CodesType;
		
		class Codes : public CodesType
		{
		public:
			explicit Codes(); //!< at least 1 Code
			virtual ~Codes() throw();
			Codes( const Codes & ); //!< shared copies
			
			
			
			Codes & operator<<( LoadMode ) throw();
			Codes & operator<<( const string & );
			Codes & operator<<( const char   * );
			Codes & operator<<( const memory::ro_buffer & );
			Codes & operator<<( ios::istream  & ); //!< two stages
			Codes & operator<<( ios::ichannel & ); //!< two stages
			
		private:
			LoadMode mode_;
			
			void _ld_file( const string &filename );
			void _ld_data( const void *, const size_t );
			
			YOCTO_DISABLE_ASSIGN(Codes);
		};
		
		class Sources : public Codes
		{
		public:
			explicit Sources() throw();
			virtual ~Sources() throw();
			Sources( const Sources & );
			
		private:
			YOCTO_DISABLE_ASSIGN(Sources);
		};
		
		
		class Binaries : public Codes
		{
		public:
			explicit Binaries() throw();
			virtual ~Binaries() throw();
			Binaries( const Binaries & );
			
		private:
			YOCTO_DISABLE_ASSIGN(Binaries);
		};
		
		
		
	}
	
}

#endif
