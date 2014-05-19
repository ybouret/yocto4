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
        
        //! cleanup
        virtual ~combination() throw();
        
        //! copy the current combination
        combination(const combination &);
        
        //! value of comb[0<=i<k]
        inline size_t operator[](size_t i) const throw()
        {
            assert(i<k);
            assert(comb);
            return comb[i];
        }
        
        friend std::ostream & operator<<( std::ostream &os, const combination &C );
        
        //! reset and count all
        uint64_t count_all() throw();
        
        void * save(void *) const;
        
    private:
        YOCTO_DISABLE_ASSIGN(combination);
        size_t         *comb;
        const ptrdiff_t nmk;
        const ptrdiff_t nmkp1;
        
    public:
        const size_t bytes_per_index; //!< bytes_for(n-1)
        const size_t bytes_per_frame; //!< byte_per_index * k
        
        
    };
    
}

#endif
