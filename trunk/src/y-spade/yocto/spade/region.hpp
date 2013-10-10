#ifndef YOCTO_SPADE_REGION_INCLUDED
#define YOCTO_SPADE_REGION_INCLUDED 1

#include "yocto/spade/types.hpp"
#include "yocto/code/bswap.hpp"

namespace yocto
{
    namespace spade
    {
        
        template <typename T,typename VERTEX>
        inline T & __component( VERTEX &v, size_t dim ) throw()
        {
            assert(dim<sizeof(VERTEX)/sizeof(T));
            return *(((T*)&v)+dim);
        }
        
        template <typename T,template <class> class VERTEX>
        class region
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef typename VERTEX<T>::type                     vertex;
            typedef typename type_traits<vertex>::parameter_type param_vertex;

            static const size_t DIMENSIONS = sizeof(vertex)/sizeof(T);
            
            const vertex vmin;
            const vertex vmax;
            const vertex length;
            
            explicit region( param_vertex amin, param_vertex amax ) throw() :
            vmin(amin),
            vmax(amax),
            length(amax-amin)
            {
                for( size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    T &cmin = __component<T,vertex>( (vertex &)vmin,dim);
                    T &cmax = __component<T,vertex>( (vertex &)vmax,dim);
                    if( cmax < cmin )
                    {
                        cswap( cmin, cmax);
                        __component<T,vertex>( (vertex &)length,dim) = cmax - cmin;
                    }
                }
            }
        
            region( const region &other ) throw() :
            vmin(   other.cmin   ),
            vmax(   other.cmax   ),
            length( other.length )
            {
            }
            
            
            virtual ~region() throw() {}
            
            
            friend inline std::ostream & operator<<( std::ostream &reg_os, const region &r )
            {
                reg_os << "{{ " << r.vmin << " --> " << r.vmax << " :: " << r.length << "}}";
                return reg_os;
            }
            
            inline region       & as_region() throw()       { return *this; }
            inline const region & as_region() const throw() { return *this; }

        private:
            YOCTO_DISABLE_ASSIGN(region);
           
            
        };
        
    }
}

#endif
