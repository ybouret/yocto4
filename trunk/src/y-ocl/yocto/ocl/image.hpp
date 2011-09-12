#ifndef YOCTO_OPENCL_IMAGE_INCLUDED
#define YOCTO_OPENCL_IMAGE_INCLUDED 1

#include "yocto/ocl/mem-object.hpp"
#include "yocto/ocl/image-format.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		
		namespace Core
		{
			
			
			class Image : public MemObject
			{
			public:
				explicit Image( cl_mem img );
				virtual ~Image() throw();
				
				
				const cl_image_format FORMAT;
				const size_t          ELEMENT_SIZE;
				const size_t          ROW_PITCH;
				const size_t          SLICE_PITCH;
				const size_t          WIDTH;
				const size_t          HEIGHT;
				const size_t          DEPTH;
				
				typedef Proxy<Image> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Image);
			};
			
			
		}
		
		class Image : public MemObject
		{
		public:
			virtual ~Image() throw();
			Image( const Image & ) throw(); // Shared Copy
			
			const Core::Image * operator->() const throw();
			
			
		protected:
			Image( cl_mem img );
			
			
		private:
			Core::Image::Pointer pImg_;
			YOCTO_DISABLE_ASSIGN( Image );
			
		};
		
		
		
		
		
	}
	
}

#endif
