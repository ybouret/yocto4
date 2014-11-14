#include "yocto/spade/vtk/record.hpp"

namespace yocto
{
    namespace spade
    {
        vtk_record:: ~vtk_record() throw() {}
        
        vtk_record:: vtk_record(const std::type_info &id, const char *k, vtk_callback cb ) :
        spec(id),
        kind(k),
        proc(cb)
        {
            assert(proc);
        }
        
        vtk_record:: vtk_record( const vtk_record &other ) :
        spec( other.spec ),
        kind(other.kind),
        proc(other.proc)
        {
        }
        
        
        const type_spec & vtk_record:: key() const throw()
        {
            return spec;
        }

    }
}
