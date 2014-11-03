#include "yocto/fame/split/quad3d.hpp"
#include "yocto/fame/split/quad1d.hpp"


namespace yocto
{
    namespace fame
    {
        layout3D quad3D:: split(const layout3D &full,
                                const int       rank,
                                const int       size,
                                const bool      xpbc,
                                quad_links     &xlinks,
                                const bool      ypbc,
                                quad_links     &ylinks,
                                const bool      zpbc,
                                quad_links     &zlinks,
                                coord3D        &ranks,
                                coord3D        &sizes)
        {
            
            assert(rank>=0);
            assert(rank<size);
            
            return full;
        }
        
    }
}
