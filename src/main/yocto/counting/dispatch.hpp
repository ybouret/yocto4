#ifndef YOCTO_COUNTING_DISPATCH_INCLUDED
#define YOCTO_COUNTING_DISPATCH_INCLUDED 1

#include "yocto/counting/combination.hpp"

namespace yocto
{
    
    class dispatch
    {
    public:
        const size_t   socks;   //!< positive
        const size_t   drawers; //!< >=2
        const size_t   walls;   //!< drawers-1
        const size_t   meta;    //!< socks+walls
        
        explicit dispatch(size_t num_socks, size_t num_drawers);
        virtual ~dispatch() throw();
        
        void     init() throw();
        bool     next() throw();
        uint64_t id() const throw();
        
        //! drawer<drawers
        inline size_t operator[](size_t drawer) const throw()
        {
            assert(drawer<drawers);
            return count[drawer];
        }
        
        friend std::ostream & operator<<( std::ostream &os, const dispatch &D );
        
    private:
        combination  C;       //!< C(meta,walls)
        size_t      *count;   //!< socks in drawer #i is count[i]
        void         update() throw(); //!< after a new combination is computed
    };
    
}

#endif
