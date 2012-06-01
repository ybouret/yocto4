#ifndef YOCTO_AQUEOUS_LIBRARY_INCLUDED
#define YOCTO_AQUEOUS_LIBRARY_INCLUDED 1

#include "yocto/aqueous/species.hpp"

namespace yocto 
{
    namespace aqueous
    {
        class library : public species::db
        {
        public:
            explicit library( size_t species_extra_bytes = 0 ) throw();
            virtual ~library() throw();
            
            
            species &add( const string &name, int z );
            
            
            template <typename T>
            void ld(const string &name, int z)
            {
                add(name,z).make<T>();
            }
            
            template <typename T>
            void ld( const string &name, int z, typename type_traits<T>::parameter_type args )
            {
                add(name,z).make<T>(args);
            }
            
            
            species       & operator[]( const string &name );
            const species & operator[]( const string &name ) const;
            
            species &       operator[]( const char *name );
            const species & operator[]( const char *name ) const;
            
        private:
            const size_t nx;
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
    }
}

#endif
