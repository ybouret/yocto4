#ifndef YOCTO_COMBINATION_INCLUDED
#define YOCTO_COMBINATION_INCLUDED 1

#include "yocto/os.hpp"
#include <iosfwd>

namespace yocto
{
    
    //! C-style combination
    class combination
    {
    public:
        const size_t   n;  //!< the number of elements, >0
        const size_t   k;  //!< the size of the set, >0
        const uint64_t id; //!< identifier
        void init() throw();
        bool next() throw();
        
        //! n>0,k>0,k<=n
        explicit combination(const size_t an, const size_t ak);
        virtual ~combination() throw();
        
        //! value of comb[0<=i<k]
        inline size_t operator[](size_t i) const throw()
        {
            assert(i<k);
            assert(comb);
            return comb[i];
        }
        
        friend std::ostream & operator<<( std::ostream &os, const combination &C );
        
        uint64_t count_all() throw();
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(combination);
        size_t         *comb;
        const ptrdiff_t nmk;
        const ptrdiff_t nmkp1;
    };
    
}

#endif
