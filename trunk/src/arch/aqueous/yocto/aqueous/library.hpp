#ifndef YOCTO_AQUEOUS_LIBRARY_INCLUDED
#define YOCTO_AQUEOUS_LIBRARY_INCLUDED 1

#include "yocto/aqueous/species.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto 
{
    namespace aqueous
    {
        //! library of species
        class library : public species::db
        {
        public:
            explicit library( size_t species_extra_bytes = 0 ) throw();
            virtual ~library() throw();
            
            
            species &add( const string &name, int z );
            species &add( const char   *id,   int z );
            
            template <typename T>
            inline void ld(const string &name, int z)
            {
                species &sp = add(name,z);
                sp.make<T>();
            }
            
            template <typename T,typename U>
            inline void ld( const string &name, int z, typename type_traits<U>::parameter_type args )
            {
                species &sp = add(name,z);
              	sp.make<T,U>(args);
            }
            
            
            template <typename T>
            inline void ld( const char *id, int z )
            {
                const string name(id);
                ld<T>(name,z);
            }
            
            template <typename T,typename U>
            inline void ld( const char *id, int z, typename type_traits<U>::parameter_type args )
            {
                const string name(id);
                ld<T,U>(name,z,args);
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
