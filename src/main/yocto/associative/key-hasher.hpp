//! \file key-hasher.hpp

#ifndef YOCTO_KEY_HASHER_INCLUDED
#define YOCTO_KEY_HASHER_INCLUDED 1

#include "yocto/hashing/sfh.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/type/args.hpp"
#include "yocto/code/class-conversion.hpp"

namespace yocto
{
	
	template <
	typename T,
	typename HASHING_FUNCTION = hashing::sfh
	>
	class key_hasher
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		inline  key_hasher() throw() : hash_() {}
		inline ~key_hasher() throw() {}
		
		inline size_t operator()( param_type args ) throw()
		{
			return hash( args, int2type< YOCTO_IS_SUPERSUBCLASS(memory::ro_buffer,mutable_type) >() );
		}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(key_hasher);
		HASHING_FUNCTION hash_;
		
		inline size_t hash( param_type args, int2type<true> ) throw()
		{
			return static_cast<hashing::function&>(hash_).key<size_t>( args.ro(), args.length() );
		}
		
		inline size_t hash( param_type args, int2type<false> ) throw()
		{
			return static_cast<hashing::function&>(hash_).key<size_t>( &args, sizeof(type) );
		}
		
	};
	
}


#endif
