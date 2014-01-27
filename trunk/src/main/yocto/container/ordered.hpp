#ifndef YOCTO_CONTAINER_ORDERED_INCLUDED
#define YOCTO_CONTAINER_ORDERED_INCLUDED 1

#include "yocto/container/container.hpp"
#include "yocto/type/args.hpp"

namespace yocto
{
	
	template <typename T>
	class ordered : public container
	{
	public:
		YOCTO_ARGUMENTS_DECL_T;
		
		virtual ~ordered() throw() {}
		
		virtual bool search( param_type args ) const throw() = 0;
		virtual bool insert( param_type args )               = 0;
		virtual bool remove( param_type args ) throw()       = 0;
        
	protected:
		explicit ordered() throw() {}
		
	private:
		YOCTO_DISABLE_COPY_AND_ASSIGN(ordered);
	};
	
}

#endif
