#ifndef YOCTO_CHEMICAL_SOLUTION_INCLUDED
#define YOCTO_CHEMICAL_SOLUTION_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
        class solution : public array<double>
        {
        public:
            explicit solution( const collection &from );
            virtual ~solution() throw();
            const collection &lib;
            
            virtual size_t size() const throw();
            
            
        private:
            const size_t n;
            size_t       m; //!< memory
            double      *C; //!< conc
            
            YOCTO_DISABLE_ASSIGN(solution);
        };
    }
}

#endif

