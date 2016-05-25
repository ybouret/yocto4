#include "yocto/stock/dynstack.hpp"

namespace yocto
{
    namespace hidden
    {
        const char dynstack_name[] = "dynstack";
    }


    stackmgr:: ~stackmgr() throw()
    {
        while(hist.size) { object::release1(hist.pop_back()); }
        while(pool.size) { object::release1(pool.query()); }
    }


    void stackmgr:: clean_history() throw()
    {
        while(hist.size)
        {
            pool.store( hist.pop_back() );
        }
    }


    void stackmgr:: save(const ptrdiff_t head,
                         const ptrdiff_t tail)
    {
        frame_t *f = (pool.size>0) ? pool.query() : object::acquire1<frame_t>();
        f->head = head;
        f->tail = tail;
        hist.push_back(f);
    }

    stackmgr:: stackmgr() throw() : hist(), pool()
    {
    }


    stackmgr:: stackmgr(const stackmgr &other) :
    hist(),
    pool()
    {
        for(const frame_t *f = other.hist.head;f;f=f->next)
        {
            frame_t *fc = object::acquire1<frame_t>();
            fc->head = f->head;
            fc->tail = f->tail;
            hist.push_back(fc);
        }
        assert(hist.size==other.hist.size);
    }

    size_t stackmgr:: frames() const throw()
    {
        return hist.size;
    }


}
