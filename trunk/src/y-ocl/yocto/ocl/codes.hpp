#ifndef YOCTO_OCL_CODES_INCLUDED
#define YOCTO_OCL_CODES_INCLUDED 1

#include "yocto/ocl/types.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/vector.hpp"


namespace yocto
{

	namespace ios { class istream; }
	
	namespace ocl
	{
		
		//! source/binary container
		class Chunk 
		{
		public:
			const size_t size;
			char        *data;
			
			static Chunk *Create( size_t );
			static void   Delete( Chunk *) throw();
			static void   Retain( Chunk &) throw();
			
		private:
			Chunk( size_t len );
			~Chunk() throw();
			size_t ref_count;
			size_t wksp;
			YOCTO_DISABLE_COPY_AND_ASSIGN(Chunk);
		};
		
		
		//! shared pointer for a chunk
		class Code 
		{
		public:
			Code( size_t len );
			~Code() throw();
			Code( const Code &code ) throw(); //!< shared copy
			
			Chunk * operator->() throw();
			const Chunk * operator->() const throw();
			
			
		private:
			Chunk *chunk;
			YOCTO_DISABLE_ASSIGN(Code);
		};
		
		enum LoadMode
		{
			LoadNone,
			LoadFile,
			LoadData
		};
		
		//! Codes = vector or Code/Chunk
		typedef vector<Code,Allocator> CodesType;
		class Codes : public CodesType
		{
		public:
			explicit Codes() throw();
			virtual ~Codes() throw();
			Codes( const Codes &other ); //!< all Code are shared !
			
			Codes & operator<<( LoadMode ) throw(); //!< modify state
			Codes & operator<<( const string & );   //!< load data or file
			Codes & operator<<( const char   * );   //!< load data or file
			Codes & operator<<( ios::istream & );   //!< must be in the load data state
			
		private:
			LoadMode mode_;
			void load_file( const string & );
			void load_data( const void *data, size_t size );
			
			YOCTO_DISABLE_ASSIGN(Codes);
		};
		
		class Sources : public Codes
		{
		public:
			inline explicit Sources() throw() {}
			inline virtual ~Sources() throw() {}
			inline Sources( const Sources &other ) : Codes( other ) {}
			
		private:
			YOCTO_DISABLE_ASSIGN(Sources);
		};
		
		class Binaries : public Codes
		{
		public:
			inline explicit Binaries() throw() {}
			inline virtual ~Binaries() throw() {}
			inline Binaries( const Binaries &other ) : Codes( other ) {}
			
		private:
			YOCTO_DISABLE_ASSIGN(Binaries);
		};
	}
	
}


#endif
