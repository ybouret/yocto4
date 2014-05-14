#include "yocto/counting/dispatch.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

#include "yocto/memory/global.hpp"

#include <iostream>

namespace yocto
{
    static inline
    size_t check_drawers(const size_t drawers)
    {
        if(drawers<2)
            throw exception("invalid #drawers=%u",unsigned(drawers));
        return drawers;
    }
    
    static inline
    size_t check_socks(const size_t socks)
    {
        if(socks<=0)
            throw exception("need at least a sock to dispatch...");
        return socks;
    }
    
    dispatch:: dispatch( size_t num_socks, size_t num_drawers) :
    socks(   check_socks(num_socks)     ),
    drawers( check_drawers(num_drawers) ),
    walls(drawers-1),
    meta(walls+socks),
    C(meta,walls),
    count((size_t*) memory::global::__calloc(drawers, sizeof(size_t) ))
    {
        update();
    }
    
    dispatch:: ~dispatch() throw()
    {
        memory::global:: __free(count);
    }
    
    void dispatch:: update() throw()
    {
        //the combination holds the position of the walls
        size_t   last = C[0];
        count[0] = last;
        //std::cerr << "#drawer[0]=" << count[0] << std::endl;
#if !defined(NDEBUG)
        size_t sum = count[0];
#endif
        for(size_t i=1;i<walls;++i)
        {
            const size_t curr = C[i];
            count[i] = (curr-last)-1;
            //std::cerr << "#drawer[" << i << "]=" << count[i] << std::endl;
            last = curr;
#if !defined(NDEBUG)
            sum += count[i];
#endif
        }
        count[walls] = meta-(++last);
        //std::cerr << "#drawer[" << walls << "]=" << count[walls] << std::endl;

#if !defined(NDEBUG)
        sum += count[walls];
        assert(socks==sum);
#endif
    }
    
    void dispatch:: init() throw()
    {
        C.init();
        update();
    }
    
    bool dispatch::next() throw()
    {
        if(C.next())
        {
            update();
            return true;
        }
        else
            return false;
    }
    
    std::ostream & operator<<( std::ostream &os, const dispatch &D )
    {
        os << "[";
        for(size_t i=0;i<D.drawers;++i) os << ' ' << D.count[i];
        os << " ]';";
        return os;
    }
    
    uint64_t dispatch::id() const throw()
    {
        return C.id;
    }

    
}