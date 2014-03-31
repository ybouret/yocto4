#ifndef YOCTO_FOVEA_VTK_INCLUDED
#define YOCTO_FOVEA_VTK_INCLUDED 1

#include "yocto/fovea/types.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/threading/singleton.hpp"

#include "yocto/associative/map.hpp"
#include "yocto/type/spec.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace fovea
    {
        
        class vtk : public singleton<vtk>
        {
        public:
            typedef void (*callback)(ios::ostream &, const vtk &, const void *);
            typedef map<type_spec,callback,key_hasher<type_spec>,memory_allocator> cbDB;
            string real_fmt; //!< for double, default is "%g"
            string unit_fmt; //!< for unit_t, default is "%d"
            
            
            void define( const std::type_info &tid, callback cb);
            template <typename T>
            inline void define(callback cb)
            {
                define( typeid(T), cb);
            }
            
            void output( ios::ostream &fp, const std::type_info &tid, const void *addr) const;
            template <typename T>
            inline ios::ostream & out( ios::ostream &fp, const T &obj ) const
            {
                output(fp, typeid(T), &obj);
                return fp;
            }
            
        private:
            friend class singleton<vtk>;
            static const threading::longevity life_time = 10; //!< TODO: set to a better value
            static const char                 name[];
            explicit vtk();
            virtual ~vtk() throw();
            cbDB db;
            
        };
        
    }
}

#endif

