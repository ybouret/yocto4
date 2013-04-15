#ifndef YOCTO_DATA_SET_INCLUDED
#define YOCTO_DATA_SET_INCLUDED 1

#include "yocto/ios/istream.hpp"
#include "yocto/container/sequence.hpp"
#include "yocto/associative/dictionary.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
	
	namespace math
	{
		
		template <typename T>
		class data_set
        {
        public:
            typedef sequence<T> column;
            typedef column *    colptr;
            typedef functor<bool,TL1(T)> callback;
            
            typedef dictionary<size_t,colptr,comparator<size_t>,memory::pooled::allocator> colmap;
            
            explicit data_set() throw();
            virtual ~data_set() throw();
            
            void use( size_t idx, column &col );
            void free_all() throw();
            
            
            void load( ios::istream &in, callback *keep = 0, size_t skip=0, size_t nmax = 0 ) const;
            
            
        private:
            mutable colmap map_;
            YOCTO_DISABLE_COPY_AND_ASSIGN(data_set);
            void parse_line( const string &line ) const;
        };
		
	}
	
}

#endif
