#ifndef YOCTO_PACK_BWT_INCLUDED
#define YOCTO_PACK_BWT_INCLUDED 1

#include "yocto/os.hpp"


namespace yocto
{
	
	namespace pack
	{
		
		struct bwt
		{
			static size_t encode( void *output, const void *input, const size_t size, size_t *indices) throw();
			static void   decode( void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index) throw();
        
            class common16
            {
            public:
                explicit common16();
                virtual ~common16() throw();
                
                //! size <= 65536
                uint16_t encode( void *output, const void *input, const size_t size) throw();
                void     decode( void *output, const void *input, const size_t size, const uint16_t primary_index) throw();
                
            private:
                size_t    numindx; //!< initially 65536
                size_t   *indices; //!< those indices
                YOCTO_DISABLE_COPY_AND_ASSIGN(common16);
            };
		};
		
	}
	
}


#endif
