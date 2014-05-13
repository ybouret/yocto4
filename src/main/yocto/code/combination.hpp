#ifndef YOCTO_COMBINATION_INCLUDED
#define YOCTO_COMBINATION_INCLUDED 1

#include "yocto/os.hpp"
#include <iosfwd>

namespace yocto
{
    
    class combination
    {
    public:
        const size_t n;
        const size_t k;
        
        void init() throw();
        bool next() throw();
        
        explicit combination(const size_t an, const size_t ak);
        virtual ~combination() throw();
        
        //! value of comb[0<=i<k]
        size_t operator[](size_t i) const throw();
        
        friend std::ostream & operator<<( std::ostream &os, const combination &C );
        
    private:
        YOCTO_DISABLE_ASSIGN(combination);
        size_t         *comb;
        const ptrdiff_t nmk;
    };
    
}

#endif
