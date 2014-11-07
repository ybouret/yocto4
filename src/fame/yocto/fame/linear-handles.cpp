#include "yocto/fame/linear-handles.hpp"

namespace yocto
{
    namespace fame
    {
        
        linear_handles::  linear_handles() throw() : chunk_size(0), handles() {}
        linear_handles:: ~linear_handles() throw() {}
        
        linear_handles:: linear_handles(size_t n) : chunk_size(0), handles(n,as_capacity) {}
        
        
        void linear_handles:: append( linear_space *l )
        {
            assert(l);
            const linear_ptr p(l);
            if( handles.insert(p) )
            {
                (size_t &)chunk_size += p->itmsz;
            }
        }

        
        void linear_handles:: remove(const linear_space *l) throw()
        {
            assert(l);
            const linear_ptr p( (linear_space *)l );
            if( handles.remove(p) )
            {
                assert(chunk_size>=p->itmsz);
                (size_t &)chunk_size -= p->itmsz;
            }
        }
        
        
        void linear_handles:: free() throw()
        {
            (size_t &)chunk_size = 0;
            handles.free();
        }
        
        linear_space & linear_handles:: operator[](size_t i) throw()
        {
            assert(i>0);
            assert(i<=handles.size());
            const linear_ptr &p = handles[i];
            return (linear_space &) *p;
        }
        
        const linear_space & linear_handles:: operator[](size_t i) const throw()
        {
            assert(i>0);
            assert(i<=handles.size());
            const linear_ptr &p = handles[i];
            return  *p;
        }


        size_t linear_handles:: size() const throw() { return handles.size(); }

        void linear_handles:: single( linear_space *l )
        {
            free();
            append(l);
        }

        
    }
}
